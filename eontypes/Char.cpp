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
				str.word( "'\\0'" );
				return;
			case 0x07:
				str.word( "'\\a'" );
				return;
			case 0x08:
				str.word( "'\\b'" );
				return;
			case 0x0C:
				str.word( "'\\f'" );
				return;
			case 0x0A:
				str.word( "'\\n'" );
				return;
			case 0x09:
				str.word( "'\\t'" );
				return;
			case 0x0B:
				str.word( "'\\v'" );
				return;
			case 0x5C:
				str.word( "'\\\\'" );
				return;
			case 0x27:
				str.word( "'\\''" );
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
				str.word( "'\\u" + hexchars + "'" );
			else if( hexchars.numBytes() < 8 )
				str.word( hexchars.padLeft( 8, '0' ) );
			else
				str.word( "'\\U" + hexchars + "'" );
		}
		else
			str.word( "'" + string( Value ) + "'" );
	}
}
