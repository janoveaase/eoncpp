#pragma once

#include <functional>
#include <map>
#include <list>
#include <iostream>
#include <sstream>
#include <regex>
#include <iomanip>
#include <chrono>
#include <filesystem>
#include <eonstring/String.h>
#include <eonstring/Name.h>
#include <eonterminal/Terminal.h>



// Internal control exception
class TestError : public std::exception
{
private:
	std::string Message;
public:
	TestError() : std::exception() {}
	TestError( const char* message ) : std::exception() { Message = message; }
	TestError( const std::string& message ) : std::exception() { Message = message; }
	TestError( std::string&& message ) noexcept : std::exception() { Message = std::move( message ); }
	const char* what() const noexcept override { return Message.c_str(); }
};


/******************************************************************************
  The 'eontest' namespace encloses all public test functionality
******************************************************************************/
namespace eontest
{
	// General macros
#define TEST_NAME( test_class, test_name ) test_class##_##test_name##_test
#define TEST_ID( test_class, test_name ) #test_class"_"#test_name
#define TEST_LINE std::to_string( __LINE__ )

	// Replace std::stringstream for user messages
	class _stringstream
	{
	public:
		_stringstream() { auto x = std::setprecision( std::numeric_limits<double>::digits10 + 2 ); }
		_stringstream( const _stringstream& other ) { *this = other; }

		_stringstream& operator=( const _stringstream& other ) { Strm << other.Strm.str(); return *this; }

		template<typename T>
		inline _stringstream& operator<<( const T& value ) { Strm << value; return *this; }
		template<typename T>
		inline _stringstream& operator<<( T* const& value )
		{
			if( value == nullptr ) Strm << "(nullptr)";
				else Strm << value;
			return *this; }
		inline _stringstream& operator<<( bool value ) { Strm << ( value ? "true" : "false" ); return *this; }
		std::string str() const
		{
			const std::string& in = Strm.str();
			std::string out;
			for( auto c : in )
			{
				if( c == '\0' ) out += "\\0";
					else out += c;
			}
			return out;
		}
	private:
		std::stringstream Strm;
	};


	// Create a test
	// Specify a test class name and the name of the test
#define TEST( test_class, test_name )\
	class TEST_NAME( test_class, test_name ) : public test_class {\
	public:\
		TEST_NAME( test_class, test_name )() noexcept {}\
	private:\
		void test_body() override;\
	};\
	bool test_class##_##test_name##_test_dummy{\
		::eontest::_EonTest::_registerEonTest_( #test_class, #test_name, TEST_LINE,\
		new ::eontest::TestFactory<TEST_NAME( test_class, test_name )>() ) };\
	void TEST_NAME( test_class, test_name )::test_body()



#define EON_MESSAGE_LOCATION( file, line, fatal )\
	::eontest::_EonTest::Report( *this, file, line, fatal ) = ::eontest::_stringstream()

#define FAILURE_MESSAGE_( fatal ) \
	EON_MESSAGE_LOCATION( __FILE__, __LINE__, fatal )

#define NONFATAL_MESSAGE \
	FAILURE_MESSAGE_( false )
#define FATAL_MESSAGE \
	FAILURE_MESSAGE_( true )

	// Stolen from GTest ...
#ifdef __INTEL_COMPILER
#	define EON_AMBIGUOUS_ELSE_BLOCKER
#else
#	define EON_AMBIGUOUS_ELSE_BLOCKER switch (0) case 0: default:  // NOLINT
#endif

#define ADD_FAILURE() NONFATAL_MESSAGE	// Trigger a non-fatal failure
#define FAIL() FATAL_MESSAGE			// Trigger a fatal failure

	// Test that something is boolean true
#define WANT_TRUE( expression ) \
	EON_AMBIGUOUS_ELSE_BLOCKER \
	if( _testTrue( static_cast<bool>( expression ), #expression ) ); else NONFATAL_MESSAGE
#define REQUIRE_TRUE( expression ) \
	EON_AMBIGUOUS_ELSE_BLOCKER \
	if( _testTrue( static_cast<bool>( expression ), #expression ) ); else FATAL_MESSAGE

	// Test that something is boolean false
#define WANT_FALSE( expression ) \
	EON_AMBIGUOUS_ELSE_BLOCKER \
	if( _testFalse( static_cast<bool>( expression ), #expression ) ); else NONFATAL_MESSAGE
#define REQUIRE_FALSE( expression ) \
	EON_AMBIGUOUS_ELSE_BLOCKER \
	if( _testFalse( static_cast<bool>( expression ), #expression ) ); else FATAL_MESSAGE

	// Test that exception isn't thrown
#define _NO_EXCEPT( expression )\
	try { expression; }\
	catch( std::exception& e )\
	{\
		Failed = true;\
		eon::term << "Expression: " << #expression << "\n";\
		eon::term << "Throws: " << e.what() << "\n";\
		eon::term << "Expected no exceptions!\n";\
	}\
	catch( ... )\
	{\
		Failed = true;\
		eon::term << "Expression: " << #expression << "\n";\
		eon::term << "Throws a non-standard exception!\n";\
		eon::term << "Expected no exceptions!\n"; }
#define WANT_NO_EXCEPT( expression )\
	_NO_EXCEPT( expression ); if( Failed ) NONFATAL_MESSAGE
#define REQUIRE_NO_EXCEPT( expression )\
	_NO_EXCEPT( expression ); if( Failed ) FATAL_MESSAGE

	// Test that a specific exception is thrown
#define _EXCEPT( expression, exception_name )\
	try { expression;\
		Failed = true;\
		eon::term << "Expression: " << #expression << "\n";\
		eon::term << "Doesn't throw any exceptions!\n";\
		eon::term << "Expected exception: " << #exception_name << "\n";\
	}\
	catch( exception_name ) { /* This is what we want! */ }\
	catch( std::exception& e )\
	{\
		Failed = true;\
		eon::term << "Expression: " << #expression << "\n";\
		eon::term << "Throws wrong exception: " << e.what() << "\n";\
		eon::term << "Expected exception: " << #exception_name << "\n";\
	}\
	catch( ... )\
	{\
		Failed = true;\
		eon::term << "Expression: " << #expression << "\n";\
		eon::term << "Throws a non-standard exception!\n";\
		eon::term << "Expected exception: " << #exception_name << "\n"; }
#define WANT_EXCEPT( expression, exception_name )\
	_EXCEPT( expression, exception_name ); if( Failed ) NONFATAL_MESSAGE
#define REQUIRE_EXCEPT( expression, exception_name )\
	_EXCEPT( expression, exception_name ); if( Failed ) FATAL_MESSAGE

	// Test that something is equal to something else
#define WANT_EQ( expected, actual )\
	EON_AMBIGUOUS_ELSE_BLOCKER\
	if( _testEq( (expected), (actual), #expected, #actual ) ); else NONFATAL_MESSAGE
#define REQUIRE_EQ( expected, actual )\
	EON_AMBIGUOUS_ELSE_BLOCKER\
	if( _testEq( (expected), (actual), #expected, #actual ) ); else FATAL_MESSAGE

	// Test that something is not equal to something else
#define WANT_NE( expected, actual )\
	if( _testNe( (expected), (actual), #expected, #actual ) ); else NONFATAL_MESSAGE
#define REQUIRE_NE( expected, actual )\
	if( _testNe( (expected), (actual), #expected, #actual ) ); else FATAL_MESSAGE

	// Test that something is less than something else
#define WANT_LT( expected, actual )\
	if( _testLt( (expected), (actual), #expected, #actual ) ); else NONFATAL_MESSAGE
#define REQUIRE_LT( expected, actual )\
	if( _testLt( (expected), (actual), #expected, #actual ) ); else FATAL_MESSAGE

	// Test that something is less than or equal to something else
#define WANT_LE( expected, actual )\
	if( _testLe( (expected), (actual), #expected, #actual ) ); else NONFATAL_MESSAGE
#define REQUIRE_LE( expected, actual )\
	if( _testLe( (expected), (actual), #expected, #actual ) ); else FATAL_MESSAGE

	// Test that something is greater than something else
#define WANT_GT( expected, actual )\
	if( _testGt( (expected), (actual), #expected, #actual ) ); else NONFATAL_MESSAGE
#define REQUIRE_GT( expected, actual )\
	if( _testGt( (expected), (actual), #expected, #actual ) ); else FATAL_MESSAGE

	// Test that something is greater than or equal to something else
#define WANT_GE( expected, actual )\
	if( _testGe( (expected), (actual), #expected, #actual ) ); else NONFATAL_MESSAGE
#define REQUIRE_GE( expected, actual )\
	if( _testGe( (expected), (actual), #expected, #actual ) ); else FATAL_MESSAGE


	class _EonTest;

	// Factory class for creating test object
	class FactoryMain
	{
	public:
		FactoryMain() = default;
		virtual ~FactoryMain() = default;
		virtual _EonTest* createTest( const std::string& test_class, const std::string& test_name ) = 0;
	};
	template<typename T>
	class TestFactory : public FactoryMain
	{
	public:
		TestFactory() = default;
		_EonTest* createTest( const std::string& test_class, const std::string& test_name ) override {
			auto t = new T(); t->_TestID = test_class + "." + test_name; return t; }
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

		inline Messages& operator <<( const char* str ) { if( Info ) *Info += str; return *this; }
		inline Messages& operator <<( const std::string& str ) { if( Info ) *Info += str; return *this; }
		template<typename T>
		Messages& operator <<( T value ) { if( Info ) *Info += std::to_string( value ); return *this; }
	};


	// Super-class for tests
	class _EonTest
	{
	public:
		std::string _TestExe;
		std::string _TestID;
		bool Failed{ false };

	public:
		_EonTest() = default;

		virtual ~_EonTest() = default;


		void _runEonTest_( const std::string& exe )
		{
			_TestExe = exe;
			try { prepare(); }
			catch( std::exception& e )
			{
				Failed = true;
				eon::term << "Failure in 'prepare' method of test!\n";
				eon::term << "Throws: " << e.what() << "\n";
			}
			catch( ... )
			{
				Failed = true;
				eon::term << "Failure in 'prepare' method of test!\n";
				eon::term << "Throws non-standard exception!\n";
			}
			if( !Failed )
			{
				try
				{
					test_body();
				}
				catch( std::string& )
				{
					// Test was aborted!
					Failed = true;
					eon::term << "Test aborted!\n";
				}
			}
			try { cleanup(); }
			catch( std::exception& e )
			{
				Failed = true;
				eon::term << "Failure in 'cleanup' method of test!\n";
				eon::term << "Throws: " << e.what() << "\n";
			}
			catch( ... )
			{
				Failed = true;
				eon::term << "Failure in 'cleanup' method of test!\n";
				eon::term << "Throws non-standard exception!\n";
			}
		}

	protected:
		virtual void test_body() = 0;

		virtual void prepare() {}
		virtual void cleanup() {}

	public:
		struct TestRef
		{
			std::string TestClass;
			std::string TestName;
			FactoryMain* Factory{ nullptr };
			TestRef() = default;
			inline TestRef( const std::string& test_class, const std::string& test_name, FactoryMain* factory ) {
				TestClass = test_class; TestName = test_name; Factory = factory; }
		};
		static std::list<TestRef>* _EonTests_;
		static std::list<std::string>* _EonClasses_;

		// Register a new test
		static bool _registerEonTest_( const std::string& test_class, const std::string& test_name, const std::string& line,
			FactoryMain* test );

		// Reset everything
		static void _resetEon_() noexcept;

	public:
		struct Report
		{
			_EonTest* Test{ nullptr };
			bool Fatal{ false };
			const char* File{ nullptr };
			int Line{ 0 };
			Report( _EonTest& test, const char* file, int line, bool fatal )
			{
				Test = &test;
				Test->Failed = true;
				Fatal = fatal;
				File = file;
				Line = line;
			}
			Report& operator=( ::eontest::_stringstream ss )
			{
				auto str = ss.str();
				if( !str.empty() )
					eon::term << str << "\n";
				Test->_reportEonLocation_( File, Line );
				if( Fatal )
					throw std::string( "" );
				return *this;
			}
		};

	public:
		inline void _reportEonLocation_( const char* file, int line ) { eon::term << "In " << file << ":" << line << "\n"; }
		std::string _encodeEon_( const std::string& str, size_t& diffpos );

		bool _testTrue( bool value, const char* expression )
		{
			if( !value )
			{
				eon::term << "Expression: " << expression << "\n";
				eon::term << "Expected 'true', got 'false'!\n";
			}
			return value;
		}
		bool _testFalse( bool value, const char* expression )
		{
			if( value )
			{
				eon::term << "Expression: " << expression << "\n";
				eon::term << "Expected 'false', got 'true'!\n";
			}
			return !value;
		}

		bool _reportDiff( const std::string& expected, const std::string& actual,
			const char* exp_expr, const char* act_expr );

		inline bool _testEq( const eon::string& expected, const char* actual, const char* exp_expr, const char* act_expr ) {
			return _testEq( expected.stdstr(), std::string( actual ), exp_expr, act_expr ); }
		inline bool _testEq( const char* expected, const eon::string& actual, const char* exp_expr, const char* act_expr ) {
			return _testEq( std::string( expected ), actual.stdstr(), exp_expr, act_expr ); }
		inline bool _testEq( const eon::string& expected, const std::string& actual,
			const char* exp_expr, const char* act_expr ) {
			return _testEq( expected.stdstr(), actual, exp_expr, act_expr ); }
		inline bool _testEq( const std::string& expected, const eon::string& actual,
			const char* exp_expr, const char* act_expr ) {
			return _testEq( expected, actual.stdstr(), exp_expr, act_expr ); }
		inline bool _testEq( const eon::string& expected, const eon::string& actual,
			const char* exp_expr, const char* act_expr ) {
			return _testEq( expected.stdstr(), actual.stdstr(), exp_expr, act_expr ); }
		inline bool _testEq( const std::string& expected, const char* actual, const char* exp_expr, const char* act_expr ) {
			return _testEq( expected, std::string( actual ), exp_expr, act_expr ); }
		inline bool _testEq( const char* expected, const std::string& actual, const char* exp_expr, const char* act_expr ) {
			return _testEq( std::string( expected ), actual, exp_expr, act_expr ); }
		inline bool _testEq( const char* expected, const char* actual, const char* exp_expr, const char* act_expr ) {
			return _testEq( std::string( expected ), std::string( actual ), exp_expr, act_expr ); }
		inline bool _testEq( const std::string& expected, const std::string& actual,
			const char* exp_expr, const char* act_expr ) { return expected == actual ? true
			: _reportDiff( expected, actual, exp_expr, act_expr ); }
		inline bool _testEq( float expected, float actual, const char* exp_expr, const char* act_expr ) {
			return _testEq( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testEq( float expected, double actual, const char* exp_expr, const char* act_expr ) {
			return _testEq( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testEq( double expected, float actual, const char* exp_expr, const char* act_expr ) {
			return _testEq( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testEq( double expected, double actual, const char* exp_expr, const char* act_expr ) {
			return _testEq( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testEq( eon::name_t expected, eon::name_t actual, const char* exp_expr, const char* act_expr ) {
			return _testEq( expected != eon::no_name ? eon::str( expected ).stdstr() : std::string(),
				actual != eon::no_name ? eon::str( actual ).stdstr() : std::string(), exp_expr, act_expr ); }
		template<typename T1, typename T2>
		bool _testEq( const T1& expected, const T2& actual, const char* exp_expr, const char* act_expr )
		{
			if( expected != actual )
			{
				Failed = true;
				eon::term << "Expected expression: " << exp_expr << "\n";
				eon::term << "  Actual expression: " << act_expr << "\n";
				eon::term << "Expected value: \"" << std::to_string( expected ) << "\"\n";
				eon::term << "  Actual value: \"" << std::to_string( actual ) << "\"\n";
				eon::term << "Expected value should be equal to actual!\n";
				return false;
			}
			else
				return true;
		}

		inline bool _testNe( const eon::string& expected, const char* actual, const char* exp_expr, const char* act_expr ) {
			return _testNe( expected.stdstr(), std::string( actual ), exp_expr, act_expr ); }
		inline bool _testNe( const char* expected, const eon::string& actual, const char* exp_expr, const char* act_expr ) {
			return _testNe( std::string( expected ), actual.stdstr(), exp_expr, act_expr ); }
		inline bool _testNe( const eon::string& expected, const std::string& actual,
			const char* exp_expr, const char* act_expr ) {
			return _testNe( expected.stdstr(), actual, exp_expr, act_expr ); }
		inline bool _testNe( const std::string& expected, const eon::string& actual,
			const char* exp_expr, const char* act_expr ) {
			return _testNe( expected, actual.stdstr(), exp_expr, act_expr ); }
		inline bool _testNe( const eon::string& expected, const eon::string& actual,
			const char* exp_expr, const char* act_expr ) {
			return _testNe( expected.stdstr(), actual.stdstr(), exp_expr, act_expr ); }
		inline bool _testNe( const std::string& expected, const char* actual, const char* exp_expr, const char* act_expr ) {
			return _testNe( expected, std::string( actual ), exp_expr, act_expr ); }
		inline bool _testNe( const char* expected, const std::string& actual, const char* exp_expr, const char* act_expr ) {
			return _testNe( std::string( expected ), actual, exp_expr, act_expr ); }
		inline bool _testNe( const char* expected, const char* actual, const char* exp_expr, const char* act_expr ) {
			return _testNe( std::string( expected ), std::string( actual ), exp_expr, act_expr ); }
		inline bool _testNe( const std::string& expected, const std::string& actual,
			const char* exp_expr, const char* act_expr ) { return expected != actual ? true
			: _reportDiff( expected, actual, exp_expr, act_expr ); }
		inline bool _testNe( float expected, float actual, const char* exp_expr, const char* act_expr ) {
			return _testNe( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testNe( float expected, double actual, const char* exp_expr, const char* act_expr ) {
			return _testNe( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testNe( double expected, float actual, const char* exp_expr, const char* act_expr ) {
			return _testNe( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testNe( double expected, double actual, const char* exp_expr, const char* act_expr ) {
			return _testNe( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testNe( eon::name_t expected, eon::name_t actual, const char* exp_expr, const char* act_expr ) {
			return _testNe( expected != eon::no_name ? eon::str( expected ).stdstr() : std::string(),
				actual != eon::no_name ? eon::str( actual ).stdstr() : std::string(), exp_expr, act_expr ); }
		template<typename T1, typename T2>
		bool _testNe( const T1& expected, const T2& actual, const char* exp_expr, const char* act_expr )
		{
			if( expected != actual )
				return true;
			Failed = true;
			eon::term << "Expected expression: " << exp_expr << "\n";
			eon::term << "  Actual expression: " << act_expr << "\n";
			eon::term << "Expected value: \"" << std::to_string( expected ) << "\"\n";
			eon::term << "  Actual value: \"" << std::to_string( actual ) << "\"\n";
			eon::term << "Expected value should not be equal to actual!\n";
			return false;
		}
		
		inline bool _testLt( const eon::string& expected, const char* actual, const char* exp_expr, const char* act_expr ) {
			return _testLt( expected.stdstr(), std::string( actual ), exp_expr, act_expr ); }
		inline bool _testLt( const char* expected, const eon::string& actual, const char* exp_expr, const char* act_expr ) {
			return _testLt( std::string( expected ), actual.stdstr(), exp_expr, act_expr ); }
		inline bool _testLt( const eon::string& expected, const std::string& actual,
			const char* exp_expr, const char* act_expr ) {
			return _testLt( expected.stdstr(), actual, exp_expr, act_expr ); }
		inline bool _testLt( const std::string& expected, const eon::string& actual,
			const char* exp_expr, const char* act_expr ) {
			return _testLt( expected, actual.stdstr(), exp_expr, act_expr ); }
		inline bool _testLt( const eon::string& expected, const eon::string& actual,
			const char* exp_expr, const char* act_expr ) {
			return _testLt( expected.stdstr(), actual.stdstr(), exp_expr, act_expr ); }
		inline bool _testLt( const std::string& expected, const char* actual, const char* exp_expr, const char* act_expr ) {
			return _testLt( expected, std::string( actual ), exp_expr, act_expr ); }
		inline bool _testLt( const char* expected, const std::string& actual, const char* exp_expr, const char* act_expr ) {
			return _testLt( std::string( expected ), actual, exp_expr, act_expr ); }
		inline bool _testLt( const char* expected, const char* actual, const char* exp_expr, const char* act_expr ) {
			return _testLt( std::string( expected ), std::string( actual ), exp_expr, act_expr ); }
		inline bool _testLt( const std::string& expected, const std::string& actual,
			const char* exp_expr, const char* act_expr ) { return expected < actual ? true
			: _reportDiff( expected, actual, exp_expr, act_expr ); }
		inline bool _testLt( float expected, float actual, const char* exp_expr, const char* act_expr ) {
			return _testLt( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testLt( float expected, double actual, const char* exp_expr, const char* act_expr ) {
			return _testLt( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testLt( double expected, float actual, const char* exp_expr, const char* act_expr ) {
			return _testLt( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testLt( double expected, double actual, const char* exp_expr, const char* act_expr ) {
			return _testLt( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testLt( eon::name_t expected, eon::name_t actual, const char* exp_expr, const char* act_expr ) {
			return _testLt( expected != eon::no_name ? eon::str( expected ).stdstr() : std::string(),
				actual != eon::no_name ? eon::str( actual ).stdstr() : std::string(), exp_expr, act_expr ); }
		template<typename T1, typename T2>
		bool _testLt( const T1& expected, const T2& actual, const char* exp_expr, const char* act_expr )
		{
			if( expected < actual )
				return true;
			Failed = true;
			eon::term << "Expected expression: " << exp_expr << "\n";
			eon::term << "  Actual expression: " << act_expr << "\n";
			eon::term << "Expected value: \"" << std::to_string( expected ) << "\"\n";
			eon::term << "  Actual value: \"" << std::to_string( actual ) << "\"\n";
			eon::term << "Expected value should be less than actual!\n";
			return false;
		}
		
		inline bool _testLe( const eon::string& expected, const char* actual, const char* exp_expr, const char* act_expr ) {
			return _testLe( expected.stdstr(), std::string( actual ), exp_expr, act_expr ); }
		inline bool _testLe( const char* expected, const eon::string& actual, const char* exp_expr, const char* act_expr ) {
			return _testLe( std::string( expected ), actual.stdstr(), exp_expr, act_expr ); }
		inline bool _testLe( const eon::string& expected, const std::string& actual,
			const char* exp_expr, const char* act_expr ) {
			return _testLe( expected.stdstr(), actual, exp_expr, act_expr ); }
		inline bool _testLe( const std::string& expected, const eon::string& actual,
			const char* exp_expr, const char* act_expr ) {
			return _testLe( expected, actual.stdstr(), exp_expr, act_expr ); }
		inline bool _testLe( const eon::string& expected, const eon::string& actual,
			const char* exp_expr, const char* act_expr ) {
			return _testLe( expected.stdstr(), actual.stdstr(), exp_expr, act_expr ); }
		inline bool _testLe( const std::string& expected, const char* actual, const char* exp_expr, const char* act_expr ) {
			return _testLe( expected, std::string( actual ), exp_expr, act_expr ); }
		inline bool _testLe( const char* expected, const std::string& actual, const char* exp_expr, const char* act_expr ) {
			return _testLe( std::string( expected ), actual, exp_expr, act_expr ); }
		inline bool _testLe( const char* expected, const char* actual, const char* exp_expr, const char* act_expr ) {
			return _testLe( std::string( expected ), std::string( actual ), exp_expr, act_expr ); }
		inline bool _testLe( const std::string& expected, const std::string& actual,
			const char* exp_expr, const char* act_expr ) { return expected <= actual ? true
			: _reportDiff( expected, actual, exp_expr, act_expr ); }
		inline bool _testLe( float expected, float actual, const char* exp_expr, const char* act_expr ) {
			return _testLe( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testLe( float expected, double actual, const char* exp_expr, const char* act_expr ) {
			return _testLe( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testLe( double expected, float actual, const char* exp_expr, const char* act_expr ) {
			return _testLe( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testLe( double expected, double actual, const char* exp_expr, const char* act_expr ) {
			return _testLe( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testLe( eon::name_t expected, eon::name_t actual, const char* exp_expr, const char* act_expr ) {
			return _testLe( expected != eon::no_name ? eon::str( expected ).stdstr() : std::string(),
				actual != eon::no_name ? eon::str( actual ).stdstr() : std::string(), exp_expr, act_expr ); }
		template<typename T1, typename T2>
		bool _testLe( const T1& expected, const T2& actual, const char* exp_expr, const char* act_expr )
		{
			if( expected <= actual )
				return true;
			Failed = true;
			eon::term << "Expected expression: " << exp_expr << "\n";
			eon::term << "  Actual expression: " << act_expr << "\n";
			eon::term << "Expected value: \"" << std::to_string( expected ) << "\"\n";
			eon::term << "  Actual value: \"" << std::to_string( actual ) << "\"\n";
			eon::term << "Expected value should be less than or equal to actual!\n";
			return false;
		}
		
		inline bool _testGt( const eon::string& expected, const char* actual, const char* exp_expr, const char* act_expr ) {
			return _testGt( expected.stdstr(), std::string( actual ), exp_expr, act_expr ); }
		inline bool _testGt( const char* expected, const eon::string& actual, const char* exp_expr, const char* act_expr ) {
			return _testGt( std::string( expected ), actual.stdstr(), exp_expr, act_expr ); }
		inline bool _testGt( const eon::string& expected, const std::string& actual,
			const char* exp_expr, const char* act_expr ) {
			return _testGt( expected.stdstr(), actual, exp_expr, act_expr ); }
		inline bool _testGt( const std::string& expected, const eon::string& actual,
			const char* exp_expr, const char* act_expr ) {
			return _testGt( expected, actual.stdstr(), exp_expr, act_expr ); }
		inline bool _testGt( const eon::string& expected, const eon::string& actual,
			const char* exp_expr, const char* act_expr ) {
			return _testGt( expected.stdstr(), actual.stdstr(), exp_expr, act_expr ); }
		inline bool _testGt( const std::string& expected, const char* actual, const char* exp_expr, const char* act_expr ) {
			return _testGt( expected, std::string( actual ), exp_expr, act_expr ); }
		inline bool _testGt( const char* expected, const std::string& actual, const char* exp_expr, const char* act_expr ) {
			return _testGt( std::string( expected ), actual, exp_expr, act_expr ); }
		inline bool _testGt( const char* expected, const char* actual, const char* exp_expr, const char* act_expr ) {
			return _testGt( std::string( expected ), std::string( actual ), exp_expr, act_expr ); }
		inline bool _testGt( const std::string& expected, const std::string& actual,
			const char* exp_expr, const char* act_expr ) { return expected > actual ? true
			: _reportDiff( expected, actual, exp_expr, act_expr ); }
		inline bool _testGt( float expected, float actual, const char* exp_expr, const char* act_expr ) {
			return _testGt( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testGt( float expected, double actual, const char* exp_expr, const char* act_expr ) {
			return _testGt( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testGt( double expected, float actual, const char* exp_expr, const char* act_expr ) {
			return _testGt( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testGt( double expected, double actual, const char* exp_expr, const char* act_expr ) {
			return _testGt( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testGt( eon::name_t expected, eon::name_t actual, const char* exp_expr, const char* act_expr ) {
			return _testGt( expected != eon::no_name ? eon::str( expected ).stdstr() : std::string(),
				actual != eon::no_name ? eon::str( actual ).stdstr() : std::string(), exp_expr, act_expr ); }
		template<typename T1, typename T2>
		bool _testGt( const T1& expected, const T2& actual, const char* exp_expr, const char* act_expr )
		{
			if( expected > actual )
				return true;
			Failed = true;
			eon::term << "Expected expression: " << exp_expr << "\n";
			eon::term << "  Actual expression: " << act_expr << "\n";
			eon::term << "Expected value: \"" << std::to_string( expected ) << "\"\n";
			eon::term << "  Actual value: \"" << std::to_string( actual ) << "\"\n";
			eon::term << "Expected value should be greater than actual!\n";
			return false;
		}
		
		inline bool _testGe( const eon::string& expected, const char* actual, const char* exp_expr, const char* act_expr ) {
			return _testGe( expected.stdstr(), std::string( actual ), exp_expr, act_expr ); }
		inline bool _testGe( const char* expected, const eon::string& actual, const char* exp_expr, const char* act_expr ) {
			return _testGe( std::string( expected ), actual.stdstr(), exp_expr, act_expr ); }
		inline bool _testGe( const eon::string& expected, const std::string& actual,
			const char* exp_expr, const char* act_expr ) {
			return _testGe( expected.stdstr(), actual, exp_expr, act_expr ); }
		inline bool _testGe( const std::string& expected, const eon::string& actual,
			const char* exp_expr, const char* act_expr ) {
			return _testGe( expected, actual.stdstr(), exp_expr, act_expr ); }
		inline bool _testGe( const eon::string& expected, const eon::string& actual,
			const char* exp_expr, const char* act_expr ) {
			return _testGe( expected.stdstr(), actual.stdstr(), exp_expr, act_expr ); }
		inline bool _testGe( const std::string& expected, const char* actual, const char* exp_expr, const char* act_expr ) {
			return _testGe( expected, std::string( actual ), exp_expr, act_expr ); }
		inline bool _testGe( const char* expected, const std::string& actual, const char* exp_expr, const char* act_expr ) {
			return _testGe( std::string( expected ), actual, exp_expr, act_expr ); }
		inline bool _testGe( const char* expected, const char* actual, const char* exp_expr, const char* act_expr ) {
			return _testGe( std::string( expected ), std::string( actual ), exp_expr, act_expr ); }
		inline bool _testGe( const std::string& expected, const std::string& actual,
			const char* exp_expr, const char* act_expr ) { return expected >= actual ? true
			: _reportDiff( expected, actual, exp_expr, act_expr ); }
		inline bool _testGe( float expected, float actual, const char* exp_expr, const char* act_expr ) {
			return _testGe( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testGe( float expected, double actual, const char* exp_expr, const char* act_expr ) {
			return _testGe( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testGe( double expected, float actual, const char* exp_expr, const char* act_expr ) {
			return _testGe( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testGe( double expected, double actual, const char* exp_expr, const char* act_expr ) {
			return _testGe( std::to_string( expected ), std::to_string( actual ), exp_expr, act_expr ); }
		inline bool _testGe( eon::name_t expected, eon::name_t actual, const char* exp_expr, const char* act_expr ) {
			return _testGe( expected != eon::no_name ? eon::str( expected ).stdstr() : std::string(),
				actual != eon::no_name ? eon::str( actual ).stdstr() : std::string(), exp_expr, act_expr ); }
		template<typename T1, typename T2>
		bool _testGe( const T1& expected, const T2& actual, const char* exp_expr, const char* act_expr )
		{
			if( expected >= actual )
				return true;
			Failed = true;
			eon::term << "Expected expression: " << exp_expr << "\n";
			eon::term << "  Actual expression: " << act_expr << "\n";
			eon::term << "Expected value: \"" << std::to_string( expected ) << "\"\n";
			eon::term << "  Actual value: \"" << std::to_string( actual ) << "\"\n";
			eon::term << "Expected value should be greater than or equal to actual!\n";
			return false;
		}

	private:
		std::vector<std::string> _splitLines( const std::string& str ) const;
		size_t _findFirstDiffLine( const std::vector<std::string>& expected, const std::vector<std::string>& actual ) const;
		size_t _findFirstDiffPos( const std::string& expected, const std::string& actual ) const;
		std::string _extractLine( const std::string& line, size_t diff_pos, size_t available_size, size_t& start_pos )
			const;
	};


	// Test without sandbox
	class EonTest : public _EonTest {};

	// Test with sandbox
	class EonTestSandbox : public _EonTest
	{
	public:
		EonTestSandbox() = delete;
		EonTestSandbox( std::string test_class, std::string test_name,
			std::filesystem::path sandbox_root = std::filesystem::path() );
		virtual ~EonTestSandbox();

		const std::filesystem::path sandbox() const noexcept { return Sandbox; }

	private:
		void _prepSandbox( std::string test_class, std::string test_name, std::filesystem::path sandbox_root );
		bool _createSandbox() noexcept;
		bool _removeSandbox() noexcept;

	private:
		std::filesystem::path Sandbox;
	};
}
