#include "Node.h"
#include "Action.h"
#include "MetaData.h"


namespace eon
{
	namespace type
	{
		Node& Node::copy( const Node& other, scope::Scope& scope )
		{
			Type = other.Type;
			Item = other.Item ? other.Item->copy( scope ) : nullptr;
			Name = other.Name;
			Children.clear();
			for( auto& child : other.Children )
			{
				if( child.isAction() )
					Children.push_back( Node::newAction( (Action*)child.Item->copy( scope ) ) );
				else if( child.isName() )
					Children.push_back( Node::newName( child.Name ) );
				else
					Children.push_back( Node::newValue( child.Item->copy( scope ) ) );
			}
			return *this;
		}




		void Node::infixStr( type::Stringifier& str ) const
		{
			if( isValue() )
			{
				Item->str( str );
				return;
			}

			str.spacingAlways();
			if( isName() )
				str.addWord( Name );
			else
			{
				std::vector<bool> need_par = _operandsNeedingParenthesis( action() );
				switch( action().actionType() )
				{
					case actions::Type::instance:
						_infixInstanceStr( action(), str, need_par );
						break;
					case actions::Type::prefix_first:
						_infixPreFirstStr( action(), str, need_par );
						break;
					case actions::Type::prefix_last:
						_infixPreLastStr( action(), str, need_par );
						break;
					case actions::Type::get:
						_infixGetStr( action(), str, need_par );
						break;
					case actions::Type::set:
						_infixSetStr( action(), str, need_par );
						break;
					case actions::Type::helper:
						_infixHelperStr( action(), str, need_par );
						break;
					case actions::Type::type:
						_infixTypeStr( action(), str, need_par );
						break;
					case actions::Type::_operator:
						_infixOpStr( action(), str, need_par );
						break;
					default:
						break;
				}
			}
		}




		std::vector<bool> Node::_operandsNeedingParenthesis( const Action& action ) const
		{
			std::vector<bool> need;
			for( auto& child : Children )
			{
				if( child.isAction() )
				{
					// If the child is an operator of lower precedence that the
					// parent action, we must enclose!
					if( child.isAction() && ( child.action().inputPrecedence() < action.inputPrecedence() ) )
						need.push_back( true );

					// Same if right-to-left associativity and the first
					// operand is an action.
					else if( action.rightToLeft() && !child.Children.empty() && child.Children[ 0 ].isAction() )
						need.push_back( true );

					// Not needed otherwise
					else
						need.push_back( false );
				}

				// Operands never need parenthesis
				else
					need.push_back( false );
			}
			return need;
		}

		void Node::_infixInstanceStr( const Action& action, Stringifier& str, const std::vector<bool>& need_par ) const
		{
			str.addWord( action.type().asName() );
			_infixStr( action, str, need_par );
		}
		void Node::_infixPreFirstStr( const Action& action, Stringifier& str, const std::vector<bool>& need_par ) const
		{
			str.addWord( action.type().asName() );
			_infixStr( action, str, need_par );
		}
		void Node::_infixPreLastStr( const Action& action, Stringifier& str, const std::vector<bool>& need_par ) const
		{
			str.addWord( action.type().asName() );
			_infixStr( action, str, need_par );
		}
		void Node::_infixGetStr( const Action& action, Stringifier& str, const std::vector<bool>& need_par ) const
		{
			_infixStr( action, str, need_par );
		}
		void Node::_infixSetStr( const Action& action, Stringifier& str, const std::vector<bool>& need_par ) const
		{
			_infixStr( action, str, need_par );
		}
		void Node::_infixHelperStr( const Action& action, Stringifier& str, const std::vector<bool>& need_par ) const
		{
			_infixStr( action, str, need_par );
		}
		void Node::_infixTypeStr( const Action& action, Stringifier& str, const std::vector<bool>& need_par ) const
		{
			str.addWord( action.type().asName() );
			str.addRaw( "'s" ); str.resetRaw();
			str.addWord( action.name() );
			_infixStr( action, str, need_par );
		}
		void Node::_infixStr( const Action& action, Stringifier& str, const std::vector<bool>& need_par ) const
		{
			bool prev_was_syntax = false;
			for( size_t arg_no = 0; arg_no < Children.size(); ++arg_no )
			{
				if( need_par[ arg_no ] )
				{
					str.addWord( "(" );
					str.markRaw();
				}

				auto& arg = action.arguments().at( arg_no );
				auto& val = Children[ arg_no ];
				if( val.isName() )
					str.addWord( val.name() );
				else
				{
					if( val.name() )
					{
						str.addWord( val.name() );
						str.addRaw( "=" );
					}

					if( arg.qualifier() == Qualifier::modify || arg.qualifier() == Qualifier::consume )
						str.addWord( mapQualifier( arg.qualifier() ) );

					val.infixStr( str );
				}

				if( need_par[ arg_no ] )
					str.addRaw( ")" );
			}
		}

		void Node::_infixOpStr( const Action& action, Stringifier& str, const std::vector<bool>& need_par ) const
		{
			auto& sequence = operators::sequence( action.opCode() );
			auto op = sequence.begin();
			auto end = sequence.end();
			switch( Children.size() )
			{
				case 1:
					for( ; op != end && op->Prefix; ++op )
						str.addWord( operators::mapCode( op->Op ) );
					Children[ 0 ].infixStr( str );
					for( ; op != end; ++op )
						str.addWord( operators::mapCode( op->Op ) );
					break;
				case 2:
					for( ; op != end && op->Prefix; ++op )
						str.addWord( operators::mapCode( op->Op ) );
					_infixOpArgStr( 0, str, need_par );
					str.addWord( operators::mapCode( op->Op ) );
					for( ++op; op != end && op->Prefix; ++op )
						str.addWord( operators::mapCode( op->Op ) );
					_infixOpArgStr( 1, str, need_par );
					for( ; op != end; ++op )
						str.addWord( operators::mapCode( op->Op ) );
					break;
				case 3:
					for( ; op != end && op->Prefix; ++op )
						str.addWord( operators::mapCode( op->Op ) );
					_infixOpArgStr( 0, str, need_par );
					str.addWord( operators::mapCode( op->Op ) );
					for( ++op; op != end && op->Prefix; ++op )
						str.addWord( operators::mapCode( op->Op ) );
					_infixOpArgStr( 1, str, need_par );
					str.addWord( operators::mapCode( op->Op ) );
					for( ++op; op != end && op->Prefix; ++op )
						str.addWord( operators::mapCode( op->Op ) );
					_infixOpArgStr( 2, str, need_par );
					for( ; op != end; ++op )
						str.addWord( operators::mapCode( op->Op ) );
					break;
				case 4:
					for( ; op != end && op->Prefix; ++op )
						str.addWord( operators::mapCode( op->Op ) );
					_infixOpArgStr( 0, str, need_par );
					str.addWord( operators::mapCode( op->Op ) );
					for( ++op; op != end && op->Prefix; ++op )
						str.addWord( operators::mapCode( op->Op ) );
					_infixOpArgStr( 1, str, need_par );
					str.addWord( operators::mapCode( op->Op ) );
					for( ++op; op != end && op->Prefix; ++op )
						str.addWord( operators::mapCode( op->Op ) );
					_infixOpArgStr( 2, str, need_par );
					str.addWord( operators::mapCode( op->Op ) );
					for( ++op; op != end && op->Prefix; ++op )
						str.addWord( operators::mapCode( op->Op ) );
					_infixOpArgStr( 3, str, need_par );
					for( ; op != end; ++op )
						str.addWord( operators::mapCode( op->Op ) );
					break;
			}
		}
		void Node::_infixOpArgStr( size_t arg_no, Stringifier& str, const std::vector<bool>& need_par ) const
		{
			if( need_par[ arg_no ] )
			{
				str.addWord( "(" );
				str.markRaw();
			}
			Children[ arg_no ].infixStr( str );
			if( need_par[ arg_no ] )
				str.addRaw( ")" );
		}
	}
}
