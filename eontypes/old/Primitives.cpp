#include "Primitives.h"
#include <eonscopes/Scope.h>
#include <unordered_set>


namespace eon
{
	void registerPrimitives( scope::Global& scope )
	{
		// Only add once!
		if( scope.get( name_bool ) == nullptr )
		{
			registerBool( scope );
			registerByte( scope );
			registerChar( scope );
			registerInt( scope );
			registerShort( scope );
			registerLong( scope );
			registerFloat( scope );
			registerLow( scope );
			registerHigh( scope );
			registerIndex( scope );
			registerName( scope );
			registerHandle( scope );
		}
	}

	static std::unordered_set<name_t> Primitives{
		name_bool, name_byte, name_char,
		name_int, name_short, name_long, name_float, name_low, name_high,
		name_index, name_name, name_handle
	};
	bool isPrimitive( name_t type_name ) noexcept
	{
		return Primitives.find( type_name ) != Primitives.end();
	}
}
