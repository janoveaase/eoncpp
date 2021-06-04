#include "Regression.h"



namespace eon
{
	TEST( ExceptTest, basic )
	{
		WANT_EXCEPT( throw exception( name::get( "Test" ) ), exception );
		try
		{
			throw exception( name::get( "Test" ) );
		}
		catch( exception& e )
		{
			WANT_EQ( "Test", e.what() );
		}
		try
		{
			throw exception( name::get( "Test" ), "This is just a test" );
		}
		catch( exception& e )
		{
			WANT_EQ( "This is just a test", e.what() );
			WANT_EQ( "Test: This is just a test", e.details().stdstr() );
		}
	}
}
