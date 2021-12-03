#include "BoolActions.h"
#include "Byte.h"
#include "Char.h"
#include "Int.h"
#include "Short.h"
#include "Long.h"
#include "Float.h"
#include "Low.h"
#include "High.h"
#include "Index.h"


namespace eon
{
	void registerBoolActions( scope::Global& scope )
	{
		scope.add( name_construct, new actions::BoolConstruct() );
		scope.add( name_copyconstruct, new actions::BoolCopyConstruct( scope ) );
		scope.add( name_copyconstruct, new actions::BoolByteConstruct( scope ) );
		scope.add( name_copyconstruct, new actions::BoolCharConstruct( scope ) );
		scope.add( name_copyconstruct, new actions::BoolIntConstruct( scope ) );
		scope.add( name_copyconstruct, new actions::BoolShortConstruct( scope ) );
		scope.add( name_copyconstruct, new actions::BoolLongConstruct( scope ) );
		scope.add( name_copyconstruct, new actions::BoolFloatConstruct( scope ) );
		scope.add( name_copyconstruct, new actions::BoolLowConstruct( scope ) );
		scope.add( name_copyconstruct, new actions::BoolHighConstruct( scope ) );
		scope.add( name_copyconstruct, new actions::BoolIndexConstruct( scope ) );
		scope.add( name_cmp, new actions::BoolCmp( scope ) );
		scope.add( name_not, new actions::BoolNot() );
		scope.add( name_and, new actions::BoolAnd( scope ) );
		scope.add( name_or, new actions::BoolOr( scope ) );
		scope.add( name_xor, new actions::BoolXor( scope ) );
	}




	namespace actions
	{
		type::Object* BoolConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new BoolInstance(); }
		type::Object* BoolCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			return new BoolInstance( arg1->value() );
		}
		type::Object* BoolByteConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			return new BoolInstance( arg1->value() != 0 );
		}
		type::Object* BoolCharConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
			return new BoolInstance( arg1->value() != 0 );
		}
		type::Object* BoolIntConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			return new BoolInstance( arg1->value() != 0 );
		}
		type::Object* BoolShortConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			return new BoolInstance( arg1->value() != 0 );
		}
		type::Object* BoolLongConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			return new BoolInstance( arg1->value() != 0 );
		}
		type::Object* BoolFloatConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatInstance>( scope, action_node, 0 );
			return new BoolInstance( arg1->value() != 0 );
		}
		type::Object* BoolLowConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LowInstance>( scope, action_node, 0 );
			return new BoolInstance( arg1->value() != 0 );
		}
		type::Object* BoolHighConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<HighInstance>( scope, action_node, 0 );
			return new BoolInstance( arg1->value() != 0 );
		}
		type::Object* BoolIndexConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			return new BoolInstance( arg1->value() != 0 );
		}

		type::Object* BoolCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			auto arg2 = _operand<BoolInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}

		type::Object* BoolNot::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			return new BoolInstance( !arg1->value() );
		}
		type::Object* BoolAnd::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			bool result = arg1->value();
			if( result )
			{
				auto arg2 = _operand<BoolInstance>( scope, action_node, 1 );
				result = arg2->value();
			}
			return new BoolInstance( result );
		}
		type::Object* BoolOr::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			bool result = arg1->value();
			if( !result )
			{
				auto arg2 = _operand<BoolInstance>( scope, action_node, 1 );
				result = arg2->value();
			}
			return new BoolInstance( result );
		}
		type::Object* BoolXor::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			auto arg2 = _operand<BoolInstance>( scope, action_node, 2 );
			return new BoolInstance( arg1->value() != arg2->value() );
		}
	}
}
