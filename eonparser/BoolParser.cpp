#include "BoolParser.h"


namespace eon
{
	namespace parser
	{
		expression::Node BoolParser::parse( State& state )
		{
			return expression::Node::newValue(
				Attribute::newExplicit(
					state.Tokens.viewed().str() == "true",
					name_bool,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					state.Tokens.viewed().source() ) );
		}
		EON_TEST_2STEP( BoolParser, parse, type,
			TestState state( string( "true" ) ),
			EON_TRUE( BoolParser().parse( state ).type() == name_bool ) );
		EON_TEST_2STEP( BoolParser, parse, value_true,
			TestState state( string( "true" ) ),
			EON_TRUE( BoolParser().parse( state ).value().value<bool>() ) );
		EON_TEST_2STEP( BoolParser, parse, value_false,
			TestState state( string( "false" ) ),
			EON_FALSE( BoolParser().parse( state ).value().value<bool>() ) );
	}
}