#include "BasicTupleActions.h"
#include "Tuple.h"


namespace eon
{
	void registerBasicTupleActions( scope::Global& scope )
	{
		scope.add( name_cmp, new actions::BasicTupleCmp( scope ) );
		scope.add( name_plus, new actions::BasicTuplePlusDynamic( scope ) );
		scope.add( name_plus, new actions::BasicTuplePlusTuple( scope ) );
		scope.add( name_minus, new actions::BasicTupleMinusDynamic( scope ) );
		scope.add( name_minus, new actions::BasicTupleMinusTuple( scope ) );
		scope.add( name_element, new actions::BasicTupleElementIndex( scope ) );
		scope.add( name_element, new actions::BasicTupleElementInt( scope ) );
		scope.add( name_element, new actions::BasicTupleElementName( scope ) );
		scope.add( name_slice, new actions::BasicTupleSlice( scope ) );
	}




	namespace actions
	{
		type::Object* BasicTupleCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto& arg1 = *_operand<DynamicTuple>( scope, action_node, 0 );
			auto& arg2 = *_operand<DynamicTuple>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1.compare( arg2 ) );
			return rval;
		}

		type::Object* BasicTuplePlusDynamic::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto& arg1 = *_operand<DynamicTuple>( scope, action_node, 0 );
			auto& arg2 = *_operand<DynamicTuple>( scope, action_node, 1 );
			auto rval = new DynamicTuple( arg1 );
			*rval += arg2;
			return rval;
		}
		type::Object* BasicTuplePlusTuple::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto& arg1 = *_operand<DynamicTuple>( scope, action_node, 0 );
			auto& arg2 = *_operand<Tuple>( scope, action_node, 1 );
			auto rval = new DynamicTuple( arg1 );
			*rval += arg2;
			return rval;
		}
		type::Object* BasicTupleMinusDynamic::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto& arg1 = *_operand<DynamicTuple>( scope, action_node, 0 );
			auto& arg2 = *_operand<DynamicTuple>( scope, action_node, 1 );
			auto rval = new DynamicTuple( arg1 );
			*rval -= arg2;
			return rval;
		}
		type::Object* BasicTupleMinusTuple::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto& arg1 = *_operand<DynamicTuple>( scope, action_node, 0 );
			auto& arg2 = *_operand<Tuple>( scope, action_node, 1 );
			auto rval = new DynamicTuple( arg1 );
			*rval -= arg2;
			return rval;
		}

		type::Object* BasicTupleElementIndex::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto& arg1 = *_operand<DynamicTuple>( scope, action_node, 0 );
			auto arg2 = _operand<IndexInstance>( scope, action_node, 1 );
			return arg1.at( arg2->value() ).value();
		}
		type::Object* BasicTupleElementInt::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto& arg1 = *_operand<DynamicTuple>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto pos = arg2->value() > 0 ? static_cast<index_t>( arg2->value() )
				: arg1.numAttributes() - static_cast<index_t>( -arg2->value() );
			return arg1.at( pos ).value();
		}
		type::Object* BasicTupleElementName::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto& arg1 = *_operand<DynamicTuple>( scope, action_node, 0 );
			auto arg2 = _operand<NameInstance>( scope, action_node, 1 );
			return arg1.at( arg2->value() ).value();
		}

		type::Object* BasicTupleSlice::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto& arg1 = *_operand<DynamicTuple>( scope, action_node, 0 );
			auto arg2 = _operand<IntInstance>( scope, action_node, 1 );
			auto arg3 = _operand<IntInstance>( scope, action_node, 2 );
			auto first = arg2->value() > 0 ? static_cast<index_t>( arg2->value() )
				: arg1.numAttributes() - static_cast<index_t>( -arg2->value() );
			auto last = arg3->value() > 0 ? static_cast<index_t>( arg3->value() )
				: arg1.numAttributes() - static_cast<index_t>( -arg3->value() );
			auto rval = new DynamicTuple();
			if( first < last )
			{
				for( auto i = first; i <= last; ++i )
					*rval += arg1.at( i ).value();
			}
			else if( first > last )
			{
				for( auto i = last; i >= first; --i )
					*rval += arg1.at( i ).value();
			}
			return rval;
		}
	}
}
