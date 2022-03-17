#include "FloatActions.h"
#include "Bool.h"
#include "Byte.h"
#include "Char.h"
#include "Integer.h"
#include "Low.h"
#include "High.h"
#include "Index.h"


namespace eon
{
	void registerFloatActions( scope::Global& scope )
	{
		scope.addAction( name_construct, new actions::FloatConstruct() );
		scope.addAction( name_construct, new actions::FloatCopyConstruct() );
		scope.addAction( name_construct, new actions::FloatBoolConstruct() );
		scope.addAction( name_construct, new actions::FloatByteConstruct() );
		scope.addAction( name_construct, new actions::FloatCharConstruct() );
		scope.addAction( name_construct, new actions::FloatIntConstruct() );
		scope.addAction( name_construct, new actions::FloatShortConstruct() );
		scope.addAction( name_construct, new actions::FloatLongConstruct() );
		scope.addAction( name_construct, new actions::FloatLowConstruct() );
		scope.addAction( name_construct, new actions::FloatHighConstruct() );
		scope.addAction( name_construct, new actions::FloatIndexConstruct() );
		scope.addAction( name_cmp, new actions::FloatCmp() );
		scope.addAction( name_op_plus, new actions::FloatPlus() );
		scope.addAction( name_op_minus, new actions::FloatMinus() );
		scope.addAction( name_op_multiply, new actions::FloatMultiply() );
		scope.addAction( name_op_divide, new actions::FloatDivide() );
	}




	namespace actions
	{
		type::Object* FloatConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			return new FloatInstance();
		}
		type::Object* FloatCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatInstance>( scope, action_node, 0 );
			return new FloatInstance( arg1->value() );
		}

		type::Object* FloatBoolConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			return new FloatInstance( arg1->value() );
		}
		type::Object* FloatByteConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			return new FloatInstance( arg1->value() );
		}
		type::Object* FloatCharConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
			return new FloatInstance( arg1->value() );
		}
		type::Object* FloatIntConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			return new FloatInstance( arg1->value() );
		}
		type::Object* FloatShortConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			return new FloatInstance( arg1->value() );
		}
		type::Object* FloatLongConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			return new FloatInstance( static_cast<flt_t>( arg1->value() ) );
		}
		type::Object* FloatLowConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LowInstance>( scope, action_node, 0 );
			return new FloatInstance( arg1->value() );
		}
		type::Object* FloatHighConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<HighInstance>( scope, action_node, 0 );
			return new FloatInstance( arg1->value() );
		}
		type::Object* FloatIndexConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			return new FloatInstance( static_cast<flt_t>( arg1->value() ) );
		}

		type::Object* FloatCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatInstance>( scope, action_node, 0 );
			auto arg2 = _operand<FloatInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}

		type::Object* FloatPlus::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatInstance>( scope, action_node, 0 );
			auto arg2 = _operand<FloatInstance>( scope, action_node, 1 );
			auto rval = new FloatInstance();
			rval->value( arg1->value() + arg2->value() );
			return rval;
		}
		type::Object* FloatMinus::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatInstance>( scope, action_node, 0 );
			auto arg2 = _operand<FloatInstance>( scope, action_node, 1 );
			auto rval = new FloatInstance();
			rval->value( arg1->value() - arg2->value() );
			return rval;
		}
		type::Object* FloatMultiply::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatInstance>( scope, action_node, 0 );
			auto arg2 = _operand<FloatInstance>( scope, action_node, 1 );
			auto rval = new FloatInstance();
			rval->value( arg1->value() * arg2->value() );
			return rval;
		}
		type::Object* FloatDivide::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg2 = _operand<FloatInstance>( scope, action_node, 1 );
			if( arg2->value() == 0.0 )
				throw DivisionByZero();
			auto arg1 = _operand<FloatInstance>( scope, action_node, 0 );
			auto rval = new FloatInstance();
			rval->value( arg1->value() / arg2->value() );
			return rval;
		}
	}
}
