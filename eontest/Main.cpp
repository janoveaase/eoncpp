#include <iostream>
#include "Test.h"



void usage( const std::string& prog )
{
	std::cout << "Usage: " << prog.c_str() << " [--eonfilter=<regex>]\n";
	std::cout << "If run with filter, "
		"only tests matching that filter regex pattern will be run.\n";
}

class Args
{
public:
	std::string Filter;
	int Result = 0;
};
Args processArgs( int argc, const char* argv[] )
{
	if( argc == 0 )
		exit( 4 );

	std::string exe( argv[ 0 ] );

	Args args;
	for( int i = 1; i < argc; ++i )
	{
		std::string arg( argv[ i ] );
		if( arg == "--help" )
		{
			usage( exe );
			args.Result = -1;
			break;
		}
		else if( arg.compare( 0, 12, "--eonfilter=" ) == 0 )
			args.Filter = arg.substr( 12 );
	}

	return args;
}

bool runTest( eontest::EonTest::TestRef& test )
{
	auto test_obj = test.Factory->createTest( test.TestClass, test.TestName );
	try
	{
		test_obj->test_body();
	}
	catch( std::string& )
	{
		// Test was aborted
	}
	auto failed = test_obj->Failed;
	delete test_obj;
	return !failed;
}

void runTests( const std::string& filter, std::list<std::string>& failed )
{
	if( !eontest::EonTest::Tests )
		throw TestError( "No tests have been defined!" );

	std::regex pattern( filter );
	for( auto& test : *eontest::EonTest::Tests )
	{
		if( !filter.empty() && !std::regex_match(
			test.TestClass + "." + test.TestName, pattern ) )
			continue;
		std::cout << "Running " << test.TestClass << "." << test.TestName
			<< ": ";
		try
		{
			if( !runTest( test ) )
				failed.push_back( test.TestClass + "." + test.TestName );
		}
		catch( std::exception& e )
		{
			std::cerr << "FAIL\nERROR: std::exception:\n";
			std::cerr << e.what() << "\n";
		}
		catch( ... )
		{
			std::cerr << "FAIL\nERROR: Unknown error\n";
		}
	}
}


int main( int argc, const char* argv[] )
{
	auto args = processArgs( argc, argv );
	if( args.Result != 0 )
		return args.Result < 0 ? 0 : args.Result;

	std::list<std::string> failed;
	runTests( args.Filter, failed );

	eontest::EonTest::reset();

	if( !failed.empty() )
	{
		std::cerr << "\nThe following tests failed:\n";
		for( auto& name : failed )
			std::cerr << "  - " << name << "\n";
		std::cerr << "\n";
		return 4;
	}

	return 0;
}
