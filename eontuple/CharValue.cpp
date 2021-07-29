#include "CharValue.h"


namespace eon
{
	namespace tup
	{
		int64_t charval::softInt( variables& vars ) const
		{
			if( Val >= '0' && Val <= '9' )
				return static_cast<int64_t>( Val ) - 48;
			else
				throw UnsupportedOperand( "Cannot convert from char '"
					+ string( Val ) + "' to 'int'!" );
		}
		double charval::softFloat( variables& vars ) const
		{
			if( Val >= '0' && Val <= '9' )
				return static_cast<double>( Val ) - 48.0;
			else
				throw UnsupportedOperand( "Cannot convert from char '"
					+ string( Val ) + "' to 'float'!" );
		}
	}
}
