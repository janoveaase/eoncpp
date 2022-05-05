#include "Dir.h"
#include "File.h"


namespace eon
{
	directory& directory::operator=( eon::path&& dir_path )
	{
		Path = std::move( dir_path );
		Path.makeDirPath();
		return *this;
	}




	directory& directory::ensureExists()
	{
		if( Path.exists() )
			return *this;
		try
		{
			std::filesystem::create_directories( Path.makeFilePath().stdpath() );
		}
		catch( std::exception& e )
		{
			throw filesys::Failure( "Failed to create directory \"" + Path.str() + "\" - " + e.what() );
		}
		return *this;
	}

	directory& directory::rename( const string& new_name )
	{
		if( !Path )
			throw filesys::Failure( "Trying to rename unnamed directory" );
		if( !exists() )
			throw filesys::Failure( "Trying to rename nonexisting directory \"" + Path.str() + "\"" );
		directory target{ Path.hasParent() ? Path.parent() / eon::path( new_name ) : eon::path( new_name ) };
		if( target.exists() )
			throw filesys::Failure( "Cannot rename directory \"" + Path.str() + "\" to target \"" + new_name
				+ "\": Target exists" );
		try
		{
			std::filesystem::rename( Path.stdpath(), target.dpath().stdpath() );
		}
		catch( std::exception& e )
		{
			throw filesys::Failure( e.what() );
		}
		return *this = std::move( target );
	}
	directory& directory::forceRename( const string& new_name )
	{
		if( !Path )
			throw filesys::Failure( "Trying to rename unnamed directory" );
		if( !exists() )
			throw filesys::Failure( "Trying to rename nonexisting directory \"" + Path.str() + "\"" );
		directory target{ Path.hasParent() ? Path.parent() / eon::path( new_name ) : eon::path( new_name ) };
		try
		{
			if( target.exists() )
				target.remove();
			std::filesystem::rename( Path.stdstr(), target.dpath().stdstr() );
		}
		catch( std::exception& e )
		{
			throw filesys::Failure( e.what() );
		}
		return *this = std::move( target );
	}

	directory& directory::move( const directory& new_parent_dir )
	{
		if( !Path )
			throw filesys::Failure( "Trying to move unnamed directory" );
		if( !exists() )
			throw filesys::Failure( "Trying to move nonexisting directory \"" + Path.str() + "\"" );
		directory target{ new_parent_dir.Path / Path.base() };
		if( target.exists() )
			throw filesys::Failure( "Cannot move \"" + Path.str() + "\" to target \"" + target.dpath().str()
				+ "\": Target exists" );
		try
		{
			std::filesystem::rename( Path.stdpath(), target.dpath().stdpath() );
		}
		catch( std::exception& e )
		{
			throw filesys::Failure( e.what() );
		}
		return *this = std::move( target );
	}
	directory& directory::forceMove( const directory& new_parent_dir )
	{
		if( !Path )
			throw filesys::Failure( "Trying to move unnamed directory" );
		if( !exists() )
			throw filesys::Failure( "Trying to move nonexisting directory \"" + Path.str() + "\"" );
		directory target{ new_parent_dir.Path / Path.base() };
		try
		{
			if( target.exists() )
				target.remove();
			std::filesystem::rename( Path.stdpath(), target.dpath().stdpath() );
		}
		catch( std::exception& e )
		{
			throw filesys::Failure( e.what() );
		}
		return *this = std::move( target );
	}

	void directory::copy( const directory& target ) const
	{
		if( !Path )
			throw filesys::Failure( "Trying to copy unnamed directory" );
		if( !exists() )
			throw filesys::Failure( "Trying to copy nonexisting directory \"" + Path.str() + "\"" );
		if( target.exists() )
			throw filesys::Failure( "Cannot copy \"" + Path.str() + "\" to target \"" + target.dpath().str()
				+ "\": Target exists" );
		try
		{
			std::filesystem::copy( Path.stdpath(), target.dpath().stdpath(), std::filesystem::copy_options::skip_existing );
		}
		catch( std::exception& e )
		{
			throw filesys::Failure( e.what() );
		}
	}
	void directory::forceCopy( const directory& target ) const
	{
		if( !Path )
			throw filesys::Failure( "Trying to copy unnamed directory" );
		if( !exists() )
			throw filesys::Failure( "Trying to copy nonexisting directory \"" + Path.str() + "\"" );
		try
		{
			if( target.exists() )
				target.remove();
			std::filesystem::copy( Path.stdpath(), target.dpath().stdpath(), std::filesystem::copy_options::recursive );
		}
		catch( std::exception& e )
		{
			throw filesys::Failure( e.what() );
		}
	}
	void directory::mergeCopy( const directory& target ) const
	{
		if( !Path )
			throw filesys::Failure( "Trying to copy unnamed directory" );
		if( !exists() )
			throw filesys::Failure( "Trying to copy nonexisting directory \"" + Path.str() + "\"" );
		try
		{
			std::filesystem::copy( Path.stdpath(), target.dpath().stdpath(), std::filesystem::copy_options::recursive );
		}
		catch( std::exception& e )
		{
			throw filesys::Failure( e.what() );
		}
	}

	directory::iterator directory::begin() const noexcept { return iterator( *this ); }
	directory::iterator directory::end() const noexcept { return iterator(); }

	void directory::remove() const
	{
		bool success{ false };
		try
		{
			success = std::filesystem::remove_all( Path.stdstr() );
		}
		catch( std::exception& e )
		{
			throw filesys::Failure( e.what() );
		}
		if( !success )
			throw filesys::Failure( "Unable to delete \"" + Path.str() + "\"" );
	}




	directory::iterator::iterator( const eon::directory& d )
	{
		Base = std::filesystem::directory_iterator( d.dpath() );
		if( *this )
		{
			Path = Base->path();
			if( Base->is_directory() )
				Path.makeDirPath();
			else
				Path.makeFilePath();
		}
		else
			Path.clear();
	}
	directory::iterator& directory::iterator::operator++() noexcept
	{
		if( *this )
		{
			++Base;
			if( *this )
			{
				Path = Base->path();
				if( Base->is_directory() )
					Path.makeDirPath();
				else
					Path.makeFilePath();
			}
			else
				Path.clear();
		}
		else
			Path.clear();
		return *this;
	}
}
