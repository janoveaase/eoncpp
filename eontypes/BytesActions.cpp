#include "BytesActions.h"
#include "Bool.h"
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
	void registerBytesActions( scope::Global& scope )
	{
		scope.add( name_construct, new actions::BytesConstruct() );
		scope.add( name_copyconstruct, new actions::BytesCopyConstruct( scope ) );
		scope.add( name_construct, new actions::BytesBoolConstruct( scope ) );
		scope.add( name_construct, new actions::BytesByteConstruct( scope ) );
		scope.add( name_construct, new actions::BytesCharConstruct( scope ) );
		scope.add( name_construct, new actions::BytesIntConstruct( scope ) );
		scope.add( name_construct, new actions::BytesShortConstruct( scope ) );
		scope.add( name_construct, new actions::BytesLongConstruct( scope ) );
		scope.add( name_construct, new actions::BytesFloatConstruct( scope ) );
		scope.add( name_construct, new actions::BytesLowConstruct( scope ) );
		scope.add( name_construct, new actions::BytesHighConstruct( scope ) );
		scope.add( name_construct, new actions::BytesIndexConstruct( scope ) );
		scope.add( name_cmp, new actions::BytesCmp( scope ) );
		scope.add( name_construct, new actions::BytesPlus( scope ) );
	}




	namespace actions
	{
		type::Object* BytesConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new BytesInstance(); }
		type::Object* BytesCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BytesInstance>( scope, action_node, 0 );
			return new BytesInstance( arg1->value() );
		}

		type::Object* BytesBoolConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr() );
		}
		type::Object* BytesByteConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr() );
		}
		type::Object* BytesCharConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr() );
		}
		type::Object* BytesIntConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IntInstance>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr() );
		}
		type::Object* BytesShortConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ShortInstance>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr() );
		}
		type::Object* BytesLongConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LongInstance>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr() );
		}
		type::Object* BytesFloatConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<FloatInstance>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr() );
		}
		type::Object* BytesLowConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<LowInstance>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr() );
		}
		type::Object* BytesHighConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<HighInstance>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr() );
		}
		type::Object* BytesIndexConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
			return new BytesInstance( string( arg1->value() ).stdstr() );
		}

		type::Object* BytesCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<BytesInstance>( scope, action_node, 0 );
			auto arg2 = _operand<BytesInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
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
