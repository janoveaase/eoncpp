#include "Name.h"


namespace eon
{
	struct hash_name { inline size_t operator()( const string* a ) const noexcept { return a->hash(); } };
	struct eq_name { inline bool operator()( const string* a, const string* b ) const noexcept { return *a == *b; } };
#ifdef _DEBUG
	std::vector<string*> _Name::Names;
#define Names _Name::Names
#else
	static std::vector<string*> Names;
#endif
	static std::unordered_map<const string*, name_t, hash_name, eq_name> NameLookup;
	static std::mutex NamesLock;

	const string& str( const name_t& name )
	{
		static const string null_str;
		std::lock_guard<std::mutex> lock( NamesLock );
#ifdef _DEBUG
		return ( name > no_name && name.Value <= Names.size() ) ? *Names[ name.Value - 1 ] : null_str;
#else
		return ( name > no_name && name <= Names.size() ) ? *Names[ name - 1 ] : null_str;
#endif
	}
	name_t name( string&& str ) noexcept
	{
		if( !validName( str ) )
			return no_name;
		std::lock_guard<std::mutex> lock( NamesLock );
		auto found = NameLookup.find( &str );
		if( found != NameLookup.end() )
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
		NameLookup[ Names[ Names.size() - 1 ] ] = id;
		return id;
	}


	bool validName( const string& str ) noexcept
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


	name_t compilerName( string&& str )
	{
		std::lock_guard<std::mutex> lock( NamesLock );
		auto found = NameLookup.find( &str );
		if( found != NameLookup.end() )
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
		NameLookup[ Names[ Names.size() - 1 ] ] = id;
		return id;
	}
}
