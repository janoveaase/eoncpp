#include "NameActions.h"
#include "Integer.h"


namespace eon
{
	void registerNameActions( scope::Global& scope )
	{
		scope.addAction( name_constructor, new actions::NameConstruct() );
		scope.addAction( name_constructor, new actions::NameCopyConstruct() );
		scope.addOperator( type::operators::code::cmp, new actions::NameCmp() );
	}




	namespace actions
	{
		type::Object* NameConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new NameInstance(); }
		type::Object* NameCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<NameInstance>( scope, action_node, 0 );
			return new NameInstance( arg1->value(), arg1->source() );
		}

		type::Object* NameCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<NameInstance>( scope, action_node, 0 );
			auto arg2 = _operand<NameInstance>( scope, action_node, 1 );
			auto rval = new IntegerInstance<int_t>();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}
	}
}
