#pragma once

#include <functional>
#include <map>
#include <list>
#include <iostream>
#include <sstream>
#include <regex>


/*
	Eon Test Library
*/

namespace eontest
{
	// General macros
#define TEST_NAME( test_class, test_name ) test_class##_##test_name##_test
#define TEST_ID( test_class, test_name ) #test_class"_"#test_name
#define TEST_LINE std::to_string( __LINE__ )



	// Create a test
	// Specify a test class name and the name of the test
#define TEST( test_class, test_name )\
	class TEST_NAME( test_class, test_name ) : public test_class {\
	public:\
		TEST_NAME( test_class, test_name )() noexcept {}\
	private:\
		void test_body() override;\
	};\
	bool TEST_NAME( test_class, test_name )_dummy{\
		::eontest::EonTest::registerTest( #test_class, #test_name, TEST_LINE,\
		new ::eontest::TestFactory<TEST_NAME( test_class, test_name )>() ) };\
	void TEST_NAME( test_class, test_name )::test_body()



#define EON_MESSAGE_LOCATION( file, line, fatal ) \
	::eontest::EonTest::Report( *this, file, line, fatal ) = std::stringstream()
//	_makeLocation( file, line, message, fatal )
//	::testing::internal::AssertHelper( fatal, file, line, message ) = ::testing::Message()

#define FAILURE_MESSAGE_( fatal ) \
	EON_MESSAGE_LOCATION( __FILE__, __LINE__, fatal )

#define NONFATAL_MESSAGE \
	FAILURE_MESSAGE_( false )
#define FATAL_MESSAGE \
	FAILURE_MESSAGE_( true )

/*#define GTEST_TEST_BOOLEAN_( expression, text, actual, expected, fail ) \
	if( const ::testing::AssertionResult gtest_ar_ = ::testing::AssertionResult( expression ) ) \
		; \
	else \
		fail(::testing::internal::GetBoolAssertionFailureMessage( gtest_ar_, text, #actual, #expected ).c_str() )*/

		// Add a non-fatal failure
#define ADD_FAILURE() NONFATAL_MESSAGE
	// Trigger a fatal failure
#define FAIL() FATAL_MESSAGE

	// Test that something is boolean true
#define WANT_TRUE( expression ) if( static_cast<bool>( expression ) ); else NONFATAL_MESSAGE
#define REQUIRE_TRUE( expression ) if( static_cast<bool>( expression ) ); else FATAL_MESSAGE

	// Test that exception isn't thrown
#define WANT_NO_EXCEPT( expression )\
	try\
	{\
		expression;\
	}\
	catch( ... )\
	{\
		Failed = true;\
		_Messages << "Expected no exceptions\nCaught an exception\n";\
	}\
	if( Failed ) NONFATAL_MESSAGE
#define REQUIRE_NO_EXCEPT( expression )\
	try\
	{\
		expression;\
	}\
	catch( ... )\
	{\
		Failed = true;\
		_Messages << "Expected no exceptions\nCaught an exception\n";\
	}\
	if( Failed ) FATAL_MESSAGE

	// Test that something is boolean false
#define WANT_FALSE( expression ) if( !static_cast<bool>( expression ) ); else NONFATAL_MESSAGE
#define REQUIRE_FALSE( expression ) if( !static_cast<bool>( expression ) ); else FATAL_MESSAGE

	// Test that something is equal to something else
#define WANT_EQ( expected, actual ) if( _testEq( (expected), (actual) ) ); else NONFATAL_MESSAGE
#define REQUIRE_EQ( expected, actual ) if( _testEq( (expected), (actual) ) ); else FATAL_MESSAGE
//	// Report information
//#define TEST_INFO( info_message ) ::test::Tester::_info( (info_message) )

/*	// Test that something runs without throwing exceptions
#define TEST_NO_THROW( operation, fail_message, fail_operation ) try { operation; } catch( eon::Exception& e )\
	{ ::test::Tester::_noThrow( _TestID, TEST_LINE, e.name(), e.what() ); ::test::Tester::_message( fail_message ); fail_operation; }\
	catch( std::exception& e ) { ::test::Tester::_noThrow( _TestID, TEST_LINE, "std::exception", e.what() ); ::test::Tester::_message( fail_message ); fail_operation; }

	// The that something runs and throws an exception
#define TEST_THROW( operation, expected_exception, fail_message, fail_operation ) try\
	{ operation; ::test::Tester::_throwNone( _TestID, TEST_LINE, #expected_exception ); fail_operation; } catch( expected_exception& e )\
	{ ::test::Tester::_throwOK( _TestID, TEST_LINE, e.name(), e.what() ); } catch( eon::Exception& e )\
	{ ::test::Tester::_throwFail( _TestID, TEST_LINE, #expected_exception, e.name(), e.what() ); ::test::Tester::_message( fail_message ); fail_operation; }\
	catch( std::exception& e ) { ::test::Tester::_throwFail( _TestID, TEST_LINE, #expected_exception, "std::exception", e.what() ); ::test::Tester::_message( fail_message ); fail_operation; }

	// Test that something is not equal to something else
#define TEST_NE( expected, actual, fail_message, fail_operation )\
	if( !::test::Tester::_testNe( (expected), (actual), _TestID, TEST_LINE ) )\
		{::test::Tester::_message( fail_message ); fail_operation;}

	// Test that something is true or false
#define TEST_TRUE( expression, fail_message, fail_operation )\
	if( !::test::Tester::_testTrue( (expression), _TestID, TEST_LINE ) )\
		{::test::Tester::_message( fail_message ); fail_operation;}
#define TEST_FALSE( expression, fail_message, fail_operation )\
	if( !::test::Tester::_testFalse( (expression), _TestID, TEST_LINE ) )\
		{::test::Tester::_message( fail_message ); fail_operation;}*/


	class EonTest;

	// Factory class for creating test object
	class FactoryMain
	{
	public:
		virtual EonTest* createTest( const std::string& test_class,
			const std::string& test_name ) = 0;
	};
	template<typename T>
	class TestFactory : public FactoryMain
	{
	public:
		TestFactory() = default;
		EonTest* createTest( const std::string& test_class,
			const std::string& test_name ) override {
			auto t = new T();
			t->_TestID = test_class + "." + test_name; return t;
		}
	};


	class Messages
	{
	private:
		std::string* Info{ nullptr };
	public:
		Messages() = default;
		~Messages() = default;

		Messages& prep( std::string& info ) { Info = &info; return *this; }
		Messages& prep() { Info = nullptr; return *this; }

		Messages& operator <<( const char* str ) { if( Info ) *Info += str; return *this; }
		Messages& operator <<( const std::string& str ) { if( Info ) *Info += str; return *this; }
		template<typename T>
		Messages& operator <<( T value ) { if( Info ) *Info += std::to_string( value ); return *this; }
	};


	// Super-class for tests
	class EonTest
	{
	public:
		std::string _TestID;
		std::stringstream _Messages;
		bool Failed{ false };

	public:
		EonTest() = default;
		virtual ~EonTest() { std::cout << ( Failed ? "FAIL" : "OK" ) << "\n"; auto str = _Messages.str();  if( !str.empty() ) std::cout << "----------\n" << str << "\n----------\n"; }

		virtual void test_body() = 0;

	public:
		struct TestRef
		{
			std::string TestClass;
			std::string TestName;
			FactoryMain* Factory{ nullptr };
			TestRef() = default;
			TestRef( const std::string& test_class, const std::string& test_name, FactoryMain* factory ) { TestClass = test_class; TestName = test_name; Factory = factory; }
		};
		static std::list<TestRef>* Tests;
		static std::list<std::string>* Classes;

		// Register a new test
		static bool registerTest(
			const std::string& test_class, const std::string& test_name,
			const std::string& line, FactoryMain* test );

		// Reset everything
		static void reset() noexcept;

	public:
		struct Report
		{
			EonTest* Test{ nullptr };
			bool Fatal{ false };
			Report( EonTest& test, const char* file, int line, bool fatal )
			{
				Test = &test;
				Test->Failed = true;
				Fatal = fatal;
				Test->_reportLocation( file, line );
			}
			Report& operator=( std::stringstream& ss )
			{
				auto str = ss.str();
				if( !str.empty() )
					Test->_Messages << str << "\n";
				if( Fatal )
					throw std::string( "" );
				return *this;
			}
		};

	public:
		void _reportLocation( const char* file, int line )
		{
			_Messages << "In " << file << ":" << line << "\n";
		}
		template<typename T1, typename T2>
		bool _testEq( const T1& expected, const T2& actual )
		{
			if( expected != actual )
			{
				Failed = true;
				_Messages << "Failed to compare equal\n";
				_Messages << "Expected: \"" << expected << "\"\n";
				_Messages << "  Actual: \"" << actual << "\"\n";
				return false;
			}
			else
				return true;
		}




/*		bool _testEq( double expected, double actual )
		{
			if( expected != actual )
			{
				Failed = true;
				_Messages.prep( _Info ) << "Failed to compare equal in line\n";
				_Messages.prep( _Info ) << "Expected: \"" << expected << "\"\n";
				_Messages.prep( _Info ) << "  Actual: \"" << actual << "\"\n";
				return false;
			}
			else
				return true;
		}*/
	};

/*
	// Macros
#define _STR_TEST_METHOD( method, type1, type2 )\
	static bool _test##method( type1 expected, type2 actual, const std::string& test_id, const std::string& line ) {\
		return _test##method( std::string( expected ), std::string( actual ), test_id, line ); }
#define _NUM_TEST_METHOD( method, opr, type1, type2 )\
	static bool _test##method( type1 expected, type2 actual, const std::string& test_id, const std::string& line ) {\
		if( static_cast<int64_t>( expected ) opr static_cast<int64_t>( actual ) )\
			return true;\
		fail( test_id, line, {\
			"Expected equal values, but they're not!",\
			"  Expected : \"" + std::string::toString( expected ) + "\"",\
			"    Actual : \"" + std::string::toString( actual ) + "\"" } );\
		return false; }


/*	// Class for running tests
	class Tester
	{
	public:
		Tester() = default;
		~Tester();

		// Run tests based on given filter regex pattern
		bool run( const std::string& filter );

		// Register a new test
		static bool registerTest(
			const std::string& test_class, const std::string& test_name,
			const std::string& line, FactoryMain* test );


		// Tests

		static void _message( const std::string& message ) {
			FailMessages.push_back( message ); }

		static std::stringstream& _fail( const char* test_id, int line )
		{
			fail( test_id, line );
			fail( test_id, line, { ( message ) } ); }
		static void _info( const std::string& message ) {
			info( { ( message ) } ); }

		static void _noThrow( const std::string& test_id,
			const std::string& line, const std::string& x_name,
			const std::string& x_message ) { fail( test_id, line, {
				"Expected no throw, but we caught exception " + x_name + ":",
				x_message } ); }
		static void _throwOK( const std::string& test_id,
			const std::string& line, const std::string& x_name,
			const std::string& x_message ) { info( test_id, line, {
				"Expected throw of " + x_name + " and caught it:",
				x_message } ); }
		static void _throwFail( const std::string& test_id,
			const std::string& line, const std::string& expected,
			const std::string& x_name, const std::string& x_message ) {
			fail( test_id, line, { "Expected throw of " + expected
				+ ", but caught " + x_name + ":", x_message } ); }
		static void _throwNone( const std::string& test_id,
			const std::string& line, const std::string& expected ) {
			fail( test_id, line, { "Expected throw of " + expected
				+ ", but nothing was thrown!" } ); }
		static std::string encode( const std::string& in );


		static bool _testEq(
			const std::string& expected, const std::string& actual,
			const std::string& test_id, const std::string& line );

		_STR_TEST_METHOD( Eq, const char*, const char* );
		_NUM_TEST_METHOD( Eq, == , int32_t, int32_t );
		_NUM_TEST_METHOD( Eq, == , int32_t, uint32_t );
		_NUM_TEST_METHOD( Eq, == , int32_t, int64_t );
		_NUM_TEST_METHOD( Eq, == , int32_t, uint64_t );
		_NUM_TEST_METHOD( Eq, == , int32_t, double );

		_NUM_TEST_METHOD( Eq, == , uint32_t, int32_t );
		_NUM_TEST_METHOD( Eq, == , uint32_t, uint32_t );
		_NUM_TEST_METHOD( Eq, == , uint32_t, int64_t );
		_NUM_TEST_METHOD( Eq, == , uint32_t, uint64_t );
		_NUM_TEST_METHOD( Eq, == , uint32_t, double );

		_NUM_TEST_METHOD( Eq, == , uint64_t, uint64_t );
		_NUM_TEST_METHOD( Eq, == , uint64_t, uint32_t );
		_NUM_TEST_METHOD( Eq, == , uint64_t, int64_t );
		_NUM_TEST_METHOD( Eq, == , uint64_t, int32_t );
		_NUM_TEST_METHOD( Eq, == , uint64_t, double );

		_NUM_TEST_METHOD( Eq, == , int64_t, int64_t );
		_NUM_TEST_METHOD( Eq, == , int64_t, int32_t );
		_NUM_TEST_METHOD( Eq, == , int64_t, double );

		_NUM_TEST_METHOD( Eq, == , double, double );
		_NUM_TEST_METHOD( Eq, == , double, int32_t );
		_NUM_TEST_METHOD( Eq, == , double, int64_t );
/*		template<typename T1, typename T2>
		static bool _testEq( T1 expected, T2 actual,
			const std::string& test_id, const std::string& line )
		{
			if( expected == actual )
				return true;
			return false;
		}*/
		
//		template<typename T1, typename T2>
//		static bool _testEq( T1 expected, T2 actual,
//			const std::string& test_id, const std::string& line );
/*		_STR_TEST_METHOD( Eq, const char*, const char* );
//		_STR_TEST_METHOD( Eq, const char*, const std::string& );
		_STR_TEST_METHOD( Eq, const char*, std::string );
		_STR_TEST_METHOD( Eq, const std::string&, const char* );
		_STR_TEST_METHOD( Eq, const std::string&, const std::string& );
		_NUM_TEST_METHOD( Eq, == , int64_t, int64_t );
		template<typename T1, typename T2>
		static bool _testEq( T1 expected, T2 actual,
			const std::string& test_id, const std::string& line )
		{
			return _testEq( static_cast<int64_t>( expected ), static_cast<int64_t>( actual ), test_id, line );
		}*/
/*		_NUM_TEST_METHOD( Eq, ==, int64_t, int64_t );
		_NUM_TEST_METHOD( Eq, ==, size_t, int64_t );
		_NUM_TEST_METHOD( Eq, ==, int64_t, size_t );
		_NUM_TEST_METHOD( Eq, == , int32_t, int32_t );
		_NUM_TEST_METHOD( Eq, == , size_t, int32_t );
		_NUM_TEST_METHOD( Eq, == , int32_t, size_t );*/

/*		template<typename T1, typename T2>
		static bool _testEq( T1 expected, T2 actual,
			const std::string& test_id, const std::string& line )
		{
			if( expected == (T1)actual )
				return true;
			fail( test_id, line, {
				"Expected equal values, but they're not!",
				"  Expected : \"" + std::string::toString( expected ) + "\"",
				"    Actual : \"" + std::string::toString( actual ) + "\"" } );
			return false;
		}*/
/*		template<>
		static bool _testEq<const char*, const char*>(
			const char* expected, const char* actual,
			const std::string& test_id, const std::string& line ) {
			return _testEq( std::string( expected ), std::string( actual ), test_id, line ); }
		template<>
		static bool _testEq<const char*, const std::string&>(
			const char* expected, const std::string& actual,
			const std::string& test_id, const std::string& line ) {
			return _testEq( std::string( expected ), actual, test_id, line ); }
		template<>
		static bool _testEq<const std::string&, const char*>(
			const std::string& expected, const char* actual,
			const std::string& test_id, const std::string& line ) {
			return _testEq( expected, std::string( actual ), test_id, line ); }*/
/*		static bool _testEq( int expected, int actual,
			const std::string& test_id, const std::string& line )
		{
			if( expected == actual )
				return true;
			fail( test_id, line, {
				"Expected equal values, but they're not!",
				"  Expected : \"" + std::string::toString( expected ) + "\"",
				"    Actual : \"" + std::string::toString( actual ) + "\"" } );
			return false;
		}
		static bool _testEq( uint64_t expected, uint64_t actual,
			const std::string& test_id, const std::string& line )
		{
			if( expected == actual )
				return true;
			fail( test_id, line, {
				"Expected equal values, but they're not!",
				"  Expected : \"" + std::string::toString( expected ) + "\"",
				"    Actual : \"" + std::string::toString( actual ) + "\"" } );
			return false;
		}
		static bool _testEq( double expected, double actual,
			const std::string& test_id, const std::string& line )
		{
			if( expected == actual )
				return true;
			fail( test_id, line, {
				"Expected equal values, but they're not!",
				"  Expected : \"" + std::string::toString( expected ) + "\"",
				"    Actual : \"" + std::string::toString( actual ) + "\"" } );
			return false;
		}* /

		static bool _testNe(
			const std::string& expected, const std::string& actual,
			const std::string& test_id, const std::string& line )
		{
			if( expected != actual )
				return true;
			fail( test_id, line, {
				"Expected different values, but they're not!",
				"  Expected : \"" + encode( expected ) + "\"",
				"    Actual : \"" + encode( actual ) + "\"" } );
			return false;
		}
		static bool _testNe( int64_t expected, int64_t actual,
			const std::string& test_id, const std::string& line )
		{
			if( expected != actual )
				return true;
			fail( test_id, line, {
				"Expected different values, but they're not!",
				"  Expected : \"" + std::string::toString( expected ) + "\"",
				"    Actual : \"" + std::string::toString( actual ) + "\"" } );
			return false;
		}
		static bool _testNe( uint64_t expected, uint64_t actual,
			const std::string& test_id, const std::string& line )
		{
			if( expected != actual )
				return true;
			fail( test_id, line, {
				"Expected different values, but they're not!",
				"  Expected : \"" + std::string::toString( expected ) + "\"",
				"    Actual : \"" + std::string::toString( actual ) + "\"" } );
			return false;
		}
		static bool _testNe( double expected, double actual,
			const std::string& test_id, const std::string& line )
		{
			if( expected != actual )
				return true;
			fail( test_id, line, {
				"Expected different values, but they're not!",
				"  Expected : \"" + std::string::toString( expected ) + "\"",
				"    Actual : \"" + std::string::toString( actual ) + "\"" } );
			return false;
		}

		static bool _testTrue( bool result,
			const std::string& test_id, const std::string& line )
		{
			if( result )
				return true;
			fail( test_id, line, {
				"Expected result to be 'true', but it's not!" } );
			return false;
		}
		static bool _testFalse( bool result,
			const std::string& test_id, const std::string& line )
		{
			if( !result )
				return true;
			fail( test_id, line, {
				"Expected result to be 'false', but it's not!" } );
			return false;
		}

	private:
		static std::stringstream& fail( const char* test_id, int line )
		{
			_Info << "- " << test_id << " line " << line << ":";
			if( !line.empty() )
				FailMessages.push_back( "- " + test_id + " line " + line
					+ ":" );
			else
				FailMessages.push_back( "- " + test_id + ":" );
			for( auto& elm : message )
				FailMessages.push_back( "  " + elm );
		}
		static void info( const std::string& test_id, const std::string& line,
			const std::initializer_list<std::string>& message )
		{
			if( !line.empty() )
				InfoMessages.push_back( "+ " + test_id + " line " + line
					+ ":" );
			else
				InfoMessages.push_back( "+ " + test_id + ":" );
			for( auto& elm : message )
				InfoMessages.push_back( "  " + elm );
		}
		static void info( const std::initializer_list<std::string>& message )
		{
			for( auto& elm : message )
				InfoMessages.push_back( "  " + elm );
		}

	private:
		static std::map<std::string, std::map<std::string, FactoryMain*>>* Tests;
		static std::list<std::string>* Classes;
		static std::list<std::string> Failed;
		static std::list<std::string> FailMessages, InfoMessages;
	};*/
}