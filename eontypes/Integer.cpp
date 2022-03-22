#include "Integer.h"
#include "IntegerActions.h"


namespace eon
{
	void registerIntegerActions( scope::Global& scope, const std::type_info& type_id )
	{
		if( type_id == typeid( int_t ) )
			registerIntegerActions<int_t>( scope );
		else if( type_id == typeid( short_t ) )
			registerIntegerActions<short_t>( scope );
		else if( type_id == typeid( long_t ) )
			registerIntegerActions<long_t>( scope );
	}
}
