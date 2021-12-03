#include "Name.h"
#include "NameActions.h"


namespace eon
{
	void registerName( scope::Global& scope )
	{
		scope.add( name_low, new NameType() );
		registerNameActions( scope );
	}




	type::Instance* NameType::instantiate( name_t value ) const { return  new NameInstance( value ); }
}
