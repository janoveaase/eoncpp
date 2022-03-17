#include "PathActions.h"
#include "Integer.h"
#include "String.h"
#include "Index.h"


namespace eon
{
	void registerPathActions( scope::Global& scope )
	{
		scope.addAction( name_constructor, new actions::PathConstruct() );
		scope.addAction( name_constructor, new actions::PathCopyConstruct() );
		scope.addAction( name_constructor, new actions::PathStringConstruct() );
		scope.addOperator( type::operators::code::cmp, new actions::PathCmp() );
		scope.addOperator( type::operators::code::plus, new actions::PathPlus() );
	}




	namespace actions
	{
		type::Object* PathConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new PathInstance(); }
		type::Object* PathCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<PathInstance>( scope, action_node, 0 );
			return new PathInstance( arg1->value(), arg1->source() );
		}

		type::Object* PathStringConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<StringInstance>( scope, action_node, 0 );
			return new PathInstance( string( arg1->value() ).stdstr(), arg1->source() );
		}

		type::Object* PathCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<PathInstance>( scope, action_node, 0 );
			auto arg2 = _operand<PathInstance>( scope, action_node, 1 );
			auto rval = new IntegerInstance<int_t>();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}

		type::Object* PathPlus::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<PathInstance>( scope, action_node, 0 );
			auto arg2 = _operand<PathInstance>( scope, action_node, 1 );
			auto rval = new PathInstance( arg1->value() + arg2->value(), source::Ref() );
			return rval;
		}
	}
}
