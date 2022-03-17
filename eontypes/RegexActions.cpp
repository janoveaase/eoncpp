#include "RegexActions.h"
#include "Integer.h"
#include "String.h"
#include "Index.h"


namespace eon
{
	void registerRegexActions( scope::Global& scope )
	{
		scope.addAction( name_constructor, new actions::RegexConstruct() );
		scope.addAction( name_constructor, new actions::RegexCopyConstruct() );
		scope.addAction( name_constructor, new actions::RegexStringConstruct() );
		scope.addOperator( type::operators::code::cmp, new actions::RegexCmp() );
	}




	namespace actions
	{
		type::Object* RegexConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new RegexInstance(); }
		type::Object* RegexCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<RegexInstance>( scope, action_node, 0 );
			return new RegexInstance( arg1->value(), arg1->source() );
		}

		type::Object* RegexStringConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<StringInstance>( scope, action_node, 0 );
			return new RegexInstance( string( arg1->value() ).stdstr(), arg1->source() );
		}

		type::Object* RegexCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<RegexInstance>( scope, action_node, 0 );
			auto arg2 = _operand<RegexInstance>( scope, action_node, 1 );
			auto rval = new IntegerInstance<int_t>();
			rval->value( arg1->value().str().compare( arg2->value().str() ) );
			return rval;
		}
	}
}
