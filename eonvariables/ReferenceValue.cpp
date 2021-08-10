#include "ReferenceValue.h"


namespace eon
{
	namespace vars
	{
		int refval::targetCompare( const valueptr& other, variables& vars ) const
		{
			auto& lhs = target( vars, valueptr() );
			auto& rhs = other->target( vars, other );
			if( !lhs )
				return rhs ? 1 : 0;
			else if( !rhs )
				return -1;
			else
				return lhs->actualCompare( rhs );
		}
	}
}
