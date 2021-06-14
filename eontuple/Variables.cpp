#include "Variables.h"


namespace eon
{
	namespace tup
	{
		valueptr variables::get( name_t name ) const noexcept
		{
			auto found = Vars.find( name );
			return found != Vars.end() ? found->second : valueptr();
		}
		void variables::set( name_t name, const valueptr& value )
		{
			Vars[ name ] = value;
		}
	}
}
