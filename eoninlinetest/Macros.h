#pragma once



// Internal macro for constructing a test name.
#define __EONTEST_NAME( classname, methodname, testname ) classname##_##methodname##_##testname

// Internal macro for super-class constructor.
#define __EONTEST_SUPER( classname, methodname, testname )\
	::eonitest::__EonTest( ::eonitest::__EonTestName( #classname, #methodname, #testname ) )

// Internal macro for super-class constructor.
#define __EONTEST_SUPER_SANDBOX( classname, methodname, testname )\
	::eonitest::__EonTestSandbox( ::eonitest::__EonTestName( #classname, #methodname, #testname ) )

// Internal macro for including line number of a test.
#define __EONTEST_LINE std::to_string( __LINE__ )

// Interal macro for catching unexpected exceptions.
#define __EONTEST_CATCHALL( information )\
	catch( eon::exception& e ) \
	{\
		return ::eonitest::__Result::failureByException( "eon::exception", e.details(), #information );\
	}\
	catch( std::exception& e ) \
	{\
		return ::eonitest::__Result::failureByException( "std::exception", e.what(), #information );\
	}\
	catch( ... ) \
	{\
		return ::eonitest::__Result::failureByException( "exception", "unknown", #information );\
	}

// Internal macro for registering a test.
#define __EONTEST_REGISTER( classname, methodname, testname )\
	bool __EONTEST_NAME( classname, methodname, testname )_registration_dummy\
	{\
		::eonitest::__EonTestBase::_registerEonTest_(\
			eonitest::__EonTestName( #classname, #methodname, #testname ),\
			eonitest::__EonLocation( __FILE__, __EONTEST_LINE ),\
			new ::eonitest::TestFactory<__EONTEST_NAME( classname, methodname, testname )>() )\
	}

// Internal macro for defining the test body method for a compare test.
#define __EON_CMPTEST_BODY( test, expected, step1, step2, finalstep )\
	::eonitest::__Result test_body() override\
	{\
		auto error = canRun();\
		if( !error.empty() )\
			return ::eonitest::__Result::failedToConstruct( error );\
		step1;\
		step2;\
		::eonitest::__TestOptions options{ test };\
		auto expected_val = ::eonitest::__Value( (expected), options );\
		auto actual_val = ::eonitest::__Value( (finalstep), options );\
		if( options.compareOperation() )\
			return _runCompare( expected_val, options.operation(), actual_val, #expected, #finalstep );\
		else\
			return ::eonitest::__Result::failCompare(\
				options.operation(), #expected, #finalstep, expected_val.Value, actual_val.Value, \
				"Not a valid test operation" );\
	}

// Internal macro for defining the test body method for an except test.
#define __EONTEST_EXCEPT_BODY( step1, step2, finalstep )\
	::eonitest::__Result test_body() override\
	{\
		step1;\
		step2;\
		try\
		{\
			finalstep;\
			return ::eonitest::__Result::success();\
		}\
		__EONTEST_CATCHALL( "Expected no throw!" )\
	}

// Internal macro for defining the test body method for a no-except test.
#define __EONTEST_NOEXCEPT_BODY( expected_exception, step1, step2, finalstep )\
	::eonitest::__Result test_body() override\
	{\
		step1;\
		step2;\
		try\
		{\
			finalstep;\
			return ::eonitest::__Result::success();\
		}\
		catch( expected_exception )\
		{\
			return ::eonitest::__Result::success(); \
		}\
		__EONTEST_CATCHALL( "Expected different exception \"" #expected_exception "\"!" )\
	}\

// Internal macro for setting up a test class constructor method.
#define __EONTEST_CONSTRUCT( classname, methodname, testname )\
	public:\
		__EONTEST_NAME( classname, methodname, testname )() noexcept\
			: __EONTEST_SUPER( classname, methodname, testname ) {}\
	private:

// Internal macro for setting up a sandbox test class constructor method.
#define __EONTEST_CONSTRUCT_SANDBOX( classname, methodname, testname )\
	public:\
		__EONTEST_NAME( classname, methodname, testname )() noexcept\
			: __EONTEST_SUPER_SANDBOX( classname, methodname, testname ) {}\
	private:\




#if defined(EON_INLINE_TEST) && defined(EON_INLINE_TEST_PROJECT)
	// Private declaration in inline test context.
#	define PRIVATE public

	// Protected declaration in inline test context.
#	define PROTECTED public
#else
	// Private declaration in inline test context.
#	define PRIVATE private

	// Protected declaration in inline test context.
#	define PROTECTED protected
#endif




// Make test succeed if 'finalstep' is less than 'expected'!
#define EON_LT = "<:"

// Make test succeed if 'finalstep' is less than or equal to 'expected'!
#define EON_LE = "<=:"

// Make test succeed if 'finalstep' is greater than 'expected'!
#define EON_GT = ">:"

// Make test succeed if 'finalstep' is greater than or equal to 'expected'!
#define EON_GE = ">=:"

// Make test succeed if 'finalstep' is equal to 'expected'!
#define EON_EQ "==:"

// Make test succeed if 'finalstep' is not equal to 'expected'!
#define EON_NE "!=:"

// Round off decimal points.
#define EON_ROUND( decimal_places ) "roundoff=" #decimal_places ":"




#if defined(EON_INLINE_TEST) && defined(EON_INLINE_TEST_PROJECT)

	// Define a single-step unit test where 'finalstep' must produce the value to be compared.
#	define EON_CMPTEST( classname, methodname, testname, test, expected, finalstep )\
	class __EONTEST_NAME( classname, methodname, testname ) : public eonitest::__EonTest {\
		__EONTEST_CONSTRUCT( classname, methodname, testname )\
		__EON_CMPTEST_BODY( test, expected, , , finalstep )\
	}; __EONTEST_REGISTER( classname, methodname, testname )

	// Define a two-step unit test where 'finalstep' must produce the value to be compared.
#	define EON_CMPTEST_2STEP( classname, methodname, testname, step1, test, expected, finalstep )\
	class __EONTEST_NAME( classname, methodname, testname ) : public eonitest::__EonTest {\
		__EONTEST_CONSTRUCT( classname, methodname, testname )\
		__EON_CMPTEST_BODY( test, expected, step1, , finalstep )\
	}; __EONTEST_REGISTER( classname, methodname, testname )

	// Define a three-step unit test where 'finalstep' must produce the value to be compared.
#	define EON_CMPTEST_3STEP( classname, methodname, testname, step1, step2, test, expected, finalstep )\
	class __EONTEST_NAME( classname, methodname, testname ) : public eonitest::__EonTest {\
		__EONTEST_CONSTRUCT( classname, methodname, testname )\
		__EON_CMPTEST_BODY( test, expected, step1, step2, finalstep )\
	}; __EONTEST_REGISTER( classname, methodname, testname )

#else
#	define EON_CMPTEST( classname, methodname, testname, test, expected, actual )
#	define EON_CMPTEST_2STEP( classname, methodname, testname, step1, test, expected, actual )
#	define EON_CMPTEST_3STEP( classname, methodname, testname, step1, step2, test, expected, actual )
#endif




#if defined(EON_INLINE_TEST) && defined(EON_INLINE_TEST_PROJECT)

	// Define a single-step unit test where the test succeeds if no exceptions are thrown by 'finalstep'.
#	define EON_NOXTEST( classname, methodname, testname, finalstep )\
	class __EONTEST_NAME( classname, methodname, testname ) : public eonitest::__EonTest {\
		__EONTEST_CONSTRUCT( classname, methodname, testname )\
		__EONTEST_EXCEPT_BODY( , , finalstep );\
	}; __EONTEST_REGISTER( classname, methodname, testname )

	// Define a two-step unit test where the test succeeds if no exceptions are thrown by 'finalstep'.
#	define EON_NOXTEST_2STEP( classname, methodname, testname, step1, finalstep )\
	class __EONTEST_NAME( classname, methodname, testname ) : public eonitest::__EonTest {\
		__EONTEST_CONSTRUCT( classname, methodname, testname )\
		__EONTEST_EXCEPT_BODY( step1, , finalstep );\
	}; __EONTEST_REGISTER( classname, methodname, testname )

	// Define a three-step unit test where the test succeeds if no exceptions are thrown by 'finalstep'.
#	define EON_NOXTEST_3STEP( classname, methodname, testname, step1, step2, finalstep )\
	class __EONTEST_NAME( classname, methodname, testname ) : public eonitest::__EonTest {\
		__EONTEST_CONSTRUCT( classname, methodname, testname )\
		__EONTEST_EXCEPT_BODY( step1, step2, finalstep );\
	}; __EONTEST_REGISTER( classname, methodname, testname )

#else
#	define EON_NOXTEST( classname, methodname, testname, finalstep )
#	define EON_NOXTEST_2STEP( classname, methodname, testname, step1, finalstep )
#	define EON_NOXTEST_3STEP( classname, methodname, testname, step1, step2, finalstep )
#endif




#if defined(EON_INLINE_TEST) && defined(EON_INLINE_TEST_PROJECT)

	// Define a single-step unit test where the test succeeds if 'finalstep' throws a specific exception.
#	define EON_XTEST( classname, methodname, testname, expected_exception, finalstep )\
	class __EONTEST_NAME( classname, methodname, testname ) : public eonitest::__EonTest {\
		__EONTEST_CONSTRUCT( classname, methodname, testname )\
		__EONTEST_NOEXCEPT_BODY( expected_exception, , , finalstep );\
	}; __EONTEST_REGISTER( classname, methodname, testname )

	// Define a two-step unit test where the test succeeds if 'finalstep' throws a specific exception.
#	define EON_XTEST_2STEP( classname, methodname, testname, expected_exception, step1, finalstep )\
	class __EONTEST_NAME( classname, methodname, testname ) : public eonitest::__EonTest {\
		__EONTEST_CONSTRUCT( classname, methodname, testname )\
		__EONTEST_NOEXCEPT_BODY( expected_exception, step1, , finalstep );\
	}; __EONTEST_REGISTER( classname, methodname, testname )

	// Define a three-step unit test where the test succeeds if 'finalstep' throws a specific exception.
#	define EON_XTEST_3STEP( classname, methodname, testname, expected_exception, step1, step2, finalstep )\
	class __EONTEST_NAME( classname, methodname, testname ) : public eonitest::__EonTest {\
		__EONTEST_CONSTRUCT( classname, methodname, testname )\
		__EONTEST_NOEXCEPT_BODY( expected_exception, step1, step2, finalstep );\
	}; __EONTEST_REGISTER( classname, methodname, testname )

#else
#	define EON_XTEST( classname, methodname, testname, expected_exception, finalstep )
#	define EON_XTEST_2STEP( classname, methodname, testname, expected_exception, step1, finalstep )
#	define EON_XTEST_3STEP( classname, methodname, testname, expected_exception, step1, step2, finalstep )
#endif




#if defined(EON_INLINE_TEST) && defined(EON_INLINE_TEST_PROJECT)

	// Define a single-step unit test with sandbox where 'finalstep' must produce the value to be compared.
#	define EON_CMPTEST_SANDBOX( classname, methodname, testname, test, expected, finalstep )\
	class __EONTEST_NAME( classname, methodname, testname ) : public eonitest::__EonTestSandbox {\
		__EONTEST_CONSTRUCT_SANDBOX( classname, methodname, testname )\
		__EON_CMPTEST_BODY( test, expected, , , finalstep )\
	}; __EONTEST_REGISTER( classname, methodname, testname )

	// Define a two-step unit test with sandbox where 'finalstep' must produce the value to be compared.
#	define EON_CMPTEST_SANDBOX_2STEP( classname, methodname, testname, step1, test, expected, finalstep )\
	class __EONTEST_NAME( classname, methodname, testname ) : public eonitest::__EonTestSandbox {\
		__EONTEST_CONSTRUCT_SANDBOX( classname, methodname, testname )\
		__EON_CMPTEST_BODY( test, expected, step1, , finalstep )\
	}; __EONTEST_REGISTER( classname, methodname, testname )

	// Define a three-step unit test with sandbox where 'finalstep' must produce the value to be compared.
#	define EON_CMPTEST_SANDBOX_3STEP( classname, methodname, testname, step1, step2, test, expected, finalstep )\
	class __EONTEST_NAME( classname, methodname, testname ) : public eonitest::__EonTestSandbox {\
		__EONTEST_CONSTRUCT_SANDBOX( classname, methodname, testname )\
		__EON_CMPTEST_BODY( test, expected, step1, step2, finalstep )\
	}; __EONTEST_REGISTER( classname, methodname, testname )

#else
#	define EON_CMPTEST_SANDBOX( classname, methodname, testname, test, expected, finalstep )
#	define EON_CMPTEST_SANDBOX_2STEP( classname, methodname, testname, step1, test, expected, finalstep )
#	define EON_CMPTEST_SANDBOX_3STEP( classname, methodname, testname, step1, step2, test, expected, finalstep )
#endif




#if defined(EON_INLINE_TEST) && defined(EON_INLINE_TEST_PROJECT)

	// Define a single-step unit test with sandbox where the test succeeds if no exceptions are thrown by 'finalstep'.
#	define EON_NOXTEST_SANDBOX( classname, methodname, testname, finalstep )\
	class __EONTEST_NAME( classname, methodname, testname ) : public eonitest::__EonTestSandbox {\
		__EONTEST_CONSTRUCT_SANDBOX( classname, methodname, testname )\
		__EONTEST_EXCEPT_BODY( , , finalstep );\
	}; __EONTEST_REGISTER( classname, methodname, testname )

	// Define a two-step unit test with sandbox where the test succeeds if no exceptions are thrown by 'finalstep'.
#	define EON_NOXTEST_SANDBOX_2STEP( classname, methodname, testname, step1, finalstep )\
	class __EONTEST_NAME( classname, methodname, testname ) : public eonitest::__EonTestSandbox {\
		__EONTEST_CONSTRUCT_SANDBOX( classname, methodname, testname )\
		__EONTEST_EXCEPT_BODY( step1, , finalstep );\
	}; __EONTEST_REGISTER( classname, methodname, testname )

	// Define a three-step unit test with sandbox where the test succeeds if no exceptions are thrown by 'finalstep'.
#	define EON_NOXTEST_SANDBOX_3STEP( classname, methodname, testname, step1, step2, finalstep )\
	class __EONTEST_NAME( classname, methodname, testname ) : public eonitest::__EonTestSandbox {\
		__EONTEST_CONSTRUCT_SANDBOX( classname, methodname, testname )\
		__EONTEST_EXCEPT_BODY( step1, step2, finalstep );\
	}; __EONTEST_REGISTER( classname, methodname, testname )

#else
#	define EON_NOXTEST_SANDBOX( classname, methodname, testname, finalstep )
#	define EON_NOXTEST_SANDBOX_2STEP( classname, methodname, testname, step1, finalstep )
#	define EON_NOXTEST_SANDBOX_3STEP( classname, methodname, testname, step1, step2, finalstep )
#endif



#if defined(EON_INLINE_TEST) && defined(EON_INLINE_TEST_PROJECT)

	// Define a single-step unit test with sandbox where the test succeeds if 'finalstep' throws a specific exception.
#	define EON_XTEST_SANDBOX( classname, methodname, testname, expected_exception, finalstep )\
	class __EONTEST_NAME( classname, methodname, testname ) : public eonitest::__EonTestSandbox {\
		__EONTEST_CONSTRUCT_SANDBOX( classname, methodname, testname )\
		__EONTEST_NOEXCEPT_BODY( expected_exception, , , finalstep );\
	}; __EONTEST_REGISTER( classname, methodname, testname )

	// Define a two-step unit test with sandbox where the test succeeds if 'finalstep' throws a specific exception.
#	define EON_XTEST_SANDBOX_2STEP( classname, methodname, testname, expected_exception, step1, finalstep )\
	class __EONTEST_NAME( classname, methodname, testname ) : public eonitest::__EonTestSandbox {\
		__EONTEST_CONSTRUCT_SANDBOX( classname, methodname, testname )\
		__EONTEST_NOEXCEPT_BODY( expected_exception, step1, , finalstep );\
	}; __EONTEST_REGISTER( classname, methodname, testname )

	// Define a three-step unit test with sandbox where the test succeeds if 'finalstep' throws a specific exception.
#	define EON_XTEST_SANDBOX_3STEP( classname, methodname, testname, expected_exception, step1, step2, finalstep )\
	class __EONTEST_NAME( classname, methodname, testname ) : public eonitest::__EonTestSandbox {\
		__EONTEST_CONSTRUCT_SANDBOX( classname, methodname, testname )\
		__EONTEST_NOEXCEPT_BODY( expected_exception, step1, step2, finalstep );\
	}; __EONTEST_REGISTER( classname, methodname, testname )

#else
#	define EON_XTEST_SANDBOX( classname, methodname, testname, expected_exception, finalstep )
#	define EON_XTEST_SANDBOX_2STEP( classname, methodname, testname, expected_exception, step1, step2 )
#	define EON_XTEST_SANDBOX_3STEP( classname, methodname, testname, expected_exception, step1, step2, finalstep )
#endif
