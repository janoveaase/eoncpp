#include "NameData.h"


namespace eon
{
	const string NameData::NullStr;


	name_t NameData::name( string&& str ) noexcept
	{
		if( !validName( str ) )
			return no_name;
		std::lock_guard<std::mutex> lock( Lock );
		auto found = Lookup.find( &str );
		if( found != Lookup.end() )
		{
			str.clear();		// For consistency!
			return found->second;
		}
		Names.push_back( new string( std::move( str ) ) );
#ifdef _DEBUG
		name_t id( Names.size() );
#else
		auto id = static_cast<name_t>( Names.size() );
#endif
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
		std::lock_guard<std::mutex> lock( Lock );
		auto found = Lookup.find( &str );
		if( found != Lookup.end() )
		{
			str.clear();		// For consistency!
			return found->second;
		}
		Names.push_back( new string( std::move( str ) ) );
#ifdef _DEBUG
		name_t id( Names.size() );
#else
		auto id = static_cast<name_t>( Names.size() );
#endif
		Lookup[ Names[ Names.size() - 1 ] ] = id;
		return id;
	}
}
