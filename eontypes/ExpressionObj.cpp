#include "ExpressionActions.h"
#include "ExpressionObj.h"


namespace eon
{
	void registerExpression( scope::Global& scope )
	{
		scope.add( name_expression, new ExprType() );
		registerExpressionActions( scope );
	}




	type::Instance* ExprType::instantiate( const type::Expression& value ) const {
		return new ExprInstance( value, source::Ref() ); }
	type::Instance* ExprType::instantiate( type::Expression&& value ) const {
		return new ExprInstance( std::move( value ), source::Ref() ); }
}
