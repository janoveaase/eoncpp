#include "RegexActions.h"
#include "Int.h"
#include "String.h"
#include "Index.h"


namespace eon
{
	void registerRegexActions( scope::Global& scope )
	{
		scope.add( name_construct, new actions::RegexConstruct() );
		scope.add( name_copyconstruct, new actions::RegexCopyConstruct( scope ) );
		scope.add( name_construct, new actions::RegexStringConstruct( scope ) );
		scope.add( name_cmp, new actions::RegexCmp( scope ) );
	}




	namespace actions
	{
		type::Object* RegexConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new RegexInstance(); }
		type::Object* RegexCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<RegexInstance>( scope, action_node, 0 );
			return new RegexInstance( arg1->value() );
		}

		type::Object* RegexStringConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<StringInstance>( scope, action_node, 0 );
			return new RegexInstance( string( arg1->value() ).stdstr() );
		}

		type::Object* RegexCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<RegexInstance>( scope, action_node, 0 );
			auto arg2 = _operand<RegexInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value().str().compare( arg2->value().str() ) );
			return rval;
		}
	}
}
