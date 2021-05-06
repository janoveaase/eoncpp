#include "RxData.h"


namespace eon
{
	namespace rx
	{
		substring RxData::findCapture( name_t name ) const noexcept
		{
			auto found = Captures.find( name );
			if( found != Captures.end() )
				return found->second;
			else
				return substring();
		}
	}
}
