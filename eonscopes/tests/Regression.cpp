#include "Regression.h"
/*


namespace eon
{
	TEST( TupleTest, instance_bool )
	{
		tup::varscope global;
		instance _true( global, true );
		instance _false( global, false );
		REQUIRE_NO_EXCEPT( _true.boolValue() );
		REQUIRE_EXCEPT( _true.byteValue(), instance::WrongType );
		WANT_TRUE( _true.boolValue() );
		WANT_FALSE( _false.boolValue() );
	}
	TEST( TupleTest, instance_byte )
	{
		tup::varscope global;
		instance inst( global, byte_t( 'x' ) );
		REQUIRE_NO_EXCEPT( inst.byteValue() );
		REQUIRE_EXCEPT( inst.charValue(), instance::WrongType );
		WANT_EQ( 'x', inst.byteValue() );
	}
	TEST( TupleTest, instance_char )
	{
		tup::varscope global;
		instance inst( global, char_t( 'x' ) );
		REQUIRE_NO_EXCEPT( inst.charValue() );
		REQUIRE_EXCEPT( inst.byteValue(), instance::WrongType );
		WANT_EQ( 'x', inst.charValue() );
	}
	TEST( TupleTest, instance_int )
	{
		tup::varscope global;
		instance inst( global, int_t( -56 ) );
		REQUIRE_NO_EXCEPT( inst.intValue() );
		REQUIRE_EXCEPT( inst.charValue(), instance::WrongType );
		WANT_EQ( -56, inst.intValue() );
	}
	TEST( TupleTest, instance_int8 )
	{
		tup::varscope global;
		instance inst( global, int8_t( -56 ) );
		REQUIRE_NO_EXCEPT( inst.int8Value() );
		REQUIRE_EXCEPT( inst.intValue(), instance::WrongType );
		WANT_EQ( -56, inst.int8Value() );
	}
	TEST( TupleTest, instance_int16 )
	{
		tup::varscope global;
		instance inst( global, int16_t( -56 ) );
		REQUIRE_NO_EXCEPT( inst.int16Value() );
		REQUIRE_EXCEPT( inst.intValue(), instance::WrongType );
		WANT_EQ( -56, inst.int16Value() );
	}
	TEST( TupleTest, instance_int32 )
	{
		tup::varscope global;
		instance inst( global, int32_t( -56 ) );
		REQUIRE_NO_EXCEPT( inst.int32Value() );
		REQUIRE_EXCEPT( inst.int64Value(), instance::WrongType );
		WANT_EQ( -56, inst.int32Value() );
	}
	TEST( TupleTest, instance_int64 )
	{
		tup::varscope global;
		instance inst( global, int64_t( -56 ) );
		REQUIRE_NO_EXCEPT( inst.int64Value() );
		REQUIRE_EXCEPT( inst.intValue(), instance::WrongType );
		WANT_EQ( -56, inst.int64Value() );
	}
	TEST( TupleTest, instance_float )
	{
		tup::varscope global;
		instance inst( global, flt_t( -99.18 ) );
		REQUIRE_NO_EXCEPT( inst.floatValue() );
		REQUIRE_EXCEPT( inst.intValue(), instance::WrongType );
		WANT_EQ( -99.18, inst.floatValue() );
	}
	TEST( TupleTest, instance_float32 )
	{
		tup::varscope global;
		instance inst( global, flt32_t( -99.18 ) );
		REQUIRE_NO_EXCEPT( inst.float32Value() );
		REQUIRE_EXCEPT( inst.float64Value(), instance::WrongType );
		WANT_EQ( -99.18, inst.float32Value() );
	}
	TEST( TupleTest, instance_float64 )
	{
		tup::varscope global;
		instance inst( global, flt64_t( -99.18 ) );
		REQUIRE_NO_EXCEPT( inst.float64Value() );
		REQUIRE_EXCEPT( inst.float32Value(), instance::WrongType );
		WANT_EQ( -99.18, inst.float64Value() );
	}
	TEST( TupleTest, instance_float128 )
	{
		tup::varscope global;
		instance inst( global, flt128_t( -99.18 ) );
		REQUIRE_NO_EXCEPT( inst.float128Value() );
		REQUIRE_EXCEPT( inst.float64Value(), instance::WrongType );
		WANT_EQ( -99.18, inst.float128Value() );
	}
	TEST( TupleTest, instance_position )
	{
		tup::varscope global;
		instance inst( global, size_t( 36 ) );
		REQUIRE_NO_EXCEPT( inst.positionValue() );
		REQUIRE_EXCEPT( inst.float32Value(), instance::WrongType );
		WANT_EQ( 36, inst.positionValue() );
	}
	TEST( TupleTest, instance_name )
	{
		tup::varscope global;
		instance inst( global, name_bool );
		REQUIRE_NO_EXCEPT( inst.nameValue() );
		REQUIRE_EXCEPT( inst.float32Value(), instance::WrongType );
		WANT_EQ( name_bool, inst.nameValue() );
	}

	TEST( TupleTest, instance_bytes )
	{
		tup::varscope global;
		instance inst( global, new std::string( "bytes" ) );
		REQUIRE_NO_EXCEPT( inst.bytesValue() );
		REQUIRE_EXCEPT( inst.float32Value(), instance::WrongType );
		WANT_EQ( "bytes", *inst.bytesValue() );
	}
	TEST( TupleTest, instance_string)
	{
		tup::varscope global;
		instance inst( global, new string( "string" ) );
		REQUIRE_NO_EXCEPT( inst.stringValue() );
		REQUIRE_EXCEPT( inst.bytesValue(), instance::WrongType );
		WANT_EQ( "string", inst.stringValue()->stdstr() );
	}
	TEST( TupleTest, instance_regex )
	{
		tup::varscope global;
		instance inst( global, new regex( R"(/\w+/i)" ) );
		REQUIRE_NO_EXCEPT( inst.regexValue() );
		REQUIRE_EXCEPT( inst.bytesValue(), instance::WrongType );
		WANT_TRUE( inst.regexValue()->match( "alpha" ) );
		WANT_FALSE( inst.regexValue()->match( ".alpha" ) );
	}

	TEST( TupleTest, instance_path )
	{
		tup::varscope global;
		instance inst( global, new path( "/one\\two" ) );
		REQUIRE_NO_EXCEPT( inst.pathValue() );
		REQUIRE_EXCEPT( inst.stringValue(), instance::WrongType );
		WANT_EQ( "/one/two", inst.pathValue()->stdstr() );
	}
	
	TEST( TupleTest, instance_array )
	{
		tup::varscope global;
		instance inst( global, new std::vector<instance>{
			instance( global, name_bool ),
			instance( global, int_t( -3333 ) ) } );
		REQUIRE_NO_EXCEPT( inst.arrayValue() );
		REQUIRE_EXCEPT( inst.float32Value(), instance::WrongType );
		REQUIRE_EQ( 2, inst.arrayValue()->size() );
		WANT_EQ( name_bool, inst.arrayValue()->at( 0 ).nameValue() );
		WANT_EQ( -3333, inst.arrayValue()->at( 1 ).intValue() );
	}
	TEST( TupleTest, instance_list )
	{
		tup::varscope global;
		instance inst( global, new std::list<instance>{
			instance( global, name_bool ),
			instance( global, int_t( -3333 ) ) } );
		REQUIRE_NO_EXCEPT( inst.listValue() );
		REQUIRE_EXCEPT( inst.arrayValue(), instance::WrongType );
		REQUIRE_EQ( 2, inst.listValue()->size() );
		WANT_EQ( name_bool, inst.listValue()->begin()->nameValue() );
		WANT_EQ( -3333, inst.listValue()->rbegin()->intValue() );
	}

	TEST( TupleTest, instance_tuple )
	{
		auto name_sub = name::get( "sub" );
		tup::varscope global;
		instance inst{ global, new eon::tuple{ global, {
			tuple::attribute( new instance( global, true ) ),
			tuple::attribute( name_byte, new instance( global, byte_t( 'x' ) ) ),
			tuple::attribute( name_int, new instance( global, int_t( -56 ) ) ),
			tuple::attribute( new instance( global, -3.16 ) ),
			tuple::attribute( name_sub, new instance( global, new eon::tuple{ global, {
				tuple::attribute( new instance( global, name_binary ) )
			} } ) )
		} } };
		REQUIRE_NO_EXCEPT( inst.tupleValue() );
		REQUIRE_EXCEPT( inst.float32Value(), instance::WrongType );
		REQUIRE_EQ( 5, inst.tupleValue()->numAttributes() );
		WANT_TRUE( inst.tupleValue()->at( 0 ).boolValue() );
		WANT_EQ( 'x', inst.tupleValue()->at( 1 ).byteValue() );
		WANT_EQ( -56, inst.tupleValue()->at( 2 ).intValue() );
		WANT_EQ( -3.16, inst.tupleValue()->at( 3 ).floatValue() );
		REQUIRE_TRUE( inst.tupleValue()->at( 4 ).type()->isTuple() );
		REQUIRE_EQ( 1, inst.tupleValue()->at( 4 ).tupleValue()->numAttributes() );
		WANT_EQ( name_binary, inst.tupleValue()->at( 4 ).tupleValue()->at( 0 ).nameValue() );
		WANT_EQ( 1, inst.tupleValue()->type()->pos( name_byte ) );
		WANT_EQ( 2, inst.tupleValue()->type()->pos( name_int ) );
		WANT_EQ( 4, inst.tupleValue()->type()->pos( name_sub ) );
	}

	TEST( TupleTest, instance_reference )
	{
		tup::varscope global;
		auto target = new instance( global, name_bool );
		target->addRef();
		instance inst( global, target );
		REQUIRE_NO_EXCEPT( inst.referenceValue() );
		REQUIRE_EXCEPT( inst.float32Value(), instance::WrongType );
		WANT_EQ( name_bool, inst.referenceValue()->nameValue() );
		target->delRef();
	}
}
*/