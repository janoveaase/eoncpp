#include "SourceRef.h"
#include "String.h"


namespace eon
{
	namespace source
	{
#ifdef EON_TEST_MODE
		struct TestData
		{
			TestData( eon::string text ) { Src = String( "test", std::move( text ) ); Obj = Ref( Src ); }
			TestData( eon::string text, Pos start ) { Src = String( "test", std::move( text ) ); Obj = Ref( Src, start ); }
			TestData( eon::string text, Pos start, Pos end ) {
				Src = String( "test", std::move( text ) ); Obj = Ref( Src, start, end ); }
			String Src;
			Ref Obj;
		};
#endif

		EON_TEST_2STEP( Ref, Ref, empty,
			TestData data( "" ),
			EON_EQ( "", data.Obj.str() ) );
		EON_TEST_2STEP( Ref, Ref, non_empty,
			TestData data( "one two three" ),
			EON_EQ( "one two three", data.Obj.str() ) );
		EON_TEST_2STEP( Ref, Ref, start,
			TestData data( "one two three", Pos( 4, 4, 0, 4 ) ),
			EON_EQ( "two three", data.Obj.str() ) );
		EON_TEST_2STEP( Ref, Ref, start_end,
			TestData data( "one two three", Pos( 4, 4, 0, 4 ), Pos( 7, 7, 0, 7 ) ),
			EON_EQ( "two", data.Obj.str() ) );




		EON_TEST_2STEP( Ref, operator_asgn, copy,
			TestData data( "one two three" ),
			EON_EQ( "one two three", ( Ref() = data.Obj ).str() ) );

		EON_TEST_2STEP( Ref, operator_asgn, raw,
			String source( "test", "one two three" ),
			EON_EQ( "one two three", ( Ref() = source ).str() ) );

		bool Ref::moveStart( int num_chars ) noexcept
		{
			Pos orig_start = Start;
			try
			{
				Start = Source->getPosAtOffset( Start, num_chars );
				if( End < Start )
					End = Start;
			}
			catch( ... )
			{
			}
			return Start != orig_start;
		}
		EON_TEST_3STEP( Ref, moveStart, one_forward,
			TestData data( "one two three" ),
			data.Obj.moveStart( 1 ),
			EON_EQ( "ne two three", data.Obj.str() ) );
		EON_TEST_3STEP( Ref, moveStart, one_backward,
			TestData data( "one two three", Pos( 13, 13, 0, 13 ) ),
			data.Obj.moveStart( -1 ),
			EON_EQ( "e", data.Obj.str() ) );
		EON_TEST_3STEP( Ref, moveStart, many_forward,
			TestData data( "one two three" ),
			data.Obj.moveStart( 10 ),
			EON_EQ( "ree", data.Obj.str() ) );
		EON_TEST_3STEP( Ref, moveStart, many_backward,
			TestData data( "one two three", Pos( 13, 13, 0, 13 ) ),
			data.Obj.moveStart( -10 ),
			EON_EQ( " two three", data.Obj.str() ) );

		bool Ref::moveStartToStartOfLine() noexcept
		{
			if( Start.BytePos == 0 )
				return false;
			Pos orig_start = Start;
			Pos beg;
			try
			{
				auto start = Source->getPosAtOffset( Start, -1 );
				while( true )
				{
					if( Source->chr( start ) != NewlineChr )
					{
						Start = start;
						if( Start.BytePos == 0 )
							break;
						start = Source->getPosAtOffset( start, -1 );
					}
					else
						break;
				}
			}
			catch( ... )
			{
			}
			return Start != orig_start;
		}
		EON_TEST_3STEP( Ref, moveStartToStartOfLine, singleton,
			TestData data( "one two three", Pos( 4, 4, 0, 4 ) ),
			data.Obj.moveStartToStartOfLine(),
			EON_EQ( "one two three", data.Obj.str() ) );
		EON_TEST_3STEP( Ref, moveStartToStartOfLine, multiple,
			TestData data( "one\ntwo\nthree", Pos( 6, 6, 1, 2 ) ),
			data.Obj.moveStartToStartOfLine(),
			EON_EQ( "two\nthree", data.Obj.str() ) );

		bool Ref::moveEnd( int num_chars ) noexcept
		{
			Pos orig_end = End;
			try
			{
				End = Source->getPosAtOffset( End, num_chars );
				if( Start > End )
					Start = End;
			}
			catch( ... )
			{
			}
			return End != orig_end;
		}
		EON_TEST_3STEP( Ref, moveEnd, one_forward,
			TestData data( "one two three", Pos( 0, 0, 0, 0 ), Pos( 7, 7, 0, 7 ) ),
			data.Obj.moveEnd( 1 ),
			EON_EQ( "one two ", data.Obj.str() ) );
		EON_TEST_3STEP( Ref, moveEnd, one_backward,
			TestData data( "one two three", Pos( 0, 0, 0, 0 ), Pos( 7, 7, 0, 7 ) ),
			data.Obj.moveEnd( -1 ),
			EON_EQ( "one tw", data.Obj.str() ) );
		EON_TEST_3STEP( Ref, moveEnd, many_forward,
			TestData data( "one two three", Pos( 0, 0, 0, 0 ), Pos( 1, 1, 0, 1 ) ),
			data.Obj.moveEnd( 10 ),
			EON_EQ( "one two thr", data.Obj.str() ) );
		EON_TEST_3STEP( Ref, moveEnd, many_backward,
			TestData data( "one two three", Pos( 0, 0, 0, 0), Pos( 13, 13, 0, 13 ) ),
			data.Obj.moveEnd( -10 ),
			EON_EQ( "one", data.Obj.str() ) );

		bool Ref::moveEndToEndOfLine() noexcept
		{
			if( Source->chr( End ) == NewlineChr )
				return false;
			Pos orig_end = End;
			try
			{
				while( true )
				{
					End = Source->getPosAtOffset( End, 1 );
					if( Source->afterLast( End ) )
						break;
					if( Source->chr( End ) == NewlineChr )
						break;
				}
			}
			catch( ... )
			{
			}
			return End != orig_end;
		}
		EON_TEST_3STEP( Ref, moveEndToEndOfLine, singleton,
			TestData data( "one two three", Pos( 0, 0, 0, 0 ), Pos( 4, 4, 0, 4 ) ),
			data.Obj.moveEndToEndOfLine(),
			EON_EQ( "one two three", data.Obj.str() ) );
		EON_TEST_3STEP( Ref, moveEndToEndOfLine, multiple,
			TestData data( "one\ntwo\nthree", Pos( 0, 0, 0, 0 ), Pos( 6, 6, 1, 2 ) ),
			data.Obj.moveEndToEndOfLine(),
			EON_EQ( "one\ntwo", data.Obj.str() ) );
	}
}
