#include "ExpressionActions.h"
#include "Integer.h"
#include "String.h"
#include "Index.h"


namespace eon
{
	void registerExpressionActions( scope::Global& scope )
	{
		scope.addAction( name_constructor, new actions::ExprConstruct() );
		scope.addAction( name_constructor, new actions::ExprCopyConstruct() );
		scope.addOperator( type::operators::code::cmp, new actions::ExprCmp() );
		scope.addOperator( type::operators::code::assign, new actions::ExprAssign() );
	}




	namespace actions
	{
		type::Object* ExprConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new ExprInstance(); }
		type::Object* ExprCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ExprInstance>( scope, action_node, 0 );
			return new ExprInstance( arg1->value(), arg1->source() );
		}

		type::Object* ExprCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ExprInstance>( scope, action_node, 0 );
			auto arg2 = _operand<ExprInstance>( scope, action_node, 1 );
			auto rval = new IntegerInstance<int_t>();
			rval->value( arg1->value().compare( arg2->value() ) );
			return rval;
		}

		type::Object* ExprAssign::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ExprInstance>( scope, action_node, 0 );
			auto arg2 = _operand<ExprInstance>( scope, action_node, 1 );
			arg1->value( arg2->value() );
			return arg1;
		}
	}
}
