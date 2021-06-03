#include "Exclamation.h"


namespace eon
{
	namespace doc
	{
		string mapExcltype( excltype type )
		{
			switch( type )
			{
				case excltype::tip_t:
					return "TIP";
				case excltype::note_t:
					return "NOTE";
				case excltype::warning_t:
					return "WARNING";
				default:
					return "";
			}
		}
		excltype mapExcltype( const string& type )
		{
			if( type == "TIP" )
				return excltype::tip_t;
			else if( type == "NOTE" )
				return excltype::note_t;
			else if( type == "WARNING" )
				return excltype::warning_t;
			else
				return excltype::undef;
		}



		string exclamation::str( size_t max_line_len, size_t indent ) const
		{
			string s = mapExcltype( XType ) + ": ";
			
			bool new_line = false;
			size_t line_size = 0;
			for( auto& word : Words )
			{
			top:
				if( new_line )
				{
					s += "  " + word;
					line_size = word.numChars() + 2;
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
					if( !s.endsWith( SpaceChr ) )
						s += " ";
					s += word;
					line_size += word.numChars() + 1;
				}
			}
			s += "\n";
			return s;
		}
	}
}
