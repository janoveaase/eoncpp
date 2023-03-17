#include "ExpressionNode.h"
#include <eontypesystem/TupleFactory.h>
#include <eontypesystem/TypeTupleFactory.h>
#include <eontypesystem/Operators.h>

namespace eon
{
	namespace expression
	{
		Node Node::newValue( Attribute&& value, name_t name )
		{
			Node node;
			node.Name = name;
			node.Source = value.source();
			node.Type = value.type();
			node.Value = std::move( value );
			return node;
		}

		Node Node::newOperator( name_t name, source::Ref source )
		{
			Node node;
			node.Name = name;
			node.Source = source;
			return node;
		}

		Node Node::newCall( const type::Action& action, source::Ref source )
		{
			Node node;
			node.Name = name_call;
			node.Type = action.signature().at( name_return );
			node.Source = source;
			node.Executor = &action;
			return node;
		}




		Attribute Node::evaluate( Tuple& scope )
		{
			if( isValue() )
				return value();

			type::ActionExeArgs args( scope );
			_evaluateChildren( scope, args );	// Operands!

			(*Executor)( args );
			return _executionResult( args );
		}

		void Node::str( Stringifier& strf ) const
		{
			if( isOperator() )
				_writeOperator( strf );
			else if( isCall() )
				_writeCall( strf );
			else if( isValue() )
				Value.str( strf );
		}




		void Node::_evaluateChildren( Tuple& scope, type::ActionExeArgs& args )
		{
			for( auto& child : Children )
				args.values().push( child.evaluate( scope ) );
		}

		Attribute Node::_executionResult( type::ActionExeArgs& args )
		{
			if( args.values().size() == 1 )
			{
				Attribute value = std::move( args.values().top() );
				return value;
			}
			else if( args.values().empty() )
				return Attribute();
			else
			{
				// TODO: Is this even possible?
				// TODO: If so, should we return a tuple?
				Attribute value = std::move( args.values().top() );
				return value;
			}
		}

		bool Node::_bindOperator( Tuple& scope, source::Reporter& reporter )
		{
			auto req_operands = type::Operators::numOperands( Name );
			if( Children.size() != req_operands )
				return _wrongNumberOfOperands( reporter );

			if( !_childrenHaveOperatorsAndReturnTypes( scope, reporter ) )
				return false;

			if( type::Operators::isTypeImplemented( Name ) )
				return _findTypeImplementedOperator( scope, reporter );
			else
			{
				// TODO: Deal with system-implemented operators ...
				return false;
			}
		}

		bool Node::_wrongNumberOfOperands( source::Reporter& reporter )
		{
			reporter.error( "Wrong number of operands! Expected " + string( type::Operators::numOperands( Name ) )
				+ ", got " + string( Children.size() ), Source );
			return false;
		}

		bool Node::_childrenHaveOperatorsAndReturnTypes( Tuple& scope, source::Reporter& reporter )
		{
			for( auto& child : Children )
			{
				if( child.isOperator() )
				{
					if( !child.bindOperator( scope, reporter ) )
						return false;
				}
			}
			return true;
		}

		bool Node::_findTypeImplementedOperator( Tuple& scope, source::Reporter& reporter )
		{
			TypeTuple instance_type = Children.begin()->type();
			TypeTuple operator_args = _getOperatorArgs();

			auto actions = scope.signatures( Name, instance_type.nameValue(), operator_args );
			if( actions.size() == 1 )
				_resolveTrivialMatch( scope, *actions.begin() );
			else if( actions.size() > 0 )
				_resolveOnReturnType( scope, actions );
			else if( _isBinaryComparisonOperator() )
				_resolveBinaryComparisonOperator( scope, instance_type, operator_args );
			if( Executor == nullptr )
				reporter.error(
					"Operator '" + eon::str( Name ) + "' is not implemented for " + instance_type.str() + "!", Source );
			return Executor != nullptr;
		}

		bool Node::_isBinaryComparisonOperator()
		{
			static std::set<name_t> cmp_ops{ symbol_lt, symbol_le, symbol_gt, symbol_ge, symbol_eq, symbol_ne };
			return cmp_ops.find( Name ) != cmp_ops.end();
		}

		TypeTuple Node::_getOperatorArgs()
		{
			auto child = Children.begin();
			TypeTuple args;
			for( ++child; child != Children.end(); ++child )
			{
				if( child->Type.isName() )
					args.add( child->Type.nameValue() );
				else
					args.add( child->Type );
			}
			return args;
		}

		void Node::_resolveTrivialMatch( Tuple& scope, TypeTuple action )
		{
			Executor = scope.action( action );
			Type = Executor->signature().at( name_return );
		}

		void Node::_resolveOnReturnType( Tuple& scope, std::set<TypeTuple>& possible_actions )
		{
			// TODO: Check return type!
		}

		void Node::_resolveBinaryComparisonOperator( Tuple& scope, TypeTuple instance_type, TypeTuple operator_args )
		{
			auto actions = scope.signatures( symbol_cmp, instance_type.nameValue(), operator_args );
			if( actions.size() == 1 )
				_handleAliasComparisonOperator( scope, *actions.begin() );
		}

		void Node::_handleAliasComparisonOperator( Tuple& scope, TypeTuple action )
		{
			// We need to change the nodes from:
			//
			//          .-- <child 1>
			//   <op> --|
			//          '-- <child 2>
			//
			// to:
			//                      .-- <child 1>
			//          .-- '<=>' --|
			//   <op> --|           '-- <child 2>
			//          '-- 0

			_insertOperatorCmp( scope, action );

			// Now we can use the original comparison operator towards int
			_insertOriginalOperatorForInt( scope );
		}

		void Node::_insertOperatorCmp( Tuple& scope, TypeTuple action )
		{
			auto operator_cmp = Node::newOperator( symbol_cmp, source::Ref() );
			operator_cmp.Executor = scope.action( action );
			Type = name_int;
			operator_cmp.Children = std::move( Children );
			Children.push_back( std::move( operator_cmp ) );
			Children.push_back( Node::newValue( Attribute::newImplicit( int_t( 0 ), type::Qualifier::_rvalue ) ) );
		}

		void Node::_insertOriginalOperatorForInt( Tuple& scope )
		{
			auto actions = scope.signatures( Name, name_int, typetuple::convert( { { no_name, name_int } } ) );
			Executor = scope.action( *actions.begin() );
			Type = name_bool;
		}


		void Node::_writeOperator( Stringifier& strf ) const
		{
			auto& sequence = type::Operators::sequence( Name );
			auto child = Children.begin();
			for( auto& elm : sequence )
			{
				if( elm.prefix() )
					strf.op1( eon::str( elm.Op ) );
				else
				{
					child->str( strf );
					strf.op1( eon::str( elm.Op ) );
					++child;
				}
			}
			for( ; child != Children.end(); ++child )
				child->str( strf );
		}

		void Node::_writeCall( Stringifier& strf ) const
		{
			auto category = Executor->signature().at( name_category ).nameValue();
			if( category == name_constructor )
				_writeConstructorCall( strf );
			// TODO: Add support for more!
		}

		void Node::_writeConstructorCall( Stringifier& strf ) const
		{
			strf.word( eon::str( Executor->signature().at( name_type ).nameValue() ) );
			strf.start_grp2( "(" );
			bool first = true;
			for( auto& arg : Children )
			{
				if( first )
					first = false;
				else
					strf.punct( "," );
				arg.str( strf );
			}
			strf.end_grp2( ")" );
		}
	}
}
