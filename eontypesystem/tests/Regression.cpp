#include "Regression.h"



namespace eon
{
	TEST( TypeTupleTest, basic )
	{
		TypeTuple a{ name_bool };
		auto b = typetuple::newStatic( { { name( "on" ), name_bool } } );
		auto c = typetuple::newStatic( {
			{ nn, typetuple::newStatic( { name_and, name_or } ) },
			{ name_tuple, typetuple::newStatic( {
				{ name_action, name_float },
				{ nn, TypeTuple( name_int ) } } ) } } );
		WANT_EQ( "T(bool)", a.str() );
		WANT_EQ( "T(tuple, type=static, value=(on=bool))", b.str() );
		WANT_EQ(
			"T(tuple, type=static, value=((tuple, type=static, value=(and, or)), tuple=(tuple, type=static, "
			"value=(action=float, int))))", c.str() );
	}
	TEST( TypeTupleTest, compare )
	{
		TypeTuple t1( name_bool ), t2( name_bool ), t3( name_char );

		WANT_TRUE( t1 == t2 );
		WANT_TRUE( t2 == t1 );
		WANT_FALSE( t1 == t3 );
		WANT_TRUE( t1 == name_bool );
		WANT_TRUE( name_bool == t1 );
	}
	TEST( TypeTupleTest, compatible )
	{
		TypeTuple t1{ name_bool };
		TypeTuple t2{ name_bool };
		auto t3 = typetuple::newStatic( {
			{ nn, name_bool },
			{ nn, name_int },
			{ nn, name( "one" ) },
			{ name_name, name( "three" ) },
			{ nn, name_string } } );
		auto t4 = typetuple::newStatic( {
			{ name_name, name( "three" ) },
			{ nn, name_int } } );

		WANT_TRUE( t1.compatibleWith( t2 ) );
		WANT_TRUE( t2.compatibleWith( t1 ) );
		WANT_TRUE( t1.compatibleWith( t3 ) );
		WANT_FALSE( t3.compatibleWith( t1 ) );
		WANT_TRUE( t4.compatibleWith( t3 ) );
		WANT_FALSE( t3.compatibleWith( t4 ) );
	}



	TEST( TupleTest, static_unnamed )
	{
		string chars{ u8"bÆ" };
		auto tup = tuple::newStatic( {
			{ false }, { 'b' }, { *chars.begin() }, { *chars.last() },
			{ static_cast<int_t>( -54 ) }, { static_cast<short_t>( 11 ) }, { static_cast<long_t>( 987654321 ) },
			{ static_cast<flt_t>( -11.2 ) }, { static_cast<low_t>( 11.1 ) }, { static_cast<high_t>( 999.88 ) },
			{ static_cast<index_t>( 888888888 ) },
			{ Attribute( name_action ) },
			{ std::string( "bytes" ) }, { string( chars ) } } );

		REQUIRE_EXCEPT( tup.add( true ), eon::type::AccessDenied );

		auto exp_type = typetuple::newStatic( {
			name_bool, name_byte, name_char, name_char,
			name_int, name_short, name_long,
			name_float, name_low, name_high,
			name_index,
			name_name,
			name_bytes, name_string } );

		WANT_EQ( exp_type.str(), tup.type().str() );
		REQUIRE_EQ( 14, tup.numAttributes() );
		WANT_FALSE( tup.read<bool>( 0 ) );
		WANT_EQ( 'b', ( tup.read<byte_t, type::hint::byte>( 1 ) ) );
		WANT_EQ( *chars.begin(), tup.read<char_t>( 2 ) );
		WANT_EQ( *chars.last(), tup.read<char_t>( 3 ) );
		WANT_EQ( -54, tup.read<int_t>( 4 ) );
		WANT_EQ( 11, tup.read<short_t>( 5 ) );
		WANT_EQ( 987654321, tup.read<long_t>( 6 ) );
		WANT_EQ( -11.2, tup.read<flt_t>( 7 ) );
		WANT_EQ( 11.1, tup.read<low_t>( 8 ) );
		WANT_EQ( 999.88, tup.read<high_t>( 9 ) );
		WANT_EQ( 888888888, ( tup.read<index_t, type::hint::index>( 10 ) ) );
		WANT_EQ( name_action, ( tup.read<name_t>( 11 ) ) );
		WANT_EQ( "bytes", tup.read<std::string>( 12 ) );
		WANT_EQ( chars, tup.read<string>( 13 ) );
	}
	TEST( TupleTest, static_named )
	{
		string chars{ u8"bÆ" };
		auto tup = tuple::newStatic( {
			{ name_bool, false }, { name_byte, 'b' }, { name_char, *chars.begin() },
			{ name_int, static_cast<int_t>( -54 ) },
			{ name_short, static_cast<short_t>( 11 ) }, { name_long, static_cast<long_t>( 987654321 ) },
			{ name_float, static_cast<flt_t>( -11.2 ) },
			{ name_low, static_cast<low_t>( 11.1 ) }, { name_high, static_cast<high_t>( 999.88 ) },
			{ name_index, static_cast<index_t>( 888888888 ) },
			{ name_name, Attribute( name_action ) },
			{ name_bytes, std::string( "bytes" ) }, { name_string, string( chars ) } } );

		auto exp_type = typetuple::newStatic( {
			{ name_bool, name_bool },
			{ name_byte, name_byte },
			{ name_char, name_char },
			{ name_int, name_int },
			{ name_short, name_short },
			{ name_long, name_long },
			{ name_float, name_float },
			{ name_low, name_low },
			{ name_high, name_high },
			{ name_index, name_index },
			{ name_name, name_name },
			{ name_bytes, name_bytes },
			{ name_string, name_string } } );

		WANT_EQ( exp_type.str(), tup.type().str() );
		REQUIRE_EQ( 13, tup.numAttributes() );
		WANT_FALSE( tup.read<bool>( name_bool ) );
		WANT_EQ( 'b', tup.read<byte_t>( name_byte ) );
		WANT_EQ( *chars.begin(), tup.read<char_t>( name_char ) );
		WANT_EQ( -54, tup.read<int_t>( name_int ) );
		WANT_EQ( 11, tup.read<short_t>( name_short ) );
		WANT_EQ( 987654321, tup.read<long_t>( name_long ) );
		WANT_EQ( -11.2, tup.read<flt_t>( name_float ) );
		WANT_EQ( 11.1, tup.read<low_t>( name_low ) );
		WANT_EQ( 999.88, tup.read<high_t>( name_high ) );
		WANT_EQ( 888888888, tup.read<index_t>( name_index ) );
		WANT_EQ( name_action, ( tup.read<name_t>( name_name ) ) );
		WANT_EQ( "bytes", tup.read<std::string>( name_bytes ) );
		WANT_EQ( chars, tup.read<string>( name_string ) );
	}
	TEST( TupleTest, static_str_1 )
	{
		auto tup = tuple::newStatic( { { Attribute( name_and ) },
			{ Attribute( name_or ) } } );
		auto act = tup.str();
		string exp{ "static(and, or)" };
		WANT_EQ( exp, act );
	}
	TEST( TupleTest, static_str_2 )
	{
		auto tup = tuple::newStatic( { { name( "one" ), Attribute( name_and ) },
			{ name("two" ), Attribute( name_or ) } } );
		auto act = tup.str();
		string exp{ "static(one=and, two=or)" };
		WANT_EQ( exp, act );
	}
	TEST( TupleTest, static_str_3 )
	{
		auto tup = tuple::newStatic( {
			{ tuple::newStatic( { { static_cast<byte_t>( 'a' ) }, { string( "string" ) } } ) },
			{ name( "second" ), tuple::newStatic( {
				{ tuple::newStatic( { { Attribute( name_and ) },
					{ Attribute( name_or ) } } ) },
				{ name( "another" ), tuple::newStatic( { { Attribute( name_action ) },
					{ Attribute( name_at ) } } ) } } ) } } );
		auto act = tup.str();
		string exp{ "static((B'a', \"string\"), second=((and, or), another=(action, at)))" };
		//string exp{
		//	"static(\n"
		//	"  (B'a', \"string\"),\n"
		//	"  second:\n"
		//	"    (and, or),\n"
		//	"    another:\n"
		//	"      action, at)"
		//};
		WANT_EQ( exp, act );
	}
	TEST( TupleTest, static_compare )
	{
		auto tup = tuple::newStatic( {
			{ tuple::newStatic( {
				{ static_cast<byte_t>( 'a' ) }, { string( "string" ) } } ) },
			{ name( "second" ), tuple::newStatic( {
				{ tuple::newStatic( {
					{ Attribute( name_and ) },
					{ Attribute( name_or ) } } ) },
				{ name( "another" ), tuple::newStatic( {
					{ Attribute( name_action ) },
					{ Attribute( name_at ) } } ) } } ) } } );
		auto act = tup.str();
		string exp{ "static((B'a', \"string\"), second=((and, or), another=(action, at)))" };
		//string exp{
		//	"static(\n"
		//	"  (B'a', \"string\"),\n"
		//	"  second:\n"
		//	"    (and, or),\n"
		//	"    another:\n"
		//	"      action, at)"
		//};
		WANT_EQ( exp, act );
	}
	TEST( TupleTest, data_str )
	{
		auto txt1 = tuple::newData( { { name_type, name( "text" ) }, { name( "value" ), string( "First" ) } } );
		auto txt2 = tuple::newData( { { name_type, name( "text" ) }, { name( "value" ), string( "Second" ) } } );
		auto grp1 = tuple::newData( { { name_type, name( "text" ) } } );
		grp1.addTuple( name( "group1"), txt1 );
		auto grp2 = tuple::newData( { { name_type, name( "text" ) } } );
		grp2.addTuple( txt2 );
		auto tup = tuple::newData();
		tup.addTuple( grp1 ).addTuple( grp2 );
		Stringifier strf;
//		strf.hardLineWidth( 80 );
		tup.str( strf );
		auto act = strf.generateString();
		string exp{ "data((type=text, group1=(type=text, value=\"First\")), (type=text, (type=text, value=\"Second\")))" };
		//string exp{
		//	"data(\n"
		//	"  (type=text,\n"
		//	"    group1:\n"
		//	"      type=text, value=\"First\"),\n"
		//	"  (type=text,\n"
		//	"    (type=text, value=\"Second\")))"
		//};
		WANT_EQ( exp, act );
	}
}
