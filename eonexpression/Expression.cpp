#include "Expression.h"
#include <eontokenizer/Tokenizer.h>
#include <eonof/Parser.h>
#include "OperatorNode.h"
#include "OperandNode.h"
#include <eonvariables/Value.h>
#include <eonvariables/VariableValue.h>


namespace eon
{
	expression& expression::operator=( const expression& other )
	{
		Roots.clear();
		for( auto& root : other.Roots )
			Roots.push_back( root->copy() );
		return *this;
	}

	void expression::setContext( vars::valueptr context )
	{
		for( auto& root : Roots )
			root->setContext( context );
	}




	string expression::postfixStr() const
	{
		string str;
		for( auto& root : Roots )
		{
			if( !str.empty() )
				str += "; ";
			str += root->postorderStr();
		}
		return str;
	}
	string expression::infixStr() const
	{
		string str;
		for( auto& root : Roots )
		{
			if( !str.empty() )
				str += "; ";
			str += root->inorderStr();
		}
		return str;
	}

	vars::valueptr expression::evaluate() const {
		vars::variables vars; return evaluate( vars ); }
	vars::valueptr expression::evaluate( vars::variables& vars ) const
	{
		vars::valueptr result;
		for( auto& root : Roots )
		{
			result = root->evaluate( vars );
			if( result->isControl() )
				break;
		}
		return result;
	}

	bool expression::equal( const expression& other ) const noexcept
	{
		if( empty() && other.empty() )
			return true;
		if( empty() || other.empty() )
			return false;
		if( Roots.size() != other.Roots.size() )
			return false;
		for( auto l = Roots.begin(), r = other.Roots.begin(); l != Roots.end(); ++l, ++r )
			if( !( *l )->equal( *r ) )
				return false;
		return true;
	}




	void expression::_parse( string&& expr, vars::variables& vars )
	{
		auto count = expr.count( ';' ) + 1;
		source src( "expression", std::move( expr ) );
		auto tokens = tokenizer()( src );
		std::stack<vars::operators::code> op_stack;
		op_stack.push( vars::operators::code::undef );
		std::stack<expr::nodeptr> tree_stack;
		tokenparser parser( std::move( tokens ) );
		vars::operators::code last_type{ vars::operators::code::undef };
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
		if( tree_stack.size() == 0 )
			throw BadExpression( "Malformed expression!" );
		if( tree_stack.size() > count )
			throw BadExpression( "Malformed expression!" );

		std::list<expr::nodeptr> nodes;
		while( !tree_stack.empty() )
		{
			nodes.push_front( tree_stack.top() );
			tree_stack.pop();
		}
		Roots.clear();
		for( auto& node : nodes )
			Roots.push_back( node );
	}

	inline void runStack( tokenparser& parser,
		std::stack<vars::operators::code>& op_stack, std::stack<expr::nodeptr>& tree_stack )
	{
		auto root = expr::nodeptr( new expr::operatornode( op_stack.top() ) );
		for( size_t i = 0, num_ops = vars::operators::numOperands( op_stack.top() ); i < num_ops; ++i )
		{
			if( tree_stack.empty() )
				throw BadExpression(
					( parser ? parser.current().source().textRefRange() : parser.last().source().textRefRange() )
					+ ": Operator '" + root->str() + "' is missing " + string( num_ops - i ) + " operand(s)!" );
			root->addChild( tree_stack.top() );
			tree_stack.pop();
		}
		op_stack.pop();
		tree_stack.push( std::move( root ) );
	}
	void expression::_processToken( tokenparser& parser, vars::variables& vars, vars::operators::code& last_type,
		std::stack<vars::operators::code>& op_stack, std::stack<expr::nodeptr>& tree_stack )
	{
		vars::operators::code type{ vars::operators::code::undef };
		if( parser )
		{
			if( parser.current().symbol() || parser.current().match( "!" ) || parser.current().match( "-" ) )
			{
				// Since we allow up to 2 symbols to form an operator, we must
				// always check if the next token is a symbol and forms an
				// operator joined with current.
				if( parser.exists() && parser.ahead().symbol() )
				{
					type = vars::operators::mapCode( parser.current().substr() + parser.ahead().substr() );
					if( type == vars::operators::code::value )
						type = vars::operators::code::undef;
					else
					{
						parser.forward();
						parser.forward();
					}
				}
			}
			if( type == vars::operators::code::undef )
			{
				type = vars::operators::mapCode( parser.current().substr() );
				if( type != vars::operators::code::value )
					parser.forward();
			}
		}
		if( last_type != vars::operators::code::value
			&& type != vars::operators::code::value
			&& last_type != vars::operators::code::close_brace )
		{
			if( type == vars::operators::code::minus )
				type = vars::operators::code::unary_minus;
		}
		last_type = type;
		switch( type )
		{
			case vars::operators::code::value:
				tree_stack.push( _parseValue( parser, vars ) );
				break;
			case vars::operators::code::separator:
				while( !op_stack.empty() && op_stack.top() != vars::operators::code::undef )
					runStack( parser, op_stack, tree_stack );
				if( op_stack.size() > 1 )
					throw BadExpression(
						( parser ? parser.current().source().textRefRange() : parser.last().source().textRefRange() )
						+ ": Separator found before expression was completed" );
				break;
			case vars::operators::code::close_brace:
				while( !op_stack.empty() && op_stack.top() != vars::operators::code::open_brace )
					runStack( parser, op_stack, tree_stack );
				if( op_stack.top() == vars::operators::code::open_brace )
					op_stack.pop();
				else
					throw BadExpression( "Unbalanced parenthesis!" );
				break;
			case vars::operators::code::if_else2:
				while( op_stack.top() != vars::operators::code::if_else )
					runStack( parser, op_stack, tree_stack );
				break;
			case vars::operators::code::close_square:
				while( !op_stack.empty() )
				{
					auto opensq = op_stack.top() == vars::operators::code::open_square;
					runStack( parser, op_stack, tree_stack );
					if( opensq )
						break;
				}
				if( op_stack.empty() )
					throw BadExpression( "Unbalanced square brackets!" );
				break;
			default:
				while( vars::operators::inputPrecedence( type )
					<= vars::operators::stackPrecedence( op_stack.top() ) )
					runStack( parser, op_stack, tree_stack );
				op_stack.push( type );
				break;
		}
	}

	expr::nodeptr expression::_parseValue( tokenparser& parser, vars::variables& vars )
	{
		tf::Parser tfparser( parser.last().source() );
		auto value = tfparser.parseValue( parser, vars, tf::Parser::ContextType::expression, false );
		if( value )
			return expr::nodeptr( new expr::operandnode( value ) );
		else
			throw BadExpression( parser.current().source().textRefRange()
				+ ": Don't recognize this: " + parser.current().substr() );
	}
}
