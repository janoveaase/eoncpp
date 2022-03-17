#include "StandardTypes.h"


namespace eon
{
	void registerStandardTypes( scope::Global& scope )
	{
		registerBytes( scope );
		registerString( scope );
		registerExpression( scope );
		registerRegex( scope );
	}




	bool isStandard( name_t type_name )
	{
		static std::unordered_set<name_t> types{
			name_bytes, name_string, name_regex,
			name_namepath, name_path,
			name_expression
			// TODO: Add more once supported!
		};
		return types.find( type_name ) != types.end();
	}
}
