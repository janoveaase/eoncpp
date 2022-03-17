#include "Primitives.h"


namespace eon
{
	void registerPrimitives( scope::Global& scope )
	{
		registerBool( scope );
		registerByte( scope );
		registerChar( scope );
		registerInteger<int_t>( scope );
		registerInteger<short_t>( scope );
		registerInteger<long_t>( scope );
		registerFloatintpt<flt_t>( scope );
		registerFloatintpt<low_t>( scope );
		registerIndex( scope );
		registerHandle( scope );
		registerName( scope );
	}
	bool isPrimitive( name_t type_name )
	{
		static std::unordered_set<name_t> types{
			name_bool, name_byte, name_char,
			name_int, name_short, name_long,
			name_float, name_low, name_high,
			name_index, name_handle, name_name
		};
		return types.find( type_name ) != types.end();
	}
}
