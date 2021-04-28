#include "StringSource.h"


StringSource::StringSource( String&& code ) : Source()
{
	Name = "String (#" + String( code.hash() + ")" );
	String line;
	for( auto c : code )
	{
		if( c == '\n' )
		{
			line += c;
			Lines.push_back( std::move( line ) );
		}
		else
			line += c;
	}
}
