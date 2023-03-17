#include "FloatParser.h"


namespace eon
{
	namespace parser
	{
		expression::Node FloatParser::parse( State& state )
		{
			const high_t value = state.Tokens.viewed().str().toHighT();
			if( value <= EON_FLOAT_MAX )
				return _floatValue( state, static_cast<flt_t>( value ) );
			else
				return _highValue( state, value );
		}
		EON_TEST_2STEP( FloatParser, parse, type_zero,
			TestState state( string( "0.0" ) ),
			EON_EQ( "T(float)", FloatParser().parse( state ).type().str() ) );
		EON_TEST_2STEP( FloatParser, parse, type_positive,
			TestState state( string( "7.0" ) ),
			EON_EQ( "T(float)", FloatParser().parse( state ).type().str() ) );
		EON_TEST_2STEP( FloatParser, parse, value_zero,
			TestState state( string( "0.0" ) ),
			EON_EQ( 0.0, FloatParser().parse( state ).value().value<flt_t>() ) );
		EON_TEST_2STEP( FloatParser, parse, value_positive,
			TestState state( string( "7.8" ) ),
			EON_EQ( 7.8, FloatParser().parse( state ).value().value<flt_t>() ) );




		expression::Node FloatParser::_floatValue( State& state, flt_t value )
		{
			return expression::Node::newValue(
				Attribute::newExplicit<flt_t>(
					value,
					name_float,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					state.Tokens.viewed().source() ) );
		}
		expression::Node FloatParser::_highValue( State& state, high_t value )
		{
			return expression::Node::newValue(
				Attribute::newExplicit<high_t>(
					value,
					name_high,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					state.Tokens.viewed().source() ) );
		}
	}
}