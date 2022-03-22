#include "Floatingpt.h"
#include "FloatingptActions.h"


namespace eon
{
	void registerFloatingptActions( scope::Global& scope, const std::type_info& type_id )
	{
		if( type_id == typeid( flt_t ) )
			registerFloatingptActions<flt_t>( scope );
		else if( type_id == typeid( low_t ) )
			registerFloatingptActions<low_t>( scope );
		else if( type_id == typeid( high_t ) )
			registerFloatingptActions<high_t>( scope );
	}
}
