#include "Float.h"
#include "FloatActions.h"


namespace eon
{
	void registerFloat( scope::Global& scope )
	{
		scope.add( name_float, new FloatType() );
		registerFloatActions( scope );
	}




	type::Instance* FloatType::instantiate( flt_t value ) const { return new FloatInstance( value ); }
}
