#include "TestBase.h"
#include <fstream>


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




	bool EonTestSandbox::exists( eon::string name ) const noexcept {
		if( name.contains( ".." ) )
			return false;
		return std::filesystem::exists( sandboxDir() / name.stdwstr() ); }

	bool EonTestSandbox::makeDir( eon::string name ) const
	{
		if( name.contains( ".." ) )
			return false;
		std::filesystem::path full_path = sandboxDir() / name.stdwstr();
		return _makeDir( full_path );
	}

	void EonTestSandbox::saveFile( eon::string name, eon::string contents ) const
	{
		if( name.contains( ".." ) )
			return;
		std::filesystem::path path = sandboxDir() / name.stdwstr();
		std::ofstream file;
		file.open( path, std::ios_base::out | std::ios_base::binary );
		file.write( contents.c_str(), contents.numBytes() );
		file.close();
	}

	eon::string EonTestSandbox::loadFile( eon::string name ) const
	{
		if( name.contains( ".." ) )
			return "";
		std::filesystem::path path = sandboxDir() / name.stdwstr();
		std::ifstream file;
		file.open( path, std::ios_base::in | std::ios_base::binary );
		std::string raw;
		char buffer[ 1024 ]{ 0 };
		while( file )
		{
			file.read( buffer, 1024 );
			raw += buffer;
		}
		file.close();
		return eon::string( std::move( raw ) );
	}

	bool EonTestSandbox::deleteFile( eon::string name ) const
	{
		if( name.contains( ".." ) )
			return false;
		std::filesystem::path path = sandboxDir() / name.stdwstr();
		return std::filesystem::remove( path );
	}

	bool EonTestSandbox::deleteDir( eon::string name ) const
	{
		if( name.contains( ".." ) )
			return false;
		std::filesystem::path path = sandboxDir() / name.stdwstr();
		return _removeDir( path );
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

	bool EonTestSandbox::_enum( std::filesystem::path& path ) const
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

	bool EonTestSandbox::_removeDir( const std::filesystem::path& path ) const noexcept
	{
		std::error_code error;
		return std::filesystem::remove_all( path, error ) && !std::filesystem::exists( path );
	}

	bool EonTestSandbox::_makeDir( const std::filesystem::path& path ) const noexcept
	{
		std::error_code error;
		return std::filesystem::create_directories( path, error ) && std::filesystem::exists( path );
	}
}
