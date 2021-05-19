#include "Name.h"


namespace eon
{
	static name NameObj;




	name_t name::get( string&& source ) noexcept
	{
		if( !valid( source ) )
			return no_name;
		std::lock_guard<std::mutex> lock( NameObj.NamesLock );
		auto found = NameObj.NameLookup.find( &source );
		if( found != NameObj.NameLookup.end() )
		{
			source.clear();	// For consistency!
			return *found;
		}
		NameObj.Names.push_back( new string( std::move( source ) ) );
		auto id = static_cast<name_t>(
			NameObj.Names[ NameObj.Names.size() - 1 ] );
		NameObj.NameLookup.insert( id );
		return id;
	}

	bool name::registered( name_t value )
	{
		if( value == no_name )
			return false;
		std::lock_guard<std::mutex> lock( NameObj.NamesLock );
		return NameObj.NameLookup.find( value ) != NameObj.NameLookup.end();
	}

	bool name::valid( const string& source ) noexcept
	{
		if( source.empty() )
			return false;
		size_t non_numerals = 0;
		for( auto c : source )
		{
			if( string::isLetter( c ) || c == '_' )
				++non_numerals;
			else if( !string::isNumber( c ) )
				return false;
		}
		return non_numerals > 0;
	}




	name::~name()
	{
		std::lock_guard<std::mutex> lock( NameObj.NamesLock );
		for( auto elm : Names )
		{
			if( elm != nullptr )
				delete elm;
		}
	}




	const name_t name_bool = name::get( "bool" );
	const name_t name_byte = name::get( "byte" );
	const name_t name_char = name::get( "char" );
	const name_t name_int = name::get( "int" );
	const name_t name_int8 = name::get( "int8" );
	const name_t name_int16 = name::get( "int16" );
	const name_t name_int32 = name::get( "int32" );
	const name_t name_int64 = name::get( "int64" );
	const name_t name_float = name::get( "float" );
	const name_t name_pos = name::get( "pos" );
	const name_t name_name = name::get( "name" );
	const name_t name_type = name::get( "type" );
	const name_t name_tuple = name::get( "tuple" );
	const name_t name_bytes = name::get( "bytes" );
	const name_t name_string = name::get( "string" );
	const name_t name_regex = name::get( "regex" );
	const name_t name_true = name::get( "true" );
	const name_t name_false = name::get( "false" );
	const name_t name_key = name::get( "key" );
	const name_t name_value = name::get( "value" );
	const name_t name_raw = name::get( "raw" );
	const name_t name_meta = name::get( "meta" );
	const name_t name_reference = name::get( "reference" );
	const name_t name_docs = name::get( "docs" );
	const name_t name_b = name::get( "b" );
	const name_t name_test = name::get( "test" );
	const name_t name_title = name::get( "title" );
}
