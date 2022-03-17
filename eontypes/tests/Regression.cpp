#include "Regression.h"
#include <eontypes/Bool.h>
#include <eontypes/Byte.h>
#include <eontypes/Char.h>
#include <eontypes/Integer.h>
#include <eontypes/Floatingpt.h>
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
		using tt = IntegerType<int_t>;
		using ti = IntegerInstance<int_t>;
		type::Handler::init();

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
		using tt = IntegerType<short_t>;
		using ti = IntegerInstance<short_t>;
		type::Handler::init();

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
		using tt = IntegerType<long_t>;
		using ti = IntegerInstance<long_t>;
		type::Handler::init();

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
		using tt = FloatingptType<flt_t>;
		using ti = FloatingptInstance<flt_t>;
		type::Handler::init();

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_EQ( 0, i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( 0 ) ) ) );
		WANT_EQ( 0, i->value() );

		auto val = static_cast<flt_t>( -99.18 );
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( val ) ) ) );
		WANT_EQ( val, i->value() );
	}

	TEST( LowTest, instance )
	{
		using tt = FloatingptType<low_t>;
		using ti = FloatingptInstance<low_t>;
		type::Handler::init();

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_EQ( 0, i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( 0 ) ) ) );
		WANT_EQ( 0, i->value() );

		auto val = static_cast<low_t>( -99.18 );
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( val ) ) ) );
		WANT_EQ( val, i->value() );
	}

	TEST( HighTest, instance )
	{
		using tt = FloatingptType<high_t>;
		using ti = FloatingptInstance<high_t>;
		type::Handler::init();

		tt t;
		std::shared_ptr<ti> i;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate() ) ) );
		WANT_EQ( 0, i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( 0 ) ) ) );
		WANT_EQ( 0, i->value() );

		auto val = static_cast<high_t>( -99.18 );
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

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( namepath( "" ) ) ) ) );
		WANT_EQ( "@", i->value().str() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( namepath( "@one/two/three" ) ) ) ) );
		WANT_EQ( "@one/two/three", i->value().str() );
	}

	TEST( TupleTest, plain )
	{
		type::Handler::init();
		Tuple tup;
		tup.addRaw( false );
		tup.addRaw( static_cast<int_t>( -54 ) );
		tup.addRaw( name_active );
		tup.addRaw( "I am string!" );
		tup.finalize();
		REQUIRE_EXCEPT( tup.addRaw( 3.14 ), type::AccessDenied );

		std::vector<TypeElement*> attributes{ new NameElement( name_bool ), new NameElement( name_int ),
			new NameElement( name_name ), new NameElement( name_string ) };
		TypeTuple exp_type( attributes, name_plain, TypeTuple::own_attributes::yes );
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
		tup.addRaw( true );
		tup.addRaw( eon::name( "two" ), static_cast<int_t>( -54 ) );
		tup.addRaw( name_active );
		tup.addRaw( eon::name( "four" ), string( "I am string!" ) );
		tup.finalize();

		REQUIRE_EQ( 4, tup.numAttributes() );
		REQUIRE_EQ( name_bool, tup[ 0 ].type().asName() );
		REQUIRE_EQ( name_int, tup[ 1 ].type().asName() );
		REQUIRE_EQ( name_name, tup[ 2 ].type().asName() );
		REQUIRE_EQ( name_string, tup[ 3 ].type().asName() );
		WANT_TRUE( tup.value<bool>( 0 ) );
		WANT_EQ( -54, tup.value<int_t>( eon::name( "two" ) ) );
		WANT_EQ( name_active, tup.value<name_t>( 2 ) );
		WANT_EQ( "I am string!", tup.value<string>( eon::name( "four" ) ) );
	}
	TEST( TupleTest, plain_deep )
	{
		type::Handler::init();
		Tuple tup;
		tup.addRaw( "unnamed" );
		tup.addRaw( eon::name( "named" ), "Name" );
		auto child = tup.addPlainTuple( eon::name( "deep" ) );
		auto grandchild = child->addPlainTuple( eon::name( "deeper" ) );
		grandchild->addRaw( "Rock" );
		grandchild->addRaw( "bottom" );
		grandchild->finalize();
		child->finalize();
		tup.finalize();

		type::Stringifier str;
		REQUIRE_NO_EXCEPT( tup.str( str ) );
		WANT_EQ( "p(\"unnamed\", named=\"Name\", deep=(deeper=(\"Rock\", \"bottom\")))", str.output() );
	}
	TEST( TupleTest, plain_compare )
	{
		type::Handler::init();
		Tuple base, eq, ne, lt, gt;

		base.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		base.addRaw( name_active );
		base.addRaw( eon::name( "three" ), string( "Str" ) );
		base.finalize();

		eq.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		eq.addRaw( name_active );
		eq.addRaw( eon::name( "three" ), string( "Str" ) );
		eq.finalize();

		ne.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		ne.addRaw( name_active );
		ne.addRaw( eon::name( "two" ), string( "Str" ) );	// "two" vs. "three"
		ne.finalize();

		lt.addRaw( eon::name( "one" ), static_cast<int_t>( -55 ) );	// -55 vs. -54
		lt.addRaw( name_active );
		lt.addRaw( eon::name( "three" ), string( "Str" ) );
		lt.finalize();

		gt.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		gt.addRaw( name_active );
		gt.addRaw( eon::name( "three" ), string( "str" ) );	// "str" vs. "Str"
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

		base.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		base.addRaw( name_active );
		base.addRaw( eon::name( "three" ), string( "Str" ) );
		base.finalize();

		// Names in different places
		c1.addRaw( eon::name( "three" ), string( "Str" ) );
		c1.addRaw( name_active );
		c1.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		c1.finalize();

		// Unnamed, incomplete
		c2.addRaw( static_cast<int_t>( -54 ) );
		c2.addRaw( name_active );
		c2.finalize();

		// Wrong type
		n1.addRaw( eon::name( "one" ), string( "Str" ) );
		n1.addRaw( name_active );
		n1.addRaw( eon::name( "three" ), static_cast<int_t>( -55 ) );
		n1.finalize();

		// Too many attributes
		n2.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		n2.addRaw( name_active );
		n2.addRaw( eon::name( "three" ), string( "str" ) );	// "str" vs. "Str"
		n2.addRaw( std::string( "bytes" ) );
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
		tup.addRaw( false );
		tup.addRaw( static_cast<int_t>( -54 ) );
		tup.addRaw( name_active );
		tup.addRaw( string( "I am string!" ) );
		tup.finalize();
		REQUIRE_NO_EXCEPT( tup.addRaw( 3.14 ) );

		std::vector<TypeElement*> attributes{ new NameElement( name_bool ), new NameElement( name_int ),
			new NameElement( name_name ), new NameElement( name_string ), new NameElement{ name_float } };
		TypeTuple exp_type( attributes, name_dynamic, TypeTuple::own_attributes::yes );
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
		tup.addRaw( true );
		tup.addRaw( eon::name( "two" ), static_cast<int_t>( -54 ) );
		tup.addRaw( name_active );
		tup.addRaw( eon::name( "four" ), string( "I am string!" ) );
		tup.finalize();

		REQUIRE_EQ( 4, tup.numAttributes() );
		REQUIRE_EQ( name_bool, tup[ 0 ].type().asName() );
		REQUIRE_EQ( name_int, tup[ 1 ].type().asName() );
		REQUIRE_EQ( name_name, tup[ 2 ].type().asName() );
		REQUIRE_EQ( name_string, tup[ 3 ].type().asName() );
		WANT_TRUE( tup.value<bool>( 0 ) );
		WANT_EQ( -54, tup.value<int_t>( eon::name( "two" ) ) );
		WANT_EQ( name_active, tup.value<name_t>( 2 ) );
		WANT_EQ( "I am string!", tup.value<string>( eon::name( "four" ) ) );
	}
	TEST( TupleTest, dynamic_deep )
	{
		type::Handler::init();
		DynamicTuple tup;
		tup.addRaw( "unnamed" );
		tup.addRaw( eon::name( "named" ), "Name" );
		auto child = tup.addPlainTuple( eon::name( "deep" ) );
		auto grandchild = child->addDynamicTuple( eon::name( "deeper" ) );
		grandchild->addRaw( "Rock" );
		grandchild->addRaw( "bottom" );
		grandchild->finalize();
		child->finalize();
		tup.finalize();

		type::Stringifier str;
		REQUIRE_NO_EXCEPT( tup.str( str ) );
		WANT_EQ( "dynamic(\"unnamed\", named=\"Name\", deep=(deeper=(\"Rock\", \"bottom\")))", str.output() );
	}
	TEST( TupleTest, dynamic_compare )
	{
		type::Handler::init();
		DynamicTuple base, eq, ne, lt, gt;

		base.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		base.addRaw( name_active );
		base.addRaw( eon::name( "three" ), string( "Str" ) );
		base.finalize();

		eq.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		eq.addRaw( name_active );
		eq.addRaw( eon::name( "three" ), string( "Str" ) );
		eq.finalize();

		ne.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		ne.addRaw( name_active );
		ne.addRaw( eon::name( "two" ), string( "Str" ) );	// "two" vs. "three"
		ne.finalize();

		lt.addRaw( eon::name( "one" ), static_cast<int_t>( -55 ) );	// -55 vs. -54
		lt.addRaw( name_active );
		lt.addRaw( eon::name( "three" ), string( "Str" ) );
		lt.finalize();

		gt.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		gt.addRaw( name_active );
		gt.addRaw( eon::name( "three" ), string( "str" ) );	// "str" vs. "Str"
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

		base.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		base.addRaw( name_active );
		base.addRaw( eon::name( "three" ), string( "Str" ) );
		base.finalize();

		// Names in different places
		c1.addRaw( eon::name( "three" ), string( "Str" ) );
		c1.addRaw( name_active );
		c1.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		c1.finalize();

		// Unnamed, incomplete
		c2.addRaw( static_cast<int_t>( -54 ) );
		c2.addRaw( name_active );
		c2.finalize();

		// Wrong type
		n1.addRaw( eon::name( "one" ), string( "Str" ) );
		n1.addRaw( name_active );
		n1.addRaw( eon::name( "three" ), static_cast<int_t>( -55 ) );
		n1.finalize();

		// Too many attributes
		n2.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		n2.addRaw( name_active );
		n2.addRaw( eon::name( "three" ), string( "str" ) );	// "str" vs. "Str"
		n2.addRaw( std::string( "bytes" ) );
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
		tup.addRaw( false );
		tup.addRaw( static_cast<int_t>( -54 ) );
		tup.addRaw( name_active );
		tup.addRaw( string( "I am string!" ) );
		tup.finalize();
		REQUIRE_NO_EXCEPT( tup.addRaw( 3.14 ) );

		std::vector<TypeElement*> attributes{ new NameElement( name_bool ), new NameElement( name_long ),
			new NameElement( name_name ), new NameElement( name_string ), new NameElement{ name_high } };
		TypeTuple exp_type( attributes, name_data, TypeTuple::own_attributes::yes );
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
		tup.addRaw( true );
		tup.addRaw( eon::name( "two" ), static_cast<int_t>( -54 ) );
		tup.addRaw( name_active );
		tup.addRaw( eon::name( "four" ), string( "I am string!" ) );
		tup.finalize();

		REQUIRE_EQ( 4, tup.numAttributes() );
		REQUIRE_EQ( name_bool, tup[ 0 ].type().asName() );
		REQUIRE_EQ( name_long, tup[ 1 ].type().asName() );
		REQUIRE_EQ( name_name, tup[ 2 ].type().asName() );
		REQUIRE_EQ( name_string, tup[ 3 ].type().asName() );
		WANT_TRUE( tup.value<bool>( 0 ) );
		WANT_EQ( -54, tup.value<long_t>( eon::name( "two" ) ) );
		WANT_EQ( name_active, tup.value<name_t>( 2 ) );
		WANT_EQ( "I am string!", tup.value<string>( eon::name( "four" ) ) );
	}
	TEST( TupleTest, data_deep )
	{
		type::Handler::init();
		DataTuple tup;
		tup.addRaw( "unnamed" );
		tup.addRaw( eon::name( "named" ), "Name" );
		REQUIRE_EXCEPT( tup.addPlainTuple( eon::name( "deep" ) ), type::AccessDenied );
		auto child = tup.addDataTuple( eon::name( "deep" ) );
		auto grandchild = child->addDataTuple( eon::name( "deeper" ) );
		grandchild->addRaw( "Rock" );
		grandchild->addRaw( "bottom" );
		grandchild->finalize();
		child->finalize();
		tup.finalize();

		type::Stringifier str;
		REQUIRE_NO_EXCEPT( tup.str( str ) );
		WANT_EQ( "data(\"unnamed\", named=\"Name\", deep:\n  deeper:\n    \"Rock\", \"bottom\")", str.output() );
	}
	TEST( TupleTest, data_compare )
	{
		type::Handler::init();
		DataTuple base, eq, ne, lt, gt;

		base.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		base.addRaw( name_active );
		base.addRaw( eon::name( "three" ), string( "Str" ) );
		base.finalize();

		eq.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		eq.addRaw( name_active );
		eq.addRaw( eon::name( "three" ), string( "Str" ) );
		eq.finalize();

		ne.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		ne.addRaw( name_active );
		ne.addRaw( eon::name( "two" ), string( "Str" ) );	// "two" vs. "three"
		ne.finalize();

		lt.addRaw( eon::name( "one" ), static_cast<int_t>( -55 ) );	// -55 vs. -54
		lt.addRaw( name_active );
		lt.addRaw( eon::name( "three" ), string( "Str" ) );
		lt.finalize();

		gt.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		gt.addRaw( name_active );
		gt.addRaw( eon::name( "three" ), string( "str" ) );	// "str" vs. "Str"
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

		base.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		base.addRaw( name_active );
		base.addRaw( eon::name( "three" ), string( "Str" ) );
		base.finalize();

		// Names in different places
		c1.addRaw( eon::name( "three" ), string( "Str" ) );
		c1.addRaw( name_active );
		c1.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		c1.finalize();

		// Unnamed, incomplete
		c2.addRaw( static_cast<int_t>( -54 ) );
		c2.addRaw( name_active );
		c2.finalize();

		// Wrong type
		n1.addRaw( eon::name( "one" ), string( "Str" ) );
		n1.addRaw( name_active );
		n1.addRaw( eon::name( "three" ), static_cast<int_t>( -55 ) );
		n1.finalize();

		// Too many attributes
		n2.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		n2.addRaw( name_active );
		n2.addRaw( eon::name( "three" ), string( "str" ) );	// "str" vs. "Str"
		n2.addRaw( std::string( "bytes" ) );
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
		auto name_sub = eon::name( "sub" );
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
