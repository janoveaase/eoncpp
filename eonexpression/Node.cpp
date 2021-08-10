#include "Node.h"
#include <eonvariables/BoolValue.h>


namespace eon
{
	namespace expr
	{
		vars::valueptr node::evaluate( vars::variables& vars ) const
		{
			auto value = evaluateAccurate( vars );
			return value ? value : vars::boolval::create( false );
		}
	}
}
