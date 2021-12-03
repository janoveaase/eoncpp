#include "Bool.h"
#include "BoolActions.h"


namespace eon
{
	void registerBool( scope::Global& scope )
	{
		scope.add( name_bool, new BoolType() );
		registerBoolActions( scope );
	}




	type::Instance* BoolType::instantiate( bool value ) const { return new BoolInstance( value ); }
}
