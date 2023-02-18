#include "Token.h"
#include <eonsource/String.h>


namespace eon
{
#ifdef EON_TEST_MODE
	struct Sample
	{
		Sample()
		{
			Src = source::String( "test", "one two three" );
			Obj = Token( source::Ref( Src, source::Pos( 4, 4, 0, 4 ), source::Pos( 7, 7, 0, 7 ) ), name_name );
		}
		void setEmpty() {
			Obj = Token( source::Ref( Src, source::Pos( 4, 4, 0, 4 ), source::Pos( 4, 4, 0, 4 ) ), name_name ); }
		string str() { return eon::str( Obj.type() ) + "=" + Obj.str(); }
		const Token& cObj() const { return Obj; }

		source::String Src;
		Token Obj;
	};
#endif

	EON_TEST_3STEP( Token, operator_asgn, copy,
		Sample data,
		Token obj = data.Obj,
		EON_EQ( "two", obj.str() ) );

	EON_TEST_3STEP( Token, extend, small,
		Sample data,
		data.Obj.extend( source::Pos( 8, 8, 0, 8 ) ),
		EON_EQ( "name=two ", data.str() ) );
	EON_TEST_3STEP( Token, extend, to_end_of_source,
		Sample data,
		data.Obj.extend( source::Pos( 13, 13, 0, 13 ) ),
		EON_EQ( "name=two three", data.str() ) );
	EON_TEST_2STEP( Token, extend, beyond_end_of_source1,
		Sample data,
		EON_FALSE( data.Obj.extend( source::Pos( 14, 14, 0, 14 ) ) ) );
	EON_TEST_3STEP( Token, extend, beyond_end_of_source2,
		Sample data,
		data.Obj.extend( source::Pos( 14, 14, 0, 14 ) ),
		EON_EQ( "name=two", data.str() ) );

	EON_TEST_3STEP( Token, extendWithNewType, small,
		Sample data,
		data.Obj.extendWithNewType( source::Pos( 8, 8, 0, 8 ), name_digits ),
		EON_EQ( "digits=two ", data.str() ) );
	EON_TEST_3STEP( Token, extendWithNewType, to_end_of_source,
		Sample data,
		data.Obj.extendWithNewType( source::Pos( 13, 13, 0, 13 ), name_digits ),
		EON_EQ( "digits=two three", data.str() ) );
	EON_TEST_2STEP( Token, extendWithNewType, beyond_end_of_source1,
		Sample data,
		EON_FALSE( data.Obj.extendWithNewType( source::Pos( 14, 14, 0, 14 ), name_digits ) ) );
	EON_TEST_3STEP( Token, extendWithNewType, beyond_end_of_source2,
		Sample data,
		data.Obj.extendWithNewType( source::Pos( 14, 14, 0, 14 ), name_digits ),
		EON_EQ( "name=two", data.str() ) );




	EON_TEST_2STEP( Token, operator_bool, true,
		Sample data,
		EON_TRUE( static_cast<bool>( data.Obj ) ) );
	EON_TEST( Token, operator_bool, false,
		EON_FALSE( Token() ) );

	EON_TEST( Token, empty, no_source,
		EON_TRUE( Token().empty() ) );
	EON_TEST_3STEP( Token, empty, empty,
		Sample data,
		data.setEmpty(),
		EON_TRUE( data.Obj.empty() ) );
	EON_TEST_2STEP( Token, empty, not_empty,
		Sample data,
		EON_FALSE( data.Obj.empty() ) );

	EON_TEST_2STEP( Token, source, const_none,
		Token obj,
		EON_FALSE( ( (const Token*)&obj )->source() ) );
	EON_TEST_2STEP( Token, source, const_non_empty1,
		Sample data,
		EON_TRUE( data.cObj().source() ) );
	EON_TEST_2STEP( Token, source, const_non_empty2,
		Sample data,
		EON_EQ( "two", data.cObj().source().str() ) );

	EON_TEST_3STEP( Token, source, modify,
		Sample data,
		data.Obj.source() = source::Ref( data.Src, source::Pos( 0, 0, 0, 0 ), source::Pos( 3, 3, 0, 3 ) ),
		EON_EQ( "one", data.Obj.str() ) );

	EON_TEST_2STEP( Token, type, basic,
		Sample data,
		EON_EQ( name_name, data.Obj.type() ) );

	EON_TEST_2STEP( Token, is, true,
		Sample data,
		EON_TRUE( data.Obj.is( name_name ) ) );
	EON_TEST_2STEP( Token, is, false,
		Sample data,
		EON_FALSE( data.Obj.is( name_digits ) ) );

	EON_TEST_2STEP( Token, match, false,
		Sample data,
		EON_FALSE( data.Obj.match( "one" ) ) );
	EON_TEST_2STEP( Token, match, true,
		Sample data,
		EON_TRUE( data.Obj.match( "two" ) ) );

	EON_TEST_2STEP( Token, startsWith, false,
		Sample data,
		EON_FALSE( data.Obj.startsWith( "th" ) ) );
	EON_TEST_2STEP( Token, startsWith, true,
		Sample data,
		EON_TRUE( data.Obj.startsWith( "tw" ) ) );

	EON_TEST_2STEP( Token, endsWith, false,
		Sample data,
		EON_FALSE( data.Obj.endsWith( "we" ) ) );
	EON_TEST_2STEP( Token, endsWith, true,
		Sample data,
		EON_TRUE( data.Obj.endsWith( "wo" ) ) );

	EON_TEST_2STEP( Token, constainsOnly, false,
		Sample data,
		EON_FALSE( data.Obj.containsOnly( 't' ) ) );
	EON_TEST_3STEP( Token, constainsOnly, true,
		Sample data,
		data.Obj.source() = source::Ref( data.Src, source::Pos( 4, 4, 0, 4 ), source::Pos( 5, 5, 0, 5 ) ),
		EON_TRUE( data.Obj.containsOnly( 't' ) ) );

	EON_TEST_2STEP( Token, str, basic,
		Sample data,
		EON_EQ( "two", data.Obj.str() ) );

	EON_TEST_2STEP( Token, bytes, basic,
		Sample data,
		EON_EQ( "two", data.Obj.bytes() ) );
}
