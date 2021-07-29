#include "TupleValue.h"


namespace eon
{
	namespace tup
	{
		string tupleval::str( size_t& pos_on_line, size_t indentation_level )
			const noexcept
		{
			if( Val.empty() )
			{
				pos_on_line += 2;
				return "{}";
			}
			else
			{
				if( Val.braced() )
					++pos_on_line;
				auto s = Val.str( pos_on_line, indentation_level + 1 );
				if( Val.braced() )
					return "{" + s + "}";
				else
					return s;
			}
		}
	}
}
