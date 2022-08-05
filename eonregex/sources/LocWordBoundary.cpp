#include "LocWordBoundary.h"


namespace eon
{
	namespace rx
	{
		bool LocWordEnd::_match( RxData& data, index_t steps )
		{
			if( data.bounds() )
			{
				return !string::isSeparatorSpace( data.prev() ) && ( !data || string::isSeparatorSpace( data() ) );
			}
			else
			{
				return !string::isSeparator( data.prev() ) && !string::isPunctuation( data.prev() )
					&& ( !data || string::isSeparator( data() ) || string::isPunctuation( data() ) );
			}
		}
	}
}
