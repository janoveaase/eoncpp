#include "CaptureGroup.h"


namespace eon
{
	namespace rx
	{
		bool CaptureGroup::_match( RxData& data, index_t steps )
		{
			Start = data.pos();
			Captured = false;
			if( NodeGroup::_match( data, steps ) )
			{
				if( !Captured )
					data.registerCapture( Name, substring( Start, data.pos() ) );
				return true;
			}
			return false;
		}
	}
}
