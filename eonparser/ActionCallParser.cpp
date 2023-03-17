#include "ActionCallParser.h"
#include "ExpressionParser.h"
#include "ActionFinder.h"


namespace eon
{
	namespace parser
	{
		expression::Node ActionCallParser::parse( State& state, Tuple& scope )
		{
			auto action_name = eon::name( state.Tokens.viewed().str() );
			auto source = state.Tokens.viewed().source();
			state.Tokens.forward( 2 );		// Skip name + '('
			return _parse( state, scope, action_name, source );
		}
		EON_TEST_2STEP( ActionCallParser, parse, type,
			TestState state( string( "int()" ) ),
			EON_EQ( "T(int)", ActionCallParser().parse( state, scope::global() ).type().str() ) );
		EON_TEST_2STEP( ActionCallParser, parse, name,
			TestState state( string( "int()" ) ),
			EON_EQ( "call", eon::str( ActionCallParser().parse( state, scope::global() ).name() ) ) );
/*		EON_TEST_2STEP( ActionCallParser, parse, type_prefixed,
			State state( string( "#one" ) ),
			EON_EQ( "T(name)", ActionCallParser().parse( state ).type().str() ) );
		EON_TEST_2STEP( ActionCallParser, parse, value,
			State state( string( "one" ) ),
			EON_EQ( "one", eon::str( ActionCallParser().parse( state ).value().value<name_t>() ) ) );
		EON_TEST_2STEP( ActionCallParser, parse, value_prefixed,
			State state( string( "#one" ) ),
			EON_EQ( "one", eon::str( ActionCallParser().parse( state ).value().value<name_t>() ) ) );*/




		expression::Node ActionCallParser::_parse( State& state, Tuple& scope, name_t action_name, source::Ref source )
		{
			auto args = _parseActionCallArgs( state, scope );
			if( state.Tokens.atEnd() )
			{
				state.Report->error( "Action call is not properly ended!", source );
				return expression::Node();
			}

			// If the action name is a type, we have a constructor
			if( scope.isType( action_name ) )
				return _processConstructorCall( state, scope, action_name, args, source );

			// TODO: Add support for other action types!
			state.Report->error( "Action call is not supported!", source );
			return expression::Node();
		}

		ActionCallParser::CallArgs ActionCallParser::_parseActionCallArgs( State& state, Tuple& scope )
		{
			CallArgs args;

			// Each argument is an expression we must evaluate.
			// Read arguments until we see a ')'!
			while( !state.Tokens.atEnd() && !state.Tokens.viewed().is( symbol_close_round ) )
			{
				_parseArgument( state, scope, args );
				if( state.Tokens.viewed().is( symbol_comma ) )
					state.Tokens.forward();
			}


			return args;
		}

		void ActionCallParser::_parseArgument( State& state, Tuple& scope, CallArgs& args )
		{
			static ExpressionParser parser;
			auto arg_name = _parseAttributeName( state );
			auto expression = parser.parse( state, scope, { ",", ")" } );
			if( expression )
			{
				if( arg_name == no_name )
					args.ArgTypes.add( expression.type() );
				else
					args.ArgTypes.set( arg_name, expression.type() );
				args.Args.push_back( std::move( expression ) );
			}
		}

		expression::Node ActionCallParser::_processConstructorCall(
			State& state, Tuple& scope, name_t action_name, CallArgs& args, source::Ref source )
		{
			static ActionFinder action_finder;
			auto constructor_signature = action_finder.find(
				ActionFinder::Details( *state.Report, source ).actionName( name_constructor ).category(
					name_constructor ).typeName( action_name ).args( args.ArgTypes ) );
			if( !constructor_signature )
				return expression::Node();
			auto call = expression::Node::newCall( *scope.action( constructor_signature ), source );
			for( auto& arg : args.Args )
				call.addOperand( arg.relaseRoot() );
			return call;
		}
	}
}