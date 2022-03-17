#include "TokenParser.h"


namespace eon
{
	size_t TokenParser::lineStart() const
	{
		for( size_t i = Cur; i > 0; --i )
		{
			if( at( i - 1 ).is( name_newline ) )
				return i;
		}
		return 0;
	}
}
