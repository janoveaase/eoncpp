#include "LocWordBoundary.h"


namespace eon
{
	namespace rx
	{
		bool LocWordEnd::_match( RxData& data, index_t steps )
		{
			return string::isWordChar( data.prev() )
				&& ( !data || string::isSpaceChar( data() ) || string::isPunctuation( data() ) )
				? data.advance() : false;
		}
	}
}
