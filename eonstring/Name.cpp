#include "Name.h"
#include <eoninlinetest/InlineTest.h>


namespace eon
{
#ifndef _DEBUG
	std::unique_ptr<NameData> Data;
#endif


	inline void init()
	{
		if( !Data )
			Data = std::make_unique<NameData>();
	}

	const string& str( const name_t& name )
	{
		init();
		return Data->str( name );
	}
	EON_TEST( name, str, basic,
		EON_EQ( string( "test_name" ), str( name( "test_name" ) ) ) );

	name_t name( string&& str ) noexcept
	{
		init();
		return Data->name( std::move( str ) );
	}
	EON_TEST( name, name, basic,
		EON_EQ( string( "name_test" ), str( name( "name_test" ) ) ) );
	EON_TEST( name, name, invalid,
		EON_EQ( no_name, name( "invalid name" ) ) );

	name_t compilerName( string&& str )
	{
		init();
		return Data->compilerName( std::move( str ) );
	}
	EON_TEST( name, compilerName, basic,
		EON_EQ( string( "!compiler name" ), str( compilerName( "!compiler name" ) ) ) );
}
