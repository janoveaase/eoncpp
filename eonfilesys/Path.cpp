#include "Path.h"


namespace eon
{
	string path::systemStr( name_t system_name )
	{
		if( system_name == name_windows )
			return str().replace( ForwSlashChr, BackSlashChr );
		else
			return str();
	}




	path& path::operator/=( const path& other )
	{
		if( other.absolute() )
			throw filesys::BadPath( "Cannot append absolute path" );
		if( file() )
			return _parse( Full + "/" + other.Full );
		else
			return _parse( Full + other.Full );
	}




	path& path::ext( const substring& part )
	{
		if( dir() )
			throw filesys::BadPath( "Cannot set extension on a directory path" );
		if( part.count( PointChr ) > 0 )
			return _parse( Full + "." + part );
		else
		{
			Full += "." + part;
			if( Base )
				Base = Full.substr( Base.begin(), Full.end() );
			else
				Base = Full.substr( Full.last() - part.numChars(), Full.end() );
			if( Ext )
				Ext = Full.substr( Ext.begin(), Ext.begin() + part.numChars() );
			else
				Ext = Full.substr( Full.last() - part.numChars(), Full.end() );
			Name = Full.substr( Base.begin(), Ext.begin() - 1 );
			return *this;
		}
	}




	path& path::_parse( const substring& full )
	{
		// This will normalize the path!

		clear();
		if( full.empty() )
			return *this;

		// Split path into raw elements first and normalize
		auto raw_elements = _rawSplit( full );
		if( !_normalize( raw_elements ) )
			throw filesys::BadPath( "Not a valid path: \"" + string( full ) + "\"!" );

		// Reconstruct into full path again
		if( !_reconstruct( raw_elements ) )
			throw filesys::BadPath( "Not a valid path: \"" + string( full ) + "\"!" );

		return *this;
	}
	std::list<string> path::_rawSplit( const substring& full ) const
	{
		// Very simply split the path into elements consisting of separators
		// and non-separators only
		std::list<string> raw_elements;
		string elm;
		for( auto c : full )
		{
			if( c == ForwSlashChr || c == BackSlashChr )
			{
				if( elm.empty() || elm.startsWith( ForwSlashChr ) )
					elm += c;
				else
				{
					raw_elements.push_back( std::move( elm ) );
					elm = ForwSlashChr;
				}
			}
			else
			{
				if( elm.empty() || !elm.startsWith( ForwSlashChr ) )
					elm += c;
				else
				{
					raw_elements.push_back( std::move( elm ) );
					elm = c;
				}
			}
		}
		if( !elm.empty() )
			raw_elements.push_back( std::move( elm ) );
		return raw_elements;
	}
	bool path::_normalize( std::list<string>& elements ) const
	{
		if( elements.empty() )
			return true;
		auto elm = elements.begin();
		if( *elm == "/" )
			++elm;
		else if( *elm == "//" )
		{
			++elm;
			if( elm != elements.end() )
				++elm;
		}
		else if( elm->startsWith( ForwSlashChr ) )
			return false;
		for( ; elm != elements.end(); )
		{
			// Single separator!
			if( elm->numChars() > 1 && elm->startsWith( ForwSlashChr ) )
				*elm = ForwSlashChr;

			// Remove "./" elements
			else if( *elm == PointChr )
			{
				auto next = elm;
				if( ++next != elements.end() && *next == "/" )
				{
					elm = elements.erase( elm );
					elm = elements.erase( elm );
					continue;
				}
			}

			// Remove "<non-dot-dot-element>../"
			else if( !elm->startsWith( ForwSlashChr ) && !elm->startsWith( PointChr ) )
			{
				auto next = elm;
				if( ++next != elements.end() && *next == "/" )
				{
					if( ++next != elements.end() && *next == ".." )
					{
						if( ++next != elements.end() && *next == "/" )
						{
							elm = elements.erase( elm );
							elm = elements.erase( elm );
							elm = elements.erase( elm );
							elm = elements.erase( elm );
							continue;
						}
					}
				}
			}

			// Remove all remaining "../" elements
			else if( *elm == ".." )
			{
				auto next = elm;
				if( ++next != elements.end() && *next == "/" )
				{
					elm = elements.erase( elm );
					elm = elements.erase( elm );
					continue;
				}
			}

			++elm;
		}

		return true;
	}

	bool path::_reconstruct( std::list<string>& elements )
	{
		Full = string().join<std::list<string>>( elements );
		auto elm = elements.begin(), next = elm;
		++next;
		if( *elm == "/" )
		{
			Root = Full.substr( Full.begin(), Full.begin() + 1 );
			elm = next;
			if( next != elements.end() )
				++next;
		}
		else if( *elm == "//" )
		{
			if( next == elements.end() )		// UNC name
				return false;
			auto next2 = next;
			if( ++next2 == elements.end() )		// End of UNC name
			{
				Full += "/";
				Root = Full.substr();
				elm = next2;
				next = next2;
			}
			else
			{
				Root = Full.substr( Full.begin(), Full.begin() + 2 + next->numChars() + next2->numChars() );
				elm = next2;
				next = next2;
				++next;
			}
		}
		else if( elm->numChars() == 2
			&& ( ( *elm->begin() >= 'A' && *elm->begin() <= 'Z' ) || ( *elm->begin() >= 'a' && *elm->begin() <= 'z' ) )
			&& *elm->last() == ColonChr )
		{
			if( next == elements.end() )
			{
				Full += "/";
				Root = Full.substr();
				elm = next;
			}
			else
			{
				Root = Full.substr( Full.begin(), Full.begin() + elm->numChars() + next->numChars() );
				elm = ++next;
				if( next != elements.end() )
					++next;
			}
		}

		if( elm == elements.end() )
		{
			Parent = Root;
			return true;
		}

		if( Full.endsWith( ForwSlashChr ) )	// Directory
		{
			auto slash = Full.findLast( ForwSlashChr, Full.substr( Full.begin(), Full.last() ) );
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
		return true;
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
					Full.c_str(), Full.numBytes(), Full.numChars(), Full.c_str() + other.Root.begin().numByte() ),
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(), Full.c_str() + other.Root.end().numByte() ) );
		if( other.Dirs )
			Dirs = substring(
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(), Full.c_str() + other.Dirs.begin().numByte() ),
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(), Full.c_str() + other.Dirs.end().numByte() ) );
		if( other.Parent )
			Parent = substring(
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(), Full.c_str() + other.Parent.begin().numByte() ),
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(), Full.c_str() + other.Parent.end().numByte() ) );
		if( other.Base )
			Base = substring(
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(), Full.c_str() + other.Base.begin().numByte() ),
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(), Full.c_str() + other.Base.end().numByte() ) );
		if( other.Name )
			Name = substring(
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(), Full.c_str() + other.Name.begin().numByte() ),
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(), Full.c_str() + other.Name.end().numByte() ) );
		if( other.Ext )
			Ext = substring(
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(), Full.c_str() + other.Ext.begin().numByte() ),
				string_iterator(
					Full.c_str(), Full.numBytes(), Full.numChars(), Full.c_str() + other.Ext.end().numByte() ) );
		return *this;
	}

	bool path::_isRoot( const string& str ) noexcept
	{
		if( str == "/" || str.startsWith( "//" ) )	// Unix root or UNC path
			return true;
		if( str.numChars() == 2						// Windows drive
			&& Characters::get().isAsciiLetter( *str.begin() ) && *str.last() == ColonChr )
			return true;
		if( str.numChars() == 3
			&& Characters::get().isAsciiLetter( *str.begin() )
			&& *( str.begin() + 1 ) == ColonChr && *str.last() == ForwSlashChr )
			return true;
		return false;
	}
}
