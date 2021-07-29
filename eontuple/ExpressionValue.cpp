#include "ExpressionValue.h"


namespace eon
{
	namespace tup
	{
		int expressionval::softCompare( const valueptr& other,
			variables& vars ) const
		{
			auto a = Val.evaluate( vars );
			while( a->isVar() || a->isExpression() )
			{
				if( a->isVar() )
					a = vars.get( a->hardVar() );
				else
					a = a->hardExpression().evaluate( vars );
			}
			auto o = other;
			while( o->isVar() || o->isExpression() )
			{
				if( o->isVar() )
					o = vars.get( o->hardVar() );
				else
					o = o->hardExpression().evaluate( vars );
			}
			return a->softCompare( o, vars );
		}
	}
}
