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

	void ByteInstance::str( Stringifier& str ) const
	{
		switch( Value )
		{
			case 0x07:
				str.pushWord( "b'\\a'" );
				return;
			case 0x08:
				str.pushWord( "b'\\b'" );
				return;
			case 0x0C:
				str.pushWord( "b'\\f'" );
				return;
			case 0x0A:
				str.pushWord( "b'\\n'" );
				return;
			case 0x09:
				str.pushWord( "b'\\t'" );
				return;
			case 0x0B:
				str.pushWord( "b'\\v'" );
				return;
			case 0x5C:
				str.pushWord( "b'\\\\'" );
				return;
			case 0x27:
				str.pushWord( "b'\\''" );
				return;
			default:
				if( Value < 0x20 || Value > 0x7E )
				{
					string octal;
					int div{ 0 }, rem{ 0 };
					for( int val = static_cast<int>( Value ), i = 0; i == 0 || val != 0; ++i )
					{
						div = val / 8;
						rem = val - ( div * 8 );
						octal += string( rem );
						val = div;
					}
					str.pushWord( "b'\\" + octal.reverse() + "'" );
				}
				else
					str.pushWord( string( "b'" ) + Value + "'" );
		}
	}
}
