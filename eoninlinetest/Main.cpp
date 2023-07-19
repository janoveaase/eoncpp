#include "TestBase.h"
#include "Timer.h"
#include <eonstring/GlobPattern.h>



void usage( const std::filesystem::path& prog )
{
	auto cwd = std::filesystem::current_path();
	auto c = cwd.begin(), p = prog.begin();
	for( ; c != cwd.end() && p != prog.end() && *c == *p; ++c, ++p )
		;
	std::filesystem::path exe;
	for( ; p != prog.end(); ++p )
		exe /= *p;
	eon::term << "Usage: " << eon::style::cyan << exe.c_str() << eon::style::normal << " ";

	eon::term << eon::style::weak << "[" << eon::style::yellow << "-c" << eon::style::normal << "="
		<< eon::style::yellow << "<class name>" << eon::style::weak << "]" << eon::style::normal;
	eon::term << eon::style::weak << "[" << eon::style::yellow << "-f" << eon::style::normal << "="
		<< eon::style::yellow << "<function name>" << eon::style::weak << "]" << eon::style::normal;
	eon::term << eon::style::weak << "[" << eon::style::yellow << "-t" << eon::style::normal << "="
		<< eon::style::yellow << "<test name>" << eon::style::weak << "]" << eon::style::normal;

	eon::term << "\n";
	eon::term << eon::style::strong << "Options:" << eon::style::normal << "\n";
	eon::term << "  " << eon::style::yellow << "-c" << eon::style::normal
		<< " : Name of class to run tests for. Default is " << eon::style::weak << "\""
		<< eon::style::yellow << "*" << eon::style::weak << "\"" << eon::style::normal << "!\n";
	eon::term << "  " << eon::style::yellow << "-f" << eon::style::normal
		<< " : Name of function/method to run tests for. Default is " << eon::style::weak << "\""
		<< eon::style::yellow << "*" << eon::style::weak << "\"" << eon::style::normal << "!\n";
	eon::term << "  " << eon::style::yellow << "-t" << eon::style::normal
		<< " : Name of test to run (for specified class(es) and function(s)). Default is " << eon::style::weak << "\""
		<< eon::style::yellow << "*" << eon::style::weak << "\"" << eon::style::normal << "!\n";

	eon::term << "All names can contain wildcards '" << eon::style::code << "*" << eon::style::normal << "' and '"
		<< eon::style::code << "?" << eon::style::normal << "'! (Match zero or more arbitrary ";
	eon::term << "characters and match exactly one arbitrary character respectively.)\n";
	eon::term << eon::style::note << "NOTE:" << eon::style::normal << " Will ignore case when matching names!\n";
}

class Args
{
public:
	std::filesystem::path Exe;
	eon::string Cls, Func, Tst;

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

		else if( arg.startsWith( "-c=" ) )
			args.Cls = arg.substr( arg.begin() + 3 );
		else if( arg.startsWith( "-f=" ) )
			args.Func = arg.substr( arg.begin() + 3 );
		else if( arg.startsWith( "-t=" ) )
			args.Tst = arg.substr( arg.begin() + 3 );

		else
		{
			eon::term << eon::style::error << "ERROR: Invalid argument!" << eon::style::normal << "\n";
			eon::term << "Argument: " << eon::style::red << arg << eon::style::normal << "\n";
			exit( 4 );
		}
	}

	return args;
}

eonitest::TestPtr create( eonitest::TestBase::TestRef& test ) noexcept
{
	try
	{
		return test.Factory->create();
	}
	catch( ... )
	{
		return eonitest::TestPtr();
	}
}


eonitest::TestPtr run( eonitest::TestBase::TestRef& test ) noexcept
{
	eonitest::TestPtr test_obj;
	try
	{
		test_obj = test.Factory->create();
	}
	catch( ... ) {}
	if( !test_obj )
		return test_obj;

	try
	{
		eon::locale base_eon_locale = eon::locale::get();
		std::locale base_std_locale;
		bool success = test_obj->runTest();
		std::locale::global( base_std_locale );
		eon::locale::set( base_eon_locale );
	}
	catch( eon::exception& e )
	{
		test_obj->Details.Success = false;
		test_obj->Details.Log << eonitest::style::error << "ERROR!" << " Got an eon::exception!" << '\n';
		test_obj->Details.Log << "  " << eonitest::style::key << "Name"
			<< eonitest::style::value << eon::str( e.name() ) << '\n';
		test_obj->Details.Log << "  " << eonitest::style::key << "Info" << eonitest::style::value << e.info() << '\n';
	}
	catch( std::exception& e )
	{
		test_obj->Details.Success = false;
		test_obj->Details.Log << eonitest::style::error << "ERROR!" << " Got an std::exception!" << '\n';
		eon::string type = typeid( e ).name();
		eon::string what = eon::string( e.what() ).trim();
		test_obj->Details.Log << "  " << eonitest::style::key << "Type"
			<< eonitest::style::value << type << '\n';
		if( !what.empty() )
			test_obj->Details.Log << "  " << eonitest::style::key << "What" << eonitest::style::value << what << '\n';
	}
	catch( ... )
	{
		test_obj->Details.Success = false;
		test_obj->Details.Log << eonitest::style::error << "ERROR!" << " Got an unknown exception!" << '\n';
	}
	return test_obj;
}


void reportSpecifications( eon::string cls_name, eon::string func_name, eon::string tst_name )
{
	if( cls_name == "*" )
		cls_name = "";
	if( func_name == "*" )
		func_name = "";
	if( tst_name == "*" )
		tst_name = "";
	if( cls_name.empty() && func_name.empty() && tst_name.empty() )
		return;
	eon::term << "Running only tests matching";
	if( !cls_name.empty() )
		eon::term << eon::style::weak << " class=" << eon::style::strong << cls_name;
	if( !func_name.empty() )
		eon::term << eon::style::weak << " function=" << eon::style::strong << func_name;
	if( !tst_name.empty() )
		eon::term << eon::style::weak << " test=" << eon::style::strong << tst_name;
	eon::term << eon::style::normal << "!\n\n";
}

void reportTestStart( const eonitest::TestName& name )
{
	eon::term << eon::style::weak << "Class=" << eon::style::strong << name.ClsName << eon::style::normal;
	eon::term << eon::style::weak << " Func=" << eon::style::strong << name.FuncName << eon::style::normal;
	eon::term << eon::style::weak << " Test=" << eon::style::strong << name.TstName << eon::style::normal;
	if( name.length() < 50 )
		eon::term << eon::string().padRight( 50 - name.length() );
}

void reportTestOK()
{
	eon::term << eon::style::success << " OK " << eon::style::normal << "\n";
}

void reportTestFail( const eonitest::TestDetails& details, const eonitest::TestLocation& location )
{
	eon::term << eon::style::error << "FAIL" << eon::style::normal << "\n";
	eon::term << "  " << eon::style::strong << "Location  : " << eon::style::yellow << location.File << eon::style::normal
		<< ":" << eon::style::magenta << location.NumLine << eon::style::normal << "\n";
	for( auto& line : details.Log.Lines )
	{
		for( auto& item : line.Items )
			eon::term << item.Style << item.Item;
		eon::term << eon::style::normal << "\n";
	}
}

size_t runTests( eon::string cls_name, eon::string func_name, eon::string tst_name, std::list<eonitest::TestName>& failed )
{
	if( !eonitest::TestBase::TestsList || eonitest::TestBase::TestsList->empty() )
		return 0;

	reportSpecifications( cls_name, func_name, tst_name );
	eon::globpattern cls( cls_name.empty() ? "*" : cls_name );
	cls.ignoreCase();
	eon::globpattern func( func_name.empty() ? "*" : func_name );
	func.ignoreCase();
	eon::globpattern tst( tst_name.empty() ? "*" : tst_name );
	tst.ignoreCase();
	size_t total = 0;
	for( auto& test : *eonitest::TestBase::TestsList )
	{
		if( !cls.match( test.Name.ClsName ) )
			continue;
		if( !func.match( test.Name.FuncName ) )
			continue;
		if( !tst.match( test.Name.TstName ) )
			continue;

		++total;
		reportTestStart( test.Name );
		eon::string error;
		eonitest::Timer timer;
		auto test_ptr = run( test );
		if( !test_ptr )
			continue;
		eon::term << " " << eon::style::magenta << timer.elapsed() << eon::style::normal << " ";
		if( test_ptr->Details.Success )
			reportTestOK();
		else
		{
			reportTestFail( test_ptr->Details, test.Location );
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

	std::list<eonitest::TestName> failed;
	eonitest::Timer timer;
	auto total = runTests( args.Cls, args.Func, args.Tst, failed );
	auto duration = timer.elapsed();
	if( total > 0 )
		eon::term << "Running " << eon::style::magenta << total << eon::style::normal << " test"
		<< ( total > 1 ? "s" : "" ) << " took: " << eon::style::magenta << duration << eon::style::normal << "\n";

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
			<< eon::style::weak << "Class=" << eon::style::strong << name.ClsName << eon::style::normal
			<< eon::style::weak << " Function=" << eon::style::strong << name.FuncName << eon::style::normal
			<< eon::style::weak << " Test=" << eon::style::strong << name.TstName << eon::style::normal
			<< "\n";
		eon::term << "\n";
		return 4;
	}
}
