#include "Byte.h"
#include "ByteActions.h"


namespace eon
{
	void registerByte( scope::Global& scope )
	{
		scope.add( name_byte, new ByteType() );
		registerByteActions( scope );
	}




	type::Instance* ByteType::instantiate( byte_t value ) const { return new ByteInstance( value, source::Ref() ); }

	void ByteInstance::str( type::Stringifier& str ) const
	{
		switch( Value )
		{
			case 0x07:
				str.addWord( "b'\\a'" );
				return;
			case 0x08:
				str.addWord( "b'\\b'" );
				return;
			case 0x0C:
				str.addWord( "b'\\f'" );
				return;
			case 0x0A:
				str.addWord( "b'\\n'" );
				return;
			case 0x09:
				str.addWord( "b'\\t'" );
				return;
			case 0x0B:
				str.addWord( "b'\\v'" );
				return;
			case 0x5C:
				str.addWord( "b'\\\\'" );
				return;
			case 0x27:
				str.addWord( "b'\\''" );
				return;
			default:
				if( Value < 0x20 || Value > 0x7E )
				{
					string octal;
					int div{ 0 }, rem{ 0 };
					for( int val = static_cast<int>( Value ); val != 0; )
					{
						div = val / 8;
						rem = val - ( div * 8 );
						octal += string( rem );
						val = div;
					}
					str.addWord( "b'\\" + octal.reverse() + "'" );
				}
				else
					str.addWord( string( "b'" ) + Value + "'" );
		}
	}
}
