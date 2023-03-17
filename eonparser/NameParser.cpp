#include "NameParser.h"


namespace eon
{
	namespace parser
	{
		expression::Node NameParser::parse( State& state )
		{
			auto name = eon::name( state.Tokens.viewed().str().startsWith( "#" )
				? state.Tokens.viewed().str().slice( 1, -1 ) : state.Tokens.viewed().str() );
			return expression::Node::newValue(
				Attribute::newExplicit<name_t>(
					name,
					name_name,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					state.Tokens.viewed().source() ) );
		}
		EON_TEST_2STEP( NameParser, parse, type,
			TestState state( string( "one" ) ),
			EON_EQ( "T(name)", NameParser().parse( state ).type().str() ) );
		EON_TEST_2STEP( NameParser, parse, type_prefixed,
			TestState state( string( "#one" ) ),
			EON_EQ( "T(name)", NameParser().parse( state ).type().str() ) );
		EON_TEST_2STEP( NameParser, parse, value,
			TestState state( string( "one" ) ),
			EON_EQ( "one", eon::str( NameParser().parse( state ).value().value<name_t>() ) ) );
		EON_TEST_2STEP( NameParser, parse, value_prefixed,
			TestState state( string( "#one" ) ),
			EON_EQ( "one", eon::str( NameParser().parse( state ).value().value<name_t>() ) ) );
	}
}