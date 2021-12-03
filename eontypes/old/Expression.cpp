#include "Expression.h"
#include "BoolActions.h"
#include <eonscopes/Scope.h>


namespace eon
{
	void registerExpression()
	{
		scope::Scope::global.add( name_expression, new ExpressionType( false ) );
		scope::Scope::global.registerDef( std::type_index( typeid( int_t ) ),
			(type::Definition*)scope::Scope::global.get( name_int ) );
		registerExpressionActions();
	}




	index_t ExpressionType::stackSize() const noexcept { return sizeof( ExpressionInstance ); }
	type::Instance* ExpressionType::instantiate( expression& value, byte_t* memory_start ) const
	{
		return memory_start ? new( memory_start ) ExpressionInstance( *this, value, true )
			: new ExpressionInstance( *this, value, false );
	}
}
