#include "TestBase.h"
#include "Timer.h"



void usage( const std::filesystem::path& prog )
{
	eon::term << "Usage: " << eon::style::cyan << prog.c_str() << eon::style::normal << " ";
	eon::term << eon::style::weak << "[" << eon::style::yellow << "--class" << eon::style::normal << "="
		<< eon::style::yellow << "<name>" << eon::style::weak << "]" << eon::style::normal;
	eon::term << " " << eon::style::weak << "[" << eon::style::yellow << "--method" << eon::style::normal << "="
		<< eon::style::yellow << "<name>" << eon::style::weak << "]" << eon::style::normal;
	eon::term << " " << eon::style::weak << "[" << eon::style::yellow << "--test" << eon::style::normal << "="
		<< eon::style::yellow << "<name>" << eon::style::weak << "]" << eon::style::normal;
	eon::term << "\n";
	eon::term << eon::style::strong << "Options:" << eon::style::normal << "\n";
	eon::term << "  " << eon::style::yellow << "--class" << eon::style::normal
		<< "  : Name of class to run tests for. Default is to run for all classes!\n";
	eon::term << "  " << eon::style::yellow << "--method" << eon::style::normal
		<< " : Name of method to run tests for. Default is to run for all methods!\n";
	eon::term << "  " << eon::style::yellow << "--test" << eon::style::normal
		<< "   : Name of test to run (for specified class(es) and method(s)).\n";
	eon::term << "             Default is to run all tests!\n";
}

class Args
{
public:
	std::filesystem::path Exe;
	eon::string Class;
	eon::string Method;
	eon::string Test;
	int Result = 0;
};

Args processArgs( int argc, const char* argv[] )
{
	if( argc == 0 )
		exit( 4 );

	Args args;
	args.Exe = argv[ 0 ];
	for( int i = 1; i < argc; ++i )
	{
		eon::string arg( argv[ i ] );
		if( arg == "--help" || arg == "/?" || arg == "-h" || arg == "-?" )
		{
			usage( args.Exe );
			args.Result = -1;
			break;
		}
		else if( arg.startsWith( "--class=" ) )
			args.Class = arg.substr( arg.begin() + 8 );
		else if( arg.startsWith( "--method=" ) )
			args.Method = arg.substr( arg.begin() + 9 );
		else if( arg.startsWith( "--test=" ) )
			args.Test = arg.substr( arg.begin() + 7 );
		else
		{
			eon::term << eon::style::error << "ERROR: Invalid argument!" << eon::style::normal << "\n";
			eon::term << "Argument: " << eon::style::red << arg << eon::style::normal << "\n";
			exit( 4 );
		}
	}

	return args;
}

eonitest::__EonTestPtr createTest( eonitest::__EonTestBase::TestRef& test ) noexcept
{
	try
	{
		return test.Factory->createTest();
	}
	catch( ... )
	{
		return eonitest::__EonTestPtr();
	}
}

eonitest::__Result runTest( eonitest::__EonTestBase::TestRef& test ) noexcept
{
	eonitest::__EonTestPtr test_obj;
	try {
		test_obj = test.Factory->createTest(); }
	catch( ... ) { }
	if( !test_obj )
		return eonitest::__Result::failedToConstruct();

	try
	{
		return test_obj->_runEonTest_();
	}
	catch( eon::exception& e )
	{
		return eonitest::__Result::failureByException( "eon::exception", e.details() );
	}
	catch( std::exception& e )
	{
		eon::string details = typeid( e ).name();
		eon::string what = e.what();
		details += what.trim();
		return eonitest::__Result::failureByException( "std::exception", details );
	}
	catch( ... )
	{
		return eonitest::__Result::failureByException( "exception", "Unknown" );
	}
}

void reportTestStart( const eonitest::__EonTestName& name )
{
	eon::term << "Class=" << eon::style::green << name.className() << eon::style::normal;
	eon::term << " Method=" << eon::style::blue << name.methodName() << eon::style::normal;
	eon::term << " Test=" << eon::style::yellow << name.testName() << eon::style::normal;
	if( name.length() < 50 )
		eon::term << eon::string().padRight( 50 - name.length() );
}

void reportTestOK()
{
	eon::term << eon::style::success << " OK " << eon::style::normal << "\n";
}

void reportTestFail( const ::eonitest::__Result& result, const ::eonitest::__EonLocation& location )
{
	eon::term << eon::style::error << "FAIL" << eon::style::normal << "\n";
	eon::term << "  Location : " << eon::style::yellow << location.file() << eon::style::normal << ":"
		<< eon::style::magenta << location.line() << eon::style::normal << "\n";
	if( result.constructError() )
		eon::term << "  Failed to construct test!\n";
	else if( result.exceptionError() )
	{
		eon::term << "  Got unexpected exception!\n";
		eon::term << "  Type   : " << eon::style::cyan << result.ExceptionType << eon::style::normal << "\n";
		eon::term << "  Details: " << eon::style::code << result.ExceptionDetails << eon::style::normal << "\n";
		if( !result.Details.empty() )
			eon::term << "  " << result.Details << "\n";
	}
	else if( result.noThrowError() )
	{
		eon::term << "  Expected exception but none were thrown!\n";
		eon::term << "  Expected : " << eon::style::cyan << result.ExceptionDetails << eon::style::normal << "\n";
		if( !result.Details.empty() )
			eon::term << "  " << result.Details << "\n";
	}
	else if( result.trewError() )
	{
		eon::term << "  Expected exception not to be thrown but it was!\n";
		eon::term << "  Exception: " << eon::style::cyan << result.ExceptionDetails << eon::style::normal << "\n";
	}
	else if( result.missingOpError() )
		eon::term << "  Missing valid test operation!";
	else
	{
		eon::term << "  Operation: " << eon::style::left << result.ExpectedExpr << eon::style::normal
			<< " " << eon::style::strong << result.OperatorName << eon::style::normal << " "
			<< eon::style::right << result.ActualExpr << eon::style::normal << "\n";
		eon::term << "  " << result.ExpectedExpr << " = \""
			<< eon::style::left_value << result.ExpectedVal << eon::style::normal << "\"\n";
		eon::term << "  " << result.ActualExpr << " = \""
			<< eon::style::right_value << result.ActualVal << eon::style::normal << "\"\n";
		if( !result.Details.empty() )
			eon::term << "  " << result.Details << "\n";
	}
}

size_t runTests( eon::string classname, eon::string methodname, eon::string testname, std::list<eonitest::__EonTestName>& failed )
{
	if( !eonitest::__EonTestBase::TestsList || eonitest::__EonTestBase::TestsList->empty() )
		return 0;

	size_t total = 0;
	for( auto& test : *eonitest::__EonTestBase::TestsList )
	{
		if( !classname.empty() && classname.iCompare( test.Name.className() ) != 0 )
			continue;
		if( !methodname.empty() && methodname.iCompare( test.Name.methodName() ) != 0 )
			continue;
		if( !testname.empty() && testname.iCompare( test.Name.testName() ) != 0 )
			continue;

		++total;
		reportTestStart( test.Name );
		eon::string error;
		eonitest::Timer timer;
		auto result = runTest( test );
		eon::term << " " << eon::style::magenta << timer.elapsed() << eon::style::normal << " ";
		if( result )
			reportTestOK();
		else
		{
			reportTestFail( result, test.Location );
			failed.push_back( test.Name );
		}
	}
	eon::term << "\n";
	return total;
}


int main( int argc, const char* argv[] )
{
	auto args = processArgs( argc, argv );
	if( args.Result != 0 )
		return args.Result < 0 ? 0 : args.Result;

	std::list<eonitest::__EonTestName> failed;
	eonitest::Timer timer;
	auto total = runTests( args.Class, args.Method, args.Test, failed );
	auto duration = timer.elapsed();
	if( total > 0 )
		eon::term << "Running all tests took: " << eon::style::magenta << duration << eon::style::normal << "\n";

	if( total == 0 )
	{
		eon::term << eon::style::error << " No tests have been defined! " << eon::style::normal << "\n";
		return 4;
	}
	else if( failed.empty() )
	{
		eon::term << eon::style::success << " All tests passed! " << eon::style::normal << "\n";
		return 0;
	}
	else
	{
		eon::term << "\n" << eon::style::error << "The following tests failed:" << eon::style::normal << "\n";
		for( auto& name : failed )
			eon::term << eon::style::red << "  - " << eon::style::normal
			<< "Class=" << eon::style::cyan << name.className() << eon::style::normal
			<< " Method=" << eon::style::cyan << name.methodName() << eon::style::normal
			<< " Test=" << eon::style::cyan << name.testName() << eon::style::normal
			<< "\n";
		eon::term << "\n";
		return 4;
	}
}
