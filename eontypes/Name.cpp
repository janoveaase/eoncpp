#include "Name.h"
#include "NameActions.h"


namespace eon
{
	void registerName( scope::Global& scope )
	{
		scope.add( name_name, new NameType() );
		registerNameActions( scope );
	}




	type::Instance* NameType::instantiate( name_t value ) const { return  new NameInstance( value, source::Ref() ); }
}
