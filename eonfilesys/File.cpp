#include "File.h"
#include <filesystem>


namespace eon
{
	file& file::operator=( eon::path&& file_path )
	{
		if( !file_path.file() )
			throw filesys::BadPath( "Cannot assign a non-file path to a file object" );
		Path = std::move( file_path );
		return *this;
	}
	EON_XTEST_2STEP( file, operator_assign, dir_path,
		filesys::BadPath, path dpath( "one/two/" ), file f = std::move( dpath ) );
	EON_NOXTEST_2STEP( file, operator_assign, file_path, path fpath( "one/two" ), file f = std::move( fpath ) );




	void file::touch() const
	{
		if( !exists() )
			save( std::string() );
		else
			std::filesystem::last_write_time( Path.stdpath(),
				std::filesystem::file_time_type::clock::now() );
	}
	EON_CMPTEST_SANDBOX_3STEP( file, touch, basic,
		file a( sandboxDirStr() + "/dummy.tmp" ), a.touch(), true, EON_EQ, a.exists() );

	file& file::rename( const string& new_name )
	{
		if( !Path )
			throw filesys::Failure( "Trying to rename unnamed file" );
		if( !exists() )
			throw filesys::Failure( "Trying to rename nonexisting file \"" + Path.str() + "\"" );
		file target{ Path.hasParent() ? Path.parent() / eon::path( new_name ) : eon::path( new_name ) };
		if( target.exists() )
			throw filesys::Failure( "Cannot rename file \"" + Path.str() + "\" to target \"" + new_name
				+ "\": Target exists" );
		try
		{
			std::filesystem::rename( Path.stdpath(), target.fpath().stdpath() );
		}
		catch( std::exception& e )
		{
			throw filesys::Failure( e.what() );
		}
		return *this = std::move( target );
	}
	EON_XTEST_SANDBOX_2STEP( file, rename, unnamed, filesys::Failure, file f, f.rename( "new" ) );
	EON_XTEST_SANDBOX_2STEP( file, rename, no_source, filesys::Failure,
		file f( sandboxDirStr() + "/old" ), f.rename( "new" ) );
	EON_XTEST_SANDBOX_3STEP( file, rename, existing_target, filesys::Failure,
		file f( sandboxDirStr() + "/old" ), file( sandboxDirStr() + "/new" ).touch(), f.rename( "new" ) );
	EON_NOXTEST_SANDBOX_3STEP( file, rename, success,
		file f( sandboxDirStr() + "/old" ), f.touch(), f.rename( "new" ) );

	file& file::forceRename( const string& new_name )
	{
		if( !Path )
			throw filesys::Failure( "Trying to rename unnamed file" );
		if( !exists() )
			throw filesys::Failure( "Trying to rename nonexisting file \"" + Path.str() + "\"" );
		file target{ Path.hasParent() ? Path.parent() / eon::path( new_name ) : eon::path( new_name ) };
		try
		{
			std::filesystem::rename( Path.stdpath(), target.Path.stdpath() );
		}
		catch( std::exception& e )
		{
			throw filesys::Failure( e.what() );
		}
		return *this = std::move( target );
	}

	file& file::move( const eon::path& target_dir )
	{
		if( !Path )
			throw filesys::Failure( "Trying to move unnamed file" );
		if( !exists() )
			throw filesys::Failure( "Trying to move nonexisting file \"" + Path.str() + "\"" );
		file target{ target_dir / Path.base() };
		if( target.exists() )
			throw filesys::Failure( "Cannot move \"" + Path.str() + "\" to target \"" + target.fpath().str()
				+ "\": Target exists" );
		try
		{
			std::filesystem::rename( Path.stdpath(), target.fpath().stdpath() );
		}
		catch( std::exception& e )
		{
			throw filesys::Failure( e.what() );
		}
		return *this = std::move( target );
	}
	file& file::forceMove( const eon::path& target_dir )
	{
		if( !Path )
			throw filesys::Failure( "Trying to move unnamed file" );
		if( !exists() )
			throw filesys::Failure( "Trying to move nonexisting file \"" + Path.str() + "\"" );
		file target{ target_dir / Path.base() };
		try
		{
			std::filesystem::rename( Path.stdpath(), target.fpath().stdpath() );
		}
		catch( std::exception& e )
		{
			throw filesys::Failure( e.what() );
		}
		return *this = std::move( target );
	}

	void file::copy( const file& target ) const
	{
		if( !Path )
			throw filesys::Failure( "Trying to copy unnamed file" );
		if( !exists() )
			throw filesys::Failure( "Trying to copy nonexisting file \"" + Path.str() + "\"" );
		if( target.exists() )
			throw filesys::Failure( "Cannot copy \"" + Path.str() + "\" to target \"" + target.fpath().str()
				+ "\": Target exists" );
		try
		{
			std::filesystem::copy( Path.stdpath(), target.fpath().stdpath(), std::filesystem::copy_options::skip_existing );
		}
		catch( std::exception& e )
		{
			throw filesys::Failure( e.what() );
		}
	}
	void file::forceCopy( const file& target ) const
	{
		if( !Path )
			throw filesys::Failure( "Trying to copy unnamed file" );
		if( !exists() )
			throw filesys::Failure( "Trying to copy nonexisting file \"" + Path.str() + "\"" );
		try
		{
			std::filesystem::copy( Path.stdpath(), target.fpath().stdpath(),
				std::filesystem::copy_options::overwrite_existing );
		}
		catch( std::exception& e )
		{
			throw filesys::Failure( e.what() );
		}
	}

	string file::loadText( std::uintmax_t max_chars ) const
	{
		filebuffer buffer( Path, filesys::mode::input );
		buffer.open();
		string str;
		char_t chr;
		for( size_t i = 0; i < max_chars && buffer.read( chr ); )
		{
			if( chr == CReturnChr )
				continue;
			str += chr;
			++i;
		}
		return str;
	}
	std::string file::loadBinary( std::uintmax_t max_bytes ) const
	{
		filebuffer buffer( Path, filesys::mode::input );
		buffer.open();
		std::string str;
		byte_t byte;
		for( size_t i = 0; i < max_bytes && buffer.read( byte ); ++i )
			str += byte;
		return str;
	}

	file::iterator file::begin() const noexcept { return iterator( *this ); }
	file::iterator file::end() const noexcept { return iterator(); }

	void file::save( const string& text ) const
	{
		filebuffer buffer( Path, filesys::mode::output );
		buffer.open();
		char_t prev = '\0';
		for( auto c : text )
		{
#ifdef EON_WINDOWS
			if( c == NewlineChr && prev != CReturnChr )
				buffer.write( CReturnChr );
#endif
			buffer.write( c );
			prev = c;
		}
		buffer.close();
	}
	void file::save( const std::string& bytes ) const
	{
		filebuffer buffer( Path, filesys::mode::output );
		buffer.open();
		for( auto byte : bytes )
			buffer.write( static_cast<byte_t>( byte ) );
		buffer.close();
	}

	void file::remove() const
	{
		bool success{ false };
		try
		{
			success = std::filesystem::remove( Path.stdpath() );
		}
		catch( std::exception& e )
		{
			throw filesys::Failure( e.what() );
		}
		if( !success )
			throw filesys::Failure( "Unable to delete \"" + Path.str() + "\"" );
	}




	file::iterator::iterator( const file& f )
	{
		Buffer = filebuffer( f.fpath(), filesys::mode::input );
		Buffer.open();
		_readLine();
	}
	void file::iterator::_readLine()
	{
		Line.clear();
		if( !*this )
			return;
		char_t chr;
		size_t i = 0;
		for( ; Buffer.read( chr ) && chr != NewlineChr; ++i )
		{
			if( chr == CReturnChr )
				continue;
			Line += chr;
		}
		if( i == 0 )
			Buffer = filebuffer();
	}
}
