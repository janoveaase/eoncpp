#include "HandleActions.h"
#include "Integer.h"


namespace eon
{
	void registerHandleActions( scope::Global& scope )
	{
		scope.addAction( name_constructor, new actions::HandleConstruct() );
		scope.addAction( name_constructor, new actions::HandleCopyConstruct() );
		scope.addOperator( type::operators::code::cmp, new actions::HandleCmp() );
	}




	namespace actions
	{
		type::Object* HandleConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new HandleInstance(); }

		type::Object* HandleCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<HandleInstance>( scope, action_node, 0 );
			return new HandleInstance( arg1->value(), arg1->source() );
		}

		type::Object* HandleCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<HandleInstance>( scope, action_node, 0 );
			auto arg2 = _operand<HandleInstance>( scope, action_node, 1 );
			auto rval = new IntegerInstance<int_t>();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}
	}
}
