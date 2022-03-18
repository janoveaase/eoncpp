#include "Regression.h"
#include <set>
#include <cfloat>



namespace eon
{
	TEST( Source, basic_string )
	{
		string str{ "This is a 2 line test\nLine #2!" };
		source::String src( "test", std::move( str ) );
		WANT_EQ( "test", src.name().stdstr() ) << "Wrong name!";
		source::Pos beg;
		WANT_EQ( 0, beg.line() );
		WANT_EQ( 0, beg.posOnLine() );
		WANT_EQ( 0, beg.charPos() );
		WANT_EQ( 0, beg.bytePos() );
		WANT_EQ( 'T', src.chr( beg ) ) << "Wrong first character!";
		
		auto next = src.push( beg, 1 );
		WANT_EQ( 0, next.line() );
		WANT_EQ( 1, next.posOnLine() );
		WANT_EQ( 1, next.charPos() );
		WANT_EQ( 1, next.bytePos() );
		WANT_EQ( 'h', src.chr( next ) ) << "Wrong second character!";

		auto eol = src.push( beg, 21 );
		WANT_EQ( 0, eol.line() );
		WANT_EQ( 21, eol.posOnLine() );
		WANT_EQ( 21, eol.charPos() );
		WANT_EQ( 21, eol.bytePos() );
		WANT_EQ( "This is a 2 line test", src.str( beg, eol ).stdstr() ) << "Wrong first line!";

		auto l2 = src.push( eol, 1 );
		WANT_EQ( 1, l2.line() );
		WANT_EQ( 0, l2.posOnLine() );
		WANT_EQ( 22, l2.charPos() );
		WANT_EQ( 22, l2.bytePos() );
		WANT_EQ( 'L', src.chr( l2 ) );

		auto eos = src.push( l2, 99 );
		WANT_EQ( 1, eos.line() );
		WANT_EQ( 8, eos.posOnLine() );
		WANT_EQ( 30, eos.charPos() );
		WANT_EQ( 30, eos.bytePos() );
		WANT_EQ( "Line #2!", src.str( l2, eos ).stdstr() ) << "Wrong second line!";

		WANT_EXCEPT( src.push( eos, 1 ), source::EndOfSource ) << "Didn't throw at end of source!";

		auto first = src.pull( next, beg, 1 );
		WANT_EQ( 0, first.line() );
		WANT_EQ( 0, first.posOnLine() );
		WANT_EQ( 0, first.charPos() );
		WANT_EQ( 0, first.bytePos() );
		WANT_EQ( 'T', src.chr( first ) );

		auto pos = src.pull( eos, next, 8 );
		WANT_EQ( 1, pos.line() );
		WANT_EQ( 0, pos.posOnLine() );
		WANT_EQ( 22, pos.charPos() );
		WANT_EQ( 22, pos.bytePos() );
		WANT_EQ( 'L', src.chr( pos ) );

		pos = src.pull( l2, beg, 1 );
		WANT_EQ( 0, pos.line() );
		WANT_EQ( 21, pos.posOnLine() );
		WANT_EQ( 21, pos.charPos() );
		WANT_EQ( 21, pos.bytePos() );
		WANT_EQ( '\n', src.chr( pos ) );

		pos = src.pull( eos, beg, 9 );
		WANT_EQ( 0, pos.line() );
		WANT_EQ( 21, pos.posOnLine() );
		WANT_EQ( 21, pos.charPos() );
		WANT_EQ( 21, pos.bytePos() );

		pos = src.push( pos, 1 );
		WANT_EQ( 1, pos.line() );
		WANT_EQ( 0, pos.posOnLine() );
		WANT_EQ( 22, pos.charPos() );
		WANT_EQ( 22, pos.bytePos() );

		auto raw2 = src.reclaim();
		WANT_EQ( "This is a 2 line test\nLine #2!", raw2.stdstr() )
			<< "Wrong reclaimed string!";
	}
	TEST( Source, basic_file )
	{
		REQUIRE_TRUE( prepareTestFile( "This is a 2 line test\nLine #2!" ) );
		source::File src( TestFile );
		WANT_EQ( TestFile, src.name().stdstr() ) << "Wrong name!";
		source::Pos beg;
		WANT_EQ( 0, beg.line() );
		WANT_EQ( 0, beg.posOnLine() );
		WANT_EQ( 0, beg.charPos() );
		WANT_EQ( 0, beg.bytePos() );
		WANT_EQ( 'T', src.chr( beg ) ) << "Wrong first character!";

		auto next = src.push( beg, 1 );
		WANT_EQ( 0, next.line() );
		WANT_EQ( 1, next.posOnLine() );
		WANT_EQ( 1, next.charPos() );
		WANT_EQ( 1, next.bytePos() );
		WANT_EQ( 'h', src.chr( next ) ) << "Wrong second character!";

		auto eol = src.push( beg, 21 );
		WANT_EQ( 0, eol.line() );
		WANT_EQ( 21, eol.posOnLine() );
		WANT_EQ( 21, eol.charPos() );
		WANT_EQ( 21, eol.bytePos() );
		WANT_EQ( '\n', src.chr( eol ) ) << "Wrong eol character!";
		WANT_EQ( "This is a 2 line test", src.str( beg, eol ).stdstr() ) << "Wrong first line!";

		auto l2 = src.push( eol, 1 );
		WANT_EQ( 1, l2.line() );
		WANT_EQ( 0, l2.posOnLine() );
		WANT_EQ( 22, l2.charPos() );
		WANT_EQ( 22, l2.bytePos() );
		WANT_EQ( 'L', src.chr( l2 ) );

		auto eos = src.push( l2, 99 );
		WANT_EQ( 1, eos.line() );
		WANT_EQ( 8, eos.posOnLine() );
		WANT_EQ( 30, eos.charPos() );
		WANT_EQ( 30, eos.bytePos() );
		WANT_EQ( "Line #2!", src.str( l2, eos ).stdstr() ) << "Wrong second line!";

		WANT_EXCEPT( src.push( eos, 1 ), source::EndOfSource ) << "Didn't throw at end of source!";

		auto first = src.pull( next, beg, 1 );
		WANT_EQ( 0, first.line() );
		WANT_EQ( 0, first.posOnLine() );
		WANT_EQ( 0, first.charPos() );
		WANT_EQ( 0, first.bytePos() );
		WANT_EQ( 'T', src.chr( first ) );

		auto pos = src.pull( eos, next, 8 );
		WANT_EQ( 1, pos.line() );
		WANT_EQ( 0, pos.posOnLine() );
		WANT_EQ( 22, pos.charPos() );
		WANT_EQ( 22, pos.bytePos() );
		WANT_EQ( 'L', src.chr( pos ) );

		pos = src.pull( l2, beg, 1 );
		WANT_EQ( 0, pos.line() );
		WANT_EQ( 21, pos.posOnLine() );
		WANT_EQ( 21, pos.charPos() );
		WANT_EQ( 21, pos.bytePos() );
		WANT_EQ( '\n', src.chr( pos ) );

		pos = src.pull( eos, beg, 9 );
		WANT_EQ( 0, pos.line() );
		WANT_EQ( 21, pos.posOnLine() );
		WANT_EQ( 21, pos.charPos() );
		WANT_EQ( 21, pos.bytePos() );

		pos = src.push( pos, 1 );
		WANT_EQ( 1, pos.line() );
		WANT_EQ( 0, pos.posOnLine() );
		WANT_EQ( 22, pos.charPos() );
		WANT_EQ( 22, pos.bytePos() );
	}


	TEST( SourceRef, basic )
	{
		string str{ "This is a 2 line test\nLine #2!" };
		source::String src( "test", std::move( str ) );
		source::Ref ref( src );
		WANT_EQ( "test", ref.name().stdstr() ) << "Wrong name!";
	}
	TEST( SourceRef, string )
	{
		string str{ "This is a 2 line test\nLine #2!" };
		source::String src( "test", std::move( str ) );
		source::Ref ref( src );
		WANT_EQ( "1:1", ref.startStr().stdstr() );

		ref.pushStart( 5 );
		WANT_EQ( "1:6", ref.startStr().stdstr() );
		WANT_EQ( "is a 2 line test\nLine #2!", ref.str().stdstr() );

		ref.end( src.push( ref.start(), 16 ) );
		WANT_EQ( "is a 2 line test", ref.str().stdstr() );

		ref.pushEnd( 7 );
		ref.pushStart( 12 );
		WANT_EQ( "test\nLine #", ref.str().stdstr() );
	}
	TEST( SourceRef, file )
	{
		REQUIRE_TRUE( prepareTestFile( "This is a 2 line test\nLine #2!" ) );
		source::File src( TestFile );
		WANT_EQ( TestFile, src.name().stdstr() ) << "Wrong name!";
		source::Ref ref( src );
		WANT_EQ( "1:1", ref.startStr().stdstr() );

		ref.pushStart( 5 );
		WANT_EQ( "1:6", ref.startStr().stdstr() );
		WANT_EQ( "is a 2 line test\nLine #2!", ref.str().stdstr() );

		ref.end( src.push( ref.start(), 16 ) );
		WANT_EQ( "is a 2 line test", ref.str().stdstr() );

		ref.pushEnd( 7 );
		ref.pushStart( 12 );
		WANT_EQ( "test\nLine #", ref.str().stdstr() );
	}


	TEST( SourceReporter, basic )
	{
		string str{ "This is a 2 line test\nLine #2!" };
		source::String src( "test", std::move( str ) );
		source::Reporter rep;
		rep.add( *this );

		rep.fatal( "one", source::Ref( src, source::Pos( 5, 5, 0, 5 ), source::Pos( 7, 7, 0, 7 ) ) );
		rep.error( "two", source::Ref( src, source::Pos( 10, 10, 0, 10 ), source::Pos( 11, 11, 0, 11 ) ) );
		rep.serious( "three", source::Ref( src, source::Pos( 12, 12, 0, 12 ), source::Pos( 21, 21, 0, 21 ) ) );
		rep.warning( "four", source::Ref( src, source::Pos( 17, 17, 0, 17 ), source::Pos( 26, 26, 1, 4 ) ) );
		rep.note( "five", source::Ref( src, source::Pos( 27, 27, 1, 5 ), source::Pos( 28, 28, 1, 6 ) ) );
		rep.info( "six", source::Ref( src, source::Pos( 29, 29, 1, 7 ), source::Pos( 30, 30, 1, 8 ) ) );

		auto act = string( "\n" ).join( Messages );
		string exp{
			"  FATAL in \"test\" (1:6-1:8): one\n1|This is a 2 line test\n |-----^^--------------\n"
			"  ERROR in \"test\" (1:11): two\n1|This is a 2 line test\n |----------^----------\n"
			"SERIOUS in \"test\" (1:13-1:22): three\n1|This is a 2 line test\n |------------^^^^^^^^^\n"
			"WARNING in \"test\" (1:18-2:5): four\n1|This is a 2 line test\n |-----------------^^^^\n2|Line #2!\n |^^^^----\n"
			"   NOTE in \"test\" (2:6): five\n2|Line #2!\n |-----^--\n"
			"   INFO in \"test\" (2:8): six\n2|Line #2!\n |-------^"
		};
		WANT_EQ( exp, act );
	}
	TEST( SourceReporter, empty )
	{
		string str{ "This is a 2 line test\nLine #2!" };
		source::String src( "test", std::move( str ) );
		source::Reporter rep;
		rep.add( *this );

		rep.fatal( "one", source::Ref( src ) );
		rep.error( "two", source::Ref( src ) );
		rep.serious( "three", source::Ref( src ) );
		rep.warning( "four", source::Ref( src ) );
		rep.note( "five", source::Ref( src ) );
		rep.info( "six", source::Ref( src ) );

		auto act = string( "\n" ).join( Messages );
		string exp{
			"  FATAL in \"test\": one\n"
			"  ERROR in \"test\": two\n"
			"SERIOUS in \"test\": three\n"
			"WARNING in \"test\": four\n"
			"   NOTE in \"test\": five\n"
			"   INFO in \"test\": six"
		};
		WANT_EQ( exp, act );
	}
}
