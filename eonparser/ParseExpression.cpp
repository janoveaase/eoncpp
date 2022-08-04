/*#include "ParseExpression.h"
#include <eontypes/Bool.h>
#include <eontypes/Byte.h>
#include <eontypes/Char.h>
#include <eontypes/Integer.h>
#include <eontypes/Floatingpt.h>
#include <eontypes/Name.h>
#include <eontypes/Handle.h>
#include <eontypes/String.h>
#include <eontypes/Bytes.h>
#include <eontypes/Path.h>
#include <eontypes/Regex.h>
#include <eontypes/ExpressionObj.h>
#include <eontypes/NamePath.h>
#include <eontypes/OperatorAction.h>
#include <eontypes/DataTuple.h>
#include <eontypes/EonType.h>
#include <eontypes/DynamicTuple.h>


namespace eon
{
	namespace parser
	{
		bool ParseExpression::operator()( ToolBox& tools, type::Node& result )
		{
			Tools = &tools;
			if( !Tools->ready() )
				Tools->reset();

			// Start context
			Tools->push( ToolBox::Context::expression );

			bool status = _parse( result );

			// End context
			Tools->popContext();
			return status;
		}




		bool ParseExpression::_parse( type::Node& result )
		{
			source::Ref src = Tools->current().source();

			for( ; *Tools; )
			{
				// If we are in an expression context, the following will end
				// the expression: ')' (if operator stack is empty), ';'

				// If we are in an expression context, there are some considerations we need to make
				if( Tools->context() == ToolBox::Context::expression )
				{
					// Closing parenthesis and empty operator stack will end expression
					if( Tools->current().is( name_close ) && Tools->opStack().empty() )
						break;

					// Semi-colon will always end
					if( Tools->current().is( name_symbol ) && Tools->current().symbValue() == ';' )
						break;

					// If we have a tuple above, then comma will end
					if( Context == name_tuple && Tools->current().is( name_operator )
						&& Tools->current().opValue() == type::operators::code::comma )
						break;
				}

				if( !_processElement() )
					return false;
			}
			src.end( *Tools ? Tools->current().source().start() : Tools->last().source().end() );
			
			if( !Tools->opStack().empty() )
				_popOperatorsUntil( type::operators::code::_end );

			if( Tools->treeStack().size() != 1 )
			{
				Tools->reporter().error( "Not a valid expression!", src );
				return false;
			}
			result.claim( std::move( Tools->treeStack().top() ) );
			Tools->treeStack().pop();
			if( result.isValue() && result.value()->generalType() == name_expression )
				( (type::Expression*)result.value() )->resultType();
		
			return true;
		}

		bool ParseExpression::_processElement()
		{
			if( Tools )
			{
				switch( Tools->context() )
				{
					case ToolBox::Context::expression:
						if( !_processExpressionElement() )
							return false;
						break;
					case ToolBox::Context::tuple:
						if( !_parseOpenTuple() )
							return false;
						break;
					case ToolBox::Context::data_tuple:
						if( !_parseDataTuple() )
							return false;
						break;
					case ToolBox::Context::plain_tuple:
						if( !_parsePlainTuple() )
							return false;
						break;
					case ToolBox::Context::dynamic_tuple:
						if( !_parseDynamicTuple() )
							return false;
						break;
					case ToolBox::Context::type_tuple:
						if( !_parseTypeTuple() )
							return false;
						break;

					default:
						// TODO: Find out if this is possible!
						break;
				}
			}
			return true;
		}

/*		void ParseExpression::_checkContext()
		{
			// Must have a next element for this to be relevant
			if( !Tools->exists() )
				return;
			
			// Check if we have a plain tuple
			if( Tools->ahead().is( name_symbol ) && Tools->ahead().symbValue() == ',' )
				Tools->push( ToolBox::Context::tuple );

			// Check for type tuple
			else if( Tools->current().is( name_name) && *Tools->current().nameValue() == "T"
				&& Tools->ahead().is( name_brace ) && Tools->ahead().nameValue() == name_open )
				Tools->push( ToolBox::Context::type_tuple );

			// Check for data tuple
			else if( Tools->current().is( name_name ) && *Tools->current().nameValue() == "data"
				&& Tools->ahead().is( name_brace ) && Tools->ahead().nameValue() == name_open )
				Tools->push( ToolBox::Context::data_tuple );
		}* /

		bool ParseExpression::_processExpressionElement()
		{
			if( Tools->current().is( name_operator ) )
				return _parseOperator( Tools->current().opValue() );
			else if( Tools->current().is( name_open ) )
				return _parseOperator( type::operators::code::open_brace );
			else if( Tools->current().is( name_close ) )
				return _parseOperator( type::operators::code::close_brace );
			else if( Tools->current().is( name_name ) && eon::str( Tools->current().nameValue() ) == "e"
				&& Tools->exists() && Tools->peek().is( name_open ) )
				return _parseExpression();
			else if( Tools->current().is( name_defvar ) )
				return _processDefineVar();
			else if( Tools->current().is( name_symbol ) )
				return _processSymbolElement();

			else if( Tools->current().is( name_name ) )
				return _processNameElement();
			else if( Tools->current().is( name_string ) || Tools->current().is( name_bytes )
				|| Tools->current().is( name_path ) || Tools->current().is( name_char )
				|| Tools->current().is( name_byte ) || Tools->current().is( name_indentation )
				|| Tools->current().is( name_regex ) || Tools->current().is( name_bool )
				|| Tools->current().is( name_float ) || Tools->current().is( name_namepath )
				|| Tools->current().is( name_int ) || Tools->current().is( name_short )
				|| Tools->current().is( name_long ) || Tools->current().is( name_low ) || Tools->current().is( name_high ) )
			{
//				if( Tools->context() == ToolBox::Context::expression
//					&& Tools->exists() && Tools->peek().is( name_symbol ) && Tools->peek().symbValue() == ',' )
//				{
//					Tools->push( ToolBox::Context::tuple );
//					return true;
//				}
				Tools->treeStack().push( type::Node::newValue( Tools->consumeObject() ) );
				Tools->forward();
			}

			else if( Tools->current().is( name_indentation ) )
				_parseIndentation();

			return true;
		}

		bool ParseExpression::_processNameElement()
		{
			// Name followed by parenthesis has special meaning
			if( Tools->exists() && Tools->peek().is( name_open ) )
			{
				// It is either a tuple of some kind
				if( eon::str( Tools->current().nameValue() ) == "p" )
				{
					Tools->push( ToolBox::Context::plain_tuple );
					Tools->forward();
					return true;
				}
				else if( eon::str( Tools->current().nameValue() ) == "T" )
				{
					Tools->push( ToolBox::Context::type_tuple );
					Tools->forward();
					return true;
				}
				else if( Tools->current().nameValue() == name_dynamic )
				{
					Tools->push( ToolBox::Context::dynamic_tuple );
					Tools->forward();
					return true;
				}
				else if( Tools->current().nameValue() == name_data )
				{
					Tools->push( ToolBox::Context::data_tuple );
					Tools->forward();
					return true;
				}

				// Or an action call
				else
					return _parseActionCall();
			}

			// If the name matches that of an operator, it may - or may not -
			// be one
			auto op_type = type::operators::mapCode( eon::str( Tools->current().nameValue() ) );
			if( op_type != type::operators::code::undef )
				return _parseOperator( op_type );

			// Seems to be just another name
			return _parseName();
		}
		bool ParseExpression::_processSymbolElement()
		{
			// TODO: Handle ...
			return false;
		}

		bool ParseExpression::_processDefineVar()
		{
			Tools->forward();		// Skip "define"

			// We know that we have name + "as" or "="
			auto name = Tools->current().nameValue();
			source::Ref name_source = Tools->current().source();
			Tools->forward();

			if( Tools->current().is( name_name ) )		// Must be "as"
			{
				Tools->forward();

				// TODO: Implement!"
			}
			else										// Must be "="
			{
				Tools->forward();
				type::Node value;
				auto sub = Tools->split();
				if( ParseExpression()( sub, value ) )
				{
					Tools->sync( std::move( sub ) );

					auto type = value.resultType();
					if( type.name() == name_plain )
					{
						// We need to create a new assignment operation with
						// our new variable on the left side and 'value' on
						// the right.
						auto operators = Tools->scope().global().getActions( compilerName( "$op_=" ), type,
							EonType( { type } ), type );

						actions::OperatorAction act( type::operators::code::assign, 2, source::Ref() );
						if( !_checkOperator( operators, act, type.str() + ", " + type.str(), type ) )
							return false;

						auto assign = type::Node::newOperator( (actions::OperatorAction*)*operators.begin() );
						assign.add( type::Node::newName( name, name_source, true ) );
						assign.add( std::move( value ) );
						Tools->treeStack().push( std::move( assign ) );
						return true;
					}
					else
					{
						auto type_obj = Tools->scope().find( type.name() );
						if( type_obj && type_obj->generalType() == name_type )
						{
//							auto val = ( (type::TypeDef*)type_obj )->instantiate();
//							Tools->scope().add( name, val );

							// We need to create a new assignment operation with
							// our new variable on the left side and 'value' on
							// the right.
							auto operators = Tools->scope().global().getActions( compilerName( "$op_=" ), type,
								EonType( { type } ), type );

							actions::OperatorAction act( type::operators::code::assign, 2, source::Ref() );
							if( !_checkOperator( operators, act, type.str() + ", " + type.str(), type ) )
								return false;

							auto assign = type::Node::newOperator( (actions::OperatorAction*)*operators.begin() );
							assign.add( type::Node::newName( name, name_source, true ) );
//							assign.add( type::Node::newValue( val ) );
							assign.add( std::move( value ) );
							Tools->treeStack().push( std::move( assign ) );
							return true;
						}
					}
				}
			}

			return false;
		}

		bool ParseExpression::_parseActionCall()
		{
			// An action call is identified by a name and a '('
			auto name = Tools->current().nameValue();
			Tools->forward( 2 );

			// Everything up to matching ')' is part of the arguments
			while( *Tools && !Tools->current().is( name_close ) )
			{
			}

			return false;
		}
		bool ParseExpression::_parseOperator( type::operators::code type )
		{
			Tools->forward();
			if( type == type::operators::code::close_brace )
			{
				auto result = _popOperatorsUntil( type::operators::code::open_brace );
				if( result && !Tools->opStack().empty() )
				{
					Tools->opStack().pop();
					return true;
				}
				Tools->reporter().error( "Missing matching '('!", Tools->peek( -1 ).source() );
				return false;
			}
			else
			{
				auto& sequence = type::operators::sequence( type );
				if( sequence.size() > 1 && type != sequence[ 0 ].Op )
				{
					// We can ignore all opereators except the first and the last!
					if( sequence[ sequence.size() - 1 ].Op == type )
						return _popOperatorsUntil( sequence[ 0 ].Op );
					else
						return true;
				}
				else
					_popOperatorsFor( type );
			}

			return _pushOperatorAction( type );
		}
		bool ParseExpression::_pushOperatorAction( type::operators::code type )
		{
			// For operators, we always know exactly how many arguments/
			// operands are required, while the type of arguments depend on the
			// type of the first operand - which we won't know until the
			// expression is complete.
			Tools->opStack().push(
				new actions::OperatorAction( type, type::operators::numOperands( type ), Tools->current().source() ) );
			return true;
		}

		bool ParseExpression::_parseExpression()
		{
			source::Ref src = Tools->current().source();
			Tools->forward( 1 );
			if( Tools )
			{
				type::Node expr;
				auto sub = Tools->split();
				if( ParseExpression()( sub, expr ) )
				{
					Tools->sync( std::move( sub ) );
					Tools->treeStack().push( type::Node::newValue( new type::Expression( std::move( expr ), true ) ) );
					return true;
				}
			}
			else
			{
				src.end( Tools->last().source().end() );
				Tools->reporter().error( "Literal expression started!", src );
			}
			return false;
		}

		bool ParseExpression::_parseName()
		{
			// Is it 'me' or 'my'?
			if( Tools->current().nameValue() == name_me || eon::str( Tools->current().nameValue() ) == "my" )
			{
				auto me = Tools->scope().find( name_me );
				if( !me )
				{
					Tools->reporter().error(
						"Use of \"" + eon::str( Tools->current().nameValue() ) + "\" only possible inside actions!",
						Tools->current().source() );
					return false;
				}
				Tools->treeStack().push( type::Node::newValue( me ) );
				Tools->forward();
				return true;
			}

			// Is it a known entity?
			auto var = Tools->scope().find( Tools->current().nameValue() );
			if( var )
			{
				if( var->generalType() == name_action )
					Tools->opStack().push( (type::Action*)var );
				else
					Tools->treeStack().push( type::Node::newValue( var ) );
				Tools->forward();
				return true;
			}

			// Unknown entity
			Tools->treeStack().push( type::Node::newName( Tools->current().nameValue(), Tools->current().source() ) );
			Tools->forward();
			return true;
		}
		void ParseExpression::_parseIndentation()
		{
		}




		bool ParseExpression::_parseOpenTuple()
		{
			source::Ref source = Tools->current().source();

			// May or may not be parenthesized
			name_t end_of_tuple = Tools->current().is( name_open ) ? name_close : no_name;
			if( end_of_tuple )
				Tools->forward();

			// Tuple runs until ")" if parenthesized and any operator/action or ';' if not
			type::BasicTuple tuple( name_open, source::Ref() );
			if( !_parseTuple( tuple, name_open, end_of_tuple ) )
				return false;
			source.end( *Tools ? Tools->current().source().start() : Tools->last().source().start() );
			tuple.source( source );
			Tools->treeStack().push( type::Node::newValue( new Tuple( std::move( tuple ) ) ) );
			Tools->popContext();
			return true;
		}
		bool ParseExpression::_parsePlainTuple()
		{
			// Data tuples are always parenthesized
			source::Ref source = Tools->current().source();
			Tools->forward();

			// Tuple runs until ")"
			Tuple tuple;
			if( !_parseTuple( tuple, name_data, name_close ) )
				return false;
			source.end( *Tools ? Tools->current().source().start() : Tools->last().source().start() );
			tuple.source( source );
			Tools->treeStack().push( type::Node::newValue( new Tuple( std::move( tuple ) ) ) );
			Tools->popContext();
			return true;
		}
		bool ParseExpression::_parseDataTuple()
		{
			// Data tuples are always parenthesized
			source::Ref source = Tools->current().source();
			Tools->forward();

			// Tuple runs until ")"
			DataTuple tuple;
			if( !_parseTuple( tuple, name_data, name_close ) )
				return false;
			source.end( *Tools ? Tools->current().source().start() : Tools->last().source().start() );
			tuple.source( source );
			Tools->treeStack().push( type::Node::newValue( new DataTuple( std::move( tuple ) ) ) );
			Tools->popContext();
			return true;
		}
		bool ParseExpression::_parseDynamicTuple()
		{
			// Type tuples are always parenthesized
			source::Ref source = Tools->current().source();
			Tools->forward();

			// Tuple runs until ")"
			DynamicTuple tuple;
			if( !_parseTuple( tuple, name_data, name_close ) )
				return false;
			source.end( *Tools ? Tools->current().source().start() : Tools->last().source().start() );
			tuple.source( source );
			Tools->treeStack().push( type::Node::newValue( new DynamicTuple( std::move( tuple ) ) ) );
			Tools->popContext();
			return true;
		}
		bool ParseExpression::_parseTuple( type::BasicTuple& tuple, name_t tuple_type, name_t end_of_tuple )
		{
			if( !_endOfTuple( end_of_tuple ) )
			{
				while( Tools )
				{
					if( !_parseTupleAttribute( tuple, tuple_type ) )
						return false;

					if( Tools->current().is( name_operator ) && Tools->current().opValue() == type::operators::code::comma )
						Tools->forward();
					else if( _endOfTuple( end_of_tuple ) )
						return true;
					else
					{
						Tools->reporter().error( "Illegal part of tuple!", Tools->current().source() );
						return false;
					}
				}
			}
			return true;
		}
		bool ParseExpression::_endOfTuple( name_t end_of_tuple )
		{
			if( end_of_tuple != no_name )
			{
				if( Tools->current().is( end_of_tuple ) )
				{
					Tools->forward();
					return true;
				}
			}

			// For others, either ';' or reduction in indentation or an action/operator.
			else
			{
				if( Tools->current().is( name_symbol ) && Tools->current().symbValue() == ';' )
				{
					Tools->forward();
					return true;
				}
				else if( Tools->current().is( name_indentation )
					&& Tools->current().numValue() <= Tools->indentLevel() )
					return true;
			}

			return false;
		}
		bool ParseExpression::_parseTupleAttribute( type::BasicTuple& tuple, name_t tuple_type )
		{
			// Format: [[[[<qualifier>] <name>] [as <type>]] = ][<value>]
			// Format: [T(<type>) = [<value>]]

			source::Ref src = Tools->current().source();

			// Get each attribute element in order:

			auto qualifier = _parseAttributeQualifier( tuple_type );
			if( qualifier == no_name )
				return false;

			source::Ref name_src;
			auto name = _parseAttributeName( name_src );

			EonType type;
			if( !_parseAttributeType( type, name != no_name ) )
				return false;

			type::Expression expr;
			if( !_parseAttributeValue( expr ) )
				return false;
			type::Object* value{ nullptr };
			if( expr.value().isValue() )
				value = expr.value().consumeValue();
			else
				value = new type::Expression( std::move( expr ) );

			if( Tools )
				src.end( Tools->current().source().start() );
			else
				src.end( Tools->last().source().end() );

			if( !type )
			{
				if( !_getTypeFromValue( type, value, src ) )
				{
					delete value;
					return false;
				}
			}
			else if( type && value )
			{
				if( !_valueMatchesType( value, type, src ) )
				{
					delete value;
					return false;
				}
			}

			// Deal with qualifiers
			if( qualifier == name_read )
			{
				if( type.name() == name_plain || !isPrimitive( type.name() ) )
					value = new Reference( value );
			}
			else if( qualifier == name_modify )
				value = new Modifiable( value );

			// Finalize
			try
			{
				tuple += type::Attribute( name, type, value );
			}
			catch( type::AccessDenied )
			{
				Tools->reporter().error( "Not a legal attribute for this type of tuple!", src );
				delete value;
				return false;
			}
			catch( type::DuplicateName )
			{
				Tools->reporter().error( "This name is already in use!", name_src );
				delete value;
				return false;
			}
			return true;
		}
		name_t ParseExpression::_parseAttributeQualifier( name_t tuple_type )
		{
			static std::set<name_t> qualifiers{ name_read, name_modify, name_take };
			if( Tools->current().is( name_name )
				&& qualifiers.find( Tools->current().nameValue() ) != qualifiers.end() )
			{
				if( Tools->peek().is( name_name ) && Tools->peek().nameValue() != name_as )
				{
					if( tuple_type != name_args && tuple_type != name_return )
					{
						Tools->reporter().error( "Qualifiers are only permitted in argument and return type definitions!",
							Tools->current().source() );
						return no_name;
					}
				}
				name_t qual = Tools->current().nameValue();
				Tools->forward();
				return qual;
			}
			return name_read;
		}
		name_t ParseExpression::_parseAttributeName( source::Ref& name_src )
		{
			if( !Tools->current().is( name_name ) )
				return no_name;

			auto name = Tools->current().nameValue();
			name_src = Tools->current().source();
			Tools->forward();
			return name;
		}
		bool ParseExpression::_parseAttributeType( EonType& output, bool have_name )
		{
			auto have_as = Tools->current().is( name_name ) && Tools->current().nameValue() == name_as;
			if( !have_name && have_as )
			{
				Tools->reporter().error( "Nameless attributes do not specify type using \"as\"! Use a type-tuple.",
					Tools->current().source() );
				return false;
			}
			if( have_as )
				Tools->forward();

			if( Tools->current().is( name_typetuple ) )
			{
				Tools->forward();
				return __parseTypeTuple( output, no_name, name_close );
			}

			if( Tools->current().is( name_name ) )
				return __parseTypeTuple( output, no_name, no_name );

			// We don't have a type specified
			return true;
		}
		bool ParseExpression::_parseAttributeValue( type::Expression& value )
		{
			if( Tools->current().is( name_operator ) && Tools->current().opValue() == type::operators::code::assign )
				Tools->forward();

			if( Tools->current().is( name_ellipsis ) )
			{
				Tools->forward();
				value = type::Expression( type::Node::newEllipsis() );
				return true;
			}
			else
			{
				// Get value as expression
				auto& source = Tools->current().source();
				type::Node expr;
				auto sub = Tools->split();
				if( !ParseExpression( name_tuple )( sub, expr ) )
				{
					Tools->sync( std::move( sub ) );
					Tools->reporter().error( "Expected a value here!", source );
					return false;
				}
				Tools->sync( std::move( sub ) );

				value = type::Expression( std::move( expr ) );
				value.resultType();
				return true;
			}
		}
		bool ParseExpression::_getTypeFromValue( EonType& output, type::Object* value, source::Ref src )
		{
			if( !value )
			{
				Tools->reporter().error( "Attribute must have a type or a value!", src );
				return false;
			}
			output = value->type();
			return true;
		}
		bool ParseExpression::_valueMatchesType( type::Object* value, const EonType& type, source::Ref src )
		{
			if( value->generalType() == name_action )
			{
				// TODO: Find a way to get type!
			}
			else
			{
				if( value->type() != type )
				{
					Tools->reporter().error( "Attribute specifies type " + type.str() + " but value has type "
						+ value->type().str() + "!", src );
					return false;
				}
			}
			return true;
		}

		bool ParseExpression::_parseTypeTuple()
		{
			// We have seen the following tokens: "T", "("
			source::Ref source = Tools->current().source();
			Tools->forward( 2 );

			EonType type;
			if( !__parseTypeTuple( type, no_name, name_close ) )
				return false;
			source.end( *Tools ? Tools->current().source().start() : Tools->last().source().start() );
			type.source( source );
			Tools->treeStack().push( type::Node::newValue( new type::EonTypeObject( std::move( type ) ) ) );
			return true;
		}
		bool ParseExpression::__parseTypeTuple( EonType& output, name_t name, name_t end_of_tuple )
		{
			EonType tuple;
			if( name != no_name )
				tuple.name( name );
			if( !_endOfTypeTuple( end_of_tuple ) )
			{
				while( Tools )
				{
					if( !_parseTypeTupleAttribute( tuple ) )
						return EonType();

					if( Tools->current().is( name_operator ) && Tools->current().opValue() == type::operators::code::comma )
						Tools->forward();
					else if( _endOfTuple( end_of_tuple ) )
						break;
					else
					{
						Tools->reporter().error( "Illegal part of type tuple!", Tools->current().source() );
						return EonType();
					}
				}
			}
			return tuple;
		}
		bool ParseExpression::_endOfTypeTuple( name_t end_of_tuple )
		{
			if( end_of_tuple )
			{
				if( Tools->current().is( end_of_tuple ) )
				{
					Tools->forward();
					return true;
				}
			}

			// For others, '='.
			else
			{
				if( Tools->current().is( name_operator ) && Tools->current().opValue() == type::operators::code::assign )
					return true;
			}

			return false;
		}
		bool ParseExpression::_parseTypeTupleAttribute( EonType& tuple )
		{
			// We accept the following constructs:
			// 1. "("
			// 2. <name>=<type-name/tuple>
			// 3. <type-name>

			// Do we have a sub-tuple?
			if( Tools->current().is( name_open ) )
			{
				Tools->forward();
				return _parseSubTypeTuple( tuple );
			}

			// Do we have name and type?
			else if( Tools->peek().is( name_operator ) && Tools->peek().opValue() == type::operators::code::assign )
				return _parseTypeTupleNameAndType( tuple );

			// Must be single type name
			else if( Tools->current().is( name_name ) )
			{
				tuple = EonType( Tools->current().nameValue() );
				Tools->forward();
				return true;
			}

			Tools->reporter().error( "Expected an attribute name or type-tuple here!", Tools->current().source() );
			return false;
		}
		bool ParseExpression::_parseSubTypeTuple( EonType& tuple, name_t sub_name )
		{
			EonType sub;
			if( !__parseTypeTuple( sub, sub_name, name_close ) )
				return false;
			tuple = sub;
			return true;
		}
		bool ParseExpression::_parseTypeTupleNameAndType( EonType& tuple )
		{
			if( !Tools->current().is( name_name ) )
			{
				Tools->reporter().error( "Expected a valid type name here!", Tools->current().source() );
				return false;
			}
			auto name = Tools->current().nameValue();
			Tools->forward( 2 );

			// Next is either a name or a type tuple
			if( Tools->current().is( name_name ) )
			{
				Tools->forward();
				tuple = EonType( name, Tools->current().nameValue() );
				Tools->forward();
				return true;
			}
			else if( Tools->current().is( name_open ) )
			{
				Tools->forward();
				return _parseSubTypeTuple( tuple, name );
			}
			Tools->reporter().error( "Expected a type name or type tuple here!", Tools->current().source() );
			return false;
		}

		bool ParseExpression::_popOperatorsUntil( type::operators::code op )
		{
			while( !Tools->opStack().empty() && Tools->opStack().top()->instanceType().name() == name_operator
				&& ((actions::OperatorAction*)Tools->opStack().top())->opCode() != op )
			{
				if( !_popOperatorArgs() )
					return false;
			}
			return true;
		}
		bool ParseExpression::_popOperatorsFor( type::operators::code op )
		{
			auto input_prec = type::operators::inputPrecedence( op );
			while( !Tools->opStack().empty() && input_prec <= Tools->opStack().top()->stackPrecedence() )
				_popOperatorArgs();
			return true;
		}
		bool ParseExpression::_popOperatorArgs()
		{
			auto action = (actions::OperatorAction*)Tools->opStack().top();

			// The arguments are in reverse ordering, which needs to be corrected
			auto args = _getArgsInOrder( action->numArguments(), action->opCode() );
			if( args.size() < action->numArguments() )
				return false;

			// The node for these arguments
			type::Node op_node;

			// If we have the comma operator, we actually have a tuple
			if( action->opCode() == type::operators::code::comma )
				op_node = _finalizeTuple( std::move( args ) );

			// Otherwise, we have a proper operator
			else
				op_node = _finalizeOperator( action, std::move( args ) );

			Tools->opStack().pop();
			Tools->treeStack().push( std::move( op_node ) );
			return true;
		}
		std::list<type::Node> ParseExpression::_getArgsInOrder( index_t num_args, type::operators::code op )
		{
			std::list<type::Node> args;
			for( index_t i = 0; i < num_args; ++i )
			{
				if( Tools->treeStack().empty() )
				{
					Tools->reporter().error( "Missing operand #" + string( num_args - i ) + " for operator \""
						+ type::operators::mapCode( op ) + "\"!", Tools->current().source() );
					break;
				}
				args.push_front( std::move( Tools->treeStack().top() ) );
				Tools->treeStack().pop();
			}
			return args;
		}
		type::Node ParseExpression::_finalizeTuple( std::list<type::Node>&& args )
		{
			Tuple* tuple{ nullptr };

			// If the first argument is a non-finalized tuple, we add the second argument as an attribute to it
			auto arg = args.begin();
			if( arg->isValue() && arg->value()->generalType() == name_tuple
				&& ( (type::BasicTuple*)arg->value() )->isPlain() && !( (type::BasicTuple*)arg->value() )->finalized() )
				tuple = (Tuple*)arg->consumeValue();

			// Otherwise, we create a tuple and add the first argument as the first attribute
			else
			{
				tuple = new Tuple();
				tuple->source( arg->source() );
				if( arg->isValue() )
					tuple->add( arg->consumeValue() );
				else if( arg->isOperator() )
					tuple->add( new ExprInstance( std::move( *arg ), arg->source() ) );
				else if( arg->isAction() )
					tuple->add( new ExprInstance( std::move( *arg ), arg->source() ) );
			}

			// Add the second argument as attribute
			++arg;
			if( arg->isValue() )
				tuple->add( arg->consumeValue() );
			else if( arg->isOperator() )
				tuple->add( new ExprInstance( std::move( *arg ), arg->source() ) );
			else if( arg->isAction() )
				tuple->add( new ExprInstance( std::move( *arg ), arg->source() ) );
			source::Ref source = tuple->source();
			source.end( arg->source().end() );
			tuple->source( source );
			return type::Node::newValue( tuple );
		}
		type::Node ParseExpression::_finalizeOperator( actions::OperatorAction* action, std::list<type::Node>&& args )
		{
			// Add the arguments to an operator node
			auto op_node = type::Node::newOperator( action );
			for( auto& arg : args )
				op_node.add( std::move( arg ) );

			// Now we need to figure out if there is an action for this
			// operator based on the first operand - if any
			if( op_node.numChildren() > 0 )
			{
				auto& child = op_node.child( 0 );
				EonType type = child.isValue() ? child.value()->type()
					: child.isAction() ? child.action().returnType()
					: child.isOperator() ? child.opr().returnType()
					: child.isName() ? EonType( name_name )
					: child.isEllipsis() ? EonType( name_ellipsis ) : EonType();
				EonType args;
				for( size_t i = 1; i < op_node.numChildren(); ++i )
				{
					if( op_node.child( i ).isValue() )
						args = EonType( op_node.child( i ).value()->type() );
					else if( op_node.child( i ).isOperator() )
						args = EonType( op_node.child( i ).opr().returnType() );
					else if( op_node.child( i ).isAction() )
						args = EonType( op_node.child( i ).action().returnType() );
				}
				// TODO: Find out if we can identify return type and include it below!
				auto operators = Tools->scope().global().getActions( op_node.opr().name(), type, args );
				if( !_checkOperator( operators, *action, _args( op_node ), EonType() ) )
					return false;
				op_node.action( **operators.begin() );
			}
			return op_node;
		}

		string ParseExpression::_args( const type::Node& node ) const
		{
			string args;
			for( size_t i = 0; i < node.numChildren(); ++i )
			{
				if( !args.empty() )
					args += ", ";
				auto& child = node.child( i );
				if( child.isOperator() )
					args += child.opr().returnType().str();
				else if( child.isAction() )
					args += child.action().returnType().str();
				else if( child.isValue() )
					args += child.value()->type().str();
			}
			return args;
		}

		bool ParseExpression::_checkOperator( const std::list<type::Action*>& operators, actions::OperatorAction& op,
			string args, const EonType& return_type ) const
		{
			string rtype = return_type ? "->" + return_type.str() : "";
			if( operators.empty() )
			{
				Tools->reporter().error( "No action implemented for operator \""
					+ type::operators::mapCode( op.opCode() ) + "\" (" + args + ")" + rtype,
					op.source() );
				return false;
			}
			if( operators.size() > 1 )
			{
				Tools->reporter().error( "Ambiguous operator \"" + type::operators::mapCode( op.opCode() )
					+ "\" (" + args + ")" + rtype, op.source() );
				for( auto& action : operators )
				{
					string args;
					for( auto& arg : action->arguments() )
					{
						if( !args.empty() )
							args += ", ";
						args += arg.type().str();
					}
					string rtype = action->returnType() ? "->" + action->returnType().str() : "";
					Tools->reporter().note( "Could be \"" + eon::str( op.name() ) + "\" (" + args + ")" + rtype,
						op.source() );
				}
				return false;
			}
			return true;
		}
	}
}
*/