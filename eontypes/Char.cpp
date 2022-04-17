#include "Char.h"
#include "CharActions.h"
#include <eonhex/Hex.h>


namespace eon
{
	void registerChar( scope::Global& scope )
	{
		scope.add( name_char, new CharType() );
		registerCharActions( scope );
	}




	type::Instance* CharType::instantiate( char_t value ) const { return new CharInstance( value, source::Ref() ); }

	void CharInstance::str( Stringifier& str ) const
	{
		switch( Value )
		{
			case 0x00:
				str.pushWord( "'\\0'" );
				return;
			case 0x07:
				str.pushWord( "'\\a'" );
				return;
			case 0x08:
				str.pushWord( "'\\b'" );
				return;
			case 0x0C:
				str.pushWord( "'\\f'" );
				return;
			case 0x0A:
				str.pushWord( "'\\n'" );
				return;
			case 0x09:
				str.pushWord( "'\\t'" );
				return;
			case 0x0B:
				str.pushWord( "'\\v'" );
				return;
			case 0x5C:
				str.pushWord( "'\\\\'" );
				return;
			case 0x27:
				str.pushWord( "'\\''" );
				return;
			default:
				break;
		}
		auto& chars = Characters::get();
		if( chars.isOther( Value ) )
		{
			hex h( (const char*)&Value );
			string hexchars = h.value();
			if( hexchars.numChars() < 4 )
				hexchars = hexchars.padLeft( 4, '0' );
			if( hexchars.numChars() == 4 )
				str.pushWord( "'\\u" + hexchars + "'" );
			else if( hexchars.numBytes() < 8 )
				str.pushWord( hexchars.padLeft( 8, '0' ) );
			else
				str.pushWord( "'\\U" + hexchars + "'" );
		}
		else
			str.pushWord( "'" + string( Value ) + "'" );
	}
}
