#include "Regression.h"
#include <set>
#include <cfloat>



namespace eon
{
	TEST( Source, basic_string )
	{
		string str{ "This is a 2 line test\nLine #2!" };
		source::String src( "test", std::move( str ) );
		WANT_EQ( "test", src.sourceName() ) << "Wrong name!";
		source::Pos beg;
		WANT_EQ( 0, beg.Line );
		WANT_EQ( 0, beg.PosOnLine );
		WANT_EQ( 0, beg.CharPos );
		WANT_EQ( 0, beg.BytePos );
		WANT_EQ( 'T', src.chr( beg ) ) << "Wrong first character!";

		auto next = src.getPosAtOffset( beg, 1 );
		WANT_EQ( 0, next.Line );
		WANT_EQ( 1, next.PosOnLine );
		WANT_EQ( 1, next.CharPos );
		WANT_EQ( 1, next.BytePos );
		WANT_EQ( 'h', src.chr( next ) ) << "Wrong second character!";

		auto eol = src.getPosAtOffset( beg, 21 );
		WANT_EQ( 0, eol.Line );
		WANT_EQ( 21, eol.PosOnLine );
		WANT_EQ( 21, eol.CharPos );
		WANT_EQ( 21, eol.BytePos );
		WANT_EQ( "This is a 2 line test", src.str( beg, eol ).stdstr() ) << "Wrong first line!";

		auto l2 = src.getPosAtOffset( eol, 1 );
		WANT_EQ( 1, l2.Line );
		WANT_EQ( 0, l2.PosOnLine );
		WANT_EQ( 22, l2.CharPos );
		WANT_EQ( 22, l2.BytePos );
		WANT_EQ( 'L', src.chr( l2 ) );

		auto eos = src.getPosAtOffset( l2, 99 );
		WANT_EQ( 1, eos.Line );
		WANT_EQ( 8, eos.PosOnLine );
		WANT_EQ( 30, eos.CharPos );
		WANT_EQ( 30, eos.BytePos );
		WANT_EQ( "Line #2!", src.str( l2, eos ).stdstr() ) << "Wrong second line!";

		WANT_EXCEPT( src.getPosAtOffset( eos, 1 ), source::EndOfSource ) << "Didn't throw at end of source!";

		auto first = src.getPosAtOffset( next, -1 );
		WANT_EQ( 0, first.Line );
		WANT_EQ( 0, first.PosOnLine );
		WANT_EQ( 0, first.CharPos );
		WANT_EQ( 0, first.BytePos );
		WANT_EQ( 'T', src.chr( first ) );

		auto pos = src.getPosAtOffset( eos, -8 );
		WANT_EQ( 1, pos.Line );
		WANT_EQ( 0, pos.PosOnLine );
		WANT_EQ( 22, pos.CharPos );
		WANT_EQ( 22, pos.BytePos );
		WANT_EQ( 'L', src.chr( pos ) );

		pos = src.getPosAtOffset( l2, -1 );
		WANT_EQ( 0, pos.Line );
		WANT_EQ( 21, pos.PosOnLine );
		WANT_EQ( 21, pos.CharPos );
		WANT_EQ( 21, pos.BytePos );
		WANT_EQ( '\n', src.chr( pos ) );

		pos = src.getPosAtOffset( eos, -9 );
		WANT_EQ( 0, pos.Line );
		WANT_EQ( 21, pos.PosOnLine );
		WANT_EQ( 21, pos.CharPos );
		WANT_EQ( 21, pos.BytePos );

		pos = src.getPosAtOffset( pos, 1 );
		WANT_EQ( 1, pos.Line );
		WANT_EQ( 0, pos.PosOnLine );
		WANT_EQ( 22, pos.CharPos );
		WANT_EQ( 22, pos.BytePos );

		auto raw2 = src.reclaim();
		WANT_EQ( "This is a 2 line test\nLine #2!", raw2.stdstr() )
			<< "Wrong reclaimed string!";
	}
	TEST( Source, basic_file )
	{
		REQUIRE_TRUE( prepareTestFile( "This is a 2 line test\nLine #2!" ) );
		source::File src( TestFile );
		WANT_EQ( TestFile, src.sourceName() ) << "Wrong name!";
		source::Pos beg;
		WANT_EQ( 0, beg.Line );
		WANT_EQ( 0, beg.PosOnLine );
		WANT_EQ( 0, beg.CharPos );
		WANT_EQ( 0, beg.BytePos );
		WANT_EQ( 'T', src.chr( beg ) ) << "Wrong first character!";

		auto next = src.getPosAtOffset( beg, 1 );
		WANT_EQ( 0, next.Line );
		WANT_EQ( 1, next.PosOnLine );
		WANT_EQ( 1, next.CharPos );
		WANT_EQ( 1, next.BytePos );
		WANT_EQ( 'h', src.chr( next ) ) << "Wrong second character!";

		auto eol = src.getPosAtOffset( beg, 21 );
		WANT_EQ( 0, eol.Line );
		WANT_EQ( 21, eol.PosOnLine );
		WANT_EQ( 21, eol.CharPos );
		WANT_EQ( 21, eol.BytePos );
		WANT_EQ( '\n', src.chr( eol ) ) << "Wrong eol character!";
		WANT_EQ( "This is a 2 line test", src.str( beg, eol ).stdstr() ) << "Wrong first line!";

		auto l2 = src.getPosAtOffset( eol, 1 );
		WANT_EQ( 1, l2.Line );
		WANT_EQ( 0, l2.PosOnLine );
		WANT_EQ( 22, l2.CharPos );
		WANT_EQ( 22, l2.BytePos );
		WANT_EQ( 'L', src.chr( l2 ) );

		auto eos = src.getPosAtOffset( l2, 99 );
		WANT_EQ( 1, eos.Line );
		WANT_EQ( 8, eos.PosOnLine );
		WANT_EQ( 30, eos.CharPos );
		WANT_EQ( 30, eos.BytePos );
		WANT_EQ( "Line #2!", src.str( l2, eos ).stdstr() ) << "Wrong second line!";

		WANT_EXCEPT( src.getPosAtOffset( eos, 1 ), source::EndOfSource ) << "Didn't throw at end of source!";

		auto first = src.getPosAtOffset( next, -1 );
		WANT_EQ( 0, first.Line );
		WANT_EQ( 0, first.PosOnLine );
		WANT_EQ( 0, first.CharPos );
		WANT_EQ( 0, first.BytePos );
		WANT_EQ( 'T', src.chr( first ) );

		auto pos = src.getPosAtOffset( eos, -8 );
		WANT_EQ( 1, pos.Line );
		WANT_EQ( 0, pos.PosOnLine );
		WANT_EQ( 22, pos.CharPos );
		WANT_EQ( 22, pos.BytePos );
		WANT_EQ( 'L', src.chr( pos ) );

		pos = src.getPosAtOffset( l2, -1 );
		WANT_EQ( 0, pos.Line );
		WANT_EQ( 21, pos.PosOnLine );
		WANT_EQ( 21, pos.CharPos );
		WANT_EQ( 21, pos.BytePos );
		WANT_EQ( '\n', src.chr( pos ) );

		pos = src.getPosAtOffset( eos, -9 );
		WANT_EQ( 0, pos.Line );
		WANT_EQ( 21, pos.PosOnLine );
		WANT_EQ( 21, pos.CharPos );
		WANT_EQ( 21, pos.BytePos );

		pos = src.getPosAtOffset( pos, 1 );
		WANT_EQ( 1, pos.Line );
		WANT_EQ( 0, pos.PosOnLine );
		WANT_EQ( 22, pos.CharPos );
		WANT_EQ( 22, pos.BytePos );
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

		ref.moveStart( 5 );
		WANT_EQ( "1:6", ref.startStr().stdstr() );
		WANT_EQ( "is a 2 line test\nLine #2!", ref.str().stdstr() );

		ref.end( src.getPosAtOffset( ref.start(), 16 ) );
		WANT_EQ( "is a 2 line test", ref.str().stdstr() );

		ref.moveEnd( 7 );
		ref.moveStart( 12 );
		WANT_EQ( "test\nLine #", ref.str().stdstr() );
	}
	TEST( SourceRef, file )
	{
		REQUIRE_TRUE( prepareTestFile( "This is a 2 line test\nLine #2!" ) );
		source::File src( TestFile );
		WANT_EQ( TestFile, src.sourceName() ) << "Wrong name!";
		source::Ref ref( src );
		WANT_EQ( "1:1", ref.startStr().stdstr() );

		ref.moveStart( 5 );
		WANT_EQ( "1:6", ref.startStr().stdstr() );
		WANT_EQ( "is a 2 line test\nLine #2!", ref.str().stdstr() );

		ref.end( src.getPosAtOffset( ref.start(), 16 ) );
		WANT_EQ( "is a 2 line test", ref.str().stdstr() );

		ref.moveEnd( 7 );
		ref.moveStart( 12 );
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
