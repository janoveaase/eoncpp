#include "LowActions.h"
#include "Bool.h"
#include "Byte.h"
#include "Char.h"
#include "Integer.h"
#include "Float.h"
#include "High.h"
#include "Index.h"


namespace eon
{
	void registerLowActions( scope::Global& scope )
	{
		scope.add( name_construct, new actions::LowConstruct() );
		scope.add( name_construct, new actions::LowCopyConstruct() );
		scope.add( name_construct, new actions::LowBoolConstruct() );
		scope.add( name_construct, new actions::LowByteConstruct() );
		scope.add( name_construct, new actions::LowCharConstruct() );
		scope.add( name_construct, new actions::LowIntConstruct() );
		scope.add( name_construct, new actions::LowShortConstruct() );
		scope.add( name_construct, new actions::LowLongConstruct() );
		scope.add( name_construct, new actions::LowFloatConstruct() );
		scope.add( name_construct, new actions::LowHighConstruct() );
		scope.add( name_construct, new actions::LowIndexConstruct() );
		scope.add( name_cmp, new actions::LowCmp() );
		scope.add( name_plus, new actions::LowPlus() );
		scope.add( name_minus, new actions::LowMinus() );
		scope.add( name_multiply, new actions::LowMultiply() );
		scope.add( name_divide, new actions::LowDivide() );
	}




	namespace actions
	{
		type::Object* LowConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new LowInstance(); }
		type::Object* LowCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LowInstance>( scope, action_node, 0 );
			return new LowInstance( arg1->value() );
		}

		type::Object* LowBoolConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			return new LowInstance( arg1->value() );
		}
		type::Object* LowByteConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			return new LowInstance( arg1->value() );
		}
		type::Object* LowCharConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
			return new LowInstance( static_cast<low_t>( arg1->value() ) );
		}
		type::Object* LowIntConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			return new LowInstance( static_cast<low_t>( arg1->value() ) );
		}
		type::Object* LowShortConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			return new LowInstance( arg1->value() );
		}
		type::Object* LowLongConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			return new LowInstance( static_cast<low_t>( arg1->value() ) );
		}
		type::Object* LowFloatConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatInstance>( scope, action_node, 0 );
			return new LowInstance( static_cast<low_t>( arg1->value() ) );
		}
		type::Object* LowHighConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<HighInstance>( scope, action_node, 0 );
			return new LowInstance( static_cast<low_t>( arg1->value() ) );
		}
		type::Object* LowIndexConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			return new LowInstance( static_cast<low_t>( arg1->value() ) );
		}

		type::Object* LowCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LowInstance>( scope, action_node, 0 );
			auto arg2 = _operand<LowInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}

		type::Object* LowPlus::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LowInstance>( scope, action_node, 0 );
			auto arg2 = _operand<LowInstance>( scope, action_node, 1 );
			auto rval = new LowInstance();
			rval->value( arg1->value() + arg2->value() );
			return rval;
		}
		type::Object* LowMinus::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LowInstance>( scope, action_node, 0 );
			auto arg2 = _operand<LowInstance>( scope, action_node, 1 );
			auto rval = new LowInstance();
			rval->value( arg1->value() - arg2->value() );
			return rval;
		}
		type::Object* LowMultiply::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LowInstance>( scope, action_node, 0 );
			auto arg2 = _operand<LowInstance>( scope, action_node, 1 );
			auto rval = new LowInstance();
			rval->value( arg1->value() * arg2->value() );
			return rval;
		}
		type::Object* LowDivide::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg2 = _operand<LowInstance>( scope, action_node, 1 );
			if( arg2->value() == 0.0 )
				throw DivisionByZero();
			auto arg1 = _operand<LowInstance>( scope, action_node, 0 );
			auto rval = new LowInstance();
			rval->value( arg1->value() / arg2->value() );
			return rval;
		}
	}
}
