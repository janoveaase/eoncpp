#include "Name.h"


namespace eon
{
#ifndef _DEBUG
	NameData* Data{ nullptr };
#endif


	inline void init()
	{
		if( !Data )
			Data = new NameData();
	}

	const string& str( const name_t& name )
	{
		init();
		return Data->str( name );
	}
	name_t name( string&& str ) noexcept
	{
		init();
		return Data->name( std::move( str ) );
	}
	name_t compilerName( string&& str )
	{
		init();
		return Data->compilerName( std::move( str ) );
	}
}
