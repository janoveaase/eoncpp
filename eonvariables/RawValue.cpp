#include "RawValue.h"


namespace eon
{
	namespace vars
	{
		string rawval::str( size_t& pos_on_line, size_t indentation_level )
			const noexcept
		{
			string s{ BarChr };
			if( Val.size() == 0 )
				return s;
			else if( Val.size() == 1 )
			{
				s += Val[ 0 ];
				pos_on_line = Val[ 0 ].numChars();
				return s;
			}

			++pos_on_line;
			string indent( ( 1 + indentation_level ) * 2, SpaceChr );
			bool new_line = true;
			for( auto& line : Val )
			{
				if( new_line )
				{
					s += NewlineChr;
					s += indent;
					pos_on_line = indent.numChars();
				}
				s += line;
				pos_on_line += line.numChars();
				new_line = true;
			}
			return s;
		}

		int rawval::actualCompare( const valueptr& other ) const
		{
			if( other->isRaw() )
			{
				auto& o = other->actualRaw();
				int cmp = 0;
				size_t i = 0, j = 0;
				for( ; i < Val.size() && j < o.size(); ++i, ++j )
				{
					cmp = Val[ i ].compare( o[ j ] );
					if( cmp != 0 )
						break;
				}
				return cmp;
			}
			else
				return value::actualCompare( other );
		}
	}
}
