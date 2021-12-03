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




	type::Instance* CharType::instantiate( char_t value ) const { return new CharInstance( value ); }

	void CharInstance::str( type::Stringifier& str ) const
	{
		switch( Value )
		{
			case 0x00:
				str.addWord( "'\\0'" );
				return;
			case 0x07:
				str.addWord( "'\\a'" );
				return;
			case 0x08:
				str.addWord( "'\\b'" );
				return;
			case 0x0C:
				str.addWord( "'\\f'" );
				return;
			case 0x0A:
				str.addWord( "'\\n'" );
				return;
			case 0x09:
				str.addWord( "'\\t'" );
				return;
			case 0x0B:
				str.addWord( "'\\v'" );
				return;
			case 0x5C:
				str.addWord( "'\\\\'" );
				return;
			case 0x27:
				str.addWord( "'\\''" );
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
				str.addWord( "'\\u" + hexchars + "'" );
			else if( hexchars.numBytes() < 8 )
				str.addWord( hexchars.padLeft( 8, '0' ) );
			else
				str.addWord( "'\\U" + hexchars + "'" );
		}
		else
			str.addWord( "'" + string( Value ) + "'" );
	}
}
