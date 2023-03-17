#include "ExpressionParser.h"
#include "BoolParser.h"
#include "ByteParser.h"
#include "CharParser.h"
#include "IntParser.h"
#include "FloatParser.h"
#include "BytesParser.h"
#include "StringParser.h"
#include "RegexParser.h"
#include "NamePathParser.h"
#include "PathParser.h"
#include "NameParser.h"
#include "TupleParser.h"
#include "TypeTupleParser.h"
#include "ActionCallParser.h"
#include "ActionFinder.h"
#ifdef EON_TEST_MODE
#	include <eontypesystem/TupleFactory.h>
#endif


namespace eon
{
	namespace parser
	{
		Expression ExpressionParser::parse( parser::State& state, Tuple& scope, std::set<string> terminate_on )
		{
			ParserData data( state, scope, terminate_on );
			if( _parseExpressionDetails( data ) )
				return Expression( std::move( data.Operands.top() ) );
			return Expression();
		}

		expression::Node ExpressionParser::parseNode( parser::State& state, Tuple& scope, std::set<string> terminate_on )
		{
			ParserData data( state, scope, terminate_on );
			if( _parseExpressionDetails( data ) )
				return std::move( data.Operands.top() );
			return expression::Node();
		}




		bool ExpressionParser::_parseExpressionDetails( ParserData& data )
		{
			while( !data.endOfSmallExpression() )
			{
				if( !_parseDetails( data ) )
					break;
			}
			_finishExpressionDetails( data );
			return !data.Operands.empty();
		}

		bool ExpressionParser::_parseDetails( ParserData& data )
		{
			if( _parseLiteral( data ) )
			{
				data.LastSeen = Seen::_operand;
				data.State->Tokens.forward();
			}
			else if( _parseOperator( data ) )
				data.State->Tokens.forward();
			else if( _parseTuple( data ) )
				data.State->Tokens.forward();
			else if( _parseName( data ) )
				data.State->Tokens.forward();
			else if( _parseExpression( data ) )
				data.State->Tokens.forward();
			else
			{
				_reportUnknownElementError( data );
				return false;
			}
			return true;
		}

#ifdef EON_TEST_MODE
		struct TestData
		{
			TestData( string source )
			{
				TestState::resetGlobal();
				Scope->add( name( "a" ), 100 );
				Report.defaultTargets();
				State = parser::State( std::move( source ), Report );
				Data = std::make_shared< ExpressionParser::ParserData>( State, *Scope, std::set<string>() );
			}
			parser::State State;
			source::Reporter Report;
			Tuple* Scope = &scope::global();
			std::shared_ptr<ExpressionParser::ParserData> Data;
			ExpressionParser Obj;
		};
#endif

		bool ExpressionParser::_parseLiteral( ParserData& data )
		{
			auto type = data.State->Tokens.viewed().type();
			if( type == name_bool )
				return _parseBoolLiteral( data );
			else if( type == name_byte )
				return _parseByteLiteral( data );
			else if( type == name_char )
				return _parseCharLiteral( data );
			else if( type == name_int )
				return _parseIntLiteral( data );
			else if( type == name_float )
				return _parseFloatLiteral( data );
			else if( type == name_digits )
				return _parseIntLiteral( data );
			else if( type == name_bytes )
				return _parseBytesLiteral( data );
			else if( type == name_string )
				return _parseStringLiteral( data );
			else if( type == name_regex )
				return _parseRegexLiteral( data );
			else if( type == name_namepath )
				return _parseNamepathLiteral( data );
			else if( type == name_path )
				return _parsePathLiteral( data );
			return false;
		}

		bool ExpressionParser::_parseBoolLiteral( ParserData& data )
		{
			static BoolParser parser;
			data.Operands.push( parser.parse( *data.State ) );
			return true;
		}
		EON_TEST_3STEP( ExpressionParser, _parseBoolLiteral, basic,
			TestData data( "true" ),
			data.Obj._parseBoolLiteral( *data.Data ),
			EON_TRUE( data.Data->Operands.top().value().value<bool>() ) );

		bool ExpressionParser::_parseByteLiteral( ParserData& data )
		{
			static ByteParser parser;
			data.Operands.push( parser.parse( *data.State ) );
			return true;
		}
		EON_TEST_3STEP( ExpressionParser, _parseByteLiteral, basic,
			TestData data( "B'b'" ),
			data.Obj._parseByteLiteral( *data.Data ),
			EON_EQ( 'b', data.Data->Operands.top().value().value<byte_t>() ) );

		bool ExpressionParser::_parseCharLiteral( ParserData& data )
		{
			static CharParser parser;
			data.Operands.push( parser.parse( *data.State ) );
			return true;
		}
		EON_TEST_3STEP( ExpressionParser, _parseCharLiteral, basic,
			TestData data( "'b'" ),
			data.Obj._parseCharLiteral( *data.Data ),
			EON_EQ( 'b', data.Data->Operands.top().value().value<char_t>() ) );

		bool ExpressionParser::_parseIntLiteral( ParserData& data )
		{
			static IntParser parser;
			data.Operands.push( parser.parse( *data.State ) );
			return true;
		}
		EON_TEST_3STEP( ExpressionParser, _parseIntLiteral, basic,
			TestData data( "345" ),
			data.Obj._parseIntLiteral( *data.Data ),
			EON_EQ( 345, data.Data->Operands.top().value().value<int_t>() ) );

		bool ExpressionParser::_parseFloatLiteral( ParserData& data )
		{
			static FloatParser parser;
			data.Operands.push( parser.parse( *data.State ) );
			return true;
		}
		EON_TEST_3STEP( ExpressionParser, _parseFloatLiteral, basic,
			TestData data( "345.678" ),
			data.Obj._parseFloatLiteral( *data.Data ),
			EON_RANGE( 345.677, data.Data->Operands.top().value().value<flt_t>(), 345.679 ) );

		bool ExpressionParser::_parseBytesLiteral( ParserData& data )
		{
			static BytesParser parser;
			data.Operands.push( parser.parse( *data.State ) );
			return true;
		}
		EON_TEST_3STEP( ExpressionParser, _parseBytesLiteral, basic,
			TestData data( "B\"byTes\"" ),
			data.Obj._parseBytesLiteral( *data.Data ),
			EON_EQ( "byTes", data.Data->Operands.top().value().value<std::string>() ) );

		bool ExpressionParser::_parseStringLiteral( ParserData& data )
		{
			static StringParser parser;
			data.Operands.push( parser.parse( *data.State ) );
			return true;
		}
		EON_TEST_3STEP( ExpressionParser, _parseStringLiteral, basic,
			TestData data( "\"stRIng\"" ),
			data.Obj._parseStringLiteral( *data.Data ),
			EON_EQ( "stRIng", data.Data->Operands.top().value().value<string>() ) );

		bool ExpressionParser::_parseRegexLiteral( ParserData& data )
		{
			static RegexParser parser;
			data.Operands.push( parser.parse( *data.State ) );
			return true;
		}
		EON_TEST_3STEP( ExpressionParser, _parseRegexLiteral, basic,
			TestData data( "r\"\\w+\"" ),
			data.Obj._parseRegexLiteral( *data.Data ),
			EON_EQ( "\\w+", data.Data->Operands.top().value().value<regex>().str() ) );

		bool ExpressionParser::_parseNamepathLiteral( ParserData& data )
		{
			static NamePathParser parser;
			data.Operands.push( parser.parse( *data.State ) );
			return true;
		}
		EON_TEST_3STEP( ExpressionParser, _parseNamepathLiteral, basic,
			TestData data( "@one/two" ),
			data.Obj._parseNamepathLiteral( *data.Data ),
			EON_EQ( "@one/two", data.Data->Operands.top().value().value<namepath>().str() ) );

		bool ExpressionParser::_parsePathLiteral( ParserData& data )
		{
			static PathParser parser;
			data.Operands.push( parser.parse( *data.State ) );
			return true;
		}
		EON_TEST_3STEP( ExpressionParser, _parsePathLiteral, basic,
			TestData data( "p\"dir/file.txt\"" ),
			data.Obj._parsePathLiteral( *data.Data ),
			EON_EQ( "dir/file.txt", data.Data->Operands.top().value().value<path>().str() ) );

		bool ExpressionParser::_parseNameLiteral( ParserData& data )
		{
			static NameParser parser;
			data.Operands.push( parser.parse( *data.State ) );
			return true;
		}
		EON_TEST_3STEP( ExpressionParser, _parseNameLiteral, basic,
			TestData data( "hello" ),
			data.Obj._parseNameLiteral( *data.Data ),
			EON_EQ( "hello", eon::str( data.Data->Operands.top().value().value<name_t>() ) ) );

		bool ExpressionParser::_parseName( ParserData& data )
		{
			if( data.State->Tokens.viewed().type() == name_literal )
				return _parseNameLiteral( data );
			else if( data.State->Tokens.viewed().type() != name_name )
				return false;
			else
				return _parseName( data, eon::name( data.State->Tokens.viewed().str() ) );
		}
		EON_TEST_3STEP( ExpressionParser, _parseName, implicit_name,
			TestData data( "hello" ),
			data.Obj._parseName( *data.Data ),
			EON_EQ( "hello", eon::str( data.Data->Operands.top().value().value<name_t>() ) ) );
		EON_TEST_3STEP( ExpressionParser, _parseName, explicit_name,
			TestData data( "#hello" ),
			data.Obj._parseName( *data.Data ),
			EON_EQ( "hello", eon::str( data.Data->Operands.top().value().value<name_t>() ) ) );
		EON_TEST_3STEP( ExpressionParser, _parseName, call_nonexisting,
			TestData data( "hello()" ),
			data.Obj._parseName( *data.Data ),
			EON_TRUE( data.Data->Operands.empty() ) );
		EON_TEST_3STEP( ExpressionParser, _parseName, call_constructor,
			TestData data( "int()" ),
			data.Obj._parseName( *data.Data ),
			EON_EQ( "int()", Expression( std::move( data.Data->Operands.top() ) ).str() ) );
		EON_TEST_3STEP( ExpressionParser, _parseName, call_operator,
			TestData data( "mod" ),
			data.Obj._parseName( *data.Data ),
			EON_EQ( "mod", eon::str( data.Data->Operators.top().Op ) ) );
		EON_TEST_3STEP( ExpressionParser, _parseName, variable_name,
			TestData data( "a" ),
			data.Obj._parseName( *data.Data ),
			EON_EQ( "a", eon::str( data.Data->Operands.top().name() ) ) );
		EON_TEST_3STEP( ExpressionParser, _parseName, variable_type,
			TestData data( "a" ),
			data.Obj._parseName( *data.Data ),
			EON_EQ( "T(int)", data.Data->Operands.top().type().str() ) );


		bool ExpressionParser::_parseName( ParserData& data, name_t name )
		{
			// NOTE: The name can be: a variable, a name value, an operator or part of an operator, or an action call.

			// If the name is followed by a '(' it must be an action call!
			if( data.State->Tokens.exists() && data.State->Tokens.peekAhead().is( symbol_open_round ) )
				return _parseActionCall( data, name );

			if( type::Operators::isOperator( name ) )
				return _processOperator( data, name, data.State->Tokens.viewed().source() );

			// If followed by ' or " it must be a misspelled prefix
			if( data.State->Tokens.exists()
				&& ( data.State->Tokens.peekAhead().is( name_string ) || data.State->Tokens.peekAhead().is( name_char ) ) )
				return _misspelledPrefixError( data, name );

			// Can be a variable
			auto var = data.Scope->findAttributeIncludeParent( name );
			if( var )
				return _processVariable( data, *var, name );

			// Assuming name value!
			data.State->Tokens.backward();
			return _parseNameLiteral( data );
		}

		bool ExpressionParser::_parseActionCall( ParserData& data, name_t action_name )
		{
			static ActionCallParser parser;
			return _addOperand(  data, parser.parse( *data.State, *data.Scope ) );
		}

		bool ExpressionParser::_misspelledPrefixError( ParserData& data, name_t prefix )
		{
			static std::map<name_t, string> suggestions{
				{ eon::name( "b" ), "'B' for Byte(s)" },
				{ eon::name( "P" ), "'p' for path" },
				{ eon::name( "R" ), "'r' for regex" } };
			data.Errors = true;
			string tip;
			auto suggestion = suggestions.find( prefix );
			if( suggestion != suggestions.end() )
				tip = " Perhaps you meant " + suggestion->second + "?";
			data.State->Report->error( "This is not a supported prefix!" + tip, data.State->Tokens.viewed().source() );

			// We handled this "name" so move on to the next token!
			return true;
		}

		bool ExpressionParser::_processVariable( ParserData& data, Attribute& variable, name_t name )
		{
			data.Operands.push( expression::Node::newValue( Attribute( variable ), name ) );
			data.Operands.top().source( data.State->Tokens.viewed().source() );
			return true;
		}

		bool ExpressionParser::_parseTuple( ParserData& data )
		{
			auto type = data.State->Tokens.viewed().type();
			if( type == name_static )
				return _parseStaticTuple( data );
			else if( type == name_typetuple )
				return _parseTypeTuple( data );
			else if( type == name_dynamic || type == name_data )
			{
				data.Errors = true;
				data.State->Report->error(
					"Cannot declare " + eon::str( type ) + " tuple inside expression! (Only static tuple!)",
					data.State->Tokens.viewed().source() );
				data.State->Tokens.forward();
				return false;
			}
			else
				return false;
		}

		bool ExpressionParser::_parseStaticTuple( ParserData& data )
		{
			static TupleParser parser;
			return _addOperand( data, parser.parse( *data.State, *data.Scope ) );
		}

		bool ExpressionParser::_parseTypeTuple( ParserData& data )
		{
			static TypeTupleParser parser;
			return _addOperand( data, parser.parse( *data.State, *data.Scope ) );
		}

		bool ExpressionParser::_addOperand( ParserData& data, expression::Node&& node )
		{
			if( node )
			{
				data.Operands.push( std::move( node ) );
				return true;
			}
			data.Errors = true;
			return false;
		}

		bool ExpressionParser::_parseExpression( ParserData& data )
		{
			if( !data.State->Tokens.viewed().is( name_expression ) )
				return false;

			static ExpressionParser parser;
			source::Ref source = data.State->Tokens.viewed().source();
			data.State->Tokens.forward();	// Skip 'ex('
			return _addOperand( data, parser.parseNode( *data.State, *data.Scope, { ")" } ) );
		}


		void ExpressionParser::_reportUnknownElementError( ParserData& data )
		{
			data.Errors = true;
			data.State->Report->error(
				"Unknown element \"" + data.State->Tokens.viewed().str() + "\"", data.State->Tokens.viewed().source() );
			data.State->Tokens.forward();
		}


		bool ExpressionParser::_parseOperator( ParserData& data )
		{
			auto type = data.State->Tokens.viewed().type();
			if( type == name_operator )
				return _processOperator( data );
			else if( type::Operators::isOperator( type ) )
				return _processOperatorName( data, type );
			else if( type == symbol_semicolon )
				return _processOperator( data, type, data.State->Tokens.viewed().source() );
			else if( type == symbol_assign )
				return _processOperator( data, symbol_assign, data.State->Tokens.viewed().source() );

			return false;
		}

		bool ExpressionParser::_processOperator( ParserData& data )
		{
			// This is one of: +*=~&|^<>[]
			name_t op_name = compilerName( data.State->Tokens.viewed().str() );
			return _processOperator( data, op_name, data.State->Tokens.viewed().source() );
		}

		bool ExpressionParser::_processOperatorName( ParserData& data, name_t name )
		{
			if( compilerName( data.State->Tokens.viewed().str() ) == symbol_minus && data.LastSeen == Seen::_operator )
				name = name_unary_minus;
			return _processOperator( data, name, data.State->Tokens.viewed().source() );
		}

		bool ExpressionParser::_processOperator( ParserData& data, name_t op_name, source::Ref op_source )
		{
			if( op_name == symbol_semicolon )
				return _processEndOfSmallExpression( data );
			else if( op_name == symbol_close_round )
				return _processCloseParen( data );
			else
				return _processPlainOperator( data, op_name, op_source );
			return false;
		}

		bool ExpressionParser::_processEndOfSmallExpression( ParserData& data )
		{
			while( !data.Operators.empty() )
				_bindOperator( data );
			data.LastSeen = Seen::_operator;
			return true;
		}

		bool ExpressionParser::_processCloseParen( ParserData& data )
		{
			while( !data.Operators.empty() && data.Operators.top().Op != symbol_open_round )
				_bindOperator( data );
			if( !data.Operators.empty() )
				data.Operators.pop();
			else
			{
				data.Errors = true;
				data.State->Report->error( "Unbalanced parenthesis!", data.State->Tokens.viewed().source() );
			}
			data.LastSeen = Seen::_operator;
			return true;
		}

		bool ExpressionParser::_processPlainOperator( ParserData& data, name_t new_op_name, source::Ref new_op_source )
		{
			while( _canBindOperator( data, new_op_name ) )
			{
				if( new_op_name == symbol_open_round )
				{
					data.Errors = true;
					data.State->Report->error( "Unbalanced parenthesis!", data.State->Tokens.viewed().source() );
				}
				else
					_bindOperator( data );
			}
			if( type::Operators::isFirstInSequence( new_op_name ) )
				_resolveOperatorSequence( data, new_op_name, new_op_source );
			data.LastSeen = Seen::_operator;
			return true;
		}

		bool ExpressionParser::_canBindOperator( ParserData& data, name_t new_op_name )
		{
			return !data.Operators.empty()
				&& type::Operators::inputPrecedence( new_op_name )
				<= type::Operators::stackPrecedence( data.Operators.top().Op );
		}


		void ExpressionParser::_finishExpressionDetails( ParserData& data )
		{
			if( !data.State->Tokens.atEnd() && data.State->Tokens.viewed().is( symbol_semicolon ) )
				data.State->Tokens.forward();
			while( !data.Operators.empty() )
				_bindOperator( data );
		}


		void ExpressionParser::_bindOperator( ParserData& data )
		{
			if( data.Operators.top().Op == symbol_assign )
				_bindAssign( data );
			else
				_bindNormal( data );
		}

		void ExpressionParser::_bindAssign( ParserData& data )
		{
			expression::Node lhs, rhs;
			if( !_getOperands( data, { &rhs, &lhs } ) )
				return;

			if( !_isLvalue( data, lhs ) )
				return;

			// Option 1: The types are identical - we can do a direct copy or move!
			if( lhs.type() == rhs.type() )
				_assignDirect( data, lhs, rhs );

			// Option 2: Implicit conversion available - we can convert and do a direct copy or move!
			else if( _canConvertDirectly( data, lhs.type(), rhs ) )
				_assignDirectByConversion( data, lhs, rhs );

			// Option 3: Tuple copy (direct or compatible)!
			else if( rhs.type().isTuple() )
				_assignTuple( data, lhs, rhs );

			// Option 4: Conversion to same type by constructor!
			else
				_assignByConstructor( data, lhs, rhs );
		}

		void ExpressionParser::_bindNormal( ParserData& data )
		{
			auto op = expression::Node::newOperator( data.Operators.top().Op, data.Operators.top().Source );
			_bindOperands( data, op );
			_bindOperatorAction( data, op );
			data.Operands.push( std::move( op ) );
			data.Operators.pop();
		}

		void ExpressionParser::_bindOperands( ParserData& data, expression::Node& op )
		{
			for( size_t i = 0; i < type::Operators::numOperands( op.name() ); ++i )
			{
				if( data.Operands.empty() )
				{
					data.Errors = true;
					data.State->Report->error( "Operator is missing operand!", op.source() );
					break;
				}
				op.addOperand( std::move( data.Operands.top() ) );
				data.Operands.pop();
			}
		}

		void ExpressionParser::_bindOperatorAction( ParserData& data, expression::Node& op )
		{
			if( !op.bindOperator( *data.Scope, *data.State->Report ) )
			{
				data.Errors = true;
				data.State->Report->fatal( "Operator not implemented!", op.source() );
			}
		}

		bool  ExpressionParser::_getOperands( ParserData& data, std::initializer_list<expression::Node*> operands )
		{
			for( auto op : operands )
			{
				if( data.Operands.empty() )
				{
					data.Errors = true;
					data.State->Report->error( "Operator is missing operand!", data.Operators.top().Source );
					data.Operators.pop();
					return false;
				}
				*op = std::move( data.Operands.top() );
				data.Operands.pop();
			}
			return true;
		}

		bool  ExpressionParser::_isLvalue( ParserData& data, const expression::Node& operand ) const
		{
			if( operand.value().isRvalue() )
			{
				data.Errors = true;
				data.State->Report->error( "Cannot assign to an rvalue!", operand.source() );
				data.Operators.pop();
				return false;
			}
			return true;
		}

		void ExpressionParser::_assignDirect( ParserData& data, expression::Node& lhs, expression::Node& rhs ) const
		{
			static ActionFinder action_finder;
			name_t type = lhs.type().name();
			auto args = typetuple::convert( { lhs.type() } );
			auto action_signature = rhs.value().isRvalue()
				? action_finder.find(
					ActionFinder::Details(
						*data.State->Report,
						data.Operators.top().Source ).actionName( name_take ).category( name_operator ).typeName(
							type ).args( args ) )
				: action_finder.find(
					ActionFinder::Details(
						*data.State->Report,
						data.Operators.top().Source ).actionName( type::name_copy ).category(
							name_operator ).typeName( type ).args( args ) );
			if( !action_signature )
			{
				data.Operators.pop();
				return;
			}

			_bindAction( data, lhs, rhs, action_signature );
		}

		void ExpressionParser::_bindAction(
			ParserData& data, expression::Node& lhs, expression::Node& rhs, TypeTuple action_signature ) const
		{
			auto action = scope::global().action( action_signature );
			auto assign = expression::Node::newCall( *action, data.Operators.top().Source );
			assign.addOperand( std::move( rhs ) );
			assign.addOperand( std::move( lhs ) );
			data.Operands.push( std::move( assign ) );
			data.Operators.pop();
		}

		void ExpressionParser::_assignDirectByConversion(
			ParserData& data, expression::Node& lhs, expression::Node& rhs ) const
		{
			// We need to convert rhs to the type of lhs
			rhs = expression::Node::newValue( data.Converter.convert( rhs, lhs.type().name() ) );

			// Now we can do direct conversion
			_assignDirect( data, lhs, rhs );
		}

		void ExpressionParser::_assignTuple( ParserData& data, expression::Node& lhs, expression::Node& rhs ) const
		{
			if( !rhs.type().compatibleWith( lhs.type() ) )
			{
				_incompatibleTupleError( data, lhs, rhs );
				return;
			}

			static ActionFinder action_finder;
			auto action_signature = action_finder.find(
				ActionFinder::Details(
					*data.State->Report,
					data.Operators.top().Source ).actionName( symbol_assign ).category( name_operator ).typeName(
						name_tuple ).args( typetuple::convert( { name_tuple } ) ) );
			if( action_signature )
				_bindAction( data, lhs, rhs, action_signature );
			else
				data.Operators.pop();
		}

		void ExpressionParser::_incompatibleTupleError(
			ParserData& data, expression::Node& lhs, expression::Node& rhs ) const
		{
			data.Errors = true;
			data.State->Report->error(
				"Cannot assign incompatible right operand " + rhs.type().str() + " to left operand "
				+ lhs.type().str() + "!",
				lhs.source() );
			data.Operators.pop();
		}

		void ExpressionParser::_assignByConstructor( ParserData& data, expression::Node& lhs, expression::Node& rhs ) const
		{
			static ActionFinder action_finder;
			auto constructor_signature = action_finder.find(
				ActionFinder::Details( *data.State->Report, data.Operators.top().Source ).actionName(
					name_constructor ).category( name_constructor ).typeName( lhs.type().name() ).args(
						typetuple::convert( { rhs.type() } ) ) );
			if( !constructor_signature )
				return;
			auto assign_signature = action_finder.find(
				ActionFinder::Details( *data.State->Report, data.Operators.top().Source ).actionName(
					name_take ).category( name_operator ).typeName( lhs.type().name() ).args(
						typetuple::convert( { lhs.type().name() } ) ) );
			if( !assign_signature )
				return;

			auto constructor_action = scope::global().action( constructor_signature );
			auto constructor = expression::Node::newCall( *constructor_action, data.Operators.top().Source );
			constructor.addOperand( std::move( rhs ) );
			auto assign_action = scope::global().action( assign_signature );
			auto assign = expression::Node::newCall( *assign_action, data.Operators.top().Source );
			assign.addOperand( std::move( lhs ) );
			assign.addOperand( std::move( constructor ) );
			data.Operands.push( std::move( assign ) );
			data.Operators.pop();
		}

		bool  ExpressionParser::_canConvertDirectly(
			ParserData& data, const TypeTuple& lhs, const expression::Node& rhs ) const noexcept
		{
			if( !rhs.value().isLiteral() )		// Rhs must be a literal value!
				return false;
			return data.Converter.legal( lhs.name(), rhs );
		}

		void ExpressionParser::_resolveOperatorSequence( ParserData& data, name_t new_op_name, source::Ref new_op_source )
		{
			// If this is a sequence operator, then parse whatever is inbetween as if parenthesis
			name_t op_name = _resolveOperator( data, new_op_name );
			if( op_name != no_name )
				data.Operators.push( Operator( op_name, new_op_source ) );
		}

		name_t ExpressionParser::_resolveOperator( ParserData& data, name_t op_name )
		{
			std::vector<name_t> all_options = type::Operators::mapOperator( op_name );
			if( all_options.empty() )
			{
				data.State->Report->error( "Internal issue with this operator!", data.State->Tokens.viewed().source() );
				throw InvalidExpression();	// Impossible!?
			}

			if( _singleOptionNoSequence( all_options ) )
				return all_options[ 0 ];
			else
				return _resolveNonTrivalOperator( all_options, data, op_name );
		}

		name_t ExpressionParser::_resolveNonTrivalOperator(
			std::vector<name_t>& all_options, ParserData& data, name_t op_name )
		{
			// We have a single option with a sequence, or multiple options (which means all have sequences)!
			std::vector<expression::Node> operands;
			for( index_t seq_no = 1; ; ++seq_no )
			{
				auto result = _resolveNonTrivalOperator( all_options, data, op_name, operands, seq_no );
				if( result != name_nn )
					return result;
			}
			return no_name;
		}

		name_t ExpressionParser::_resolveNonTrivalOperator(
			std::vector<name_t>& all_options,
			ParserData& data,
			name_t op_name,
			std::vector<expression::Node>& operands,
			index_t seq_no )
		{
			name_t singular_op_name = _identifySingularOperator( data, all_options, op_name, seq_no );
			if( singular_op_name != no_name )
				return _processSequenceOperator( data, singular_op_name, std::move( operands ) );

			auto remaining_options = _discardPrefixOperatorOptions( all_options, seq_no );
			if( remaining_options.empty() )
				return _operatorMissingElementSyntaxError( data, all_options, seq_no );

			// If we have only one option left, we can continue with that.
			if( remaining_options.size() == 1 )
				return _processSequenceOperator( data, remaining_options[ 0 ], std::move( operands ) );

			// We still have multiple options left and must parse the next
			// operand before we can get to the next operator name
			source::Ref orig_source = data.State->Tokens.viewed().source();
			_parseNextNonTrivalOperatorOperand( data, remaining_options, operands, seq_no );

			// If at the end of the tokens, we have a syntax error
			if( data.State->Tokens.atEnd() )
				return _incompleteOperatorError( data, orig_source );

			// Eliminate options for which the next token does not match the next operator in sequence
			all_options = _matchNextSequenceOperator( data, remaining_options, seq_no );

			return name_nn;
		}

		name_t ExpressionParser::_identifySingularOperator(
			ParserData& data, const std::vector<name_t>& options, name_t op_name, index_t seq_no )
		{
			for( auto& option : options )
			{
				auto& sequence = type::Operators::sequence( option );
				if( seq_no >= sequence.size() )
					return option;	// All operators in the sequence have been matched!
				if( !sequence[ seq_no ].prefix() )
					continue;

				auto next_type = data.State->Tokens.peekAhead().type();
				if( next_type == sequence[ seq_no ].Op
					|| ( next_type == name_name && name( data.State->Tokens.peekAhead().str() ) == sequence[ seq_no ].Op )
					|| ( next_type == name_operator && compilerName(
						data.State->Tokens.peekAhead().str() ) == sequence[ 1 ].Op ) )
				{
					data.State->Tokens.forward();
					return option;
				}
			}
			return no_name;
		}

		std::vector<name_t> ExpressionParser::_discardPrefixOperatorOptions(
			const std::vector<name_t>& options, index_t seq_no )
		{
			std::vector<name_t> remaining;
			for( auto& option : options )
			{
				auto& sequence = type::Operators::sequence( option );
				if( seq_no < sequence.size() && !sequence[ seq_no ].prefix() )
					remaining.push_back( option );
			}
			return remaining;
		}

		name_t ExpressionParser::_operatorMissingElementSyntaxError(
			ParserData& data, std::vector<name_t>& all_options, index_t seq_no )
		{
			data.Errors = true;
			data.State->Report->error(
				"This operator requires an additional element!", data.State->Tokens.viewed().source() );
			for( auto& option : all_options )
			{
				auto& sequence = type::Operators::sequence( option );
				if( seq_no < sequence.size() )
				{
					string lead = option == all_options[ 0 ] ? "  Perhaps" : "  or perhaps";
					data.State->Report->note( lead + " this one? : " + eon::str( sequence[ seq_no ].Op ), source::Ref() );
				}
			}
			return no_name;
		}

		std::set<string> ExpressionParser::_findStopOn( const std::vector<name_t>& options, index_t seq_no )
		{
			std::set<string> stop_on;
			for( auto& option : options )
				stop_on.insert( eon::str( type::Operators::sequence( option )[ seq_no ].Op ) );
			return stop_on;
		}

		std::vector<name_t> ExpressionParser::_matchNextSequenceOperator(
			ParserData& data, const std::vector<name_t>& options, index_t seq_no )
		{
			std::vector<name_t> remaining;
			for( auto& option : options )
			{
				auto& sequence = type::Operators::sequence( option );
				if( seq_no >= sequence.size() )	// Not possible!?
					continue;
				auto next_type = data.State->Tokens.viewed().type();
				if( next_type == sequence[ seq_no ].Op
					|| ( next_type == name_name && name( data.State->Tokens.viewed().str() ) == sequence[ seq_no ].Op )
					|| ( next_type == name_operator
						&& compilerName( data.State->Tokens.viewed().str() ) == sequence[ seq_no ].Op ) )
					remaining.push_back( option );
			}
			return remaining;
		}

		name_t ExpressionParser::_processSequenceOperator(
			ParserData& data, name_t op_name, std::vector<expression::Node>&& operands )
		{
			// Get remaining operands - if any
			auto& sequence = type::Operators::sequence( op_name );
			for( index_t seq_no = operands.size() + 1; seq_no < sequence.size(); ++seq_no )
			{
				data.State->Tokens.forward();	// Must move past the operator!
				ParserData data2( data, { eon::str( sequence[ seq_no ].Op ) } );
				if( _parseExpressionDetails( data2 ) )
					operands.push_back( std::move( data2.Operands.top() ) );
				else
				{
					data.Errors = true;
					return no_name;
				}
			}

			// Push all that's left and we're done!
			for( auto& operand : operands )
				data.Operands.push( std::move( operand ) );
			return op_name;
		}

		void ExpressionParser::_parseNextNonTrivalOperatorOperand(
			ParserData& data,
			const std::vector<name_t>& options,
			std::vector<expression::Node>& operands,
			index_t seq_no )
		{
			data.State->Tokens.forward();	// Must move past the operator!
			ParserData data2( data, _findStopOn( options, seq_no ) );
			if( _parseExpressionDetails( data2 ) )
				operands.push_back( std::move( data2.Operands.top() ) );
		}

		name_t ExpressionParser::_incompleteOperatorError( ParserData& data, source::Ref orig_source )
		{
			data.Errors = true;
			data.State->Report->error( "Incomplete operator!", orig_source );
			return no_name;
		}
	}
}