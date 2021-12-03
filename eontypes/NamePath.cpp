#include "NamePath.h"
#include "NamePathActions.h"


namespace eon
{
	void registerNamePath( scope::Global& scope )
	{
		scope.add( name_namepath, new NamePathType() );
		registerNamePathActions( scope );
	}




	type::Instance* NamePathType::instantiate( const nameref& value ) const { return new NamePathInstance( value ); }
	type::Instance* NamePathType::instantiate( nameref&& value ) const { return new NamePathInstance( std::move( value ) ); }
}
