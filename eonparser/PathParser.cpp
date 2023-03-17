#include "PathParser.h"


namespace eon
{
	namespace parser
	{
		expression::Node PathParser::parse( State& state )
		{
			return expression::Node::newValue(
				Attribute::newExplicit(
					path( state.Tokens.viewed().str() ),
					name_path,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					state.Tokens.viewed().source() ) );
		}
		EON_TEST_2STEP( PathParser, parse, type,
			TestState state( string( "p\"root\"" ) ),
			EON_EQ( "T(path)", PathParser().parse( state ).type().str() ) );
		EON_TEST_2STEP( PathParser, parse, value,
			TestState state( string( "p\"root\"" ) ),
			EON_EQ( "root", PathParser().parse( state ).value().value<path>().str() ) );
	}
}