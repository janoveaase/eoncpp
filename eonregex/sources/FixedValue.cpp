#include "FixedValue.h"


namespace eon
{
	namespace rx
	{
		bool FixedValue::_match( RxData& data, size_t steps )
		{
			RxData param_b{ data };

			for( auto c : Value )
			{
				if( param_b() == c )
					param_b.advance();
				else
					return false;
			}
			data = std::move( param_b );
			return true;
		}
	}
}
