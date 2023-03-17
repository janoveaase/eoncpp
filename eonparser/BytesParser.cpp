#include "BytesParser.h"


namespace eon
{
	namespace parser
	{
		expression::Node BytesParser::parse( State& state )
		{
			return expression::Node::newValue(
				Attribute::newExplicit(
					state.Tokens.viewed().bytes(),
					name_bytes,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					state.Tokens.viewed().source() ) );
		}
		EON_TEST_2STEP( BytesParser, parse, type_empty,
			TestState state( string( "B\"\"" ) ),
			EON_EQ( "T(bytes)", BytesParser().parse( state ).type().str() ) );
		EON_TEST_2STEP( BytesParser, parse, type_nonempty,
			TestState state( string( "B\"byTes\"" ) ),
			EON_EQ( "T(bytes)", BytesParser().parse( state ).type().str() ) );
		EON_TEST_2STEP( BytesParser, parse, value_empty,
			TestState state( string( "B\"\"" ) ),
			EON_EQ( "", BytesParser().parse( state ).value().value<std::string>() ) );
		EON_TEST_2STEP( BytesParser, parse, value_nonempty,
			TestState state( string( "B\"byTes\"" ) ),
			EON_EQ( "byTes", BytesParser().parse( state ).value().value<std::string>() ) );
	}
}