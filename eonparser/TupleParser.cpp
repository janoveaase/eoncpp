#include "TupleParser.h"
#include "ExpressionParser.h"


namespace eon
{
	namespace parser
	{
		expression::Node TupleParser::parse( State& state, Tuple& scope )
		{
			auto type = state.Tokens.viewed().type();
			source::Ref source = state.Tokens.viewed().source();
			state.Tokens.forward();	// Skip tuple identifier

			auto attributes = _parseAttributes( state, scope );
			if( state.Tokens.atEnd() )
			{
				state.Report->error( "Tuple is not properly ended!", source );
				return expression::Node();
			}

			auto tuple = eon::tuple::newCustom( type, std::move( attributes ) );
			return expression::Node::newValue(
				Attribute::newTuple( std::move( tuple ), type::Qualifier::_rvalue, source ) );
		}
/*		EON_TEST_2STEP( TupleParser, parse, type_empty,
			State state( string( "T()" ) ),
			EON_EQ( "T(bytes)", TupleParser().parse( state ).type().str() ) );
/*		EON_TEST_2STEP( TupleParser, parse, type_nonempty,
			State state( string( "B\"byTes\"" ) ),
			EON_EQ( "T(bytes)", TupleParser().parse( state ).type().str() ) );
		EON_TEST_2STEP( TupleParser, parse, value_empty,
			State state( string( "B\"\"" ) ),
			EON_EQ( "", TupleParser().parse( state ).value().value<std::string>() ) );
		EON_TEST_2STEP( TupleParser, parse, value_nonempty,
			State state( string( "B\"byTes\"" ) ),
			EON_EQ( "byTes", TupleParser().parse( state ).value().value<std::string>() ) );*/




		std::vector<AttributePair> TupleParser::_parseAttributes( State& state, Tuple& scope )
		{
			std::vector<AttributePair> attributes;

			// Each attribute is an expression we must evaluate.
			// Read attributes until we see a ')'!
			while( !state.Tokens.atEnd() && !state.Tokens.viewed().is( symbol_close_round ) )
			{
				_parseAttribute( state, scope, attributes );
				if( state.Tokens.viewed().is( symbol_comma ) )
					state.Tokens.forward();
			}

			return attributes;
		}

		void TupleParser::_parseAttribute( State& state, Tuple & scope, std::vector<AttributePair>& attributes )
		{
			static ExpressionParser parser;
			auto attrib_name = _parseAttributeName( state );
			auto root = parser.parseNode( state, scope, { ",", ")" } );
			if( root )
				attributes.push_back( AttributePair( attrib_name, std::move( root.value() ) ) );
		}
	}
}