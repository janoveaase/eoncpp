#include "RawValue.h"
#include "Tuple.h"


namespace eon
{
	namespace tup
	{
		string rawval::str( size_t& pos_on_line, size_t indentation_level, bool named )
			const noexcept
		{
			string s{ BarChr };
			++pos_on_line;
			string indent( indentation_level * 2, SpaceChr );
			if( Val.size() > 1 || ( Val.size() == 1 && Val[ 0 ].numChars() + pos_on_line > 79 ) )
			{
				s += NewlineChr;
				indent += "  ";
				s += indent;
				pos_on_line = indent.numChars();
			}
			else
				indent += SpaceChr;
			bool new_line = false;
			for( auto& line : Val )
			{
				if( new_line )
				{
					s += NewlineChr;
					s += indent;
					pos_on_line = indent.numChars();
				}
				if( line.numChars() - pos_on_line - indentation_level <= 79 )
				{
					s += line;
					new_line = true;
				}
				else
				{
					// TODO: Split into lines of max 79 characters right margin!
					s += line;
					new_line = true;
				}
			}
			return s;
		}
	}
}
