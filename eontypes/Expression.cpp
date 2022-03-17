#include "Expression.h"


namespace eon
{
	namespace type
	{
		void Expression::str( type::Stringifier& str ) const
		{
			if( Variable )
			{
				str.addWord( "e(" );
				str.markRaw();
			}
			Value.infixStr( str );
			if( Variable )
			{
				str.addRaw( ")" );
				str.resetRaw();
			}
		}
	}
}
