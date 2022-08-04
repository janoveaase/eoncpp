#include "Regression.h"



namespace eon
{
	TEST( EdtTest, type_bool )
	{
		eon::string exp{ "true\nfalse" };
		auto from_str = edt( "Test", eon::string( exp ) ).load();
		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}
	TEST( EdtTest, type_byte )
	{
		eon::string exp{ "b'y'\nb'\\0'" };
		auto from_str = edt( "Test", eon::string( exp ) ).load();
		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}
	TEST( EdtTest, type_char )
	{
		eon::string exp{ u8"'y'\n'β'" };
		auto from_str = edt( "Test", eon::string( exp ) ).load();
		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}
	TEST( EdtTest, type_int )
	{
		eon::string exp{ "12345\n-12345" };
		auto from_str = edt( "Test", eon::string( exp ) ).load();
		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}
	TEST( EdtTest, type_short )
	{
		eon::string exp{ "456S\n-456S" };
		auto from_str = edt( "Test", eon::string( exp ) ).load();
		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}
	TEST( EdtTest, type_long )
	{
		eon::string exp{ "9876543210L\n-9876543210L" };
		auto from_str = edt( "Test", eon::string( exp ) ).load();
		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}
	TEST( EdtTest, type_float )
	{
		eon::string exp{ "12345.6789\n-12345.6789" };
		auto from_str = edt( "Test", eon::string( exp ) ).load();
		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}
	TEST( EdtTest, type_low )
	{
		eon::string exp{ "1.1L\n-1.2L" };
		auto from_str = edt( "Test", eon::string( exp ) ).load();
		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}
	TEST( EdtTest, type_high )
	{
		eon::string exp{ "123456789.0119999945H\n-123456789.0119999945H" };
		auto from_str = edt( "Test", eon::string( exp ) ).load();
		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}
	TEST( EdtTest, type_name )
	{
		eon::string exp{ "one\n_2\n0a" };
		auto from_str = edt( "Test", eon::string( exp ) ).load();
		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}
	TEST( EdtTest, type_string )
	{
		eon::string exp{ u8"\"Hello World! β\"\n\"\"" };
		auto from_str = edt( "Test", eon::string( exp ) ).load();
		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}
	TEST( EdtTest, type_bytes )
	{
		eon::string exp{ "b\"Hello World!\"\nb\"\"" };
		auto from_str = edt( "Test", eon::string( exp ) ).load(); 
		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}
	TEST( EdtTest, type_regex )
	{
		eon::string exp{ u8"r\"\\w+\"\nr\"[^β]\"" };
		auto from_str = edt( "Test", eon::string( exp ) ).load();
		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}
	TEST( EdtTest, type_namepath )
	{
		eon::string exp{ "@one/two/three\n@one" };
		auto from_str = edt( "Test", eon::string( exp ) ).load();
		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}
	TEST( EdtTest, type_path )
	{
		eon::string exp{ "p\"one / and /two\"\np\"/a/b/c\"" };
		auto from_str = edt( "Test", eon::string( exp ) ).load();
		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}
	TEST( EdtTest, type_type_tuple )
	{
		eon::string exp{ "T(bool, byte)\na = T(((char, int), short), x=long)" };
		auto from_str = edt( "Test", eon::string( exp ) ).load();
		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}
	TEST( EdtTest, type_data_tuple )
	{
		eon::string exp{ "(one, two)\n((one, two), (three, four))" };
		auto from_str = edt( "Test", eon::string( exp ) ).load();
		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}
	TEST( EdtTest, top_level_unnamed )
	{
		eon::string exp{
			"true\nfalse\n"											// bool
			"b'y'\n"												// byte
			u8"'y'\n'β'\n"											// char
			"12345\n-12345\n"										// int
			"456S\n-456S\n"											// short
			"9876543210L\n-9876543210L\n"							// long
			"12345.6789\n-12345.6789\n"								// float
			"12.34L\n-12.34L\n"										// low
			"123456789.0119999945H\n-123456789.0119999945H\n"		// high
			"one\n_2\n"												// name
			u8"\"Hello World! β\"\n\"\"\n"							// string
			"b\"Hello World!\"\nb\"\"\n"							// bytes
			"r\"\\w+\"\n"											// regex
			"@one/two/three\n"										// namepath
			"p\"one / and /two\"\n"									// path
			"T(int, bool)\na = T(((int, bool), char), x=string)\n"	// type tuple
			"(one, two)\n((one, two), (three, four))"				// data tuple
		};
		auto from_str = edt( "Test", eon::string( exp ) ).load();

		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}
	TEST( EdtTest, deep )
	{
		eon::string exp{
			"root:\n"
			"  top:\n"
			"    high:\n"
			"      middle:\n"
			"        low:\n"
			"          bottom:\n"
			"            \"End of data!\""
		};
		auto from_str = edt( "Test", eon::string( exp ) ).load();

		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}
	TEST( EdtTest, wide )
	{
		eon::string inp{
			"\"A very long string that is longer than what is permitted for the maximum line length. Such a string shall not be broken up into smaller lines!\"\n"
			"(A, very, long, line, of, names, that, shall, be, broken, up, into, to, lines, where, the, second, line, is, indented, compared, to, the, first)\n"
		};
		eon::string exp{
			"\"A very long string that is longer than what is permitted for the maximum line length. Such a string shall not be broken up into smaller lines!\"\n"
			"(A, very, long, line, of, names, that, shall, be, broken, up, into, to, lines, where, the, second, line, is, indented,\n  compared, to, the, first)"
		};
		auto from_str = edt( "Test", eon::string( inp ) ).load();

		auto act = edt( from_str ).str();
		WANT_EQ( exp, act );
	}

	TEST( EdtTest, to_string )
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
			"(\"Third string\", byte = b'X')"
		};

		auto act = edt( dt ).str();
		WANT_EQ( act, exp );
	}

	TEST( EdtTest, from_string )
	{
		DataTuple dt( {
			type::Attribute( name( "text" ), new StringInstance( "A string", source::Ref() ) ),
			type::Attribute( name( "number" ), new FloatingptInstance<flt_t>( -9.999, source::Ref() ) ),
			type::Attribute( new IntegerInstance<int_t>( 12345, source::Ref() ) ),
			type::Attribute( name( "data" ), new DataTuple( {
				type::Attribute( new StringInstance( "Another string", source::Ref() ) ),
				type::Attribute( name( "char" ), new CharInstance( 'X', source::Ref() ) )
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

		auto tuple = edt( "Test", eon::string( exp ) ).load();
		WANT_TRUE( dt == tuple );
	}
}
