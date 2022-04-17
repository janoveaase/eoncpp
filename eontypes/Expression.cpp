#include "Expression.h"


namespace eon
{
	namespace type
	{
		void Expression::str( Stringifier& str ) const
		{
			if( Variable )
				str.pushWord( "e" ).pushOpen( "(" );
			Value.infixStr( str );
			if( Variable )
				str.pushClose( ")" );
		}
	}
}
