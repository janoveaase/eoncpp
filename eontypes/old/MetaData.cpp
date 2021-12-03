#include "MetaData.h"


namespace eon
{
	void MetaData::str( type::Stringifier& str ) const
	{
		str.addRaw( "M(" );
		BasicTuple::str( str );
		str.addRaw( ")" );
	}
}
