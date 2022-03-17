#include "Low.h"
#include "LowActions.h"


namespace eon
{
	void registerLow( scope::Global& scope )
	{
		scope.add( name_low, new LowType() );
		registerLowActions( scope );
	}




	type::Instance* LowType::instantiate( low_t value ) const { return new LowInstance( value ); }
}
