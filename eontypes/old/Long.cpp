#include "Long.h"
#include "LongActions.h"


namespace eon
{
	void registerLong( scope::Global& scope )
	{
		scope.add( name_long, new LongType() );
		registerLongActions( scope );
	}




	type::Instance* LongType::instantiate( long_t value ) const { return new LongInstance( value ); }}
