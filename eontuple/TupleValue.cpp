#include "TupleValue.h"


namespace eon
{
	namespace tup
	{
		string tupleval::str( size_t& pos_on_line, size_t indentation_level,
			perm format ) const noexcept
		{
			bool oneliner = static_cast<bool>( format & perm::allow_oneliner );
			bool multiliner = static_cast<bool>(
				format & perm::allow_multiliner );
			if( Val.empty() )
			{
				pos_on_line += 2;
				return "()";
			}
			else
			{
				auto s = Val.str( ++pos_on_line, indentation_level + 1,
					format );
				if( !multiliner || ( !s.contains( NewlineChr )
					&& pos_on_line + 2 <= 79 ) )
					return "(" + s + ")";
				if( multiliner )
				{
					if( s.contains( NewlineChr ) )
						return "- " + s;
					else
						return "(\n" + string( ( indentation_level + 1 ) * 2,
							SpaceChr ) + s + "\n" + string(
								indentation_level * 2, SpaceChr ) + ")";
				}
				else
					return s;
			}
		}
	}
}
