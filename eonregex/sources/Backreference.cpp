#include "Backreference.h"


namespace eon
{
	namespace rx
	{
		bool Backreference::_match( RxData& data, index_t steps )
		{
			auto group = data.findCapture( Name );
			if( group )
			{
				if( !data )
					return false;
				RxData param_b{ data };

				for( auto c = group.begin(); c != group.end() && param_b; ++c )
				{
					if( param_b() == *c )
						param_b.advance();
					else
						return false;
				}
				data = std::move( param_b );
				return true;
			}
			return false;
		}
	}
}
