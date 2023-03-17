#include "NamePathParser.h"


namespace eon
{
	namespace parser
	{
		expression::Node NamePathParser::parse( State& state )
		{
			return expression::Node::newValue(
				Attribute::newExplicit(
					namepath( state.Tokens.viewed().str().slice( 1, -1 ) ),
					name_namepath,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					state.Tokens.viewed().source() ) );
		}
		EON_TEST_2STEP( NamePathParser, parse, type_basic,
			TestState state( string( "@one" ) ),
			EON_EQ( "T(namepath)", NamePathParser().parse( state ).type().str() ) );
		EON_TEST_2STEP( NamePathParser, parse, type_long,
			TestState state( string( "@one/two/three" ) ),
			EON_EQ( "T(namepath)", NamePathParser().parse( state ).type().str() ) );
		EON_TEST_2STEP( NamePathParser, parse, value_basic,
			TestState state( string( "@one" ) ),
			EON_EQ( "@one", NamePathParser().parse( state ).value().value<namepath>().str() ) );
		EON_TEST_2STEP( NamePathParser, parse, value_long,
			TestState state( string( "@one/two/three" ) ),
			EON_EQ( "@one/two/three", NamePathParser().parse( state ).value().value<namepath>().str() ) );
	}
}