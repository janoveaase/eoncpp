#include "Path.h"


namespace eon
{
	namespace tup
	{
		int path::compare( const path& other ) const noexcept
		{
			auto i = Value.begin(), j = other.Value.begin();
			for( ; i != Value.end() && j != other.Value.end(); ++i, ++j )
			{
				auto cmp = ( *i )->compare( **j );
				if( cmp != 0 )
					return cmp;
			}
			return i == Value.end() && j == other.Value.end()
				? 0 : i != Value.end() ? -1 : 1;
		}
	}
}
