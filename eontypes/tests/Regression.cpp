#include "Regression.h"
#include <eontypes/Bool.h>
#include <eontypes/Byte.h>
#include <eontypes/Char.h>
#include <eontypes/Int.h>
#include <eontypes/Short.h>
#include <eontypes/Long.h>
#include <eontypes/Float.h>
#include <eontypes/Low.h>
#include <eontypes/High.h>
#include <eontypes/Index.h>
#include <eontypes/Name.h>
#include <eontypes/Handle.h>
#include <eontypes/Bytes.h>
#include <eontypes/String.h>
#include <eontypes/Regex.h>
#include <eontypes/Path.h>
#include <eontypes/NamePath.h>
#include <eontypes/DynamicTuple.h>
#include <eontypes/DataTuple.h>
#include <eontypes/MetaData.h>



namespace eon
{
	TEST( BoolTest, instance )
	{
		using tt = BoolType;
		using ti = BoolInstance;

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_FALSE( i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( false ) ) ) );
		WANT_FALSE( i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( true ) ) ) );
		WANT_TRUE( i->value() );
	}

	TEST( ByteTest, instance )
	{
		using tt = ByteType;
		using ti = ByteInstance;

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_EQ( '\0', i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( '\0' ) ) ) );
		WANT_EQ( '\0', i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( 'x' ) ) ) );
		WANT_EQ( 'x', i->value() );
	}

	TEST( CharTest, instance )
	{
		using tt = CharType;
		using ti = CharInstance;

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_EQ( '\0', i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( 'x' ) ) ) );
		WANT_EQ( 'x', i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( 0x1F60A ) ) ) );
		WANT_EQ( 0x1F60A, i->value() );
	}

	TEST( IntTest, instance )
	{
		using tt = IntType;
		using ti = IntInstance;

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_EQ( 0, i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( 0 ) ) ) );
		WANT_EQ( 0, i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( -56 ) ) ) );
		WANT_EQ( -56, i->value() );
	}

	TEST( ShortTest, instance )
	{
		using tt = ShortType;
		using ti = ShortInstance;

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_EQ( 0, i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( 0 ) ) ) );
		WANT_EQ( 0, i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( -56 ) ) ) );
		WANT_EQ( -56, i->value() );
	}

	TEST( LongTest, instance )
	{
		using tt = LongType;
		using ti = LongInstance;

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_EQ( 0, i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( 0 ) ) ) );
		WANT_EQ( 0, i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( -56 ) ) ) );
		WANT_EQ( -56, i->value() );
	}

	TEST( FloatTest, instance )
	{
		using tt = FloatType;
		using ti = FloatInstance;

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_EQ( 0, i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( 0 ) ) ) );
		WANT_EQ( 0, i->value() );

		flt_t val = static_cast<flt_t>( -99.18 );
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( val ) ) ) );
		WANT_EQ( val, i->value() );
	}

	TEST( LowTest, instance )
	{
		using tt = LowType;
		using ti = LowInstance;

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_EQ( 0, i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( 0 ) ) ) );
		WANT_EQ( 0, i->value() );

		flt_t val = static_cast<flt_t>( -99.18 );
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( val ) ) ) );
		WANT_EQ( val, i->value() );
	}

	TEST( HighTest, instance )
	{
		using tt = HighType;
		using ti = HighInstance;

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_EQ( 0, i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( 0 ) ) ) );
		WANT_EQ( 0, i->value() );

		flt_t val = static_cast<flt_t>( -99.18 );
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( val ) ) ) );
		WANT_EQ( val, i->value() );
	}

	TEST( IndexTest, instance )
	{
		using tt = IndexType;
		using ti = IndexInstance;

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_EQ( 0, i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( 0 ) ) ) );
		WANT_EQ( 0, i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( 34 ) ) ) );
		WANT_EQ( 34, i->value() );
	}

	TEST( NameTest, instance )
	{
		using tt = NameType;
		using ti = NameInstance;

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_EQ( no_name, i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( no_name ) ) ) );
		WANT_EQ( no_name, i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( name_bool ) ) ) );
		WANT_EQ( name_bool, i->value() );
	}

	TEST( HandleTest, instance )
	{
		using tt = HandleType;
		using ti = HandleInstance;

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_EQ( no_handle, i->value() );

		handle_t h1 = no_handle, h2 = (handle_t)199;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( h1 ) ) ) );
		WANT_EQ( h1, i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( h2 ) ) ) );
		WANT_EQ( h2, i->value() );
	}

	TEST( BytesTest, instance )
	{
		using tt = BytesType;
		using ti = BytesInstance;

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_EQ( "", i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( "" ) ) ) );
		WANT_EQ( "", i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( "hello" ) ) ) );
		WANT_EQ( "hello", i->value() );
	}

	TEST( StringTest, instance )
	{
		using tt = StringType;
		using ti = StringInstance;

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_EQ( "", i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( "" ) ) ) );
		WANT_EQ( "", i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( "hello" ) ) ) );
		WANT_EQ( "hello", i->value() );
	}

	TEST( RegexTest, instance )
	{
		using tt = RegexType;
		using ti = RegexInstance;

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_EQ( "", i->value().str() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( "r\"\\w+\"" ) ) ) );
		WANT_EQ( "r\"\\w+\"", i->value().str() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( "r\"\\w+\"i" ) ) ) );
		WANT_EQ( "r\"\\w+\"i", i->value().str() );
	}

	TEST( PathTest, instance )
	{
		using tt = PathType;
		using ti = PathInstance;

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_EQ( "", i->value().str() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( "C:\\Program Files\\eon" ) ) ) );
		WANT_EQ( "C:/Program Files/eon", i->value().str() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( "/usr/bin/eon" ) ) ) );
		WANT_EQ( "/usr/bin/eon", i->value().str() );
	}

	TEST( NamePathTest, instance )
	{
		using tt = NamePathType;
		using ti = NamePathInstance;

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_EQ( "@", i->value().str() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( nameref( "" ) ) ) ) );
		WANT_EQ( "@", i->value().str() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( nameref( "@one/two/three" ) ) ) ) );
		WANT_EQ( "@one/two/three", i->value().str() );
	}

	TEST( TupleTest, plain )
	{
		type::Handler::init();
		Tuple tup;
		tup += false;
		tup += static_cast<int_t>( -54 );
		tup += name_active;
		tup += "I am string!";
		tup.finalize();
		REQUIRE_EXCEPT( tup += 3.14, type::AccessDenied );

		TypeTuple exp_type( { name_plain, name_bool, name_int, name_name, name_string } );
		WANT_EQ( exp_type.asName(), tup.type().asName() );

		REQUIRE_EQ( 4, tup.numAttributes() );
		REQUIRE_EQ( name_bool, tup[ 0 ].type().asName() );
		REQUIRE_EQ( name_int, tup[ 1 ].type().asName() );
		REQUIRE_EQ( name_name, tup[ 2 ].type().asName() );
		REQUIRE_EQ( name_string, tup[ 3 ].type().asName() );
		WANT_FALSE( tup.value<bool>( 0 ) );
		WANT_EQ( -54, tup.value<int_t>( 1 ) );
		WANT_EQ( name_active, tup.value<name_t>( 2 ) );
		WANT_EQ( "I am string!", tup.value<string>( 3 ) );
	}
	TEST( TupleTest, plain_named )
	{
		type::Handler::init();
		Tuple tup;
		tup += true;
		tup.add( name::get( "two" ), static_cast<int_t>( -54 ) );
		tup += name_active;
		tup.add( name::get( "four" ), string( "I am string!" ) );
		tup.finalize();

		REQUIRE_EQ( 4, tup.numAttributes() );
		REQUIRE_EQ( name_bool, tup[ 0 ].type().asName() );
		REQUIRE_EQ( name_int, tup[ 1 ].type().asName() );
		REQUIRE_EQ( name_name, tup[ 2 ].type().asName() );
		REQUIRE_EQ( name_string, tup[ 3 ].type().asName() );
		WANT_TRUE( tup.value<bool>( 0 ) );
		WANT_EQ( -54, tup.value<int_t>( name::get( "two" ) ) );
		WANT_EQ( name_active, tup.value<name_t>( 2 ) );
		WANT_EQ( "I am string!", tup.value<string>( name::get( "four" ) ) );
	}
	TEST( TupleTest, plain_deep )
	{
		type::Handler::init();
		Tuple tup;
		tup += "unnamed";
		tup.add( name::get( "named" ), "Name" );
		auto child = tup.addPlainTuple( name::get( "deep" ) );
		auto grandchild = child->addPlainTuple( name::get( "deeper" ) );
		*grandchild += "Rock";
		*grandchild += "bottom";
		grandchild->finalize();
		child->finalize();
		tup.finalize();

		type::Stringifier str;
		REQUIRE_NO_EXCEPT( tup.str( str ) );
		WANT_EQ( "\"unnamed\", named=\"Name\", deep=(deeper=(\"Rock\", \"bottom\"))", str.output() );
	}
	TEST( TupleTest, plain_compare )
	{
		type::Handler::init();
		Tuple base, eq, ne, lt, gt;

		base.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		base += name_active;
		base.add( name::get( "three" ), string( "Str" ) );
		base.finalize();

		eq.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		eq += name_active;
		eq.add( name::get( "three" ), string( "Str" ) );
		eq.finalize();

		ne.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		ne += name_active;
		ne.add( name::get( "two" ), string( "Str" ) );	// "two" vs. "three"
		ne.finalize();

		lt.add( name::get( "one" ), static_cast<int_t>( -55 ) );	// -55 vs. -54
		lt += name_active;
		lt.add( name::get( "three" ), string( "Str" ) );
		lt.finalize();

		gt.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		gt += name_active;
		gt.add( name::get( "three" ), string( "str" ) );	// "str" vs. "Str"
		gt.finalize();

		WANT_FALSE( base < eq );
		WANT_TRUE( base <= eq );
		WANT_FALSE( base > eq );
		WANT_TRUE( base >= eq );
		WANT_TRUE( base == eq );
		WANT_FALSE( base != eq );

		// NOTE: We cannot test relational differences on 'ne' since only a name is different
		WANT_FALSE( base == ne );
		WANT_TRUE( base != ne );

		WANT_FALSE( base < lt );
		WANT_FALSE( base <= lt );
		WANT_TRUE( base > lt );
		WANT_TRUE( base >= lt );
		WANT_FALSE( base == lt );
		WANT_TRUE( base != lt );

		WANT_TRUE( base < gt );
		WANT_TRUE( base <= gt );
		WANT_FALSE( base > gt );
		WANT_FALSE( base >= gt );
		WANT_FALSE( base == gt );
		WANT_TRUE( base != gt );
	}
	TEST( TupleTest, plain_compatible )
	{
		type::Handler::init();
		Tuple base, c1, c2, n1, n2;

		base.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		base += name_active;
		base.add( name::get( "three" ), string( "Str" ) );
		base.finalize();

		// Names in different places
		c1.add( name::get( "three" ), string( "Str" ) );
		c1 += name_active;
		c1.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		c1.finalize();

		// Unnamed, incomplete
		c2 += static_cast<int_t>( -54 );
		c2 += name_active;
		c2.finalize();

		// Wrong type
		n1.add( name::get( "one" ), string( "Str" ) );
		n1 += name_active;
		n1.add( name::get( "three" ), static_cast<int_t>( -55 ) );
		n1.finalize();

		// Too many attributes
		n2.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		n2 += name_active;
		n2.add( name::get( "three" ), string( "str" ) );	// "str" vs. "Str"
		n2 += std::string( "bytes" );
		n2.finalize();

		WANT_TRUE( c1.type().compatibleWith( base.type() ) );
		WANT_TRUE( c2.type().compatibleWith( base.type() ) );
		WANT_FALSE( n1.type().compatibleWith( base.type() ) );
		WANT_FALSE( n2.type().compatibleWith( base.type() ) );
	}

	TEST( TupleTest, dynamic )
	{
		type::Handler::init();
		DynamicTuple tup;
		tup += false;
		tup += static_cast<int_t>( -54 );
		tup += name_active;
		tup += string( "I am string!" );
		tup.finalize();
		REQUIRE_NO_EXCEPT( tup += 3.14 );

		TypeTuple exp_type( { name_dynamic, name_bool, name_int, name_name, name_string, name_float } );
		WANT_EQ( exp_type.asName(), tup.type().asName() );

		REQUIRE_EQ( 5, tup.numAttributes() );
		REQUIRE_EQ( name_bool, tup[ 0 ].type().asName() );
		REQUIRE_EQ( name_int, tup[ 1 ].type().asName() );
		REQUIRE_EQ( name_name, tup[ 2 ].type().asName() );
		REQUIRE_EQ( name_string, tup[ 3 ].type().asName() );
		REQUIRE_EQ( name_float, tup[ 4 ].type().asName() );
		WANT_FALSE( tup.value<bool>( 0 ) );
		WANT_EQ( -54, tup.value<int_t>( 1 ) );
		WANT_EQ( name_active, tup.value<name_t>( 2 ) );
		WANT_EQ( "I am string!", tup.value<string>( 3 ) );
		WANT_EQ( 3.14, tup.value<flt_t>( 4 ) );
	}
	TEST( TupleTest, dynamic_named )
	{
		type::Handler::init();
		DynamicTuple tup;
		tup += true;
		tup.add( name::get( "two" ), static_cast<int_t>( -54 ) );
		tup += name_active;
		tup.add( name::get( "four" ), string( "I am string!" ) );
		tup.finalize();

		REQUIRE_EQ( 4, tup.numAttributes() );
		REQUIRE_EQ( name_bool, tup[ 0 ].type().asName() );
		REQUIRE_EQ( name_int, tup[ 1 ].type().asName() );
		REQUIRE_EQ( name_name, tup[ 2 ].type().asName() );
		REQUIRE_EQ( name_string, tup[ 3 ].type().asName() );
		WANT_TRUE( tup.value<bool>( 0 ) );
		WANT_EQ( -54, tup.value<int_t>( name::get( "two" ) ) );
		WANT_EQ( name_active, tup.value<name_t>( 2 ) );
		WANT_EQ( "I am string!", tup.value<string>( name::get( "four" ) ) );
	}
	TEST( TupleTest, dynamic_deep )
	{
		type::Handler::init();
		DynamicTuple tup;
		tup += "unnamed";
		tup.add( name::get( "named" ), "Name" );
		auto child = tup.addPlainTuple( name::get( "deep" ) );
		auto grandchild = child->addDynamicTuple( name::get( "deeper" ) );
		*grandchild += "Rock";
		*grandchild += "bottom";
		grandchild->finalize();
		child->finalize();
		tup.finalize();

		type::Stringifier str;
		REQUIRE_NO_EXCEPT( tup.str( str ) );
		WANT_EQ( "\"unnamed\", named=\"Name\", deep=(deeper=(\"Rock\", \"bottom\"))", str.output() );
	}
	TEST( TupleTest, dynamic_compare )
	{
		type::Handler::init();
		DynamicTuple base, eq, ne, lt, gt;

		base.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		base += name_active;
		base.add( name::get( "three" ), string( "Str" ) );
		base.finalize();

		eq.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		eq += name_active;
		eq.add( name::get( "three" ), string( "Str" ) );
		eq.finalize();

		ne.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		ne += name_active;
		ne.add( name::get( "two" ), string( "Str" ) );	// "two" vs. "three"
		ne.finalize();

		lt.add( name::get( "one" ), static_cast<int_t>( -55 ) );	// -55 vs. -54
		lt += name_active;
		lt.add( name::get( "three" ), string( "Str" ) );
		lt.finalize();

		gt.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		gt += name_active;
		gt.add( name::get( "three" ), string( "str" ) );	// "str" vs. "Str"
		gt.finalize();

		WANT_FALSE( base < eq );
		WANT_TRUE( base <= eq );
		WANT_FALSE( base > eq );
		WANT_TRUE( base >= eq );
		WANT_TRUE( base == eq );
		WANT_FALSE( base != eq );

		// NOTE: We cannot test relational differences on 'ne' since only a name is different
		WANT_FALSE( base == ne );
		WANT_TRUE( base != ne );

		WANT_FALSE( base < lt );
		WANT_FALSE( base <= lt );
		WANT_TRUE( base > lt );
		WANT_TRUE( base >= lt );
		WANT_FALSE( base == lt );
		WANT_TRUE( base != lt );

		WANT_TRUE( base < gt );
		WANT_TRUE( base <= gt );
		WANT_FALSE( base > gt );
		WANT_FALSE( base >= gt );
		WANT_FALSE( base == gt );
		WANT_TRUE( base != gt );
	}
	TEST( TupleTest, dynamic_compatible )
	{
		type::Handler::init();
		DynamicTuple base, c1, c2, n1, n2;

		base.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		base += name_active;
		base.add( name::get( "three" ), string( "Str" ) );
		base.finalize();

		// Names in different places
		c1.add( name::get( "three" ), string( "Str" ) );
		c1 += name_active;
		c1.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		c1.finalize();

		// Unnamed, incomplete
		c2 += static_cast<int_t>( -54 );
		c2 += name_active;
		c2.finalize();

		// Wrong type
		n1.add( name::get( "one" ), string( "Str" ) );
		n1 += name_active;
		n1.add( name::get( "three" ), static_cast<int_t>( -55 ) );
		n1.finalize();

		// Too many attributes
		n2.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		n2 += name_active;
		n2.add( name::get( "three" ), string( "str" ) );	// "str" vs. "Str"
		n2 += std::string( "bytes" );
		n2.finalize();

		WANT_TRUE( c1.type().compatibleWith( base.type() ) );
		WANT_TRUE( c2.type().compatibleWith( base.type() ) );
		WANT_FALSE( n1.type().compatibleWith( base.type() ) );
		WANT_FALSE( n2.type().compatibleWith( base.type() ) );
	}

	TEST( TupleTest, data )
	{
		type::Handler::init();
		DataTuple tup;
		tup += false;
		tup += static_cast<int_t>( -54 );
		tup += name_active;
		tup += string( "I am string!" );
		tup.finalize();
		REQUIRE_NO_EXCEPT( tup += 3.14 );

		TypeTuple exp_type( { name_data, name_bool, name_long, name_name, name_string, name_high } );
		WANT_EQ( exp_type.asName(), tup.type().asName() );

		REQUIRE_EQ( 5, tup.numAttributes() );
		REQUIRE_EQ( name_bool, tup[ 0 ].type().asName() );
		REQUIRE_EQ( name_long, tup[ 1 ].type().asName() );
		REQUIRE_EQ( name_name, tup[ 2 ].type().asName() );
		REQUIRE_EQ( name_string, tup[ 3 ].type().asName() );
		REQUIRE_EQ( name_high, tup[ 4 ].type().asName() );
		WANT_FALSE( tup.value<bool>( 0 ) );
		WANT_EQ( -54, tup.value<long_t>( 1 ) );
		WANT_EQ( name_active, tup.value<name_t>( 2 ) );
		WANT_EQ( "I am string!", tup.value<string>( 3 ) );
		WANT_EQ( 3.14, tup.value<high_t>( 4 ) );
	}
	TEST( TupleTest, data_named )
	{
		type::Handler::init();
		DataTuple tup;
		tup += true;
		tup.add( name::get( "two" ), static_cast<int_t>( -54 ) );
		tup += name_active;
		tup.add( name::get( "four" ), string( "I am string!" ) );
		tup.finalize();

		REQUIRE_EQ( 4, tup.numAttributes() );
		REQUIRE_EQ( name_bool, tup[ 0 ].type().asName() );
		REQUIRE_EQ( name_long, tup[ 1 ].type().asName() );
		REQUIRE_EQ( name_name, tup[ 2 ].type().asName() );
		REQUIRE_EQ( name_string, tup[ 3 ].type().asName() );
		WANT_TRUE( tup.value<bool>( 0 ) );
		WANT_EQ( -54, tup.value<long_t>( name::get( "two" ) ) );
		WANT_EQ( name_active, tup.value<name_t>( 2 ) );
		WANT_EQ( "I am string!", tup.value<string>( name::get( "four" ) ) );
	}
	TEST( TupleTest, data_deep )
	{
		type::Handler::init();
		DataTuple tup;
		tup += "unnamed";
		tup.add( name::get( "named" ), "Name" );
		REQUIRE_EXCEPT( tup.addPlainTuple( name::get( "deep" ) ), type::AccessDenied );
		auto child = tup.addDataTuple( name::get( "deep" ) );
		auto grandchild = child->addDataTuple( name::get( "deeper" ) );
		*grandchild += "Rock";
		*grandchild += "bottom";
		grandchild->finalize();
		child->finalize();
		tup.finalize();

		type::Stringifier str;
		REQUIRE_NO_EXCEPT( tup.str( str ) );
		WANT_EQ( "\"unnamed\", named=\"Name\", deep:\n  deeper:\n    \"Rock\", \"bottom\"", str.output() );
	}
	TEST( TupleTest, data_compare )
	{
		type::Handler::init();
		DataTuple base, eq, ne, lt, gt;

		base.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		base += name_active;
		base.add( name::get( "three" ), string( "Str" ) );
		base.finalize();

		eq.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		eq += name_active;
		eq.add( name::get( "three" ), string( "Str" ) );
		eq.finalize();

		ne.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		ne += name_active;
		ne.add( name::get( "two" ), string( "Str" ) );	// "two" vs. "three"
		ne.finalize();

		lt.add( name::get( "one" ), static_cast<int_t>( -55 ) );	// -55 vs. -54
		lt += name_active;
		lt.add( name::get( "three" ), string( "Str" ) );
		lt.finalize();

		gt.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		gt += name_active;
		gt.add( name::get( "three" ), string( "str" ) );	// "str" vs. "Str"
		gt.finalize();

		WANT_FALSE( base < eq );
		WANT_TRUE( base <= eq );
		WANT_FALSE( base > eq );
		WANT_TRUE( base >= eq );
		WANT_TRUE( base == eq );
		WANT_FALSE( base != eq );

		// NOTE: We cannot test relational differences on 'ne' since only a name is different
		WANT_FALSE( base == ne );
		WANT_TRUE( base != ne );

		WANT_FALSE( base < lt );
		WANT_FALSE( base <= lt );
		WANT_TRUE( base > lt );
		WANT_TRUE( base >= lt );
		WANT_FALSE( base == lt );
		WANT_TRUE( base != lt );

		WANT_TRUE( base < gt );
		WANT_TRUE( base <= gt );
		WANT_FALSE( base > gt );
		WANT_FALSE( base >= gt );
		WANT_FALSE( base == gt );
		WANT_TRUE( base != gt );
	}
	TEST( TupleTest, data_compatible )
	{
		type::Handler::init();
		DataTuple base, c1, c2, n1, n2;

		base.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		base += name_active;
		base.add( name::get( "three" ), string( "Str" ) );
		base.finalize();

		// Names in different places
		c1.add( name::get( "three" ), string( "Str" ) );
		c1 += name_active;
		c1.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		c1.finalize();

		// Unnamed, incomplete
		c2 += static_cast<int_t>( -54 );
		c2 += name_active;
		c2.finalize();

		// Wrong type
		n1.add( name::get( "one" ), string( "Str" ) );
		n1 += name_active;
		n1.add( name::get( "three" ), static_cast<int_t>( -55 ) );
		n1.finalize();

		// Too many attributes
		n2.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		n2 += name_active;
		n2.add( name::get( "three" ), string( "str" ) );	// "str" vs. "Str"
		n2 += std::string( "bytes" );
		n2.finalize();

		WANT_TRUE( c1.type().compatibleWith( base.type() ) );
		WANT_TRUE( c2.type().compatibleWith( base.type() ) );
		WANT_FALSE( n1.type().compatibleWith( base.type() ) );
		WANT_FALSE( n2.type().compatibleWith( base.type() ) );
	}

	TEST( TupleTest, meta )
	{
		type::Handler::init();
		MetaData tup;
		tup += false;
		tup += static_cast<int_t>( -54 );
		tup += name_active;
		tup += string( "I am string!" );
		tup.finalize();
		REQUIRE_NO_EXCEPT( tup += 3.14 );

		TypeTuple exp_type( { name_meta, name_bool, name_long, name_name, name_string, name_high } );
		WANT_EQ( exp_type.asName(), tup.type().asName() );

		REQUIRE_EQ( 5, tup.numAttributes() );
		REQUIRE_EQ( name_bool, tup[ 0 ].type().asName() );
		REQUIRE_EQ( name_long, tup[ 1 ].type().asName() );
		REQUIRE_EQ( name_name, tup[ 2 ].type().asName() );
		REQUIRE_EQ( name_string, tup[ 3 ].type().asName() );
		REQUIRE_EQ( name_high, tup[ 4 ].type().asName() );
		WANT_FALSE( tup.value<bool>( 0 ) );
		WANT_EQ( -54, tup.value<long_t>( 1 ) );
		WANT_EQ( name_active, tup.value<name_t>( 2 ) );
		WANT_EQ( "I am string!", tup.value<string>( 3 ) );
		WANT_EQ( 3.14, tup.value<high_t>( 4 ) );
	}
	TEST( TupleTest, meta_named )
	{
		type::Handler::init();
		MetaData tup;
		tup += true;
		tup.add( name::get( "two" ), static_cast<int_t>( -54 ) );
		tup += name_active;
		tup.add( name::get( "four" ), string( "I am string!" ) );
		tup.finalize();

		REQUIRE_EQ( 4, tup.numAttributes() );
		REQUIRE_EQ( name_bool, tup[ 0 ].type().asName() );
		REQUIRE_EQ( name_long, tup[ 1 ].type().asName() );
		REQUIRE_EQ( name_name, tup[ 2 ].type().asName() );
		REQUIRE_EQ( name_string, tup[ 3 ].type().asName() );
		WANT_TRUE( tup.value<bool>( 0 ) );
		WANT_EQ( -54, tup.value<long_t>( name::get( "two" ) ) );
		WANT_EQ( name_active, tup.value<name_t>( 2 ) );
		WANT_EQ( "I am string!", tup.value<string>( name::get( "four" ) ) );
	}
	TEST( TupleTest, meta_deep )
	{
		type::Handler::init();
		MetaData tup;
		tup += "unnamed";
		tup.add( name::get( "named" ), "Name" );
		REQUIRE_EXCEPT( tup.addPlainTuple( name::get( "deep" ) ), type::AccessDenied );
		REQUIRE_EXCEPT( tup.addDataTuple( name::get( "deep" ) ), type::AccessDenied );
		auto child = tup.addMetaData( name::get( "deep" ) );
		auto grandchild = child->addMetaData( name::get( "deeper" ) );
		*grandchild += "Rock";
		*grandchild += "bottom";
		grandchild->finalize();
		child->finalize();
		tup.finalize();

		type::Stringifier str;
		REQUIRE_NO_EXCEPT( tup.str( str ) );
		WANT_EQ( "M(\"unnamed\", named=\"Name\", deep=(deeper=(\"Rock\", \"bottom\")))", str.output() );
	}
	TEST( TupleTest, meta_compare )
	{
		type::Handler::init();
		MetaData base, eq, ne, lt, gt;

		base.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		base += name_active;
		base.add( name::get( "three" ), string( "Str" ) );
		base.finalize();

		eq.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		eq += name_active;
		eq.add( name::get( "three" ), string( "Str" ) );
		eq.finalize();

		ne.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		ne += name_active;
		ne.add( name::get( "two" ), string( "Str" ) );	// "two" vs. "three"
		ne.finalize();

		lt.add( name::get( "one" ), static_cast<int_t>( -55 ) );	// -55 vs. -54
		lt += name_active;
		lt.add( name::get( "three" ), string( "Str" ) );
		lt.finalize();

		gt.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		gt += name_active;
		gt.add( name::get( "three" ), string( "str" ) );	// "str" vs. "Str"
		gt.finalize();

		WANT_FALSE( base < eq );
		WANT_TRUE( base <= eq );
		WANT_FALSE( base > eq );
		WANT_TRUE( base >= eq );
		WANT_TRUE( base == eq );
		WANT_FALSE( base != eq );

		// NOTE: We cannot test relational differences on 'ne' since only a name is different
		WANT_FALSE( base == ne );
		WANT_TRUE( base != ne );

		WANT_FALSE( base < lt );
		WANT_FALSE( base <= lt );
		WANT_TRUE( base > lt );
		WANT_TRUE( base >= lt );
		WANT_FALSE( base == lt );
		WANT_TRUE( base != lt );

		WANT_TRUE( base < gt );
		WANT_TRUE( base <= gt );
		WANT_FALSE( base > gt );
		WANT_FALSE( base >= gt );
		WANT_FALSE( base == gt );
		WANT_TRUE( base != gt );
	}
	TEST( TupleTest, meta_compatible )
	{
		type::Handler::init();
		MetaData base, c1, c2, n1, n2;

		base.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		base += name_active;
		base.add( name::get( "three" ), string( "Str" ) );
		base.finalize();

		// Names in different places
		c1.add( name::get( "three" ), string( "Str" ) );
		c1 += name_active;
		c1.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		c1.finalize();

		// Unnamed, incomplete
		c2 += static_cast<int_t>( -54 );
		c2 += name_active;
		c2.finalize();

		// Wrong type
		n1.add( name::get( "one" ), string( "Str" ) );
		n1 += name_active;
		n1.add( name::get( "three" ), static_cast<int_t>( -55 ) );
		n1.finalize();

		// Too many attributes
		n2.add( name::get( "one" ), static_cast<int_t>( -54 ) );
		n2 += name_active;
		n2.add( name::get( "three" ), string( "str" ) );	// "str" vs. "Str"
		n2 += std::string( "bytes" );
		n2.finalize();

		WANT_TRUE( c1.type().compatibleWith( base.type() ) );
		WANT_TRUE( c2.type().compatibleWith( base.type() ) );
		WANT_FALSE( n1.type().compatibleWith( base.type() ) );
		WANT_FALSE( n2.type().compatibleWith( base.type() ) );
	}

/*
	
	TEST( TypesTest, instance_array )
	{
		tup::varscope global;
		instance i( global, new std::vector<instance>{
			instance( global, name_bool ),
			instance( global, int_t( -3333 ) ) } );
		REQUIRE_NO_EXCEPT( i.arrayValue() );
		REQUIRE_EXCEPT( i.float32Value(), instance::WrongType );
		REQUIRE_EQ( 2, i.arrayValue()->size() );
		WANT_EQ( name_bool, i.arrayValue()->at( 0 ).nameValue() );
		WANT_EQ( -3333, i.arrayValue()->at( 1 ).intValue() );
	}
	TEST( TypesTest, instance_list )
	{
		tup::varscope global;
		instance i( global, new std::list<instance>{
			instance( global, name_bool ),
			instance( global, int_t( -3333 ) ) } );
		REQUIRE_NO_EXCEPT( i.listValue() );
		REQUIRE_EXCEPT( i.arrayValue(), instance::WrongType );
		REQUIRE_EQ( 2, i.listValue()->size() );
		WANT_EQ( name_bool, i.listValue()->begin()->nameValue() );
		WANT_EQ( -3333, i.listValue()->rbegin()->intValue() );
	}

	TEST( TypesTest, instance_tuple )
	{
		auto name_sub = name::get( "sub" );
		tup::varscope global;
		instance i{ global, new eon::tuple{ global, {
			tuple::attribute( new instance( global, true ) ),
			tuple::attribute( name_byte, new instance( global, byte_t( 'x' ) ) ),
			tuple::attribute( name_int, new instance( global, int_t( -56 ) ) ),
			tuple::attribute( new instance( global, -3.16 ) ),
			tuple::attribute( name_sub, new instance( global, new eon::tuple{ global, {
				tuple::attribute( new instance( global, name_binary ) )
			} } ) )
		} } };
		REQUIRE_NO_EXCEPT( i.tupleValue() );
		REQUIRE_EXCEPT( i.float32Value(), instance::WrongType );
		REQUIRE_EQ( 5, i.tupleValue()->numAttributes() );
		WANT_TRUE( i.tupleValue()->at( 0 ).boolValue() );
		WANT_EQ( 'x', i.tupleValue()->at( 1 ).byteValue() );
		WANT_EQ( -56, i.tupleValue()->at( 2 ).intValue() );
		WANT_EQ( -3.16, i.tupleValue()->at( 3 ).floatValue() );
		REQUIRE_TRUE( i.tupleValue()->at( 4 ).t()->isTuple() );
		REQUIRE_EQ( 1, i.tupleValue()->at( 4 ).tupleValue()->numAttributes() );
		WANT_EQ( name_binary, i.tupleValue()->at( 4 ).tupleValue()->at( 0 ).nameValue() );
		WANT_EQ( 1, i.tupleValue()->t()->pos( name_byte ) );
		WANT_EQ( 2, i.tupleValue()->t()->pos( name_int ) );
		WANT_EQ( 4, i.tupleValue()->t()->pos( name_sub ) );
	}

	TEST( TypesTest, instance_reference )
	{
		tup::varscope global;
		auto target = new instance( global, name_bool );
		target->addRef();
		instance i( global, target );
		REQUIRE_NO_EXCEPT( i.referenceValue() );
		REQUIRE_EXCEPT( i.float32Value(), instance::WrongType );
		WANT_EQ( name_bool, i.referenceValue()->nameValue() );
		target->delRef();
	}*/
}
