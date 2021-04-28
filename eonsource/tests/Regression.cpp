#include "Regression.h"
#include <set>
#include <cfloat>



namespace eon
{
	TEST( Source, basic )
	{
		string raw{ "This is a 2 line test\nLine #2!" };
		source src( "test", std::move( raw ) );
		WANT_EQ( "test", src.name().stdstr() ) << "Wrong name!";
		REQUIRE_EQ( 2, src.numLines() ) << "Wrong number of lines!";
		WANT_EQ( "This is a 2 line test", src.line( 0 ).stdstr() ) << "Wrong first line!";
		WANT_EQ( "Line #2!", src.line( 1 ).stdstr() ) << "Wrong second line!";
		auto raw2 = src.reclaim();
		WANT_EQ( "This is a 2 line test\nLine #2!", raw2.stdstr() ) << "Wrong reclaimed string!";
	}

	TEST( SourceRef, basic )
	{
		string raw{ "This is a 2 line test\nLine #2!" };
		source src( "test", std::move( raw ) );
		sourceref ref( src );
		WANT_EQ( "test", ref.name().stdstr() ) << "Wrong name!";
	}
	TEST( SourceRef, pos )
	{
		string raw{ "This is a 2 line test\nLine #2!" };
		source src( "test", std::move( raw ) );
		sourceref ref( src );
		auto pos = ref.pos();
		pos.advance( src );
		pos.advance( src );
		pos.advance( src );
		pos.advance( src );
		pos.advance( src );
		WANT_EQ( 'i', pos.chr() ) << "Wrong name!";
		pos.setEnd( pos.area().begin() + 4 );
		ref = pos;
		WANT_EQ( "is a", ref.str().stdstr() ) << "Wrong string!";
	}
}
