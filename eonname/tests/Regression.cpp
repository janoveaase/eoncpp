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
}
