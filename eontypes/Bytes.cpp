#include "Bytes.h"
#include "BytesActions.h"


namespace eon
{
	void registerBytes( scope::Global& scope )
	{
		scope.add( name_bytes, new BytesType() );
		registerBytesActions( scope );
	}




	type::Instance* BytesType::instantiate( const std::string& value ) const { return new BytesInstance( value ); }
	type::Instance* BytesType::instantiate( std::string&& value ) const { return new BytesInstance( std::move( value ) ); }
}
