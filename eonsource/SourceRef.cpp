#include "SourceRef.h"


namespace eon
{
	namespace source
	{
		bool Ref::pullStartOfLine() noexcept
		{
			if( Start.bytePos() == 0 )
				return false;
			Pos beg;
			try
			{
				auto start = Source->pull( Start, beg, 1 );
				while( true )
				{
					if( Source->chr( start ) != NewlineChr )
					{
						Start = start;
						if( Start.bytePos() == 0 )
							break;
						start = Source->pull( start, beg, 1 );
					}
					else
						break;
				}
				return true;
			}
			catch( ... ) {}
			return false;
		}

		bool Ref::pushEndOfLine() noexcept
		{
			if( Source->chr( End ) == NewlineChr )
				return false;
			try
			{
				while( true )
				{
					End = Source->push( End, 1 );
					if( Source->atEnd( End ) )
						break;
					if( Source->chr( End ) == NewlineChr )
						break;
				}
				return true;
			}
			catch( ... ) {}
			return false;
		}
	}
}
