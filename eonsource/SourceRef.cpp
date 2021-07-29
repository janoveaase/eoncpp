#include "SourceRef.h"


namespace eon
{
	string sourceref::textRefRange() const
	{
		if( Src != nullptr )
		{
			if( Pos.area().numChars() > 1 )
				return textRefPos() + "-"
				+ string( Pos.pos( *Src ) + 1 + Pos.area().numChars() );
			else
				return textRefPos();
		}
		else
			return textRefLine();
	}
	string sourceref::textRefPos() const
	{
		if( Src != nullptr )
			return textRefLine() + ":" + string( Pos.pos( *Src ) + 1 );
		else
			return textRefLine();
	}
	string sourceref::textRefLine() const
	{
		string txt;
		if( Src != nullptr )
		{
			txt += Src->name();
			txt += ":" + string( Pos.line() + 1 );
		}
		else
			txt = "N/A";
		return txt;
	}
}
