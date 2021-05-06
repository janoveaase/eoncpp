#include "CaptureGroup.h"


namespace eon
{
	namespace rx
	{
		bool CaptureGroup::_match( RxData& data, size_t steps )
		{
			auto pos = data.pos();
			if( NodeGroup::_match( data, steps ) )
			{
				data.registerCapture( Name, substring( pos, data.pos() ) );
				return true;
			}
			return false;
		}
	}
}
