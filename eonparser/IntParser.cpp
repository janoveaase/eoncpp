#include "IntParser.h"


namespace eon
{
	namespace parser
	{
		expression::Node IntParser::parse( State& state )
		{
			const long_t value = state.Tokens.viewed().str().toLongT();
			if( value <= EON_INT_MAX )
				return _intValue( state, static_cast<int_t>( value ) );
			else
				return _longValue( state, value );
		}
		EON_TEST_2STEP( IntParser, parse, type_zero,
			TestState state( string( "0" ) ),
			EON_EQ( "T(int)", IntParser().parse( state ).type().str() ) );
		EON_TEST_2STEP( IntParser, parse, type_positive,
			TestState state( string( "7" ) ),
			EON_EQ( "T(int)", IntParser().parse( state ).type().str() ) );
		EON_TEST_2STEP( IntParser, parse, type_positive_long,
			TestState state( string( "12345678909876" ) ),
			EON_EQ( "T(long)", IntParser().parse( state ).type().str() ) );
		EON_TEST_2STEP( IntParser, parse, value_zero,
			TestState state( string( "0" ) ),
			EON_EQ( 0, IntParser().parse( state ).value().value<int_t>() ) );
		EON_TEST_2STEP( IntParser, parse, value_positive,
			TestState state( string( "7" ) ),
			EON_EQ( 7, IntParser().parse( state ).value().value<int_t>() ) );
		EON_TEST_2STEP( IntParser, parse, value_positive_long,
			TestState state( string( "12345678909876" ) ),
			EON_EQ( 12345678909876, IntParser().parse( state ).value().value<long_t>() ) );




		expression::Node IntParser::_intValue( State& state, int_t value )
		{
			return expression::Node::newValue(
				Attribute::newExplicit<int_t>(
					value,
					name_int,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					state.Tokens.viewed().source() ) );
		}
		expression::Node IntParser::_longValue( State& state, long_t value )
		{
			return expression::Node::newValue(
				Attribute::newExplicit<long_t>(
					value,
					name_long,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					state.Tokens.viewed().source() ) );
		}
	}
}