#include "ByteParser.h"


namespace eon
{
	namespace parser
	{
		expression::Node ByteParser::parse( State& state )
		{
			return expression::Node::newValue(
				Attribute::newExplicit(
					static_cast<byte_t>( *state.Tokens.viewed().str().begin() ),
					name_byte,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					state.Tokens.viewed().source() ) );
		}
		EON_TEST_2STEP( ByteParser, parse, type,
			TestState state( string( "B'a'" ) ),
			EON_TRUE( ByteParser().parse( state ).type() == name_byte ) );
		EON_TEST_2STEP( ByteParser, parse, value,
			TestState state( string( "B'a'" ) ),
			EON_EQ( 'a', ByteParser().parse( state ).value().value<byte_t>() ) );
	}
}