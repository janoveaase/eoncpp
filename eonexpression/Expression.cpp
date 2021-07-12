#include "Expression.h"
#include <eontokenizer/Tokenizer.h>
#include <eonof/Parser.h>
#include <eontuple/Value.h>
#include "OperatorNode.h"
#include "OperandNode.h"
#include <eontuple/VariableValue.h>


namespace eon
{
	expression& expression::operator=( const expression& other )
	{
		Root = other.Root ? other.Root->copy() : other.Root;
		return *this;
	}




	string expression::postfixStr() const {
		return Root ? Root->postorderStr() : ""; }
	string expression::infixStr() const {
		return Root ? Root->inorderStr() : ""; }

	tup::valueptr expression::evaluate() const {
		tup::variables vars; return evaluate( vars ); }
	tup::valueptr expression::evaluate( tup::variables& vars ) const {
		return Root ? Root->evaluate( vars ) : tup::valueptr(); }

	bool expression::equal( const expression& other,
		const tup::variables& vars ) const noexcept
	{
		if( !Root && !other.Root )
			return true;
		if( !Root || !other.Root )
			return false;
		return Root->equal( other.Root, vars );
	}




	void expression::_parse( string&& expr, tup::variables& vars )
	{
		source src( "expression", std::move( expr ) );
		auto tokens = tokenizer()( src );
		std::stack<tup::operators::code> op_stack;
		op_stack.push( tup::operators::code::undef );
		std::stack<expr::nodeptr> tree_stack;
		tokenparser parser( std::move( tokens ) );
		tup::operators::code last_type{ tup::operators::code::undef };
		while( parser )
		{
			if( parser.current().space() )
			{
				parser.forward();
				continue;
			}
			_processToken( parser, vars, last_type, op_stack, tree_stack );
		}
		_processToken( parser, vars, last_type, op_stack, tree_stack );
		if( tree_stack.size() != 1 )
			throw BadExpression( "Invalid expression" );
		Root = tree_stack.top();
	}

	void expression::_processToken( tokenparser& parser, tup::variables& vars,
		tup::operators::code& last_type,
		std::stack<tup::operators::code>& op_stack,
		std::stack<expr::nodeptr>& tree_stack )
	{
		tup::operators::code type{ tup::operators::code::undef };
		if( parser )
		{
			if( parser.current().symbol() || parser.current().match( "!" ) )
			{
				// Since we allow up to 2 symbols to form an operator, we must
				// always check if the next token is a symbol and forms an
				// operator joined with current.
				if( parser.exists() && parser.ahead().symbol() )
				{
					type = tup::operators::mapCode( parser.current().substr()
						+ parser.ahead().substr() );
					if( type == tup::operators::code::value )
						type = tup::operators::code::undef;
					else
					{
						parser.forward();
						parser.forward();
					}
				}
			}
			if( type == tup::operators::code::undef )
			{
				type = tup::operators::mapCode( parser.current().substr() );
				if( type != tup::operators::code::value )
					parser.forward();
			}
		}
		if( last_type != tup::operators::code::value
			&& type != tup::operators::code::value
			&& last_type != tup::operators::code::close_brace )
		{
			if( type == tup::operators::code::minus )
				type = tup::operators::code::unary_minus;
		}
		last_type = type;
		switch( type )
		{
			case tup::operators::code::value:
				tree_stack.push( _parseValue( parser, vars ) );
				break;
			case tup::operators::code::close_brace:
				while( !op_stack.empty()
					&& op_stack.top() != tup::operators::code::open_brace )
				{
					auto root = expr::nodeptr(
						new expr::operatornode( op_stack.top() ) );
					for( size_t i = 0,
							num_ops = tup::operators::numOperands( op_stack.top() );
						i < num_ops;
						++i )
					{
						if( tree_stack.empty() )
							throw BadExpression( "Operator '" + root->str()
								+ "' is missing " + string( num_ops - i )
								+ " operand(s)" );
						root->addChild( tree_stack.top() );
						tree_stack.pop();
					}
					op_stack.pop();
					tree_stack.push( std::move( root ) );
				}
				if( op_stack.top() == tup::operators::code::open_brace )
					op_stack.pop();
				else
					throw BadExpression( "Unbalanced parenthesis" );
				break;
			case tup::operators::code::if_else2:
				while( op_stack.top() != tup::operators::code::if_else )
				{
					auto root = expr::nodeptr(
						new expr::operatornode( op_stack.top() ) );
					for( size_t i = 0,
						num_ops = tup::operators::numOperands( op_stack.top() );
						i < num_ops;
						++i )
					{
						if( tree_stack.empty() )
							throw BadExpression( "Operator '" + root->str()
								+ "' is missing " + string( num_ops - i )
								+ " operand(s)" );
						root->addChild( tree_stack.top() );
						tree_stack.pop();
					}
					op_stack.pop();
					tree_stack.push( std::move( root ) );
				}
				break;
			default:
				while( tup::operators::inputPrecedence( type )
					<= tup::operators::stackPrecedence( op_stack.top() ) )
				{
					auto root = expr::nodeptr(
						new expr::operatornode( op_stack.top() ) );
					for( size_t i = 0,
						num_ops = tup::operators::numOperands( op_stack.top() );
						i < num_ops;
						++i )
					{
						if( tree_stack.empty() )
							throw BadExpression( "Operator '" + root->str()
								+ "' is missing " + string( num_ops - i )
								+ " operand(s)" );
						root->addChild( tree_stack.top() );
						tree_stack.pop();
					}
					op_stack.pop();
					tree_stack.push( std::move( root ) );
				}
				op_stack.push( type );
				break;
		}
	}

	expr::nodeptr expression::_parseValue( tokenparser& parser,
		tup::variables& vars )
	{
		tf::Parser tfparser;
		auto value = tfparser.parseValue( parser, vars,
			tf::Parser::ContextType::expression, false );
		if( value )
			return expr::nodeptr( new expr::operandnode( value ) );
		else
			throw BadExpression( "Don't recognize this: "
				+ parser.current().substr() );
	}
}
