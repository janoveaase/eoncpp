#include "Short.h"
#include "ShortActions.h"


namespace eon
{
	void registerShort( scope::Global& scope )
	{
		scope.add( name_short, new ShortType() );
		registerShortActions( scope );
	}




	type::Instance* ShortType::instantiate( short_t value ) const { return new ShortInstance( value ); }
}
