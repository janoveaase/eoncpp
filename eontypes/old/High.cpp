#include "High.h"
#include "HighActions.h"


namespace eon
{
	void registerHigh( scope::Global& scope )
	{
		scope.add( name_high, new HighType() );
		registerHighActions( scope );
	}




	type::Instance* HighType::instantiate( high_t value ) const { return new HighInstance( value ); }
}
