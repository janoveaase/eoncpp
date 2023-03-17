#include "StringParser.h"


namespace eon
{
	namespace parser
	{
		expression::Node StringParser::parse( State& state )
		{
			return expression::Node::newValue(
				Attribute::newExplicit(
					state.Tokens.viewed().str(),
					name_string,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					state.Tokens.viewed().source() ) );
		}
		EON_TEST_2STEP( StringParser, parse, type_empty,
			TestState state( string( "\"\"" ) ),
			EON_EQ( "T(string)", StringParser().parse( state ).type().str() ) );
		EON_TEST_2STEP( StringParser, parse, type_nonempty,
			TestState state( string( "\"stRIng\"" ) ),
			EON_EQ( "T(string)", StringParser().parse( state ).type().str() ) );
		EON_TEST_2STEP( StringParser, parse, value_empty,
			TestState state( string( "\"\"" ) ),
			EON_EQ( "", StringParser().parse( state ).value().value<string>() ) );
		EON_TEST_2STEP( StringParser, parse, value_nonempty,
			TestState state( string( "\"stRIng\"" ) ),
			EON_EQ( "stRIng", StringParser().parse( state ).value().value<string>() ) );
	}
}