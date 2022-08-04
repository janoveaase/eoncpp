#include "Expression.h"


namespace eon
{
	namespace type
	{
		void Expression::str( Stringifier& str ) const
		{
			if( Variable )
				str.word( "e" ).start_grp2( "(" );
			Value.infixStr( str );
			if( Variable )
				str.end_grp2( ")" );
		}
	}
}
