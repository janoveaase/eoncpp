#include "Path.h"
#include "PathActions.h"


namespace eon
{
	void registerPath( scope::Global& scope )
	{
		scope.add( name_path, new PathType() );
		registerPathActions( scope );
	}




	type::Instance* PathType::instantiate( const path& value ) const { return new PathInstance( value, source::Ref() ); }
	type::Instance* PathType::instantiate( path&& value ) const {
		return new PathInstance( std::move( value ), source::Ref() ); }
}
