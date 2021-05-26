#include "Definition.h"


namespace eon
{
	namespace doc
	{
		string definition::str( size_t max_line_len, size_t indent ) const
		{
			string s = anchor( "", "\n" ) + "  ";
			auto oneliner_size = 79 - What.numChars() + 4;
			auto def = block::str( oneliner_size, 0 );
			if( def.numChars() <= oneliner_size )
				return s + What + ": " + def;
			else
				return s + What + ":\n    " + block::str( 75, 2 );
		}
	}
}
