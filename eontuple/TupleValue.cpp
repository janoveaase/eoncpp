#include "TupleValue.h"


namespace eon
{
	namespace tup
	{
		string tupleval::str( size_t& pos_on_line, size_t indentation_level,
			bool named ) const noexcept
		{
			if( Val.empty() )
			{
				pos_on_line += 2;
				return "()";
			}
			else
			{
				auto s = Val.str( ++pos_on_line, indentation_level + 1,
					named
						? tuple::perm::allow_multiliner
						: tuple::perm::allow_multiliner
							| tuple::perm::allow_oneliner );
				if( !s.contains( NewlineChr ) && pos_on_line + 2 <= 79 )
					return "(" + s + ")";
				if( !named )
				{
					if( s.contains( NewlineChr ) )
						return "- " + s;
					else
						return "(\n" + string( ( indentation_level + 1 ) * 2,
							SpaceChr ) + s + "\n" + string( indentation_level * 2,
								SpaceChr ) + ")";
				}
				else
					return s;
			}
		}
	}
}
