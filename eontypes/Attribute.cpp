#include "Attribute.h"


namespace eon
{
	namespace type
	{
		Qualifier mapQualifier( string qual )
		{
			if( qual == "read" )
				return Qualifier::read;
			else if( qual == "modify" )
				return Qualifier::modify;
			else if( qual == "consume" )
				return Qualifier::consume;
			else
				return Qualifier::undef;
		}
		string mapQualifier( Qualifier qual )
		{
			switch( qual )
			{
				case Qualifier::read:
					return "read";
				case Qualifier::modify:
					return "modify";
				case Qualifier::consume:
					return "consume";
				default:
					return "N/A";
			}
		}
	}
}
