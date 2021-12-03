#include "PathActions.h"
#include "Int.h"
#include "String.h"
#include "Index.h"


namespace eon
{
	void registerPathActions( scope::Global& scope )
	{
		scope.add( name_construct, new actions::PathConstruct() );
		scope.add( name_copyconstruct, new actions::PathCopyConstruct( scope ) );
		scope.add( name_construct, new actions::PathStringConstruct( scope ) );
		scope.add( name_cmp, new actions::PathCmp( scope ) );
		scope.add( name_construct, new actions::PathPlus( scope ) );
	}




	namespace actions
	{
		type::Object* PathConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new PathInstance(); }
		type::Object* PathCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<PathInstance>( scope, action_node, 0 );
			return new PathInstance( arg1->value() );
		}

		type::Object* PathStringConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<StringInstance>( scope, action_node, 0 );
			return new PathInstance( string( arg1->value() ).stdstr() );
		}

		type::Object* PathCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<PathInstance>( scope, action_node, 0 );
			auto arg2 = _operand<PathInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}

		type::Object* PathPlus::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<PathInstance>( scope, action_node, 0 );
			auto arg2 = _operand<PathInstance>( scope, action_node, 1 );
			auto rval = new PathInstance( arg1->value() + arg2->value() );
			return rval;
		}
	}
}
