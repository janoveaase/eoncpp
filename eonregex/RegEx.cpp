#include "RegEx.h"


namespace eon
{
	rx::match regex::match( const substring& str ) const
	{
		if( str.empty() || Graph.empty() )
			return rx::match();

		rx::RxData data( str );
		auto start = data.pos();
		if( Graph.match( data ) )
		{
			data.registerCapture( name_complete, substring( start, data.pos() ) );
			return rx::match( data.claimCaptures() );
		}
		else
			return rx::match();
	}

	rx::match regex::findFirst( const substring& str ) const
	{
		if( str.empty() || Graph.empty() )
			return rx::match();

		for( auto pos = str.begin(); pos != str.end(); ++pos )
		{
			rx::RxData data( substring( pos, str.end() ) );
			if( Graph.match( data ) )
				return rx::match( data.claimCaptures() );
		}
		return rx::match();
	}
	rx::match regex::findLast( const substring& str ) const
	{
		if( str.empty() || Graph.empty() )
			return rx::match();

		for( auto pos = str.last(); pos; --pos )
		{
			rx::RxData data( substring( pos, str.end() ) );
			if( Graph.match( data ) )
				return rx::match( data.claimCaptures() );
		}
		return rx::match();
	}
}
