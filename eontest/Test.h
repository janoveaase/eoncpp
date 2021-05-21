#pragma once

#include <functional>
#include <map>
#include <list>
#include <iostream>
#include <sstream>
#include <regex>
#include <iomanip>
#include <chrono>



// Internal control exception
class TestError : public std::exception
{
private:
	std::string Message;
public:
	TestError() : std::exception() {}
	TestError( const char* message ) : std::exception() {
		Message = message; }
	TestError( const std::string& message ) : std::exception() {
		Message = message; }
	TestError( std::string&& message ) noexcept : std::exception() {
		Message = std::move( message ); }
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
		_stringstream() { auto x = std::setprecision(
			std::numeric_limits<double>::digits10 + 2 ); }
		_stringstream( const _stringstream& other ) { *this = other; }
		
		_stringstream& operator=( const _stringstream& other ) {
			Strm << other.Strm.str(); return *this; }
		
		
		template<typename T>
		inline _stringstream& operator<<( const T& value )
		{
			Strm << value;
			return *this;
		}
		template<typename T>
		inline _stringstream& operator<<( T* const& value ) {
			if( value == nullptr ) Strm << "(nullptr)";
				else Strm << value;
			return *this; }
		inline _stringstream& operator<<( bool value ) {
			Strm << value ? "true" : "false"; return *this; }
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
		::eontest::EonTest::registerTest( #test_class, #test_name, TEST_LINE,\
		new ::eontest::TestFactory<TEST_NAME( test_class, test_name )>() ) };\
	void TEST_NAME( test_class, test_name )::test_body()



#define EON_MESSAGE_LOCATION( file, line, fatal )\
	::eontest::EonTest::Report( *this, file, line, fatal )\
		= ::eontest::_stringstream()

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
	if( _testTrue( static_cast<bool>( expression ), #expression ) );\
		else NONFATAL_MESSAGE
#define REQUIRE_TRUE( expression ) \
	EON_AMBIGUOUS_ELSE_BLOCKER \
	if( _testTrue( static_cast<bool>( expression ), #expression ) );\
		else FATAL_MESSAGE

	// Test that something is boolean false
#define WANT_FALSE( expression ) \
	EON_AMBIGUOUS_ELSE_BLOCKER \
	if( _testFalse( static_cast<bool>( expression ), #expression ) );\
		else NONFATAL_MESSAGE
#define REQUIRE_FALSE( expression ) \
	EON_AMBIGUOUS_ELSE_BLOCKER \
	if( _testFalse( static_cast<bool>( expression ), #expression ) );\
		else FATAL_MESSAGE

	// Test that exception isn't thrown
#define _NO_EXCEPT( expression )\
	try { expression; }\
	catch( std::exception& e )\
	{\
		Failed = true;\
		_Messages << "\nExpression: {" << #expression << "} throws:\n";\
		_Messages << e.what() << "\n";\
		_Messages << "Expected no exceptions\n";\
	}\
	catch( ... )\
	{\
		Failed = true;\
		_Messages << "\nExpression: {" << #expression << "} throws\n";\
		_Messages << "Expected no exceptions\n"; }
#define WANT_NO_EXCEPT( expression )\
	_NO_EXCEPT( expression ); if( Failed ) NONFATAL_MESSAGE
#define REQUIRE_NO_EXCEPT( expression )\
	_NO_EXCEPT( expression ); if( Failed ) FATAL_MESSAGE

	// Test that a specific exception is thrown
#define _EXCEPT( expression, exception )\
	try { expression;\
		Failed = true;\
		_Messages << "\nExpression: {" << #expression << "} doesn't throw\n";\
		_Messages << "Expected exception: " << #exception << "\n";\
	}\
	catch( exception ) { /* This is what we want! */ }\
	catch( ... )\
	{\
		Failed = true;\
		_Messages << "\nExpression: {" << #expression << "} doesn't throw \""\
			<< #exception << "\"\n";\
		_Messages << "A different exception was thrown\n"; }
#define WANT_EXCEPT( expression, exception )\
	_EXCEPT( expression, exception ); if( Failed ) NONFATAL_MESSAGE
#define REQUIRE_EXCEPT( expression, exception )\
	_EXCEPT( expression, exception ); if( Failed ) FATAL_MESSAGE

	// Test that something is equal to something else
#define WANT_EQ( expected, actual )\
	EON_AMBIGUOUS_ELSE_BLOCKER\
	if( _testEq( (expected), (actual), #expected, #actual ) );\
		else NONFATAL_MESSAGE
#define REQUIRE_EQ( expected, actual )\
	EON_AMBIGUOUS_ELSE_BLOCKER\
	if( _testEq( (expected), (actual), #expected, #actual ) );\
		else FATAL_MESSAGE

	// Test that something is not equal to something else
#define WANT_NE( expected, actual )\
	if( _testNe( (expected), (actual), #expected, #actual ) );\
		else NONFATAL_MESSAGE
#define REQUIRE_NE( expected, actual )\
	if( _testNe( (expected), (actual), #expected, #actual ) );\
		else FATAL_MESSAGE

	// Test that something is less than something else
#define WANT_LT( expected, actual )\
	if( _testLt( (expected), (actual), #expected, #actual ) );\
		else NONFATAL_MESSAGE
#define REQUIRE_LT( expected, actual )\
	if( _testLt( (expected), (actual), #expected, #actual ) );\
		else FATAL_MESSAGE

	// Test that something is less than or equal to something else
#define WANT_LE( expected, actual )\
	if( _testLe( (expected), (actual), #expected, #actual ) );\
		else NONFATAL_MESSAGE
#define REQUIRE_LE( expected, actual )\
	if( _testLe( (expected), (actual), #expected, #actual ) );\
		else FATAL_MESSAGE

	// Test that something is greater than something else
#define WANT_GT( expected, actual )\
	if( _testGt( (expected), (actual), #expected, #actual ) );\
		else NONFATAL_MESSAGE
#define REQUIRE_GT( expected, actual )\
	if( _testGt( (expected), (actual), #expected, #actual ) );\
		else FATAL_MESSAGE

	// Test that something is greater than or equal to something else
#define WANT_GE( expected, actual )\
	if( _testGe( (expected), (actual), #expected, #actual ) );\
		else NONFATAL_MESSAGE
#define REQUIRE_GE( expected, actual )\
	if( _testGe( (expected), (actual), #expected, #actual ) );\
		else FATAL_MESSAGE


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

		inline Messages& operator <<( const char* str ) {
			if( Info ) *Info += str; return *this; }
		inline Messages& operator <<( const std::string& str ) {
			if( Info ) *Info += str; return *this; }
		template<typename T>
		Messages& operator <<( T value ) {
			if( Info ) *Info += std::to_string( value ); return *this; }
	};


	// Super-class for tests
	class EonTest
	{
	public:
		std::string _TestID;
		_stringstream _Messages;
		bool Failed{ false };

	public:
		EonTest() = default;
		~EonTest()
		{
			std::cout << ( Failed ? "FAIL" : "OK" ) << "\n";
			auto str = _Messages.str();
			if( !str.empty() ) std::cout << "----------" << str
				<< "----------\n";
		}


		void runTest()
		{
			try { prepare(); }
			catch( ... )
			{
				Failed = true;
				_Messages << "Failure in 'prepare' method of test\n";
			}
			if( !Failed )
				test_body();
			try { cleanup(); }
			catch( ... )
			{
				Failed = true;
				_Messages << "Failure in 'cleanup' method of test\n";
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
			inline TestRef( const std::string& test_class,
				const std::string& test_name, FactoryMain* factory ) {
				TestClass = test_class; TestName = test_name;
				Factory = factory; }
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
			Report& operator=( _stringstream ss )
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
		inline void _reportLocation( const char* file, int line ) {
			_Messages << "In " << file << ":" << line << "\n"; }
		std::string encode( const std::string& str, size_t& diffpos )
		{
			auto start = diffpos;
			std::string encoded;
			size_t pos = 0;
			for( auto c : str )
			{
				switch( c )
				{
					case '\\':
						encoded += "\\\\";
						if( pos < start )
							++diffpos;
						break;
					case '\0':
						encoded += "\\0";
						if( pos < start )
							++diffpos;
						break;
					case '\b':
						encoded += "\\b";
						if( pos < start )
							++diffpos;
						break;
					case '\t':
						encoded += "\\t";
						if( pos < start )
							++diffpos;
						break;
					case '\n':
						encoded += "\\n";
						if( pos < start )
							++diffpos;
						break;
					case '\v':
						encoded += "\\v";
						if( pos < start )
							++diffpos;
						break;
					case '\r':
						encoded += "\\r";
						if( pos < start )
							++diffpos;
						break;
					default:
					{
						if( c < ' ' || c > '~' )
						{
							auto enc = "&#" + std::to_string(
								static_cast<int>( c ) ) + ";";
							encoded += enc;
							if( pos < start )
								diffpos += enc.size() - 1;
						}
						else
							encoded += c;
					}
				}
				++pos;
			}
			return encoded;
		}

		bool _testTrue( bool value, const char* expression )
		{
			if( !value )
				_Messages << "\nExpression: {" << expression
					<< "}=false\nExpected true\n";
			return value;
		}
		bool _testFalse( bool value, const char* expression )
		{
			if( value )
				_Messages << "\nExpression: {" << expression
					<< "}=true\nExpected false\n";
			return !value;
		}

		bool _testEq( const std::string& expected, const std::string& actual,
			const char* exp_expr, const char* act_expr )
		{
			if( expected == actual )
				return true;
			Failed = true;
			size_t diff_pos = 0;
			for( ; diff_pos < expected.size() && diff_pos < actual.size()
				&& expected[ diff_pos ] == actual[ diff_pos ]; ++diff_pos )
				;
			size_t start = diff_pos > 13 ? diff_pos - 10 : 0;
			if( start > 0 )
				diff_pos -= start;
			size_t exp_end = expected.size() - start > 40
				? start + 40 : expected.size() - start;
			size_t act_end = actual.size() - start > 40
				? start + 40 : actual.size() - start;
			_Messages << "\nFailed to compare equal (at position "
				<< std::to_string( start + diff_pos ) << ")\n";
			_Messages << "Expected expression: {" << exp_expr << "}\n";
			_Messages << "  Actual expression: {" << act_expr << "}\n";
			_Messages << "Expected value: \"";
			if( start > 0 )
			{
				diff_pos += 3;
				_Messages << "...";
			}
			_Messages << encode( expected.substr( start, exp_end - start ),
				diff_pos );
			if( exp_end < expected.size() )
				_Messages << "...";
			_Messages << "\"\n  Actual value: \"";
			if( start > 0 )
				_Messages << "...";
			size_t dummy{ 50 };
			_Messages << encode( actual.substr( start, act_end - start ),
				dummy );
			if( act_end < actual.size() )
				_Messages << "...";
			_Messages << "\"\n";
			_Messages << std::string( static_cast<size_t>( 17 + diff_pos ),
				' ' ) << "^\n";
			std::string marker{ "Different here" };
			if( diff_pos > marker.size() + 2 )
				_Messages << std::string( static_cast<size_t>( 17 + diff_pos
					- marker.size() - 1 ), ' ' ) << marker << "-'\n";
			else
				_Messages << std::string( static_cast<size_t>( 17 + diff_pos ),
					' ' ) << "'-"  << marker << "\n";
			return false;
		}
		template<typename T1, typename T2>
		bool _testEq( const T1& expected, const T2& actual,
			const char* exp_expr, const char* act_expr )
		{
			if( expected != actual )
			{
				Failed = true;
				_Messages << "\nFailed to compare equal\n";
				_Messages << "Expected expression: {" << exp_expr << "}\n";
				_Messages << "  Actual expression: {" << act_expr << "}\n";
				_Messages << "Expected value: \"" << expected << "\"\n";
				_Messages << "  Actual value: \"" << actual << "\"\n";
				return false;
			}
			else
				return true;
		}

		template<typename T1, typename T2>
		bool _testNe( const T1& expected, const T2& actual,
			const char* exp_expr, const char* act_expr )
		{
			if( expected != actual )
				return true;
			Failed = true;
			_Messages << "\nFailed to compare not equal\n";
			_Messages << "Expected expression: {" << exp_expr << "}\n";
			_Messages << "  Actual expression: {" << act_expr << "}\n";
			size_t dummy{ SIZE_MAX };
			_Messages << "Expected value: \"" << encode( expected, dummy )
				<< "\"\n";
			_Messages << "  Actual value: \"" << encode( actual, dummy )
				<< "\"\n";
			return false;
		}

		template<typename T1, typename T2>
		bool _testLt( const T1& expected, const T2& actual,
			const char* exp_expr, const char* act_expr )
		{
			if( expected < actual )
				return true;
			Failed = true;
			_Messages << "\nFailed to compare less than\n";
			_Messages << "Expected expression: {" << exp_expr << "}\n";
			_Messages << "  Actual expression: {" << act_expr << "}\n";
			size_t dummy{ SIZE_MAX };
			_Messages << "Expected value: \"" << encode( expected, dummy )
				<< "\"\n";
			_Messages << "  Actual value: \"" << encode( actual, dummy )
				<< "\"\n";
			return false;
		}

		template<typename T1, typename T2>
		bool _testLe( const T1& expected, const T2& actual,
			const char* exp_expr, const char* act_expr )
		{
			if( expected <= actual )
				return true;
			Failed = true;
			_Messages << "\nFailed to compare less than or equal to\n";
			_Messages << "Expected expression: {" << exp_expr << "}\n";
			_Messages << "  Actual expression: {" << act_expr << "}\n";
			size_t dummy{ SIZE_MAX };
			_Messages << "Expected value: \"" << encode( expected, dummy )
				<< "\"\n";
			_Messages << "  Actual value: \"" << encode( actual, dummy )
				<< "\"\n";
			return false;
		}

		template<typename T1, typename T2>
		bool _testGt( const T1& expected, const T2& actual,
			const char* exp_expr, const char* act_expr )
		{
			if( expected > actual )
				return true;
			Failed = true;
			_Messages << "\nFailed to compare greater than\n";
			_Messages << "Expected expression: {" << exp_expr << "}\n";
			_Messages << "  Actual expression: {" << act_expr << "}\n";
			size_t dummy{ SIZE_MAX };
			_Messages << "Expected value: \"" << encode( expected, dummy )
				<< "\"\n";
			_Messages << "  Actual value: \"" << encode( actual, dummy )
				<< "\"\n";
			return false;
		}

		template<typename T1, typename T2>
		bool _testGe( const T1& expected, const T2& actual,
			const char* exp_expr, const char* act_expr )
		{
			if( expected >= actual )
				return true;
			Failed = true;
			_Messages << "\nFailed to compare greater than or equal to\n";
			_Messages << "Expected expression: {" << exp_expr << "}\n";
			_Messages << "  Actual expression: {" << act_expr << "}\n";
			size_t dummy{ SIZE_MAX };
			_Messages << "Expected value: \"" << encode( expected, dummy )
				<< "\"\n";
			_Messages << "  Actual value: \"" << encode( actual, dummy )
				<< "\"\n";
			return false;
		}
	};
}
