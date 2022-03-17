#include "ByteActions.h"


namespace eon
{
	void registerByteActions( scope::Global& scope )
	{
		scope.addAction( name_constructor, new actions::ByteConstruct() );
		scope.addAction( name_constructor, new actions::ByteCopyConstruct() );
		scope.addOperator( type::operators::code::cmp, new actions::ByteCmp() );
	}




	namespace actions
	{
		type::Object* ByteConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new ByteInstance(); }
		type::Object* ByteCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			return new ByteInstance( arg1->value(), arg1->source() );
		}

		type::Object* ByteCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			auto arg2 = _operand<ByteInstance>( scope, action_node, 1 );
			auto rval = new IntegerInstance<int_t>();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}
	}
}
