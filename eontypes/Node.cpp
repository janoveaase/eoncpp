#include "Node.h"
#include "Action.h"


namespace eon
{
	namespace type
	{
		Node& Node::operator=( const Node& other )
		{
			Source = other.Source;
			Type = other.Type;
			Name = other.Name;
			Define = other.Define;
			Item = other.Item ? other.Item->copy() : nullptr;
			Children = other.Children;
			return *this;
		}
		Node& Node::operator=( Node&& other ) noexcept
		{
			Source = other.Source;
			Type = other.Type; other.Type = NodeType::undef;
			Name = other.Name; other.Name = no_name;
			Define = other.Define; other.Define = false;
			Item = other.Item; other.Item = nullptr;
			Children = std::move( other.Children );
			return *this;
		}

		Node& Node::claim( Node&& other ) noexcept
		{
			Source = other.Source;
			Type = other.Type; other.Type = NodeType::undef;
			Name = other.Name; other.Name = no_name;
			other.Define = false;
			Item = other.Item; other.Item = nullptr;
			Children = std::move( other.Children );
			return *this;
		}
		Node& Node::copy( const Node& other )
		{
			Type = other.Type;
			Item = other.Item ? other.Item->copy() : nullptr;
			Name = other.Name;
			Children.clear();
			for( auto& child : other.Children )
			{
				if( child.isAction() )
					Children.push_back( Node::newAction( (Action*)child.Item->copy() ) );
				else if( child.isName() )
					Children.push_back( Node::newName( child.Name, child.source() ) );
				else
					Children.push_back( Node::newValue( child.Item->copy() ) );
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
			else if( isOperator() )
			{
				std::vector<bool> need_par = _operandsNeedingParenthesis( opr() );
				_infixOpStr( opr(), str, need_par );
			}
			else
			{
/*				switch( action().actionType() )
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
				}*/
			}
		}




		int Node::compare( const Node& other ) const noexcept
		{
			if( Type < other.Type )
				return -1;
			else if( other.Type > Type )
				return 1;
			if( Name < other.Name )
				return -1;
			else if( other.Name < Name )
				return 1;
			if( Item )
			{
				if( other.Item )
				{
					if( Item->generalType() == name_instance )
					{
						auto cmp = ( (Instance*)Item )->compare( *(Instance*)other.Item );
						if( cmp != 0 )
							return cmp;
					}
				}
				else
					return 1;
			}
			else if( other.Item )
				return -1;

			auto my_child = Children.begin();
			auto other_child = other.Children.begin();
			for( ; my_child != Children.end() && other_child != other.Children.end(); ++my_child, ++other_child )
			{
				auto cmp = my_child->compare( *other_child );
				if( cmp != 0 )
					return cmp;
			}
			if( my_child != Children.end() )
				return 1;
			else if( other_child != other.Children.end() )
				return -1;
			else
				return 0;
		}




		std::vector<bool> Node::_operandsNeedingParenthesis( const actions::OperatorAction& opr ) const
		{
			std::vector<bool> need;
			for( auto& child : Children )
			{
				if( child.isOperator() )
				{
					// If the child is an operator of lower precedence that the
					// parent action, we must enclose!
					if( child.opr().inputPrecedence() < opr.inputPrecedence() )
						need.push_back( true );

					// Same if right-to-left associativity and the first
					// operand is an action.
					else if( opr.rightToLeft() && !child.Children.empty() && child.Children[ 0 ].isOperator() )
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

/*		void Node::_postfixStr( type::Stringifier& str ) const
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
				for( auto& child : Children )
					child.postfixStr( str );
				Item->str( str );
			}
		}*/

		void Node::_infixInstanceStr( const actions::OperatorAction& action, Stringifier& str, const std::vector<bool>& need_par ) const
		{
			str.addWord( action.type().asName() );
			_infixStr( action, str, need_par );
		}
		void Node::_infixPreFirstStr( const actions::OperatorAction& action, Stringifier& str, const std::vector<bool>& need_par ) const
		{
			str.addWord( action.type().asName() );
			_infixStr( action, str, need_par );
		}
		void Node::_infixPreLastStr( const actions::OperatorAction& action, Stringifier& str, const std::vector<bool>& need_par ) const
		{
			str.addWord( action.type().asName() );
			_infixStr( action, str, need_par );
		}
		void Node::_infixGetStr( const actions::OperatorAction& action, Stringifier& str, const std::vector<bool>& need_par ) const
		{
			_infixStr( action, str, need_par );
		}
		void Node::_infixSetStr( const actions::OperatorAction& action, Stringifier& str, const std::vector<bool>& need_par ) const
		{
			_infixStr( action, str, need_par );
		}
		void Node::_infixHelperStr( const actions::OperatorAction& action, Stringifier& str, const std::vector<bool>& need_par ) const
		{
			_infixStr( action, str, need_par );
		}
		void Node::_infixTypeStr( const actions::OperatorAction& action, Stringifier& str, const std::vector<bool>& need_par ) const
		{
			str.addWord( action.type().asName() );
			str.addRaw( "'s" ); str.resetRaw();
			str.addWord( action.name() );
			_infixStr( action, str, need_par );
		}
		void Node::_infixStr( const actions::OperatorAction& action, Stringifier& str, const std::vector<bool>& need_par ) const
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

				if( arg.value() )
				{
					if( arg.value()->generalType() == name_modify )
						str.addWord( "modify" );
					else if( arg.value()->generalType() != name_reference )
						str.addWord( "consume" );
				}

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

					val.infixStr( str );
				}

				if( need_par[ arg_no ] )
					str.addRaw( ")" );
			}
		}

		void Node::_infixOpStr( const actions::OperatorAction& action, Stringifier& str, const std::vector<bool>& need_par ) const
		{
			auto& sequence = operators::sequence( action.opCode() );
			auto op = sequence.begin();
			auto end = sequence.end();
			switch( Children.size() )
			{
				case 1:
					for( ; op != end && op->Prefix; ++op )
						str.addWord( operators::mapCode( op->Op ) );
					if( Children[ 0 ].Define )
						str.addWord( "define" );
					Children[ 0 ].infixStr( str );
					for( ; op != end; ++op )
						str.addWord( operators::mapCode( op->Op ) );
					break;
				case 2:
					for( ; op != end && op->Prefix; ++op )
						str.addWord( operators::mapCode( op->Op ) );
					if( Children[ 0 ].Define )
						str.addWord( "define" );
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
					if( Children[ 0 ].Define )
						str.addWord( "define" );
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
					if( Children[ 0 ].Define )
						str.addWord( "define" );
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
