#include "ParseExpression.h"
#include <eontypes/Bool.h>
#include <eontypes/Byte.h>
#include <eontypes/Char.h>
#include <eontypes/Int.h>
#include <eontypes/Short.h>
#include <eontypes/Long.h>
#include <eontypes/Float.h>
#include <eontypes/Low.h>
#include <eontypes/High.h>
#include <eontypes/Name.h>
#include <eontypes/Handle.h>
#include <eontypes/String.h>
#include <eontypes/Bytes.h>
#include <eontypes/Path.h>
#include <eontypes/Regex.h>
#include <eontypes/NamePath.h>
#include <eontypes/OperatorAction.h>
#include <eontypes/Expression.h>
#include <eontypes/DataTuple.h>
#include <eontypes/MetaData.h>


namespace eon
{
	namespace parser
	{
		bool ParseExpression::operator()( ToolBox& tools, type::Node& result )
		{
			Tools = &tools;
			if( !Tools->parser() )
				return false;

			if( !Tools->ready() )
			{
				Tools->reset();
				Tools->reporter().warning( "Parser reset after errors!", Tools->parser().current().source() );
			}

			// Start context
			Tools->push( ToolBox::Context::expression );
			bool status = false;

			// Accept literal exception
			if( Tools->parser().current().is( name_name )
				&& Tools->parser().current().str() == "e"
				&& Tools->parser().ahead().is( name_open ) )
			{
				Tools->parser().forward( 2 );
				Literal = true;
				status = _parse( result );
			}
			else
			{
				// Not a literal expression
				// We trust that the caller has determined that we are actually parsing
				// an expression (or at least that the source at current position is
				// not the start of any other known construct).
				status = _parse( result );
			}

			// End context
			Tools->popContext();
			return status;
		}




		bool ParseExpression::_parse( type::Node& result )
		{
			// This method is only called if there is at least one token left
			for( ; ; )
			{
				// If we are in an expression context and we have a literal
				// expression, then ")" will end it
				if( Tools->context() == ToolBox::Context::expression
					&& Literal
					&& Tools->parser().current().is( name_close ) )
				{
					Tools->parser().forward();
					break;
				}

				if( !_processToken() )
					return false;
			}

			// TODO: Complete the tree stack

			return true;
		}

		bool ParseExpression::_processToken()
		{
			if( Tools->parser() )
			{
				_checkContext();
				switch( Tools->context() )
				{
					case ToolBox::Context::expression:
						if( !_processExpressionToken() )
							return false;
						break;
					case ToolBox::Context::data_tuple:
						if( !_parseDataTuple() )
							return false;
						break;
					case ToolBox::Context::meta_data:
						if( !_parseMetaTuple() )
							return false;
						break;
					case ToolBox::Context::tuple:
						if( !_parseTuple() )
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

		void ParseExpression::_checkContext()
		{
			// Check if we have a plain tuple
			if( Tools->parser().ahead().is( name_symbol ) && Tools->parser().ahead().str() == "," )
				Tools->push( ToolBox::Context::tuple );

			// Check for type tuple
			else if( Tools->parser().match( { "T", "(" } ) )
				Tools->push( ToolBox::Context::type_tuple );

			// Check for meta data
			else if( Tools->parser().match( { "M", "(" } ) )
				Tools->push( ToolBox::Context::meta_data );

			// Check for data tuple
			else if( Tools->parser().match( { "D", "(" } ) )
				Tools->push( ToolBox::Context::data_tuple );
		}

		bool ParseExpression::_processExpressionToken()
		{
			// Try to map an explicit operator first
			auto type = type::operators::mapCode( Tools->parser().current().str() );
			if( type != type::operators::code::undef )
			{
				if( !_parseOperator( type ) )
					return false;
			}
			else
			{
				auto& token = Tools->parser().current();

				if( token.is( name_name ) )
				{
					if( !_parseName() )
						return false;
				}
				else if( token.is( name::get( "literal_name" ) ) )
					_parseLiteralName();
				else if( token.is( name_string ) )
					_parseString();
				else if( token.is( name_bytes ) )
					_parseBytes();
				else if( token.is( name_path ) )
					_parsePath();
				else if( token.is( name_char ) )
					_parseChar();
				else if( token.is( name_byte ) )
					_parseByte();
				else if( token.is( name_indentation ) )
					_parseIndentation();
				else if( token.is( name_regex ) )
					_parseRegex();
				else if( token.is( name_bool ) )
					_parseBool();
				else if( token.is( name_float ) )
					_parseFloat();
				else if( token.is( name_namepath ) )
					_parseNamePath();
				else if( token.is( name_digits ) )
					_parseInt();
			}
			return true;
		}

		bool ParseExpression::_parseTuple()
		{
			auto parenthesized = Tools->parser().current().is( name_open );
			if( parenthesized )
				Tools->parser().forward();

			// A literal tuple runs until ")" if parenthesized and ";" if not
			type::BasicTuple tuple;
			if( !_parseTuple( tuple, type::BasicTuple::Class::tuple, parenthesized ) )
				return false;
			Tools->treeStack().push( type::Node::newValue( new Tuple( std::move( tuple ) ) ) );
			return true;
		}
		bool ParseExpression::_parseDataTuple()
		{
			// Literal data tuples start with "D" + "("
			Tools->parser().forward( 2 );

			// A literal tuple runs until ")"
			type::BasicTuple tuple;
			if( !_parseTuple( tuple, type::BasicTuple::Class::data, true ) )
				return false;
			Tools->treeStack().push( type::Node::newValue( new DataTuple( std::move( tuple ) ) ) );
			return true;
		}
		bool ParseExpression::_parseMetaTuple()
		{
			// Literal data tuples start with "M" + "("
			Tools->parser().forward( 2 );

			// A literal tuple runs until ")"
			type::BasicTuple tuple;
			if( !_parseTuple( tuple, type::BasicTuple::Class::meta, true ) )
				return false;
			Tools->treeStack().push( type::Node::newValue( new MetaData( std::move( tuple ) ) ) );
			return true;
		}
		bool ParseExpression::_parseTuple( type::BasicTuple& tuple, type::BasicTuple::Class cls, bool parenthesized )
		{
			// Note that we parse as a dynamic tuple!
			while( Tools->parser() )
			{
				// Check for end of tuple
				if( parenthesized )
				{
					if( Tools->parser().current().is( name_close ) )
					{
						Tools->parser().forward();
						break;
					}
				}
				else if( Tools->parser().current().is( name_symbol ) && Tools->parser().current().str() == ";" )
				{
					Tools->parser().forward();
					break;
				}

				if( !_parseTupleAttribute( tuple ) )
					return false;

				// Expect comma, or ")" if parenthesized, ";" if not
				if( Tools->parser().current().is( name_symbol ) && Tools->parser().current().str() == "," )
					Tools->parser().forward();
				else if( parenthesized )
				{
					if( !Tools->parser().current().is( name_close ) )
					{
						Tools->reporter().error( "Expected comma or ')' here!", Tools->parser().current().source() );
						return nullptr;
					}
				}
				else if( !Tools->parser().current().is( name_symbol ) || Tools->parser().current().str() != ";" )
				{
					Tools->reporter().error( "Expected comma or ';' here!", Tools->parser().current().source() );
					return nullptr;
				}
			}
			return new type::BasicTuple( std::move( tuple ) );
		}
		bool ParseExpression::_parseTupleAttribute( type::BasicTuple& tuple )
		{
			static std::set<string> qualifiers{ "read", "modify", "consume" };
			// We accept the following constructs:
			// 1. "("
			// 2. [[<name>] [as <type>]][=[read|modify|consume] <value>|...]

			// Prepare attribute details
			type::Qualifier qual{ type::Qualifier::read };	// Default is read!
			name_t name{ no_name };
			source::Ref name_src;
			TypeTuple type;
			source::Ref type_src;
			MetaData* meta{ nullptr };
			type::Object* value{ nullptr };
			source::Ref src = Tools->parser().current().source();

			// Check for name
			if( Tools->parser().current().is( name_name )
				&& ( ( Tools->parser().ahead().is( name_name ) && Tools->parser().ahead().str() == "as" )
					|| ( Tools->parser().ahead().is( name_operator ) && Tools->parser().ahead().str() == "=" ) ) )
			{
				name = name::get( Tools->parser().current().str() );
				name_src = Tools->parser().current().source();
				Tools->parser().forward();

				if( Tools->parser().current().is( name_name ) && Tools->parser().ahead().str() == "as" )
				{
					Tools->parser().forward();
					if( Tools->parser().current().is( name_name ) )
					{
						type += name_name;
						type_src = Tools->parser().current().source();
						Tools->parser().forward();
					}
					else if( Tools->parser().match( { "T", "(" } ) )
					{
						type_src = Tools->parser().current().source();
						auto tp = __parseTypeTuple();
						type = std::move( *tp );
						type_src.end( Tools->parser().current().source().start() );
						delete tp;
					}
					else
					{
						Tools->reporter().error( "Expected a type specification here!", Tools->parser().current().source() );
						return false;
					}
				}
			}

			// We have a value if there is no name or we have a "="
			if( !name || Tools->parser().ahead().is( name_operator ) && Tools->parser().ahead().str() == "=" )
			{
				if( name )	// Skip the "="
					Tools->parser().forward();

				// Check for qualifiers
				if( Tools->parser().current().is( name_name )
					&& qualifiers.find( Tools->parser().current().str() ) != qualifiers.end() )
				{
					qual = type::mapQualifier( Tools->parser().current().str() );
					Tools->parser().forward();
				}

				// Get value
				if( Tools->parser().current().is( name_point ) && Tools->parser().current().str() == "..." )
					Tools->parser().forward();		// We can skip the ellipsis value
				else
				{
					// Get value as expression
					auto& source = Tools->parser().current().source();
					type::Node expr;
					if( !ParseExpression()( *Tools, expr ) )
					{
						Tools->reporter().error( "Expected a value here!", source );
						return false;
					}

					if( expr.isValue() )
						value = expr.consumeValue();
					else if( expr.isAction() )
						value = new type::Expression( std::move( expr ) );
				}
			}

			// If we have no value, then we must have a type
			else if( type )
			{
				if( type.isName() )
				{
					auto inst_type = Tools->scope().find( type.asName() );
					if( inst_type &&
						( inst_type->generalType() == name_definition || inst_type->generalType() == name_instance ) )
						value = ( (type::Definition*)inst_type )->instantiate();
				}
				if( !value )
				{
					Tools->reporter().error( *type.asName() + " cannot provide a default/inital value!", type_src );
					return false;
				}
			}
			else
			{
				if( Tools->parser() )
					src.end( Tools->parser().current().source().start() );
				else
					src.end( Tools->parser().last().source().start() );
				Tools->reporter().error( "Attribute defined without explicit type and without value!", src );
			}

			// If we have no type, try to deduce from value
			if( !type )
			{
				if( value->generalType() != name_action )	// The only type we can't deduce outright
					type = value->type();
			}

			// Finalize
			if( Tools->parser() )
				src.end( Tools->parser().current().source().start() );
			else
				src.end( Tools->parser().last().source().start() );
			try
			{
				tuple += type::Attribute( qual, name, meta, type, value );
			}
			catch( type::AccessDenied )
			{
				Tools->reporter().error( "Not a legal attribute for this type of tuple!", src );
				return false;
			}
			catch( type::DuplicateName )
			{
				Tools->reporter().error( "This name is already in use!", name_src );
				return false;
			}
			return true;
		}

		bool ParseExpression::_parseTypeTuple()
		{
			// We have seen the following tokens: "T", "("
			Tools->parser().forward( 2 );

			auto tuple = __parseTypeTuple();
			if( !tuple )
				return false;
			Tools->treeStack().push( type::Node::newValue( new type::TypeTupleObject( std::move( *tuple ) ) ) );
			delete tuple;
			return true;
		}
		TypeTuple* ParseExpression::__parseTypeTuple( name_t name )
		{
			TypeTuple tuple;
			if( name != no_name )
				tuple.name( name );
			while( Tools->parser() )
			{
				// We end the type tuple at matching ")"
				if( Tools->parser().current().is( name_close ) )
				{
					Tools->parser().forward();
					break;
				}

				if( !_parseTypeTupleAttribute( tuple ) )
					break;

				// Expect comma or ")"
				if( Tools->parser().current().is( name_symbol ) && Tools->parser().current().str() == "," )
					Tools->parser().forward();
				else if( !Tools->parser().current().is( name_close ) )
				{
					Tools->reporter().error( "Expected comma or ')' here!", Tools->parser().current().source() );
					return nullptr;
				}
			}
			return new TypeTuple( std::move( tuple ) );
		}
		bool ParseExpression::_parseTypeTupleAttribute( TypeTuple& tuple )
		{
			// We accept the following constructs:
			// 1. "("
			// 2. <name>=<type-name/tuple>
			// 3. <type-name>

			// Do we have a sub-tuple?
			if( Tools->parser().current().is( name_open ) )
			{
				Tools->parser().forward();
				return _parseSubTypeTuple( tuple );
			}

			// Do we have name and type?
			else if( Tools->parser().ahead().str() == "=" )
				return _parseTypeTupleNameAndType( tuple );

			// Must be single type name
			else if( Tools->parser().current().is( name_name ) )
			{
				tuple += new NameElement( name::get( Tools->parser().current().str() ) );
				Tools->parser().forward();
				return true;
			}

			Tools->reporter().error( "Expected an attribute name or type-tuple here!", Tools->parser().current().source() );
			return false;
		}
		bool ParseExpression::_parseSubTypeTuple( TypeTuple& tuple, name_t sub_name )
		{
			auto sub = __parseTypeTuple( sub_name );
			if( !sub )
				return false;
			tuple += sub;
			return true;
		}
		bool ParseExpression::_parseTypeTupleNameAndType( TypeTuple& tuple )
		{
			if( !Tools->parser().current().is( name_name ) )
			{
				Tools->reporter().error( "Expected a valid type name here!", Tools->parser().current().source() );
				return false;
			}
			auto name = eon::name::get( Tools->parser().current().str() );
			Tools->parser().forward( 2 );

			// Next is either a name or a type tuple
			if( Tools->parser().current().is( name_name ) )
			{
				Tools->parser().forward();
				tuple += new NameElement( name, name::get( Tools->parser().current().str() ) );
				Tools->parser().forward();
				return true;
			}
			else if( Tools->parser().current().is( name_open ) )
			{
				Tools->parser().forward();
				return _parseSubTypeTuple( tuple, name );
			}
			Tools->reporter().error( "Expected a type name or type tuple here!", Tools->parser().current().source() );
			return false;
		}

		bool ParseExpression::_parseOperator( type::operators::code type )
		{
			Tools->parser().forward();
			if( type == type::operators::code::close_brace )
			{
				if( !_popOpsUntil( type::operators::code::open_brace ) )
					return false;
			}
			else
			{
				auto& sequence = type::operators::sequence( type );
				if( sequence.size() > 1 && type != sequence[ 0 ].Op )
				{
					// We can ignore all opereators except the first and the last!
					if( sequence[ sequence.size() - 1 ].Op == type )
						return _popOpsUntil( sequence[ 0 ].Op );
					else
						return true;
				}
				else
					_popOpsFor( type );
			}

			_addAction( new actions::OperatorAction( type ) );
			return true;
		}
		bool ParseExpression::_parseName()
		{
			// Is it 'me' or 'my'?
			if( Tools->parser().current().str() == "me" || Tools->parser().current().str() == "my" )
			{
				auto me = Tools->scope().get( name_me );
				if( !me )
				{
					Tools->reporter().error(
						"Use of \"" + Tools->parser().current().str() + "\" only possible within an action!",
						Tools->parser().current().source() );
					return false;
				}
				Tools->treeStack().push( type::Node::newValue( me ) );
				Tools->parser().forward();
				return true;
			}

			// Is it a known entity?
			auto name = name::get( Tools->parser().current().str() );
			auto var = Tools->scope().find( name );
			if( var )
			{
				if( var->generalType() == name_action )
					_addAction( (type::Action*)var );
				else
					Tools->treeStack().push( type::Node::newValue( var ) );
				return true;
			}

			// Unknown entity
			Tools->treeStack().push( type::Node::newName( name ) );
			return true;
		}
		void ParseExpression::_parseLiteralName()
		{
			Tools->treeStack().push( type::Node::newValue(
				new NameInstance( name::get( Tools->parser().current().str() ) ) ) );
			Tools->parser().forward();
		}
		void ParseExpression::_parseString()
		{
			Tools->treeStack().push( type::Node::newValue( new StringInstance( Tools->parser().current().str() ) ) );
			Tools->parser().forward();
		}
		void ParseExpression::_parseBytes()
		{
			Tools->treeStack().push( type::Node::newValue( new BytesInstance( Tools->parser().current().str() ) ) );
			Tools->parser().forward();
		}
		void ParseExpression::_parsePath()
		{
			Tools->treeStack().push( type::Node::newValue( new PathInstance( Tools->parser().current().str() ) ) );
			Tools->parser().forward();
		}
		void ParseExpression::_parseChar()
		{
			Tools->treeStack().push( type::Node::newValue( new CharInstance( *Tools->parser().current().str().begin() ) ) );
			Tools->parser().forward();
		}
		void ParseExpression::_parseByte()
		{
			Tools->treeStack().push( type::Node::newValue( new ByteInstance( static_cast<byte_t>( *Tools->parser().current().str().begin() ) ) ) );
			Tools->parser().forward();
		}
		void ParseExpression::_parseIndentation()
		{
		}
		void ParseExpression::_parseRegex()
		{
			Tools->treeStack().push( type::Node::newValue( new RegexInstance( Tools->parser().current().str() ) ) );
			Tools->parser().forward();
		}
		void ParseExpression::_parseBool()
		{
			Tools->treeStack().push( type::Node::newValue( new BoolInstance( Tools->parser().current().str() == "true" ) ) );
			Tools->parser().forward();
		}
		void ParseExpression::_parseFloat()
		{
			Tools->treeStack().push( type::Node::newValue( new HighInstance( Tools->parser().current().str().toLongDouble() ) ) );
			Tools->parser().forward();
		}
		void ParseExpression::_parseNamePath()
		{
			Tools->treeStack().push( type::Node::newValue( new NamePathInstance( Tools->parser().current().str() ) ) );
			Tools->parser().forward();
		}
		void ParseExpression::_parseInt()
		{
			Tools->treeStack().push( type::Node::newValue( new LongInstance( Tools->parser().current().str().toInt64() ) ) );
			Tools->parser().forward();
		}

		void ParseExpression::_addAction( type::Action* action )
		{
			Tools->treeStack().push( type::Node::newAction( action ) );
		}

		bool ParseExpression::_popOpsUntil( type::operators::code op )
		{
			while( !Tools->opStack().empty() && Tools->opStack().top()->opCode() != op )
				_processPopArgs( op );
			if( !Tools->opStack().empty() && Tools->opStack().top()->opCode() == op )
			{
				Tools->opStack().pop();
				return true;
			}
			Tools->reporter().error( "Missing previous '" + type::operators::mapCode( op ) + "'!",
				Tools->parser().current().source() );
			return false;
		}
		bool ParseExpression::_popOpsFor( type::operators::code op )
		{
			auto input_prec = type::operators::inputPrecedence( op );
			while( !Tools->opStack().empty()
				&& input_prec <= type::operators::stackPrecedence( Tools->opStack().top()->opCode() ) )
				_processPopArgs( op );
			return true;
		}
		bool ParseExpression::_processPopArgs( type::operators::code op )
		{
			auto root = type::Node::newAction( Tools->opStack().top() );
			auto arg_pos = Tools->opStack().top()->arguments().numAttributes();
			for( auto& arg : Tools->opStack().top()->arguments() )
			{
				// We skip syntax elements
				if( arg.type().asName() == name_syntax )
					continue;

				if( arg.value() == nullptr && Tools->treeStack().empty() )
				{
					if( arg.name() )
						Tools->reporter().error(
							"Missing required argument \"" + *arg.name() + "\" of type " + arg.type().str() + "!",
							Tools->parser().current().source() );
					else
						Tools->reporter().error(
							"Missing required argument #" + string( arg_pos ) + " of type " + arg.type().str() + "!",
							Tools->parser().current().source() );
					return false;
				}
				root.add( std::move( Tools->treeStack().top() ) );
				Tools->treeStack().pop();
				--arg_pos;
			}
			Tools->opStack().pop();
			Tools->treeStack().push( std::move( root ) );
			return true;
		}
	}
}
