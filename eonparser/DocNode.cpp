#include "DocNode.h"


namespace eon
{
	docnode& docnode::add( string&& name )
	{
		if( Children.find( name ) != Children.end() )
			throw DuplicateNodeName( name.stdstr() );
		docnode* node = new docnode();
		Children[ std::move( name ) ] = node;
		return *node;
	}




	docnode& docnode::child( const string& name ) const
	{
		auto found = Children.find( name );
		if( found != Children.end() )
			return *found->second;
		else
			throw NoSuchNode( name.stdstr() );
	}
}
