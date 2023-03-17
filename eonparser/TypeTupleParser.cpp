#include "TypeTupleParser.h"
#include "ExpressionParser.h"


namespace eon
{
	namespace parser
	{
		expression::Node TypeTupleParser::parse( State& state, Tuple& scope )
		{
			source::Ref source = state.Tokens.viewed().source();
			auto ttuple = _parseDetails( state );
			if( !ttuple )
				return expression::Node();
			ttuple.expandSubTuples();
			return expression::Node::newValue(
				Attribute::newTypeTuple( std::move( ttuple ), type::Qualifier::_rvalue, source ) );
		}
		EON_TEST_2STEP( TypeTupleParser, parse, type_empty,
			TestState state( string( "T()" ) ),
			EON_EQ( "T()", TypeTupleParser().parse( state, scope::global() ).type().str() ) );
		EON_TEST_2STEP( TypeTupleParser, parse, type_nonempty,
			TestState state( string( "T(string)" ) ),
			EON_EQ( "T(typetuple)", TypeTupleParser().parse( state, scope::global() ).type().str() ) );
		EON_TEST_2STEP( TypeTupleParser, parse, value_simple,
			TestState state( string( "T(string)" ) ),
			EON_EQ( "T(string)", TypeTupleParser().parse( state, scope::global() ).value().value<TypeTuple>().str() ) );
		EON_TEST_2STEP( TypeTupleParser, parse, value_complex,
			TestState state( string( "T(string, (int, (float, bool), byte), long)" ) ),
			EON_EQ(
				"T(tuple, type=static, value=(string, (tuple, type=static, value=(int, (tuple, type=static, "
				"value=(float, bool)), byte)), long))",
				TypeTupleParser().parse( state, scope::global() ).value().value<TypeTuple>().str() ) );




		TypeTuple TypeTupleParser::_parseDetails( State& state )
		{
			source::Ref source = state.Tokens.viewed().source();
			state.Tokens.forward();	// Skip tuple identifier

			// A type tuple consists of attributes that are either names or parenthesized sub-tuples, and can be either
			// named or unnamed.
			TypeTuple ttuple;
			_parseAttributes( state, ttuple );

			if( state.Tokens.atEnd() )
			{
				state.Report->error( "Type tuple is not properly ended!", source );
				return TypeTuple();
			}

			return ttuple;
		}

		void TypeTupleParser::_parseAttributes( State& state, TypeTuple& ttuple )
		{
			while( !state.Tokens.atEnd() && !state.Tokens.viewed().is( symbol_close_round ) )
			{
				auto attrib_name = _parseAttributeName( state );
				_parseAttributeValue( state, attrib_name, ttuple );

				if( state.Tokens.atEnd() )
					break;
				if( state.Tokens.viewed().is( symbol_comma ) )
					state.Tokens.forward();
				else if( !state.Tokens.viewed().is( symbol_close_round ) )
					state.Report->error( "Expected comma or ')' here!", state.Tokens.viewed().source() );
			}
		}

		void TypeTupleParser::_parseAttributeValue( State& state, name_t attrib_name, TypeTuple& ttuple )
		{
			// Do we have a sub-tuple value?
			if( state.Tokens.viewed().is( symbol_open_round ) )
				_parseTupleAttributeValue( state, attrib_name, ttuple );

			// Must be a name
			else if( state.Tokens.viewed().is( name_name ) )
			{
				if( attrib_name == no_name )
					ttuple.add( eon::name( state.Tokens.viewed().str() ) );
				else
					ttuple.set( attrib_name, eon::name( state.Tokens.viewed().str() ) );
				state.Tokens.forward();
			}

			else
			{
				state.Report->error( "Invalid type tuple attribute!", state.Tokens.viewed().source() );
				state.Tokens.forward();
			}
		}

		void TypeTupleParser::_parseTupleAttributeValue( State& state, name_t attrib_name, TypeTuple& ttuple )
		{
			auto sub = _parseDetails( state );
			if( sub )
			{
				sub.expandSubTuples();
				if( attrib_name == no_name )
					ttuple.add( std::move( sub ) );
				else
					ttuple.set( attrib_name, std::move( sub ) );
				state.Tokens.forward();
			}
		}
	}
}