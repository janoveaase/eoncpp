#include "String.h"
#include "StringActions.h"


namespace eon
{
	void registerString( scope::Global& scope )
	{
		scope.add( name_string, new StringType() );
		registerStringActions( scope );
	}




	type::Instance* StringType::instantiate( const string& value ) const { return new StringInstance( value ); }
	type::Instance* StringType::instantiate( string&& value ) const { return new StringInstance( std::move( value ) ); }
}
