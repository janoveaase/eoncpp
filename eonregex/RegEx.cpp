#include "RegEx.h"


namespace eon
{
	rx::match regex::match( const substring& str ) const
	{
		if( str.empty() || Graph.empty() )
			return rx::match();

		rx::RxData data( str, Graph.flags(), ++((regex*)this)->Marker );
		string::iterator start = data.pos();
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
			rx::RxData data( substring( pos, str.end() ), Graph.flags(), ++( (regex*)this )->Marker );
			if( Graph.match( data ) )
			{
				data.registerCapture( name_complete, substring( pos, data.pos() ) );
				return rx::match( data.claimCaptures() );
			}
		}
		return rx::match();
	}
	rx::match regex::findLast( const substring& str ) const
	{
		if( str.empty() || Graph.empty() )
			return rx::match();

		for( auto pos = str.last(); pos; --pos )
		{
			rx::RxData data( substring( pos, str.end() ), Graph.flags(), ++( (regex*)this )->Marker );
			if( Graph.match( data ) )
			{
				data.registerCapture( name_complete, substring( pos - 1, data.pos() ) );
				return rx::match( data.claimCaptures() );
			}
		}
		return rx::match();
	}
	std::vector<rx::match> regex::findAll( const substring& str ) const
	{
		std::vector<rx::match> matches;
		string::iterator pos = str.begin();
		while( pos )
		{
			auto found = findFirst( substring( pos, str.end() ) );
			if( !found )
				break;
			matches.push_back( found );
			pos = found.group( name_complete ).end();
		}
		return matches;
	}
}
