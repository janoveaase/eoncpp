#include "LocWordBoundary.h"


namespace eon
{
	namespace rx
	{
		bool LocWordStart::_match( RxData& data, size_t steps )
		{
			return string::isWordChar( data() ) && (
				data.pos() == 0 || string::isSpaceChar( data.prev()
					|| string::isPunctuation( data.prev() ) ) )
				? data.advance() : false;
		}


		bool LocWordEnd::_match( RxData& data, size_t steps )
		{
			return string::isWordChar( data.prev() )
				&& ( !data || string::isSpaceChar( data() )
					|| string::isPunctuation( data() ) )
				? data.advance() : false;
		}
	}
}
