#include "TokenParser.h"
#include <eonsource/String.h>
#include "Tokenizer.h"


namespace eon
{
#ifdef EON_TEST_MODE
	struct TestParser
	{
		TestParser( size_t view = 3 )
		{
			Src = source::String( "test", "one 23 for 6" );
			_init();
			Obj.setView( view );
		}
		TestParser( string input_source, size_t view = 0 )
		{
			Src = source::String( "test", std::move( input_source ) );
			_init();
			Obj.setView( view );
		}
		void _init()
		{
			Tokenizer tokenizer;
			tokenizer.registerCharSequenceAsToken( charcat::letter, name_letters );
			tokenizer.registerCharSequenceAsToken( charcat::number_ascii_digit, name_digits );
			tokenizer.registerSingleCharAsToken( ' ', name_space );
			Obj = TokenParser( tokenizer( source::Ref( Src ) ) );
		}
		source::String Src;
		TokenParser Obj;
	};
#endif



	EON_TEST_3STEP( TokenParser, operator_asgn, move1,
		TestParser old,
		TokenParser obj = std::move( old.Obj ),
		EON_TRUE( old.Obj.atEnd() ) );
	EON_TEST_3STEP( TokenParser, operator_asgn, move2,
		TestParser old,
		TokenParser obj = std::move( old.Obj ),
		EON_FALSE( obj.atEnd() ) );

	EON_TEST_3STEP( TokenParser, reclaim, basic,
		TestParser obj,
		auto reclaimed = obj.Obj.reclaim(),
		EON_TRUE( obj.Obj.atEnd() ) );

	EON_TEST_2STEP( TokenParser, forward, default1,
		TestParser obj,
		EON_TRUE( obj.Obj.forward() ) );
	EON_TEST_3STEP( TokenParser, forward, default2,
		TestParser obj,
		obj.Obj.forward(),
		EON_EQ( 4, obj.Obj.viewedPos() ) );
	EON_TEST_2STEP( TokenParser, forward, many1,
		TestParser obj,
		EON_TRUE( obj.Obj.forward( 3 ) ) );
	EON_TEST_3STEP( TokenParser, forward, many2,
		TestParser obj,
		obj.Obj.forward( 3 ),
		EON_EQ( 6, obj.Obj.viewedPos() ) );
	EON_TEST_2STEP( TokenParser, forward, too_many1,
		TestParser obj,
		EON_FALSE( obj.Obj.forward( 5 ) ) );
	EON_TEST_3STEP( TokenParser, forward, too_many2,
		TestParser obj,
		obj.Obj.forward( 5 ),
		EON_EQ( 3, obj.Obj.viewedPos() ) );

	EON_TEST_2STEP( TokenParser, backward, default1,
		TestParser obj,
		EON_TRUE( obj.Obj.backward() ) );
	EON_TEST_3STEP( TokenParser, backward, default2,
		TestParser obj,
		obj.Obj.backward(),
		EON_EQ( 2, obj.Obj.viewedPos() ) );
	EON_TEST_2STEP( TokenParser, backward, many1,
		TestParser obj,
		EON_TRUE( obj.Obj.backward( 3 ) ) );
	EON_TEST_3STEP( TokenParser, backward, many2,
		TestParser obj,
		obj.Obj.backward( 3 ),
		EON_EQ( 0, obj.Obj.viewedPos() ) );
	EON_TEST_2STEP( TokenParser, backward, too_many1,
		TestParser obj,
		EON_FALSE( obj.Obj.backward( 5 ) ) );
	EON_TEST_3STEP( TokenParser, backward, too_many2,
		TestParser obj,
		obj.Obj.backward( 5 ),
		EON_EQ( 3, obj.Obj.viewedPos() ) );

	EON_TEST_2STEP( TokenParser, forwardIf, true1,
		TestParser obj,
		EON_TRUE( obj.Obj.forwardIf( name_space ) ) );
	EON_TEST_3STEP( TokenParser, forwardIf, true2,
		TestParser obj,
		obj.Obj.forwardIf( name_space ),
		EON_EQ( 4, obj.Obj.viewedPos() ) );
	EON_TEST_2STEP( TokenParser, forwardIf, false1,
		TestParser obj,
		EON_FALSE( obj.Obj.forwardIf( name_name ) ) );
	EON_TEST_3STEP( TokenParser, forwardIf, false2,
		TestParser obj,
		obj.Obj.forwardIf( name_name ),
		EON_EQ( 3, obj.Obj.viewedPos() ) );

	EON_TEST_3STEP( TokenParser, moveToEol, single_line,
		TestParser obj,
		obj.Obj.moveToEol(),
		EON_EQ( 7, obj.Obj.viewedPos() ) );
	EON_TEST_3STEP( TokenParser, moveToEol, multiline,
		TestParser obj( "First line\nLine no 2\nLast line", 6 ),
		obj.Obj.moveToEol(),
		EON_EQ( 9, obj.Obj.viewedPos() ) );

	EON_TEST_3STEP( TokenParser, movePastEol, multiline,
		TestParser obj( "First line\nLine no 2\nLast line", 6 ),
		obj.Obj.movePastEol(),
		EON_EQ( 10, obj.Obj.viewedPos() ) );

	EON_TEST_2STEP( TokenParser, setView, start1,
		TestParser obj,
		EON_TRUE( obj.Obj.setView( 0 ) ) );
	EON_TEST_3STEP( TokenParser, setView, start2,
		TestParser obj,
		obj.Obj.setView( 0 ),
		EON_EQ( 0, obj.Obj.viewedPos() ) );
	EON_TEST_2STEP( TokenParser, setView, end1,
		TestParser obj,
		EON_TRUE( obj.Obj.setView( 6 ) ) );
	EON_TEST_3STEP( TokenParser, setView, end2,
		TestParser obj,
		obj.Obj.setView( 6 ),
		EON_EQ( 6, obj.Obj.viewedPos() ) );
	EON_TEST_2STEP( TokenParser, setView, wild1,
		TestParser obj,
		EON_FALSE( obj.Obj.setView( 10 ) ) );
	EON_TEST_3STEP( TokenParser, setView, wild2,
		TestParser obj,
		obj.Obj.setView( 10 ),
		EON_EQ( 3, obj.Obj.viewedPos() ) );




	EON_TEST_2STEP( TokenParser, operator_bool, true,
		TestParser obj,
		EON_FALSE( obj.Obj.atEnd() ) );
	EON_TEST_3STEP( TokenParser, operator_bool, false,
		TestParser obj( 7 ),
		obj.Obj.Tokens.erase( obj.Obj.Tokens.begin() + 6 ),
		EON_TRUE( obj.Obj.atEnd() ) );

	EON_TEST_2STEP( TokenParser, viewed, basic,
		TestParser obj,
		EON_EQ( " ", obj.Obj.viewed().str() ) );

	EON_TEST_2STEP( TokenParser, exists, ahead_ok,
		TestParser obj,
		EON_TRUE( obj.Obj.exists( 2 ) ) );
	EON_TEST_2STEP( TokenParser, exists, ahead_too_far,
		TestParser obj,
		EON_FALSE( obj.Obj.exists( 5 ) ) );
	EON_TEST_2STEP( TokenParser, exists, behind_ok,
		TestParser obj,
		EON_TRUE( obj.Obj.exists( -2 ) ) );
	EON_TEST_2STEP( TokenParser, exists, behind_too_far,
		TestParser obj,
		EON_FALSE( obj.Obj.exists( -4 ) ) );

	EON_TEST_2STEP( TokenParser, peekAhead, short,
		TestParser obj,
		EON_EQ( "for", obj.Obj.peekAhead().str() ) );
	EON_TEST_2STEP( TokenParser, peekAhead, long,
		TestParser obj,
		EON_EQ( "6", obj.Obj.peekAhead( 3 ).str() ) );

	EON_TEST_2STEP( TokenParser, peekBehind, short,
		TestParser obj,
		EON_EQ( "23", obj.Obj.peekBehind().str() ) );
	EON_TEST_2STEP( TokenParser, peekBehind, long,
		TestParser obj,
		EON_EQ( "one", obj.Obj.peekBehind( 3 ).str() ) );

	EON_TEST_2STEP( TokenParser, peek, first,
		TestParser obj,
		EON_EQ( "one", obj.Obj.peek( 0 ).str() ) );
	EON_TEST_2STEP( TokenParser, peek, last,
		TestParser obj,
		EON_EQ( "6", obj.Obj.peek( 6 ).str() ) );

	EON_TEST_2STEP( TokenParser, last, basic,
		TestParser obj,
		EON_EQ( "6", obj.Obj.last().str() ) );

	size_t TokenParser::lineStart() const
	{
		for( size_t i = View; i > 0; --i )
		{
			if( peek( i - 1 ).is( name_newline ) )
				return i;
		}
		return 0;
	}
	EON_TEST_2STEP( TokenParser, lineStart, singleton,
		TestParser obj,
		EON_EQ( 0, obj.Obj.lineStart() ) );
	EON_TEST_2STEP( TokenParser, lineStart, multline,
		TestParser obj( "First line\nLine 2\nLine three", 5 ),
		EON_EQ( 4, obj.Obj.lineStart() ) );

	EON_TEST_2STEP( TokenParser, viewedPos, basic,
		TestParser obj,
		EON_EQ( 3, obj.Obj.viewedPos() ) );

	EON_TEST_2STEP( TokenParser, size, basic,
		TestParser obj,
		EON_EQ( 7, obj.Obj.size() ) );
}
