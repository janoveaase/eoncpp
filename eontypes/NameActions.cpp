#include "NameActions.h"
#include "Int.h"


namespace eon
{
	void registerNameActions( scope::Global& scope )
	{
		scope.add( name_construct, new actions::NameConstruct() );
		scope.add( name_construct, new actions::NameCopyConstruct( scope ) );;
		scope.add( name_cmp, new actions::NameCmp( scope ) );
	}




	namespace actions
	{
		type::Object* NameConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new NameInstance(); }
		type::Object* NameCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<NameInstance>( scope, action_node, 0 );
			return new NameInstance( arg1->value() );
		}

		type::Object* NameCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<NameInstance>( scope, action_node, 0 );
			auto arg2 = _operand<NameInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}
	}
}
