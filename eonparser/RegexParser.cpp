#include "RegexParser.h"


namespace eon
{
	namespace parser
	{
		expression::Node RegexParser::parse( State& state )
		{
			return expression::Node::newValue(
				Attribute::newExplicit(
					regex( state.Tokens.viewed().str() ),
					name_regex,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					state.Tokens.viewed().source() ) );
		}
		EON_TEST_2STEP( RegexParser, parse, type_empty,
			TestState state( string( "r\"\"" ) ),
			EON_EQ( "T(regex)", RegexParser().parse( state ).type().str() ) );
		EON_TEST_2STEP( RegexParser, parse, type_nonempty,
			TestState state( string( "r\"\\w+\"" ) ),
			EON_EQ( "T(regex)", RegexParser().parse( state ).type().str() ) );
		EON_TEST_2STEP( RegexParser, parse, value_empty,
			TestState state( string( "r\"\"" ) ),
			EON_EQ( "", RegexParser().parse( state ).value().value<regex>().str() ) );
		EON_TEST_2STEP( RegexParser, parse, value_nonempty,
			TestState state( string( "r\"\\w+\"" ) ),
			EON_EQ( "\\w+", RegexParser().parse( state ).value().value<regex>().str() ) );
	}
}