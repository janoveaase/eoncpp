#include "HandleActions.h"
#include "Int.h"


namespace eon
{
	void registerHandleActions( scope::Global& scope )
	{
		scope.add( name_construct, new actions::HandleConstruct() );
		scope.add( name_copyconstruct, new actions::HandleCopyConstruct( scope ) );
		scope.add( name_cmp, new actions::HandleCmp( scope ) );
	}




	namespace actions
	{
		type::Object* HandleConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new HandleInstance(); }

		type::Object* HandleCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<HandleInstance>( scope, action_node, 0 );
			return new HandleInstance( arg1->value() );
		}

		type::Object* HandleCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<HandleInstance>( scope, action_node, 0 );
			auto arg2 = _operand<HandleInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}
	}
}
