#include "Regression.h"



namespace eon
{
	TEST( EdtTest, basic )
	{
		DataTuple dt( {
			type::Attribute( name( "text" ), new StringInstance( "A string", source::Ref() ) ),
			type::Attribute( name( "number" ), new FloatingptInstance<flt_t>( -9.999, source::Ref() ) ),
			type::Attribute( new IntegerInstance<int_t>( 12345, source::Ref() ) ),
			type::Attribute( name( "data" ), new DataTuple( {
				type::Attribute( new StringInstance( "Another string", source::Ref() ) ),
				type::Attribute( name( "char" ), new CharInstance('X', source::Ref() ) )
				}, source::Ref() ) ),
			type::Attribute( new DataTuple( {
				type::Attribute( new StringInstance( "Third string", source::Ref() ) ),
				type::Attribute( name( "byte" ), new ByteInstance( 'X', source::Ref() ) )
				}, source::Ref() ) )
			}, source::Ref() );
		
		eon::string exp{
			"text = \"A string\"\n"
			"number = -9.999\n"
			"12345\n"
			"data:\n"
			"  \"Another string\"\n"
			"  char = 'X'\n"
			"(\"Third string\", byte = b'X')\n"
		};

		eon::string act;
		edt().toStr( dt, act );

		WANT_EQ( act, exp );
	}
}
