#include "DataTuple.h"

/*
namespace eon
{
	namespace type
	{
		void BasicTuple::str( Stringifier& str ) const
		{
			str.noSpacing();
			bool first = true;
			for( auto& element : Attributes )
			{
				if( first )
					first = false;
				else
				{
					str.addWord( "," );
					str.spacingOnce();
				}

				if( element.name() )
				{
					str.addWord( element.name() );
					if( element.value()->generalType() == name_data )
					{
						str.addWord( ":" );
						str.addIndent();
						str.newLine();
						element.value()->str( str );
						str.noSpacing();
						str.reduceIndent();
					}
					else
					{
						str.addWord( "=" );
						if( element.value()->generalType() == name_tuple )
							str.addWord( "(" );
						element.value()->str( str );
						str.noSpacing();
						if( element.value()->generalType() == name_tuple )
							str.addWord( ")" );
					}
				}
				else
				{
					if( element.value()->generalType() == name_tuple )
					{
						str.addRaw( "(" );
						str.addIndent();
						str.newLine();
						element.value()->str( str );
						str.noSpacing();
						str.addRaw( ")" );
						str.reduceIndent();
					}
					else
					{
						element.value()->str( str );
						str.noSpacing();
					}
				}
			}
		}
	}
}
*/