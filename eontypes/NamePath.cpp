#include "NamePath.h"
#include "NamePathActions.h"


namespace eon
{
	void registerNamePath( scope::Global& scope )
	{
		scope.add( name_namepath, new NamePathType() );
		registerNamePathActions( scope );
	}




	type::Instance* NamePathType::instantiate( const namepath& value ) const {
		return new NamePathInstance( value, source::Ref() ); }
	type::Instance* NamePathType::instantiate( namepath&& value ) const {
		return new NamePathInstance( std::move( value ), source::Ref() ); }
}
