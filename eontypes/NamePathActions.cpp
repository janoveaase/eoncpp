#include "NamePathActions.h"
#include "Int.h"
#include "String.h"
#include "Index.h"


namespace eon
{
	void registerNamePathActions( scope::Global& scope )
	{
		scope.add( name_construct, new actions::NamePathConstruct() );
		scope.add( name_copyconstruct, new actions::NamePathCopyConstruct( scope ) );
		scope.add( name_cmp, new actions::NamePathCmp( scope ) );
	}




	namespace actions
	{
		type::Object* NamePathConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new NamePathInstance(); }
		type::Object* NamePathCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<NamePathInstance>( scope, action_node, 0 );
			return new NamePathInstance( arg1->value() );
		}

		type::Object* NamePathCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<NamePathInstance>( scope, action_node, 0 );
			auto arg2 = _operand<NamePathInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}
	}
}
