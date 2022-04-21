#include <iostream>
#include "Test.h"



void usage( const std::string& prog )
{
	eon::term << "Usage: " << prog.c_str() << " [--eonfilter=<regex>]\n";
	eon::term << "If run with filter, only tests matching that filter regex pattern will be run.\n";
}

class Args
{
public:
	std::string Exe;
	std::string Filter;
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
		std::string arg( argv[ i ] );
		if( arg == "--help" )
		{
			usage( args.Exe );
			args.Result = -1;
			break;
		}
		else if( arg.compare( 0, 12, "--eonfilter=" ) == 0 )
			args.Filter = arg.substr( 12 );
	}

	return args;
}

bool runTest( const std::string& exe, eontest::EonTest::TestRef& test )
{
	eontest::_EonTest* test_obj{ nullptr };
	try
	{
		test_obj = test.Factory->createTest( test.TestClass, test.TestName );
	}
	catch( ... )
	{
		return false;
	}
	if( !test_obj->Failed )
		test_obj->_runEonTest_( exe );
	auto failed = test_obj->Failed;
	delete test_obj;
	return !failed;
}

eon::string duration( std::chrono::high_resolution_clock::time_point start,
 std::chrono::high_resolution_clock::time_point end )
{
	auto duration = end - start;
	auto dur = std::chrono::duration_cast<std::chrono::milliseconds>( duration ).count();
	std::vector<eon::string> elms;
	if( dur / 3600000 != 0 )
		elms.push_back( eon::string( dur / 3600000 ) + "h" );
	if( ( dur % 3600000 ) / 60000 != 0 )
		elms.push_back( eon::string( ( dur % 3600000 ) / 60000 ) + "m" );
	elms.push_back( eon::string( ( dur % 60000 ) / 1000 ) + "." + eon::string( dur % 1000 ).padLeft( 3, '0' ) + "s" );
	return eon::string( " " ).join( elms );
}
size_t runTests( const std::string& exe, const std::string& filter, std::list<std::string>& failed )
{
	using namespace eontest;
	if( !EonTest::_EonTests_ )
		return 0 ;

	std::regex pattern( filter );
	auto start = std::chrono::high_resolution_clock::now();
	size_t total = 0;
	for( auto& test : *EonTest::_EonTests_ )
	{
		if( !filter.empty() && !std::regex_match( test.TestClass + "." + test.TestName, pattern ) )
			continue;
		++total;
		eon::string test_name = test.TestClass + "." + test.TestName;
		size_t col_w = eon::term.width() < 70 ? eon::term.width() : 70;
		if( col_w == 0 )
			col_w = 50;
		eon::term << eon::style::note << "-- Start " << test_name << " "
			<< eon::string().padRight( col_w - test_name.numChars() + 9, '-' ) << eon::style::normal << "\n";
		auto st = std::chrono::high_resolution_clock::now();
		try
		{
			auto success = runTest( exe, test );
			auto en = std::chrono::high_resolution_clock::now();
			if( success )
			{
				eon::term << eon::style::reference << "   " + test_name + "   " << eon::style::blue
					<< duration( st, en ).padLeft( col_w - test_name.numChars() + 6 ) << " " << eon::style::success
					<< " PASS " << eon::style::normal << "\n\n";
			}
			else
			{
				eon::term << eon::style::reference << "   " + test_name + "   " << eon::style::blue
					<< duration( st, en ).padLeft( col_w - test_name.numChars() + 6 ) << " " << eon::style::error
					<< " FAIL " << eon::style::normal << "\n\n";
				failed.push_back( test.TestClass + "." + test.TestName );
			}
		}
		catch( std::exception& e )
		{
			auto en = std::chrono::high_resolution_clock::now();
			eon::term << "ERROR: std::exception:\n";
			eon::term << e.what() << "\n";
			eon::term << eon::style::reference << "   " + test_name + "   " << eon::style::blue
				<< duration( st, en ).padLeft( col_w - test_name.numChars() + 6 ) << " " << eon::style::error << " FAIL "
				<< eon::style::normal << "\n\n";
			failed.push_back( test.TestClass + "." + test.TestName );
		}
		catch( ... )
		{
			auto en = std::chrono::high_resolution_clock::now();
			eon::term << "ERROR: Unknown error\n";
			eon::term << eon::style::reference << "   " + test_name + "   " << eon::style::blue
				<< duration( st, en ).padLeft( col_w - test_name.numChars() + 6 ) << " " << eon::style::error << " FAIL "
				<< eon::style::normal << "\n\n";
			failed.push_back( test.TestClass + "." + test.TestName );
		}
	}
	eon::term << "\n";
	auto end = std::chrono::high_resolution_clock::now();
	eon::term << "Total run time: " << eon::style::blue << duration( start, end ) << eon::style::normal << "\n\n";
	return total;
}


int main( int argc, const char* argv[] )
{
	auto args = processArgs( argc, argv );
	if( args.Result != 0 )
		return args.Result < 0 ? 0 : args.Result;

	std::list<std::string> failed;
	auto total = runTests( args.Exe, args.Filter, failed );

	eontest::EonTest::_resetEon_();

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
			eon::term << "  - " << eon::style::red << name << eon::style::normal << "\n";
		eon::term << "\n";
		return 4;
	}
}
