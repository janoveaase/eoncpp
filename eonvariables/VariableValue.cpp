#include "VariableValue.h"


namespace eon
{
	namespace vars
	{
		int variableval::actualCompare( const valueptr& other ) const
		{
			if( other->isVariable() )
			{
				auto o = other->actualVariable();
				return Val < o ? -1 : Val == o ? 0 : 1;
			}
			else
				return value::actualCompare( other );
		}
		int variableval::targetCompare( const valueptr& other, variables& vars ) const
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
