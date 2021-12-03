#include "CharActions.h"
#include "Int.h"
#include "Byte.h"
#include <eonhex/Hex.h>


namespace eon
{
	void registerCharActions( scope::Global& scope )
	{
		scope.add( name_construct, new actions::CharConstruct() );
		scope.add( name_construct, new actions::CharCopyConstruct( scope ) );
		scope.add( name_construct, new actions::CharByteConstruct( scope ) );
		scope.add( name_cmp, new actions::CharCmp( scope ) );
	}




	namespace actions
	{
		type::Object* CharConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new CharInstance(); }
		type::Object* CharCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
			return new CharInstance( arg1->value() );
		}
		type::Object* CharByteConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			return new CharInstance( arg1->value() );
		}

		type::Object* CharCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
			auto arg2 = _operand<CharInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}
	}
}
