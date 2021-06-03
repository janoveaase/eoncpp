#include "Listing.h"


namespace eon
{
	namespace doc
	{
		string listing::str( size_t max_line_len, size_t indent ) const
		{
			string s = anchor( "", "\n" );
			size_t num = 0;
			size_t ind = 2;
			for( auto& elm : Elms )
			{
				s += "  ";
				if( Numbered )
					s += string( ++num );
				else
					s += "-";
				s += " ";
				s += elm->str( 79 - ind, ind );
			}
			return s;
		}
	}
}
