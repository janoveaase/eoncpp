#include "Handle.h"
#include "HandleActions.h"


namespace eon
{
	void registerHandle( scope::Global& scope )
	{
		scope.add( name_handle, new HandleType() );
		registerHandleActions( scope );
	}




	type::Instance* HandleType::instantiate( handle_t value ) const { return new HandleInstance( value, source::Ref() ); }
}
