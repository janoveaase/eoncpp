#include "ByteActions.h"


namespace eon
{
	void registerByteActions( scope::Global& scope )
	{
		scope.add( name_construct, new actions::ByteConstruct() );
		scope.add( name_construct, new actions::ByteCopyConstruct( scope ) );
		scope.add( name_cmp, new actions::ByteCmp( scope ) );
	}




	namespace actions
	{
		type::Object* ByteConstruct::execute( scope::Scope& scope, type::Node& action_node ) {
			return new ByteInstance(); }
		type::Object* ByteCopyConstruct::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			return new ByteInstance( arg1->value() );
		}

		type::Object* ByteCmp::execute( scope::Scope& scope, type::Node& action_node )
		{
			auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
			auto arg2 = _operand<ByteInstance>( scope, action_node, 1 );
			auto rval = new IntInstance();
			rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
			return rval;
		}
	}
}
