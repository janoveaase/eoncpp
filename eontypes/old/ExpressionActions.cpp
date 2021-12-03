#include "ExpressionActions.h"


namespace eon
{
	void registerExpressionActions()
	{
		scope::Scope::global.add( name_construct, new actions::ExpressionConstruct() );
		scope::Scope::global.add( name_copyconstruct, new actions::ExpressionCopyConstruct() );
		scope::Scope::global.add( name_cmp, new actions::ExpressionCmp() );
		scope::Scope::global.add( name_not, new actions::ExpressionCall() );
	}




	namespace actions
	{
		void ExpressionCopyConstruct::execute( Scope& parent_scope, scope::Frame& frame )
		{
			auto arg1 = _operand<ExpressionInstance>( parent_scope, stack, values );
			auto rval = (ExpressionInstance*)values.top();
			rval->value( arg1->value() );
		}

		void ExpressionCmp::execute( Scope& parent_scope, scope::Frame& frame )
		{
			auto arg2 = _operand<ExpressionInstance>( parent_scope, stack, values );
			auto arg1 = _operand<ExpressionInstance>( parent_scope, stack, values );
			auto rval = (ExpressionInstance*)values.top();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
		}

		void ExpressionCall::execute( Scope& parent_scope, scope::Frame& frame )
		{
			auto arg1 = _operand<ExpressionInstance>( parent_scope, stack, values );
			auto rval = (ExpressionInstance*)values.top();
			rval->value( !arg1->value() );
		}
	}
}
