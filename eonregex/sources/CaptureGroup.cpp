#include "CaptureGroup.h"


namespace eon
{
	namespace rx
	{
		bool CaptureGroup::_match( RxData& data, index_t steps )
		{
			Start = data.pos();
			if( NodeGroup::_match( data, steps ) )
			{
				data.registerCapture( Name, substring( Start, data.pos() ) );
				return true;
			}
			return false;
		}
	}
}
