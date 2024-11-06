#include "NameData.h"


namespace eon
{
	const string NameData::NullStr;


	name_t NameData::name( string&& str ) noexcept
	{
		if( !validName( str ) )
			return no_name;
		std::scoped_lock<std::mutex> lock( Lock );
		if( auto found = Lookup.find( &str );
			found != Lookup.end() )
		{
			str.clear();		// For consistency!
			return found->second;
		}
		Names.push_back( new string( std::move( str ) ) );
		name_t id( Names.size() );
		Lookup[ Names[ Names.size() - 1 ] ] = id;
		return id;
	}

	bool NameData::validName( const string& str ) noexcept
	{
		if( str.empty() )
			return false;
		size_t non_numerals = 0;
		for( auto c : str )
		{
			if( string::isLetter( c ) || c == '_' )
				++non_numerals;
			else if( !string::isNumber( c ) )
				return false;
		}
		return non_numerals > 0;
	}
	name_t NameData::compilerName( string&& str )
	{
		if( str.empty() )
			return no_name;
		std::scoped_lock<std::mutex> lock( Lock );
		if( auto found = Lookup.find( &str );
			found != Lookup.end() )
		{
			str.clear();		// For consistency!
			return found->second;
		}
		Names.push_back( new string( std::move( str ) ) );
		name_t id( Names.size() );
		Lookup[ Names[ Names.size() - 1 ] ] = id;
		return id;
	}
}
