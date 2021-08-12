#include "NameRef.h"


namespace eon
{
	int nameref::compare( const nameref& other ) const noexcept
	{
		auto i = Value.begin(), j = other.Value.begin();
		for( ; i != Value.end() && j != other.Value.end(); ++i, ++j )
		{
			auto cmp = ( *i )->compare( **j );
			if( cmp != 0 )
				return cmp;
		}
		return i == Value.end() && j == other.Value.end()
			? 0 : i != Value.end() ? -1 : 1;
	}



	nameref& nameref::operator=( const string& ref )
	{
		string name;
		for( auto c : ref )
		{
			if( c == ForwSlashChr )
			{
				if( !name.empty() && !name.isUInt() )
					add( eon::name::get( std::move( name ) ) );
			}
			else if( string::isLetter( c ) || string::isNumberAsciiDigit( c ) || c == '_' )
				name += c;
		}
		if( !name.empty() && !name.isUInt() )
			add( eon::name::get( std::move( name ) ) );
		return *this;
	}
}
