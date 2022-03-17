#include "Int.h"
#include "IntActions.h"


namespace eon
{
	void registerInt( scope::Global& scope )
	{
		scope.add( name_int, new IntType() );
		registerIntActions( scope );
	}




	type::Instance* IntType::instantiate( int_t value ) const { return new IntInstance( value ); }
}
