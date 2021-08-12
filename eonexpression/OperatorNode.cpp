#include "OperatorNode.h"
#include "OperandNode.h"
#include "Evaluate.h"
#include <eonvariables/Value.h>


namespace eon
{
	namespace expr
	{
		void operatornode::setContext( vars::valueptr context )
		{
			for( auto& child : Children )
				child->setContext( context );
		}




		string operatornode::postorderStr() const
		{
			switch( operands() )
			{
				case 1:
					return Children[ 0 ]->postorderStr() + " " + str();
				case 2:
					return Children[ 1 ]->postorderStr() + " "
						+ Children[ 0 ]->postorderStr() + " " + str();
				case 3:
					return Children[ 2 ]->postorderStr() + " "
						+ Children[ 1 ]->postorderStr() + " "
						+ Children[ 0 ]->postorderStr() + " " + str();
			}
			return string();
		}
		string operatornode::inorderStr() const
		{
			std::vector<bool> require_brace;
			auto right2left = vars::operators::inputPrecedence( Type ) > vars::operators::stackPrecedence( Type );

			// We require braces on a child if the child is an operator of
			// lower precedence that 'this'
			for( auto child : Children )
			{
				require_brace.push_back( child->isOperator()
					&& ( vars::operators::inputPrecedence( child->type() ) < vars::operators::inputPrecedence( Type ) ) );
			}		
			
			for( auto child : Children )
			{
				// Operands don't need bracing
				if( !child->isOperator() )
				{
					require_brace.push_back( false );
					continue;
				}


				// Right-to-left operators need bracing if right operand is an
				// operator
				if( right2left && child->children()[ 0 ]->isOperator() )
				{
					require_brace.push_back( true );
					continue;
				}

				require_brace.push_back( false );
			}
			switch( operands() )
			{
				case 1:
					return str() + " " + Children[ 0 ]->inorderStr();
				case 2:
				{
					string s;
					if( require_brace[ 1 ] )
						s += "( ";
					s += Children[ 1 ]->inorderStr();
					if( require_brace[ 1 ] )
						s += " )";
					s += " " + str() + " ";
					if( require_brace[ 0 ] )
						s += "( ";
					s += Children[ 0 ]->inorderStr();
					if( require_brace[ 0 ] )
						s += " )";
					return s;
				}
				case 3:
				{
					string s;
					if( require_brace[ 2 ] )
						s += "( ";
					s += Children[ 2 ]->inorderStr();
					if( require_brace[ 2 ] )
						s += " )";

					// Since we only support 1 ternary operator, we can "cheat"
					// and add the operator keywords "if" and "else" without
					// any considerations.
					s += " if ";

					if( require_brace[ 1 ] )
						s += "( ";
					s += Children[ 1 ]->inorderStr();
					if( require_brace[ 1 ] )
						s += " )";

					s += " else ";

					if( require_brace[ 0 ] )
						s += "( ";
					s += Children[ 0 ]->inorderStr();
					if( require_brace[ 0 ] )
						s += " )";
					return s;
				}
			}
			return string();
		}

		vars::valueptr operatornode::evaluateAccurate( vars::variables& vars ) const
		{
			switch( vars::operators::numOperands( Type ) )
			{
				case 1:
					return evaluate::unary( Type, vars, Children[ 0 ] );
				case 2:
					return evaluate::binary( Type, vars, Children[ 1 ], Children[ 0 ] );
				case 3:
					return evaluate::ternary( Type, vars, Children[ 2 ], Children[ 1 ], Children[ 0 ] );
			}
			return vars::valueptr();
		}

		nodeptr operatornode::copy() const
		{
			auto cpy = new operatornode( Type );
			for( auto child : Children )
				cpy->addChild( child->copy() );
			return nodeptr( cpy );
		}

		bool operatornode::equal( const nodeptr& other ) const noexcept
		{
			if( !other )
				return false;
			if( Type != other->type() )
				return false;
			if( Children.size() != other->children().size() )
				return false;
			for( size_t i = 0; i < Children.size(); ++i )
			{
				if( !Children[ i ]->equal( other->children()[ i ] ) )
					return false;
			}
			return true;
		}
	}
}
