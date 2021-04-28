#include "SourceRef.h"


namespace eon
{
	bool sourcepos::atEnd( const source& source ) const noexcept
	{
		if( Line >= source.numLines() )
			return true;
		if( Line < source.numLines() - 1 )
			return false;
		return !Area.begin() || Area.begin() == source.line( Line ).end();
	}

	bool sourcepos::advance( const source& source ) noexcept
	{
		if( atEnd( source ) )
			return false;

		Area.end() = ++Area.begin() + 1;
		if( Area.begin() > source.line( Line ).end() )
			++Line;
		return true;
	}
}
