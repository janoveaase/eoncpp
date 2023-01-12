#pragma once


// Macros for use in tests

#if defined(EON_INLINE_TEST) && defined(EON_INLINE_TEST_PROJECT)

	// To include test-only code outside of macros, use this in an #ifdef test.
#	define EON_TEST_MODE


	// Private declaration in inline test context.
#	define PRIVATE public

	// Protected declaration in inline test context.
#	define PROTECTED public

	// Specify a comma-separated list enclosed in curly braces (Comma-Separated Curly-braced).
#	define EON_CURLY( ... ) { __VA_ARGS__ }

#define utf8( value ) u8 ## value

#	define EON_TRUE( expression )\
	Details.Log << "Testing " << eonitest::style::item_1 << "expression" << eonitest::style::normal\
		<< " evaluates to " << eonitest::style::item_2 << "true" << "." << '\n';\
	Details.Log << "  " << eonitest::style::key_1 << "Expression" << eonitest::style::value << utf8( #expression ) << '\n';\
	Details.Success = static_cast<bool>( expression );\
	if( !Details.Success )\
		Details.Log << "  " << eonitest::style::key_1 << "Value" << eonitest::style::value_1 << "false" << '\n'

#	define EON_FALSE( expression )\
	Details.Log << "Testing " << eonitest::style::item_1 << "expression" << eonitest::style::normal\
		<< " evaluates to " << eonitest::style::item_2 << "false" << "." << '\n';\
	Details.Log << "  " << eonitest::style::key_1 << "Expression" << eonitest::style::value << utf8( #expression ) << '\n';\
	Details.Success = !static_cast<bool>( expression );\
	if( !Details.Success )\
		Details.Log << "  " << eonitest::style::key_1 << "Value" << eonitest::style::value_1 << "true" << '\n'

#	define EON_EQ( left_expression, right_expression )\
	Details.Log << "Testing " << eonitest::style::item_1 << "left" << eonitest::style::item << " == "\
		<< eonitest::style::item_2 << "right" << ":" << '\n';\
	Details.Log << "  " << eonitest::style::key_1 << "left" << eonitest::style::value\
		<< utf8( #left_expression ) << '\n';\
	Details.Log << "  " << eonitest::style::key_2 << "right" << eonitest::style::value\
		<< utf8( #right_expression ) << '\n';\
	auto l = ( left_expression );\
	auto r = ( right_expression );\
	if( typeid( l ) == typeid( const char* ) && typeid( r ) == typeid( const char* ) )\
		Details.Success = ::eon::string( l ) == ::eon::string( r );\
	else\
		Details.Success = ( l == r );\
	if( !Details.Success )\
	{\
		Details.Log << "  " << eonitest::style::key_1 << "Left value" << eonitest::style::value_1\
			<< eonitest::VTS::toString( l ) << '\n';\
		Details.Log << "  " << eonitest::style::key_2 << "Right value" << eonitest::style::value_2\
			<< eonitest::VTS::toString( r ) << '\n';\
	}

#	define EON_NE( left_expression, right_expression )\
	Details.Log << "Testing " << eonitest::style::item_1 << "left" << eonitest::style::item << " != "\
		<< eonitest::style::item_2 << "right" << ":" << '\n';\
	Details.Log << "  " << eonitest::style::key_1 << "left" << eonitest::style::value\
		<< utf8( #left_expression ) << '\n';\
	Details.Log << "  " << eonitest::style::key_2 << "right" << eonitest::style::value\
		<< utf8( #right_expression ) << '\n';\
	auto l = ( left_expression );\
	auto r = ( right_expression );\
	if( typeid( l ) == typeid( const char* ) && typeid( r ) == typeid( const char* ) )\
		Details.Success = ::eon::string( l ) != ::eon::string( r );\
	else\
		Details.Success = ( l != r );\
	if( !Details.Success )\
	{\
		Details.Log << "  " << eonitest::style::key_1 << "Left value" << eonitest::style::value_1\
			<< eonitest::VTS::toString( l ) << '\n';\
		Details.Log << "  " << eonitest::style::key_2 << "Right value" << eonitest::style::value_2\
			<< eonitest::VTS::toString( r ) << '\n';\
	}

#	define EON_LT( left_expression, right_expression )\
	Details.Log << "Testing " << eonitest::style::item_1 << "left" << eonitest::style::item << " < "\
		<< eonitest::style::item_2 << "right" << ":" << '\n';\
	Details.Log << "  " << eonitest::style::key_1 << "left" << eonitest::style::value\
		<< utf8( #left_expression ) << '\n';\
	Details.Log << "  " << eonitest::style::key_2 << "right" << eonitest::style::value\
		<< utf8( #right_expression ) << '\n';\
	auto l = ( left_expression );\
	auto r = ( right_expression );\
	if( typeid( l ) == typeid( const char* ) && typeid( r ) == typeid( const char* ) )\
		Details.Success = ::eon::string( l ) < ::eon::string( r );\
	else\
		Details.Success = ( l < r );\
	if( !Details.Success )\
	{\
		Details.Log << "  " << eonitest::style::key_1 << "Left value" << eonitest::style::value_1\
			<< eonitest::VTS::toString( l ) << '\n';\
		Details.Log << "  " << eonitest::style::key_2 << "Right value" << eonitest::style::value_2\
			<< eonitest::VTS::toString( r ) << '\n';\
	}

#	define EON_LE( left_expression, right_expression )\
	Details.Log << "Testing " << eonitest::style::item_1 << "left" << eonitest::style::item << " <= "\
		<< eonitest::style::item_2 << "right" << ":" << '\n';\
	Details.Log << "  " << eonitest::style::key_1 << "left" << eonitest::style::value\
		<< utf8( #left_expression ) << '\n';\
	Details.Log << "  " << eonitest::style::key_2 << "right" << eonitest::style::value\
		<< utf8( #right_expression ) << '\n';\
	auto l = ( left_expression );\
	auto r = ( right_expression );\
	if( typeid( l ) == typeid( const char* ) && typeid( r ) == typeid( const char* ) )\
		Details.Success = ::eon::string( l ) <= ::eon::string( r );\
	else\
		Details.Success = ( l <= r );\
	if( !Details.Success )\
	{\
		Details.Log << "  " << eonitest::style::key_1 << "Left value" << eonitest::style::value_1\
			<< eonitest::VTS::toString( l ) << '\n';\
		Details.Log << "  " << eonitest::style::key_2 << "Right value" << eonitest::style::value_2\
			<< eonitest::VTS::toString( r ) << '\n';\
	}

#	define EON_GT( left_expression, right_expression )\
	Details.Log << "Testing " << eonitest::style::item_1 << "left" << eonitest::style::item << " > "\
		<< eonitest::style::item_2 << "right" << ":" << '\n';\
	Details.Log << "  " << eonitest::style::key_1 << "left" << eonitest::style::value\
		<< utf8( #left_expression ) << '\n';\
	Details.Log << "  " << eonitest::style::key_2 << "right" << eonitest::style::value\
		<< utf8( #right_expression ) << '\n';\
	auto l = ( left_expression );\
	auto r = ( right_expression );\
	if( typeid( l ) == typeid( const char* ) && typeid( r ) == typeid( const char* ) )\
		Details.Success = ::eon::string( l ) > ::eon::string( r );\
	else\
		Details.Success = ( l > r );\
	if( !Details.Success )\
	{\
		Details.Log << "  " << eonitest::style::key_1 << "Left value" << eonitest::style::value_1\
			<< eonitest::VTS::toString( l ) << '\n';\
		Details.Log << "  " << eonitest::style::key_2 << "Right value" << eonitest::style::value_2\
			<< eonitest::VTS::toString( r ) << '\n';\
	}

#	define EON_GE( left_expression, right_expression )\
	Details.Log << "Testing " << eonitest::style::item_1 << "left" << eonitest::style::item << " >= "\
		<< eonitest::style::item_2 << "right" << ":" << '\n';\
	Details.Log << "  " << eonitest::style::key_1 << "left" << eonitest::style::value\
		<< utf8( #left_expression ) << '\n';\
	Details.Log << "  " << eonitest::style::key_2 << "right" << eonitest::style::value\
		<< utf8( #right_expression ) << '\n';\
	auto l = ( left_expression );\
	auto r = ( right_expression );\
	if( typeid( l ) == typeid( const char* ) && typeid( r ) == typeid( const char* ) )\
		Details.Success = ::eon::string( l ) >= ::eon::string( r );\
	else\
		Details.Success = ( l >= r );\
	if( !Details.Success )\
	{\
		Details.Log << "  " << eonitest::style::key_1 << "Left value" << eonitest::style::value_1\
			<< eonitest::VTS::toString( l ) << '\n';\
		Details.Log << "  " << eonitest::style::key_2 << "Right value" << eonitest::style::value_2\
			<< eonitest::VTS::toString( r ) << '\n';\
	}

#	define EON_RANGE( lower, actual_value, upper )\
	Details.Log << "Testing " << eonitest::style::item_1 << "lower" << eonitest::style::item << " <= "\
		<< eonitest::style::item_2 << "value" << eonitest::style::item << " <= "\
		<< eonitest::style::item_3 << "upper" << ":" << '\n';\
	Details.Log << "  " << eonitest::style::key_1 << "lower" << eonitest::style::value << utf8( #lower ) << '\n';\
	Details.Log << "  " << eonitest::style::key_2 << "value" << eonitest::style::value << utf8( #actual_value ) << '\n';\
	Details.Log << "  " << eonitest::style::key_3 << "upper" << eonitest::style::value << utf8( #upper ) << '\n';\
	auto l = ( lower );\
	auto v = ( actual_value );\
	auto u = ( upper );\
	Details.Success = (l <= v && v <= u );\
	if( !Details.Success )\
	{\
		Details.Log << "  " << eonitest::style::key_1 << "Lower value"\
			<< eonitest::style::value_1 << eonitest::VTS::toString( l ) << '\n';\
		Details.Log << "  " << eonitest::style::key_2 << "Value"\
			<< eonitest::style::value_2 << eonitest::VTS::toString( v ) << '\n';\
		Details.Log << "  " << eonitest::style::key_3 << "Upper value"\
			<< eonitest::style::value_3 << eonitest::VTS::toString( u ) << '\n';\
	}

#	define EON_RAISE( expression, expected_exception )\
	Details.Log << "Testing " << eonitest::style::item_1 << "expression" << eonitest::style::item << " throws "\
		<< eonitest::style::item_2 << "exception" << ":" << '\n';\
	try\
	{\
		expression;\
		Details.Success = false;\
		Details.Log << "  " << eonitest::style::key << "Result"\
			<< eonitest::style::value << "Did not raise any exception!" << '\n';\
	}\
	catch( expected_exception )\
	{\
		Details.Success = true;\
	}\
	catch( eon::exception& e )\
	{\
		Details.Success = false;\
		Details.Log << "  " << eonitest::style::key << "Result"\
			<< eonitest::style::error << "Raised different exception!" << '\n';\
		Details.Log << "  " << eonitest::style::key << "Type" << eonitest::style::value << "eon::exception" << '\n';\
		Details.Log << "  " << eonitest::style::key << "Name"\
			<< eonitest::style::value << eon::str( e.name() ) << '\n';\
		Details.Log << "  " << eonitest::style::key << "Info" << eonitest::style::value << e.info() << '\n';\
	}\
	catch( std::exception& e )\
	{\
		Details.Success = false;\
		Details.Log << "  " << eonitest::style::key << "Result"\
		<< eonitest::style::value << "Raised different exception!" << '\n';\
		Details.Log << "  " << eonitest::style::key << "Type" << eonitest::style::value << "std::exception" << '\n';\
		eon::string type = typeid( e ).name();\
		eon::string what = eon::string( e.what() ).trim();\
		Details.Log << "  " << eonitest::style::key << "Type"\
			<< eonitest::style::value << type << '\n';\
		Details.Log << "  " << eonitest::style::key << "What" << eonitest::style::value << what << '\n';\
	}\
	catch( ... )\
	{\
		Details.Success = false;\
		Details.Log << "  " << eonitest::style::key << "Result"\
		<< eonitest::style::value << "Raised different exception!" << '\n';\
		Details.Log << "  " << eonitest::style::key << "Type" << eonitest::style::value << "unknown" << '\n';\
	}

#	define EON_NO_X( expression )\
	Details.Log << "Testing that " << eonitest::style::item_1 << "expression"\
		<< eonitest::style::item << " throws no exceptions:" << '\n';\
	try\
	{\
		expression;\
		Details.Success = true;\
	}\
	catch( eon::exception& e )\
	{\
		Details.Success = false;\
		Details.Log << "  " << eonitest::style::key << "Result"\
			<< eonitest::style::error << "Raised an exception!" << '\n';\
		Details.Log << "  " << eonitest::style::key << "Type" << eonitest::style::value << "eon::exception" << '\n';\
		Details.Log << "  " << eonitest::style::key << "Name"\
			<< eonitest::style::value << eon::str( e.name() ) << '\n';\
		Details.Log << "  " << eonitest::style::key << "Info" << eonitest::style::value << e.info() << '\n';\
	}\
	catch( std::exception& e )\
	{\
		Details.Success = false;\
		Details.Log << "  " << eonitest::style::key << "Result"\
		<< eonitest::style::value << "Raised an exception!" << '\n';\
		Details.Log << "  " << eonitest::style::key << "Type" << eonitest::style::value << "std::exception" << '\n';\
		eon::string type = typeid( e ).name();\
		eon::string what = eon::string( e.what() ).trim();\
		Details.Log << "  " << eonitest::style::key << "Type"\
			<< eonitest::style::value << type << '\n';\
		Details.Log << "  " << eonitest::style::key << "What" << eonitest::style::value << what << '\n';\
	}\
	catch( ... )\
	{\
		Details.Success = false;\
		Details.Log << "  " << eonitest::style::key << "Result"\
		<< eonitest::style::value << "Raised an exception!" << '\n';\
		Details.Log << "  " << eonitest::style::key << "Type" << eonitest::style::value << "unknown" << '\n';\
	}



#	define TEST_NAME( class_name, function_name, test_name ) class_name##_##function_name##_##test_name
#	define TEST_SUPER( class_name, function_name, test_name ) ::eonitest::EonTest(\
		::eonitest::TestName( #class_name, #function_name, #test_name ) )
#	define TEST_SUPER_SANDBOX( class_name, function_name, test_name ) ::eonitest::EonTestSandbox(\
		::eonitest::TestName( #class_name, #function_name, #test_name ) )
#	define TEST_LINE std::to_string( __LINE__ )
#	define REGISTRATION_DUMMY( class_name, function_name, test_name )\
	class_name##_##function_name##_##test_name##_registration_dummy


#define EON_REGISTER_TEST( class_name, function_name, test_name )\
	bool REGISTRATION_DUMMY( class_name, function_name, test_name )\
	{\
		::eonitest::TestBase::registerTest(\
			eonitest::TestName( #class_name, #function_name, #test_name ),\
			eonitest::TestLocation( __FILE__, TEST_LINE ),\
			new ::eonitest::TstFactory<TEST_NAME( class_name, function_name, test_name )>() )\
	}



#	define TEST_BODY( test_expression )\
	void test_body() override\
	{\
		auto error = canRun();\
		if( !error.empty() )\
		{\
			Details.Success = false;\
			Details.Log << eonitest::style::error << "FATAL ERROR!" << " Failed to construct test." << '\n';\
			Details.Log << error << '\n';\
		}\
		test_expression;\
	}

#	define TEST_BODY_2STEP( step1, test_expression )\
	void test_body() override\
	{\
		auto error = canRun();\
		if( !error.empty() )\
		{\
			Details.Success = false;\
			Details.Log << eonitest::style::error << "FATAL ERROR!" << " Failed to construct test." << '\n';\
			Details.Log << error << '\n';\
		}\
		step1;\
		test_expression;\
	}

#	define TEST_BODY_3STEP( step1, step2, test_expression )\
	void test_body() override\
	{\
		auto error = canRun();\
		if( !error.empty() )\
		{\
			Details.Success = false;\
			Details.Log << eonitest::style::error << "FATAL ERROR!" << " Failed to construct test." << '\n';\
			Details.Log << error << '\n';\
		}\
		step1;\
		step2;\
		test_expression;\
	}

	// Define a single-step test where the specified test expression (first and
	// only step) must evaluate to true.
	// Test expressions must be one of the EON macros: EON_TRUE, EON_FALSE,
	// EON_EQ, EON_NE, EON_LT, EON_LE, EON_GT, EON_GE, EON_RAISE, and EON_NO_X.
	// Write test over two lines:
	// 1. EON_TEST( myclass, myfunc, mytest,
	// 2.     EON_EQ( expected, actual ) );
#	define EON_TEST( class_name, function_name, test_name, test_expression )\
	class TEST_NAME( class_name, function_name, test_name ) : public eonitest::EonTest\
	{\
	public:\
		TEST_NAME( class_name, function_name, test_name )() noexcept\
			: TEST_SUPER( class_name, function_name, test_name ) {}\
	private:\
		TEST_BODY( test_expression )\
	}; EON_REGISTER_TEST( class_name, function_name, test_name )

	// Define a two-step test where the specified test expression (second step)
	// must evaluate to true.
	// Test expressions must be one of the EON macros: EON_TRUE, EON_FALSE,
	// EON_EQ, EON_NE, EON_LT, EON_LE, EON_GT, EON_GE, EON_RAISE, and EON_NO_X.
	// Write test over three lines:
	// 1. EON_TEST( myclass, myfunc, mytest,
	// 2.     step1;
	// 2.     EON_EQ( expected, actual ) );
#	define EON_TEST_2STEP( class_name, function_name, test_name, step1, test_expression )\
	class TEST_NAME( class_name, function_name, test_name ) : public eonitest::EonTest\
	{\
	public:\
		TEST_NAME( class_name, function_name, test_name )() noexcept\
			: TEST_SUPER( class_name, function_name, test_name ) {}\
	private:\
		TEST_BODY_2STEP( step1, test_expression )\
	}; EON_REGISTER_TEST( class_name, function_name, test_name )

	// Define a three-step test where the specified test expression (third
	// step) must evaluate to true.
	// Test expressions must be one of the EON macros: EON_TRUE, EON_FALSE,
	// EON_EQ, EON_NE, EON_LT, EON_LE, EON_GT, EON_GE, EON_RAISE, and EON_NO_X.
	// Write test over four lines:
	// 1. EON_TEST( myclass, myfunc, mytest,
	// 2.     step1;
	// 3.     step2;
	// 4.     EON_EQ( expected, actual ) );
#	define EON_TEST_3STEP( class_name, function_name, test_name, step1, step2, test_expression )\
	class TEST_NAME( class_name, function_name, test_name ) : public eonitest::EonTest\
	{\
	public:\
		TEST_NAME( class_name, function_name, test_name )() noexcept\
			: TEST_SUPER( class_name, function_name, test_name ) {}\
	private:\
		TEST_BODY_3STEP( step1, step2, test_expression )\
	}; EON_REGISTER_TEST( class_name, function_name, test_name )


	// Define a single-step test with sandbox where the specified test
	// expression (first and only step) must evaluate to true.
	// Test expressions must be one of the EON macros: EON_TRUE, EON_FALSE,
	// EON_EQ, EON_NE, EON_LT, EON_LE, EON_GT, EON_GE, EON_RAISE, and EON_NO_X.
	// Write test over two lines:
	// 1. EON_TEST( myclass, myfunc, mytest,
	// 2.     EON_EQ( expected, actual ) );
#	define EON_TEST_SANDBOX( class_name, function_name, test_name, test_expression )\
	class TEST_NAME( class_name, function_name, test_name ) : public eonitest::EonTestSandbox\
	{\
	public:\
		TEST_NAME( class_name, function_name, test_name )() noexcept\
			: TEST_SUPER_SANDBOX( class_name, function_name, test_name ) {}\
	private:\
		TEST_BODY( test_expression )\
	}; EON_REGISTER_TEST( class_name, function_name, test_name )

	// Define a two-step test with sandbox where the specified test expression
	// (second step) must evaluate to true.
	// Test expressions must be one of the EON macros: EON_TRUE, EON_FALSE,
	// EON_EQ, EON_NE, EON_LT, EON_LE, EON_GT, EON_GE, EON_RAISE, and EON_NO_X.
	// Write test over three lines:
	// 1. EON_TEST( myclass, myfunc, mytest,
	// 2.     step1;
	// 2.     EON_EQ( expected, actual ) );
#	define EON_TEST_2STEP_SANDBOX( class_name, function_name, test_name, step1, test_expression )\
	class TEST_NAME( class_name, function_name, test_name ) : public eonitest::EonTestSandbox\
	{\
	public:\
		TEST_NAME( class_name, function_name, test_name )() noexcept\
			: TEST_SUPER_SANDBOX( class_name, function_name, test_name ) {}\
	private:\
		TEST_BODY_2STEP( step1, test_expression )\
	}; EON_REGISTER_TEST( class_name, function_name, test_name )

	// Define a three-step test with sandbox where the specified test
	// expression (third step) must evaluate to true.
	// Test expressions must be one of the EON macros: EON_TRUE, EON_FALSE,
	// EON_EQ, EON_NE, EON_LT, EON_LE, EON_GT, EON_GE, EON_RAISE, and EON_NO_X.
	// Write test over four lines:
	// 1. EON_TEST( myclass, myfunc, mytest,
	// 2.     step1;
	// 3.     step2;
	// 4.     EON_EQ( expected, actual ) );
#	define EON_TEST_3STEP_SANDBOX( class_name, function_name, test_name, step1, step2, test_expression )\
	class TEST_NAME( class_name, function_name, test_name ) : public eonitest::EonTestSandbox\
	{\
	public:\
		TEST_NAME( class_name, function_name, test_name )() noexcept\
			: TEST_SUPER_SANDBOX( class_name, function_name, test_name ) {}\
	private:\
		TEST_BODY_3STEP( step1, step2, test_expression )\
	}; EON_REGISTER_TEST( class_name, function_name, test_name )

#else

	// Private declaration in inline test context.
#	define PRIVATE private

	// Protected declaration in inline test context.
#	define PROTECTED protected

	// Define a single-step test where the specified test expression (first and
	// only step) must evaluate to true.
	// Test expressions must be one of the EON macros: EON_TRUE, EON_FALSE,
	// EON_EQ, EON_NE, EON_LT, EON_LE, EON_GT, EON_GE, EON_RAISE, and EON_NO_X.
	// Write test over two lines:
	// 1. EON_TEST( myclass, myfunc, mytest,
	// 2.     EON_EQ( expected, actual ) );
#	define EON_TEST( class_name, function_name, test_name, test_expression )

	// Define a two-step test where the specified test expression (second step)
	// must evaluate to true.
	// Test expressions must be one of the EON macros: EON_TRUE, EON_FALSE,
	// EON_EQ, EON_NE, EON_LT, EON_LE, EON_GT, EON_GE, EON_RAISE, and EON_NO_X.
	// Write test over three lines:
	// 1. EON_TEST( myclass, myfunc, mytest,
	// 2.     step1;
	// 2.     EON_EQ( expected, actual ) );
#	define EON_TEST_2STEP( class_name, function_name, test_name, step1, test_expression )

	// Define a three-step test where the specified test expression (third
	// step) must evaluate to true.
	// Test expressions must be one of the EON macros: EON_TRUE, EON_FALSE,
	// EON_EQ, EON_NE, EON_LT, EON_LE, EON_GT, EON_GE, EON_RAISE, and EON_NO_X.
	// Write test over four lines:
	// 1. EON_TEST( myclass, myfunc, mytest,
	// 2.     step1;
	// 3.     step2;
	// 4.     EON_EQ( expected, actual ) );
#	define EON_TEST_3STEP( class_name, function_name, test_name, step1, step2, test_expression )


	// Define a single-step test with sandbox where the specified test
	// expression (first and only step) must evaluate to true.
	// Test expressions must be one of the EON macros: EON_TRUE, EON_FALSE,
	// EON_EQ, EON_NE, EON_LT, EON_LE, EON_GT, EON_GE, EON_RAISE, and EON_NO_X.
	// Write test over two lines:
	// 1. EON_TEST( myclass, myfunc, mytest,
	// 2.     EON_EQ( expected, actual ) );
#	define EON_TEST_SANDBOX( class_name, function_name, test_name, test_expression )

	// Define a two-step test with sandbox where the specified test expression
	// (second step) must evaluate to true.
	// Test expressions must be one of the EON macros: EON_TRUE, EON_FALSE,
	// EON_EQ, EON_NE, EON_LT, EON_LE, EON_GT, EON_GE, EON_RAISE, and EON_NO_X.
	// Write test over three lines:
	// 1. EON_TEST( myclass, myfunc, mytest,
	// 2.     step1;
	// 2.     EON_EQ( expected, actual ) );
#	define EON_TEST_2STEP_SANDBOX( class_name, function_name, test_name, step1, test_expression )

	// Define a three-step test with sandbox where the specified test
	// expression (third step) must evaluate to true.
	// Test expressions must be one of the EON macros: EON_TRUE, EON_FALSE,
	// EON_EQ, EON_NE, EON_LT, EON_LE, EON_GT, EON_GE, EON_RAISE, and EON_NO_X.
	// Write test over four lines:
	// 1. EON_TEST( myclass, myfunc, mytest,
	// 2.     step1;
	// 3.     step2;
	// 4.     EON_EQ( expected, actual ) );
#	define EON_TEST_3STEP_SANDBOX( class_name, function_name, test_name, step1, step2, test_expression )

#endif

	// Declare that the named function shall not have a unit test.
#	define EON_NO_TEST( class_name, function_name )
