#include "Plaintext.h"


namespace eon
{
	namespace doc
	{
		string plaintext::str( size_t max_line_len, size_t indent ) const
		{
			string indt( indent * 2, SpaceChr );
			string s;
			bool new_line = false;
			size_t line_size = 0;
			for( auto& word : Words )
			{
			top:
				if( word == "\n" )
				{
					s += word + indt + "  ";
					line_size = word.numChars() + indt.numChars() + 2;
				}
				else if( new_line )
				{
					s += indt + word;
					line_size = indt.numChars() + word.numChars();
					new_line = false;
				}
				else if( line_size + word.numChars() > max_line_len )
				{
					new_line = true;
					s += "\n";
					goto top;
				}
				else
				{
					if( !s.empty() )
						s += " ";
					s += word;
					line_size += word.numChars() + 1;
				}
			}
			return s;
		}
	}
}
