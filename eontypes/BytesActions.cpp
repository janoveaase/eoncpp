#include "BytesActions.h"
#include "Bool.h"
#include "Byte.h"
#include "Char.h"
#include "Integer.h"
#include "Floatingpt.h"
#include "Index.h"


namespace eon
{
	void registerBytesActions( scope::Global& scope )
	{
		scope.addAction( name_constructor, new actions::BytesConstruct() );
		scope.addAction( name_constructor, new actions::BytesCopyConstruct() );
		scope.addAction( name_constructor, new actions::BytesBoolConstruct() );
		scope.addAction( name_constructor, new actions::BytesByteConstruct() );
		scope.addAction( name_constructor, new actions::BytesCharConstruct() );
		scope.addAction( name_constructor, new actions::BytesIntConstruct() );
		scope.addAction( name_constructor, new actions::BytesShortConstruct() );
		scope.addAction( name_constructor, new actions::BytesLongConstruct() );
		scope.addAction( name_constructor, new actions::BytesFloatConstruct() );
		scope.addAction( name_constructor, new actions::BytesLowConstruct() );
		scope.addAction( name_constructor, new actions::BytesHighConstruct() );
		scope.addAction( name_constructor, new actions::BytesIndexConstruct() );
		scope.addOperator( type::operators::code::cmp, new actions::BytesCmp() );
		scope.addOperator( type::operators::code::plus, new actions::BytesPlus() );
	}




	namespace actions
	{
		type::Object* BytesConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new BytesInstance(); }
		type::Object* BytesCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BytesInstance>( scope, action_node, 0 );
			return new BytesInstance( arg1->value(), arg1->source() );
		}

		type::Object* BytesBoolConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr(), arg1->source() );
		}
		type::Object* BytesByteConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr(), arg1->source() );
		}
		type::Object* BytesCharConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr(), arg1->source() );
		}
		type::Object* BytesIntConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntegerInstance<int_t>>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr(), arg1->source() );
		}
		type::Object* BytesShortConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntegerInstance<short_t>>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr(), arg1->source() );
		}
		type::Object* BytesLongConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntegerInstance<long_t>>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr(), arg1->source() );
		}
		type::Object* BytesFloatConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatingptInstance<flt_t>>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr(), arg1->source() );
		}
		type::Object* BytesLowConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatingptInstance<low_t>>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr(), arg1->source() );
		}
		type::Object* BytesHighConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatingptInstance<high_t>>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr(), arg1->source() );
		}
		type::Object* BytesIndexConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr(), arg1->source() );
		}

		type::Object* BytesCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BytesInstance>( scope, action_node, 0 );
			auto arg2 = _operand<BytesInstance>( scope, action_node, 1 );
			auto rval = new IntegerInstance<int_t>();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}

		type::Object* BytesPlus::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BytesInstance>( scope, action_node, 0 );
			auto arg2 = _operand<BytesInstance>( scope, action_node, 1 );
			auto rval = new BytesInstance();
			rval->value( arg1->value() + arg2->value() );
			return rval;
		}
	}
}
