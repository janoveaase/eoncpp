#include "CharActions.h"
#include "Integer.h"
#include "Byte.h"
#include <eonhex/Hex.h>


namespace eon
{
	void registerCharActions( scope::Global& scope )
	{
		scope.addAction( name_constructor, new actions::CharConstruct() );
		scope.addAction( name_constructor, new actions::CharCopyConstruct() );
		scope.addAction( name_constructor, new actions::CharByteConstruct() );
		scope.addOperator( type::operators::code::cmp, new actions::CharCmp() );
	}




	namespace actions
	{
		type::Object* CharConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new CharInstance(); }
		type::Object* CharCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
			return new CharInstance( arg1->value(), arg1->source() );
		}
		type::Object* CharByteConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			return new CharInstance( arg1->value(), arg1->source() );
		}

		type::Object* CharCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
			auto arg2 = _operand<CharInstance>( scope, action_node, 1 );
			auto rval = new IntegerInstance<int_t>();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}
	}
}
