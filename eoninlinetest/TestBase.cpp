#include "TestBase.h"


namespace eonitest
{
	std::unique_ptr<std::list<TestBase::TestRef>> TestBase::TestsList;
	std::unique_ptr<std::set<TestName>> TestBase::TestNames;




	bool TestBase::registerTest( TestName name, TestLocation location, FactoryMain* test )
	{
		if( !TestNames )
		{
			TestsList = std::make_unique<std::list<TestBase::TestRef>>();
			TestNames = std::make_unique<std::set<TestName>>();
		}
		if( TestNames->find( name ) != TestNames->end() )
			return false;
		TestNames->insert( name );
		TestsList->push_back( TestRef( name, location, test ) );
		return true;
	}




	void EonTestSandbox::_prepareSandboxDir() noexcept
	{
		SandboxDir = std::filesystem::temp_directory_path() / L"eon_sandbox"
			/ Details.Name.ClsName.stdwstr() / Details.Name.FuncName.stdwstr() / Details.Name.TstName.stdwstr();
		SandboxDirStr = SandboxDir.wstring();
		if( !_ensureNewDir( SandboxDir ) )
			SandboxDir.clear();
	}

	bool EonTestSandbox::_ensureNewDir( std::filesystem::path& path ) noexcept
	{
		if( std::filesystem::exists( path ) )
		{
			Details.Log << "Deleting existing sandbox directory " << eonitest::style::item_1 << path.string() << '\n';
			if( !_removeDir( path ) )
			{
				if( _enum( path ) )
				{
					Details.Log << eonitest::style::warning << "WARNING!"
						<< " Failed to clear out existing sandbox directory!" << '\n';
					Details.Log << "  " << eonitest::style::key << "Directory"
						<< eonitest::style::value << path.string() << '\n';
					Details.Log << "Trying with alternate directory!" << '\n';
					Details.Log << "  " << eonitest::style::key << "Directory"
						<< eonitest::style::value << path.string() << '\n';
					return _ensureNewDir( path );
				}
				else
				{
					Details.Log << eonitest::style::error << "FATAL ERROR!"
						<< " Failed to clear out existing sandbox directory!" << '\n';
					Details.Log << "  " << eonitest::style::key << "Directory: "
						<< eonitest::style::value << path.string() << '\n';
					return false;
				}
			}
		}

		if( !_makeDir( path ) )
		{
			Details.Log << eonitest::style::error << "FATAL ERROR!"
				<< "Failed to create sandbox directory!" << '\n';
			Details.Log << "  " << eonitest::style::key << "Directory: "
				<< eonitest::style::value << path.string() << '\n';
			return false;
		}

		return true;
	}

	bool EonTestSandbox::_enum( std::filesystem::path& path )
	{
		eon::string ext{ path.extension().u8string() };
		if( ext.startsWith( '.' ) )
			ext.erase( ext.substr( 0, 1 ) );
		if( ext.findFirstNotOf( "0123456789" ) )
		{
			path += ".1";
			return true;
		}
		else
		{
			auto num = ext.toUInt32();
			if( num == 9 )
				return false;
			path += "." + eon::string( num + 1 ).stdstr();
			return true;
		}
	}

	bool EonTestSandbox::_removeDir( const std::filesystem::path& path ) noexcept
	{
		std::error_code error;
		return std::filesystem::remove_all( path, error ) && !std::filesystem::exists( path );
	}

	bool EonTestSandbox::_makeDir( const std::filesystem::path& path ) noexcept
	{
		std::error_code error;
		return std::filesystem::create_directories( path, error ) && std::filesystem::exists( path );
	}






	//std::unique_ptr<std::list<__EonTestBase::TestRef>> __EonTestBase::TestsList;
	//std::unique_ptr<std::set<__EonTestName>> __EonTestBase::TestNames;


	//bool __EonTestBase::_registerEonTest_( __EonTestName name, __EonLocation location, FactoryMain* test )
	//{
	//	if( !TestNames )
	//	{
	//		TestsList = std::make_unique<std::list<__EonTestBase::TestRef>>();
	//		TestNames = std::make_unique<std::set<__EonTestName>>();
	//	}
	//	if( TestNames->find( name ) != TestNames->end() )
	//		return false;
	//	TestNames->insert( name );
	//	TestsList->push_back( TestRef( name, location, test ) );
	//	return true;
	//}


	//__Result __EonTestBase::_runCompare(
	//	const __Value& expected,
	//	const eon::string& cmp_operator,
	//	const __Value& actual,
	//	eon::string expected_expr,
	//	eon::string actual_expr ) const
	//{
	//	bool cmp_result{ false };
	//	if( cmp_operator == "<" )
	//		cmp_result = expected < actual;
	//	else if( cmp_operator == "<=" )
	//		cmp_result = expected <= actual;
	//	else if( cmp_operator == ">" )
	//		cmp_result = expected > actual;
	//	else if( cmp_operator == ">=" )
	//		cmp_result = expected >= actual;
	//	else if( cmp_operator == "==" )
	//		cmp_result = expected == actual;
	//	else if( cmp_operator == "!=" )
	//		cmp_result = expected != actual;
	//	return cmp_result ? __Result::success()
	//		: __Result::failCompare( cmp_operator, expected_expr, actual_expr, expected.Value, actual.Value );
	//}




	//void __EonTestSandbox::_prepareSandboxDir() noexcept
	//{
	//	SandboxDir = std::filesystem::temp_directory_path() / L"eon_sandbox"
	//		/ Name.className().stdwstr() / Name.methodName().stdwstr() / Name.testName().stdwstr();
	//	SandboxDirStr = SandboxDir.wstring();
	//	if( !_ensureNewDir( SandboxDir ) )
	//		SandboxDir.clear();
	//}

	//bool __EonTestSandbox::_ensureNewDir( std::filesystem::path& path ) noexcept
	//{
	//	// If the directory exists, try to delete it.
	//	if( std::filesystem::exists( path ) )
	//	{
	//		// If that doesn't work, use enumeration on the directory name and try again.
	//		if( !_removeDir( path ) )
	//		{
	//			eon::term << eon::style::warning << "WARNING! Failed to clear out existing sandbox directory!"
	//				<< eon::style::normal << "\n";
	//			eon::term << "  Directory: " << eon::style::yellow << path << eon::style::normal << "\n";
	//			if( _enum( path ) )
	//			{
	//				eon::term << "Trying again with enumeration.\n";
	//				eon::term << "  Directory: " << eon::style::yellow << path << eon::style::normal << "\n";
	//				return _ensureNewDir( path );
	//			}
	//			else
	//			{
	//				eon::term << eon::style::error << "FATAL ERROR! Unable to create sandbox directory!"
	//					<< eon::style::normal << "\n";
	//				eon::term << "  Issue    : " << eon::style::red << "Unable to clear out old existing directory!"
	//					<< eon::style::normal << "\n";
	//				eon::term << "  Directory: " << eon::style::yellow << path << eon::style::normal << "\n";
	//				return false;
	//			}
	//		}
	//	}

	//	// The directory does not exist. Now we can create a new, clean one.
	//	if( !_makeDir( path ) )
	//	{
	//		eon::term << eon::style::error << "FATAL ERROR! Unable to create sandbox directory!"
	//			<< eon::style::normal << "\n";
	//		eon::term << "  Directory: " << eon::style::yellow << path << eon::style::normal << "\n";
	//		return false;
	//	}

	//	return true;
	//}

	//bool __EonTestSandbox::_enum( std::filesystem::path& path )
	//{
	//	eon::string ext{ path.extension().u8string() };
	//	if( ext.startsWith( '.' ) )
	//		ext.erase( ext.substr( 0, 1 ) );
	//	if( ext.findFirstNotOf( "0123456789" ) )
	//	{
	//		path += ".1";
	//		return true;
	//	}
	//	else
	//	{
	//		auto num = ext.toUInt32();
	//		if( num == 9 )
	//			return false;
	//		path += "." + eon::string( num + 1 );
	//		return true;
	//	}
	//}

	//bool __EonTestSandbox::_removeDir( const std::filesystem::path& path ) noexcept
	//{
	//	std::error_code error;
	//	return std::filesystem::remove_all( path, error ) && !std::filesystem::exists( path );
	//}

	//bool __EonTestSandbox::_makeDir( const std::filesystem::path& path ) noexcept
	//{
	//	std::error_code error;
	//	return std::filesystem::create_directories( path, error ) && std::filesystem::exists( path );
	//}
}
