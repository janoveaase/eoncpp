#include "CharParser.h"


namespace eon
{
	namespace parser
	{
		expression::Node CharParser::parse( State& state )
		{
			return expression::Node::newValue(
				Attribute::newExplicit(
					static_cast<char_t>( *state.Tokens.viewed().str().begin() ),
					name_char,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					state.Tokens.viewed().source() ) );
		}
		EON_TEST_2STEP( CharParser, parse, type,
			TestState state( string( "'a'" ) ),
			EON_TRUE( CharParser().parse( state ).type() == name_char ) );
		EON_TEST_2STEP( CharParser, parse, value,
			TestState state( string( "'a'" ) ),
			EON_EQ( 'a', CharParser().parse( state ).value().value<char_t>() ) );
		EON_TEST_2STEP( CharParser, parse, value_utf8,
			TestState state( string( u8"'\u050E'" ) ),
			EON_EQ( char_t( 0x050EU ), CharParser().parse( state ).value().value<char_t>() ) );
	}
}