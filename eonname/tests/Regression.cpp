#include "Regression.h"
#include <set>
#include <cfloat>



namespace eon
{
	TEST( NameTest, basic )
	{
		name_t n1{ no_name }, n2{ name_byte };
		n1 = name::get( "byte" );
		WANT_EQ( n1, n2 );
		WANT_EQ( "byte", n2->stdstr() );
	}

	TEST( NameRefTest, basic )
	{
		nameref ref{ "one/two/three" };
		WANT_EQ( "one/two/three", ref.str().stdstr() );
		REQUIRE_EQ( 3, ref.numElms() );
		WANT_EQ( "one", ref.at( 0 )->stdstr() );
		WANT_EQ( "two", ref.at( 1 )->stdstr() );
		WANT_EQ( "three", ref.at( 2 )->stdstr() );
	}
}
