#include "Path.h"
#ifdef EON_WINDOWS
#	include <direct.h>
#else
#	include <unistd.h>
#	include <fstream>
#	define _getcwd getcwd
#endif


namespace eon
{
	path& path::operator=( const path& other )
	{
		Raw = other.Raw;
		Protocol = Raw.rebase( other.Protocol );
		Root = Raw.rebase( other.Root );
		Dir = Raw.rebase( other.Dir );
		Base = Raw.rebase( other.Base );
		Ext = Raw.rebase( other.Ext );
		return *this;
	}
	path& path::operator=( path&& other ) noexcept
	{
		Raw = std::move( other.Raw );
		Protocol = Raw.rebaseMoved( other.Protocol );
		Root = Raw.rebaseMoved( other.Root );
		Dir = Raw.rebaseMoved( other.Dir );
		Base = Raw.rebaseMoved( other.Base );
		Ext = Raw.rebaseMoved( other.Ext );
		return *this;
	}

	path& path::operator=( const substring& raw_path )
	{
		quickParse( raw_path );
		identifyElements();
		return *this;
	}




	substring path::pathWithoutRoot() const noexcept
	{
		if( !Root.empty() )
			return Raw.substr( Root.end() );
		if( !Protocol.empty() )
			return Raw.substr( Protocol.end() + 3 );
		return Raw.substr();
	}

	path path::parent() const
	{
		return path( Raw.substr( Raw.begin(), !Dir.empty()
			? Dir.end() : !Base.empty() ? Base.end() : Raw.end() ) );
	}

	string path::native() const
	{
#ifdef EON_WINDOWS
		return windows();
#else
		return Raw;
#endif
	}




	path& path::operator+=( const path& other )
	{
		if( !other.isRelative() )
		{
			if( other.root() != substring( "/" ) || other.numChars() > 1 )
				throw CompletePath( other.str() );
		}
		Raw += other.Raw;
		identifyElements();
		return *this;
	}

	path& path::endInSlash()
	{
		if( !Raw.endsWith( ForwSlashChr ) )
		{
			Raw += ForwSlashChr;
			identifyElements();
		}
		return *this;
	}
	path& path::noEndSlash()
	{
		if( Raw.endsWith( ForwSlashChr ) )
		{
			Raw.erase( Raw.substr( Raw.last() ) );
			identifyElements();
		}
		return *this;
	}

	path& path::removeProtocol()
	{
		if( !Protocol.empty() )
		{
			Raw.erase( substring( Protocol.begin(), Protocol.end() + 3 ) );
			identifyElements();
		}
		return *this;
	}
	path& path::removeRoot()
	{
		if( !Root.empty() )
		{
			if( !Dir.empty() )
				Raw.erase( Raw.substr( Raw.begin(), Dir.begin() ) );
			else
				Raw.erase( Raw.substr( Raw.begin(), Root.end() ) );
		}
		else
			return removeRoot();
		return *this;
	}
	path& path::removeBase()
	{
		if( !Base.empty() )
		{
			if( Base.begin() > Raw.begin() )
				Raw.erase( Raw.substr( Base.begin() - 1 ) );
			else
				Raw.erase( Base );
			identifyElements();
		}
		return *this;
	}
	path& path::removeExt()
	{
		if( !Ext.empty() )
		{
			if( Ext.begin() > Base.begin() )
				Raw.erase( substring( Ext.begin() - 1, Raw.end() ) );
			else
				Raw.erase( Ext );
			identifyElements();
		}
		return *this;
	}

	path& path::cwd()
	{
		char buff[ FILENAME_MAX ];
		if( _getcwd( buff, FILENAME_MAX ) != nullptr )
		{
			Raw = buff;
			identifyElements();
		}
		return *this;
	}




	void path::quickParse( const substring& path )
	{
		Raw.clear();
		for( auto c : path )
		{
			switch( c )
			{
				case '\\':
				case '/':
					if( Raw.empty() || *Raw.last() != ForwSlashChr || (
						Raw.numChars() > 2 && *( Raw.last() - 1 ) == ':'
						&& *Raw.last() == ForwSlashChr ) )
						Raw += ForwSlashChr;
					break;
				default:
					Raw += c;
					break;
			}
		}
	}
	void path::identifyElements()
	{
		identifyProtocol();
		identifyRoot();
		identifyDir();
		identifyBase();
	}
	void path::identifyProtocol()
	{
		// <protocol>://...
		auto sep = Raw.findFirst( "://" ).begin();
		if( sep && Raw.findFirst( ForwSlashChr ) == sep + 1 )
			Protocol = substring( Raw.begin(), sep );
		else
			Protocol = substring( Raw.end() );
	}
	void path::identifyRoot()
	{
		// [<protocol>://](/|<drive>:|//<unc>)...
		auto start = Protocol ? Protocol.end() + 3 : Raw.begin();

		// Unix root
		if( *start == ForwSlashChr && start + 1 != Raw.end() )
		{
			// Get the next element
			auto slash = Raw.findFirst(
				ForwSlashChr, Raw.substr( start + 1 ) ).begin();
			auto elm = string( Raw.substr( start + 1, slash ) );
#ifdef EON_UNIX
			if( std::ifstream( "/" + elm ) )
				Root = substring( start, start + 1 );
			else
#endif
			{
				// Is it a standard top level Unix directory?
				static std::set<string> dirs{
					"bin", "boot", "dev", "etc", "home", "lib", "lib64", "local",
					"lost+found", "media", "mnt", "opt", "proc", "root", "run",
					"sbin", "srv", "sys", "tmp", "usr", "var" };
				if( dirs.find( elm ) != dirs.end() )
					Root = substring( start, start + 1 );
			}
		}

		// UNC path
		else if( *start == ForwSlashChr && ( start + 1 ) != Raw.end()
			&& *( start + 1 ) == ForwSlashChr )
		{
			auto end = Raw.findFirst( ForwSlashChr,
				substring( start + 2, Raw.end() ) ).begin();
			if( end && end != start + 2 )
				Root = substring( start, end );
		}

		// Windows drive
		else if(
			( ( *start >= 'A' && *start <= 'Z' )
				|| ( *start >= 'a' && *start <= 'z' ) )
			&& start + 1 != Raw.end() && *( start + 1 ) == ColonChr )
			Root = substring( start, start + 2 );

		// No root element
		else
			Root = substring( Raw.end() );
	}
	void path::identifyDir()
	{
		// [<protocol>://][<root>]/<dir>...
		auto start = Root ? Root.end() : Protocol ? Protocol.end() + 3
			: Raw.begin();

		auto last_slash = Raw.findLast( ForwSlashChr,
			substring( start, Raw.end() ) ).begin();
		if( last_slash )
			Dir = substring( start, last_slash );
		else
			Dir = substring( Raw.end() );
	}
	void path::identifyBase()
	{
		// [<protocol>://][<root>]/<dir>/<base>
		auto start = Dir ? Dir.end() + 1 : Root ? Root.end() : Protocol
			? Protocol.end() + 3 : Raw.begin();

		if( start != Raw.end() )
		{
			Base = substring( start, Raw.end() );
			auto dot = Raw.findLast( PointChr, Base ).begin();
			if( dot )
				Ext = substring( dot + 1, Raw.end() );
			else
				Ext = substring( Raw.end() );
		}
		else
		{
			Base = substring( Raw.end() );
			Ext = Base;
		}
	}
}
