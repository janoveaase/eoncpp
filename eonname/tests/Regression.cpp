#include "Regression.h"
#include <set>
#include <cfloat>



namespace eon
{
	TEST( NameTest, basic )
	{
		name_t n1{ no_name }, n2{ name_name };
		n1 = name::get( "name" );
		WANT_EQ( n1, n2 );
		WANT_EQ( "name", n2->stdstr() );
	}
	TEST( NameTest, compiler )
	{
		auto n1 = name::compilerGet( "$name" );
		auto n2 = name::compilerGet( "@name" );
		auto n3 = name::compilerGet( "#name" );
		auto n4 = name::compilerGet( "0$@#0" );
		REQUIRE_FALSE( n1 == no_name );
		REQUIRE_FALSE( n2 == no_name );
		REQUIRE_FALSE( n3 == no_name );
		REQUIRE_FALSE( n4 == no_name );
		WANT_EQ( "$name", n1->stdstr() );
		WANT_EQ( "@name", n2->stdstr() );
		WANT_EQ( "#name", n3->stdstr() );
		WANT_EQ( "0$@#0", n4->stdstr() );
	}

	TEST( NameRefTest, basic )
	{
		nameref ref{ "one/two/three" };
		WANT_EQ( "@one/two/three", ref.str().stdstr() );
		REQUIRE_EQ( 3, ref.numElms() );
		WANT_EQ( "one", ref.at( 0 )->stdstr() );
		WANT_EQ( "two", ref.at( 1 )->stdstr() );
		WANT_EQ( "three", ref.at( 2 )->stdstr() );
	}
}
