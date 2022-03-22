#include "BoolActions.h"
#include "Byte.h"
#include "Char.h"
#include "Integer.h"
#include "Floatingpt.h"
#include "Index.h"


namespace eon
{
	void registerBoolActions( scope::Global& scope )
	{
		scope.addAction( name_constructor, new actions::BoolConstruct() );
		scope.addAction( name_constructor, new actions::BoolCopyConstruct() );
		scope.addAction( name_constructor, new actions::BoolByteConstruct() );
		scope.addAction( name_constructor, new actions::BoolCharConstruct() );
		scope.addAction( name_constructor, new actions::BoolIntConstruct() );
		scope.addAction( name_constructor, new actions::BoolShortConstruct() );
		scope.addAction( name_constructor, new actions::BoolLongConstruct() );
		scope.addAction( name_constructor, new actions::BoolFloatConstruct() );
		scope.addAction( name_constructor, new actions::BoolLowConstruct() );
		scope.addAction( name_constructor, new actions::BoolHighConstruct() );
		scope.addAction( name_constructor, new actions::BoolIndexConstruct() );
		scope.addOperator( type::operators::code::cmp, new actions::BoolCmp() );
		scope.addOperator( type::operators::code::log_not, new actions::BoolNot() );
		scope.addOperator( type::operators::code::log_and, new actions::BoolAnd() );
		scope.addOperator( type::operators::code::log_or, new actions::BoolOr() );
		scope.addOperator( type::operators::code::log_xor, new actions::BoolXor() );
	}




	namespace actions
	{
		type::Object* BoolConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new BoolInstance(); }
		type::Object* BoolCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			return new BoolInstance( arg1->value(), arg1->source() );
		}
		type::Object* BoolByteConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			return new BoolInstance( arg1->value() != 0, arg1->source() );
		}
		type::Object* BoolCharConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
			return new BoolInstance( arg1->value() != 0, arg1->source() );
		}
		type::Object* BoolIntConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntegerInstance<int_t>>( scope, action_node, 0 );
			return new BoolInstance( arg1->value() != 0, arg1->source() );
		}
		type::Object* BoolShortConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntegerInstance<short_t>>( scope, action_node, 0 );
			return new BoolInstance( arg1->value() != 0, arg1->source() );
		}
		type::Object* BoolLongConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntegerInstance<long_t>>( scope, action_node, 0 );
			return new BoolInstance( arg1->value() != 0, arg1->source() );
		}
		type::Object* BoolFloatConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatingptInstance<flt_t>>( scope, action_node, 0 );
			return new BoolInstance( arg1->value() != 0, arg1->source() );
		}
		type::Object* BoolLowConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatingptInstance<low_t>>( scope, action_node, 0 );
			return new BoolInstance( arg1->value() != 0, arg1->source() );
		}
		type::Object* BoolHighConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatingptInstance<high_t>>( scope, action_node, 0 );
			return new BoolInstance( arg1->value() != 0, arg1->source() );
		}
		type::Object* BoolIndexConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			return new BoolInstance( arg1->value() != 0, arg1->source() );
		}

		type::Object* BoolCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			auto arg2 = _operand<BoolInstance>( scope, action_node, 1 );
			auto rval = new IntegerInstance<int_t>();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}

		type::Object* BoolNot::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			return new BoolInstance( !arg1->value(), arg1->source() );
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
			return new BoolInstance( result, source::Ref() );
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
			return new BoolInstance( result, source::Ref() );
		}
		type::Object* BoolXor::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			auto arg2 = _operand<BoolInstance>( scope, action_node, 2 );
			return new BoolInstance( arg1->value() != arg2->value(), source::Ref() );
		}
	}
}
