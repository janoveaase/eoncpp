#include "HighActions.h"
#include "Bool.h"
#include "Byte.h"
#include "Char.h"
#include "Int.h"
#include "Short.h"
#include "Long.h"
#include "Float.h"
#include "Low.h"
#include "Index.h"


namespace eon
{
	void registerHighActions( scope::Global& scope )
	{
		scope.add( name_construct, new actions::HighConstruct() );
		scope.add( name_construct, new actions::HighCopyConstruct( scope ) );
		scope.add( name_construct, new actions::HighBoolConstruct( scope ) );
		scope.add( name_construct, new actions::HighByteConstruct( scope ) );
		scope.add( name_construct, new actions::HighCharConstruct( scope ) );
		scope.add( name_construct, new actions::HighIntConstruct( scope ) );
		scope.add( name_construct, new actions::HighShortConstruct( scope ) );
		scope.add( name_construct, new actions::HighLongConstruct( scope ) );
		scope.add( name_construct, new actions::HighLowConstruct( scope ) );
		scope.add( name_construct, new actions::HighFloatConstruct( scope ) );
		scope.add( name_construct, new actions::HighIndexConstruct( scope ) );
		scope.add( name_cmp, new actions::HighCmp( scope ) );
		scope.add( name_plus, new actions::HighPlus( scope ) );
		scope.add( name_minus, new actions::HighMinus( scope ) );
		scope.add( name_multiply, new actions::HighMultiply( scope ) );
		scope.add( name_divide, new actions::HighDivide( scope ) );
	}




	namespace actions
	{
		type::Object* HighConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new HighInstance(); }
		type::Object* HighCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<HighInstance>( scope, action_node, 0 );
			return new HighInstance( arg1->value() );
		}

		type::Object* HighBoolConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			return new HighInstance( arg1->value() );
		}
		type::Object* HighByteConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			return new HighInstance( arg1->value() );
		}
		type::Object* HighCharConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
			return new HighInstance( arg1->value() );
		}
		type::Object* HighIntConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			return new HighInstance( arg1->value() );
		}
		type::Object* HighShortConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			return new HighInstance( arg1->value() );
		}
		type::Object* HighLongConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			return new HighInstance( static_cast<high_t>( arg1->value() ) );
		}
		type::Object* HighLowConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LowInstance>( scope, action_node, 0 );
			return new HighInstance( arg1->value() );
		}
		type::Object* HighFloatConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatInstance>( scope, action_node, 0 );
			return new HighInstance( arg1->value() );
		}
		type::Object* HighIndexConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			return new HighInstance( static_cast<high_t>( arg1->value() ) );
		}

		type::Object* HighCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<HighInstance>( scope, action_node, 0 );
			auto arg2 = _operand<HighInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}

		type::Object* HighPlus::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<HighInstance>( scope, action_node, 0 );
			auto arg2 = _operand<HighInstance>( scope, action_node, 1 );
			auto rval = new HighInstance();
			rval->value( arg1->value() + arg2->value() );
			return rval;
		}
		type::Object* HighMinus::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<HighInstance>( scope, action_node, 0 );
			auto arg2 = _operand<HighInstance>( scope, action_node, 1 );
			auto rval = new HighInstance();
			rval->value( arg1->value() - arg2->value() );
			return rval;
		}
		type::Object* HighMultiply::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<HighInstance>( scope, action_node, 0 );
			auto arg2 = _operand<HighInstance>( scope, action_node, 1 );
			auto rval = new HighInstance();
			rval->value( arg1->value() * arg2->value() );
			return rval;
		}
		type::Object* HighDivide::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg2 = _operand<HighInstance>( scope, action_node, 1 );
			if( arg2->value() == 0.0 )
				throw DivisionByZero();
			auto arg1 = _operand<HighInstance>( scope, action_node, 0 );
			auto rval = new HighInstance();
			rval->value( arg1->value() / arg2->value() );
			return rval;
		}
	}
}
