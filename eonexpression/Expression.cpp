#include "Expression.h"


namespace eon
{
	string Expression::str() const
	{
		Stringifier strf;
		str( strf );
		return strf.generateString();
	}
}
