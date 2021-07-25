#include "Path.h"


namespace eon
{
	path& path::operator+=( const path& other )
	{
		if( other.absolute() )
			throw filesys::BadPath( "Cannot append an absolute path" );
		if( isFilePath() )
			return _parse( Full + "/" + other.Full );
		else
			return _parse( Full + other.Full );
	}



	path& path::ext( const substring& part )
	{
		if( isDirPath() )
			throw filesys::BadPath(
				"Cannot set extension on a directory path" );
		if( part.count( PointChr ) > 0 )
			return _parse( Full + "." + part );
		else
		{
			Full += "." + part;
			if( Base )
				Base = Full.substr( Base.begin(), Full.end() );
			else
				Base = Full.substr( Full.last() - part.numChars(),
					Full.end() );
			if( Ext )
				Ext = Full.substr( Ext.begin(),
					Ext.begin() + part.numChars() );
			else
				Ext = Full.substr( Full.last() - part.numChars(), Full.end() );
			Name = Full.substr( Base.begin(), Ext.begin() - 1 );
			return *this;
		}
	}



	path& path::_parse( const substring& full )
	{
		Root.clear();
		Dirs.clear();
		Parent.clear();
		Base.clear();
		Name.clear();
		Ext.clear();

		if( full.empty() )
			return *this;

		std::list<string> elms;
		string elm;
		for( auto c : full )
		{
			if( c == ForwSlashChr || c == BackSlashChr )
			{
				if( elm.empty() && elms.size() == 1 && *elms.begin() == "/" )
				{
					elm = "//";
					elms.clear();
					continue;
				}
				if( elm.startsWith( "//" && elms.empty() ) )
					elms.push_back( std::move( elm ) );
				if( elm != "/" || elms.size() == 0 )
					elm += "/";
				if( !elm.empty() )
					elms.push_back( std::move( elm ) );
			}
			else
				elm += c;
		}
		if( !elm.empty() )
			elms.push_back( std::move( elm ) );

		auto probe = elms.begin();
		while( probe != elms.end() )
		{
			if( *probe == "./" )
				probe = elms.erase( probe );
			else if( *probe == "../" )
			{
				if( probe == elms.begin() )
					probe = elms.erase( probe );
				else
				{
					auto prev = probe; --prev;
					if( prev != elms.begin() || !_isRoot( *prev ) )
						probe = elms.erase( prev );
					probe = elms.erase( probe );
				}
			}
			else
				++probe;
		}

		Full = string().join<std::list<string>>( elms );
		if( _isRoot( *elms.begin() ) )
		{
			if( elms.size() == 1
				&& !elms.begin()->endsWith( ForwSlashChr ) )
			{
				Full += "/";
				*elms.begin() += "/";
			}
			Root = Full.substr( Full.begin(),
				Full.begin() + elms.begin()->numChars() );
		}
		if( Root && Root.numChars() == Full.numChars() )
		{
			Parent = Root;
			return *this;
		}
		if( Full.endsWith( ForwSlashChr ) )	// Directory
		{
			auto slash = Full.findLast( ForwSlashChr,
				Full.substr( Full.begin(), Full.last() ) );
			if( !Root && !slash )
				slash = Full.substr( Full.begin(), Full.begin() );
			Base = Full.substr( slash.end(), Full.end() );
		}
		else								// File
		{
			auto slash = Full.findLast( ForwSlashChr );
			if( !slash && !Root )
				Base = Full.substr();
			else if( slash )
				Base = Full.substr( slash.end(), Full.end() );
		}
		if( Base )
		{
			Parent = Full.substr( Full.begin(), Base.begin() );
			if( Root )
				Dirs = Full.substr( Root.end(), Base.begin() );
			else
				Dirs = Full.substr( Full.begin(), Base.begin() );
			if( !Base.endsWith( ForwSlashChr ) )
			{
				auto dot = Base.highToLow().findLast( PointChr );
				if( dot )
					Ext = Full.substr( dot.end(), Base.end() );
			}
		}
		if( Base )
		{
			if( Ext )
				Name = Full.substr( Base.begin(), Ext.begin() - 1 );
			else
				Name = Base;
		}

		return *this;
	}
	path& path::_parse( const path& other ) noexcept
	{
		Root.clear();
		Dirs.clear();
		Parent.clear();
		Base.clear();
		Name.clear();
		Ext.clear();

		if( other.Root )
			Root = substring(
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(),
					Full.c_str() + other.Root.begin().numByte() ),
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(),
					Full.c_str() + other.Root.end().numByte() ) );
		if( other.Dirs )
			Dirs = substring(
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(),
					Full.c_str() + other.Dirs.begin().numByte() ),
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(),
					Full.c_str() + other.Dirs.end().numByte() ) );
		if( other.Parent )
			Parent = substring(
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(),
					Full.c_str() + other.Parent.begin().numByte() ),
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(),
					Full.c_str() + other.Parent.end().numByte() ) );
		if( other.Base )
			Base = substring(
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(),
					Full.c_str() + other.Base.begin().numByte() ),
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(),
					Full.c_str() + other.Base.end().numByte() ) );
		if( other.Name )
			Name = substring(
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(),
					Full.c_str() + other.Name.begin().numByte() ),
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(),
					Full.c_str() + other.Name.end().numByte() ) );
		if( other.Ext )
			Ext = substring(
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(),
					Full.c_str() + other.Ext.begin().numByte() ),
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(),
					Full.c_str() + other.Ext.end().numByte() ) );
		return *this;
	}

	bool path::_isRoot( const string& str ) noexcept
	{
		if( str == "/" || str.startsWith( "//" ) )	// Unix root or UNC path
			return true;
		if( str.numChars() == 2						// Windows drive
			&& Characters::get().isAsciiLetter( *str.begin() )
			&& *str.last() == ColonChr )
			return true;
		if( str.numChars() == 3
			&& Characters::get().isAsciiLetter( *str.begin() )
			&& *( str.begin() + 1 ) == ColonChr
			&& *str.last() == ForwSlashChr )
			return true;
		return false;
	}
}
