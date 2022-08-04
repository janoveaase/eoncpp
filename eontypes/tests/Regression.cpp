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
#include <eontypes/Scope.h>
#include <eonsource/String.h>
#include <eontypes/Caches.h>



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

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( static_cast<short_t>( 0 ) ) ) ) );
		WANT_EQ( 0, i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( static_cast<short_t>( -56 ) ) ) ) );
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

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( static_cast<long_t>( 0 ) ) ) ) );
		WANT_EQ( 0, i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( static_cast<long_t>( -56 ) ) ) ) );
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
		WANT_EQ( (uint64_t)no_handle, (uint64_t)i->value() );

		handle_t h1 = no_handle, h2 = (handle_t)199;
		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( h1 ) ) ) );
		WANT_EQ( (uint64_t)h1, (uint64_t)i->value() );

		REQUIRE_NO_EXCEPT( i = std::shared_ptr<ti>( dynamic_cast<ti*>( t.instantiate( h2 ) ) ) );
		WANT_EQ( (uint64_t)h2, (uint64_t)i->value() );
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
		tup.addName( name_active );
		tup.addRaw( "I am string!" );
		tup.finalize();
		REQUIRE_EXCEPT( tup.addRaw( 3.14 ), type::AccessDenied );

		EonType exp_type( { EonType( name_bool ), EonType( name_int ), EonType( name_name ), EonType( name_string ) } );
		WANT_EQ( exp_type.str(), tup.type().str() );

		REQUIRE_EQ( 4, tup.numAttributes() );
		REQUIRE_EQ( name_bool, tup[ 0 ].type().name() );
		REQUIRE_EQ( name_int, tup[ 1 ].type().name() );
		REQUIRE_EQ( name_name, tup[ 2 ].type().name() );
		REQUIRE_EQ( name_string, tup[ 3 ].type().name() );
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
		tup.addName( name_active );
		tup.addRaw( eon::name( "four" ), string( "I am string!" ) );
		tup.finalize();

		REQUIRE_EQ( 4, tup.numAttributes() );
		REQUIRE_EQ( name_bool, tup[ 0 ].type().name() );
		REQUIRE_EQ( name_int, tup[ 1 ].type().name() );
		REQUIRE_EQ( name_name, tup[ 2 ].type().name() );
		REQUIRE_EQ( name_string, tup[ 3 ].type().name() );
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

		Stringifier str;
		REQUIRE_NO_EXCEPT( tup.str( str ) );
		string act = str.generateString();
		string exp{ "p(\"unnamed\", named=\"Name\", deep=(deeper=(\"Rock\", \"bottom\")))" };
		WANT_EQ( exp, act );
	}
	TEST( TupleTest, plain_depth )
	{
		type::Handler::init();
		Tuple empty, single, shallow, deep;
		empty.finalize();
		WANT_EQ( 0, empty.depth() );

		single.addRaw( "alpha" );
		single.addRaw( "beta" );
		single.finalize();
		WANT_EQ( 1, single.depth() );

		shallow.addRaw( "alpha" );
		shallow.addRaw( std::move( single ) );
		shallow.addRaw( "beta" );
		shallow.finalize();
		WANT_EQ( 2, shallow.depth() );

		deep.addRaw( "alpha" );
		deep.addRaw( std::move( shallow ) );
		deep.addRaw( "beta" );
		WANT_EQ( 3, deep.depth() );
	}
	TEST( TupleTest, plain_compare )
	{
		type::Handler::init();
		Tuple base, eq, ne, lt, gt;

		base.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		base.addName( name_active );
		base.addRaw( eon::name( "three" ), string( "Str" ) );
		base.finalize();

		eq.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		eq.addName( name_active );
		eq.addRaw( eon::name( "three" ), string( "Str" ) );
		eq.finalize();

		ne.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		ne.addName( name_active );
		ne.addRaw( eon::name( "two" ), string( "Str" ) );	// "two" vs. "three"
		ne.finalize();

		lt.addRaw( eon::name( "one" ), static_cast<int_t>( -55 ) );	// -55 vs. -54
		lt.addName( name_active );
		lt.addRaw( eon::name( "three" ), string( "Str" ) );
		lt.finalize();

		gt.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		gt.addName( name_active );
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
		base.addName( name_active );
		base.addRaw( eon::name( "three" ), string( "Str" ) );
		base.finalize();

		// Names in different places
		c1.addRaw( eon::name( "three" ), string( "Str" ) );
		c1.addName( name_active );
		c1.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		c1.finalize();

		// Unnamed, incomplete
		c2.addRaw( static_cast<int_t>( -54 ) );
		c2.addName( name_active );
		c2.finalize();

		// Wrong type
		n1.addRaw( eon::name( "one" ), string( "Str" ) );
		n1.addName( name_active );
		n1.addRaw( eon::name( "three" ), static_cast<int_t>( -55 ) );
		n1.finalize();

		// Too many attributes
		n2.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		n2.addName( name_active );
		n2.addRaw( eon::name( "three" ), string( "str" ) );	// "str" vs. "Str"
		n2.addRaw( std::string( "bytes" ) );
		n2.finalize();

		WANT_TRUE( c1.compatibleWith( base ) );
		WANT_TRUE( c2.compatibleWith( base ) );
		WANT_FALSE( n1.compatibleWith( base ) );
		WANT_FALSE( n2.compatibleWith( base ) );
	}

	TEST( TupleTest, dynamic )
	{
		type::Handler::init();
		DynamicTuple tup;
		tup.addRaw( false );
		tup.addRaw( static_cast<int_t>( -54 ) );
		tup.addName( name_active );
		tup.addRaw( string( "I am string!" ) );
		tup.finalize();
		REQUIRE_NO_EXCEPT( tup.addRaw( 3.14 ) );

		EonType exp_type( name_dynamic );
		WANT_EQ( exp_type.str(), tup.type().str() );

		REQUIRE_EQ( 5, tup.numAttributes() );
		REQUIRE_EQ( name_bool, tup[ 0 ].type().name() );
		REQUIRE_EQ( name_int, tup[ 1 ].type().name() );
		REQUIRE_EQ( name_name, tup[ 2 ].type().name() );
		REQUIRE_EQ( name_string, tup[ 3 ].type().name() );
		REQUIRE_EQ( name_float, tup[ 4 ].type().name() );
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
		tup.addName( name_active );
		tup.addRaw( eon::name( "four" ), string( "I am string!" ) );
		tup.finalize();

		REQUIRE_EQ( 4, tup.numAttributes() );
		REQUIRE_EQ( name_bool, tup[ 0 ].type().name() );
		REQUIRE_EQ( name_int, tup[ 1 ].type().name() );
		REQUIRE_EQ( name_name, tup[ 2 ].type().name() );
		REQUIRE_EQ( name_string, tup[ 3 ].type().name() );
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
		auto child = tup.addDynamicTuple( eon::name( "deep" ) );
		auto grandchild = child->addDynamicTuple( eon::name( "deeper" ) );
		grandchild->addRaw( "Rock" );
		grandchild->addRaw( "bottom" );
		grandchild->finalize();
		child->finalize();
		tup.finalize();

		Stringifier str;
		REQUIRE_NO_EXCEPT( tup.str( str ) );
		WANT_EQ( "dynamic(\"unnamed\", named=\"Name\", deep=dynamic(deeper=dynamic(\"Rock\", \"bottom\")))",
			str.generateString() );
	}
	TEST( TupleTest, dynamic_compare )
	{
		type::Handler::init();
		DynamicTuple base, eq, ne, lt, gt;

		base.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		base.addName( name_active );
		base.addRaw( eon::name( "three" ), string( "Str" ) );
		base.finalize();

		eq.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		eq.addName( name_active );
		eq.addRaw( eon::name( "three" ), string( "Str" ) );
		eq.finalize();

		ne.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		ne.addName( name_active );
		ne.addRaw( eon::name( "two" ), string( "Str" ) );	// "two" vs. "three"
		ne.finalize();

		lt.addRaw( eon::name( "one" ), static_cast<int_t>( -55 ) );	// -55 vs. -54
		lt.addName( name_active );
		lt.addRaw( eon::name( "three" ), string( "Str" ) );
		lt.finalize();

		gt.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		gt.addName( name_active );
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
		base.addName( name_active );
		base.addRaw( eon::name( "three" ), string( "Str" ) );
		base.finalize();

		// Names in different places
		c1.addRaw( eon::name( "three" ), string( "Str" ) );
		c1.addName( name_active );
		c1.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		c1.finalize();

		// Unnamed, incomplete
		c2.addRaw( static_cast<int_t>( -54 ) );
		c2.addName( name_active );
		c2.finalize();

		// Wrong type
		n1.addRaw( eon::name( "one" ), string( "Str" ) );
		n1.addName( name_active );
		n1.addRaw( eon::name( "three" ), static_cast<int_t>( -55 ) );
		n1.finalize();

		// Too many attributes
		n2.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		n2.addName( name_active );
		n2.addRaw( eon::name( "three" ), string( "str" ) );	// "str" vs. "Str"
		n2.addRaw( std::string( "bytes" ) );
		n2.finalize();

		WANT_TRUE( c1.compatibleWith( base ) );
		WANT_TRUE( c2.compatibleWith( base ) );
		WANT_FALSE( n1.compatibleWith( base ) );
		WANT_FALSE( n2.compatibleWith( base ) );
	}

	TEST( TupleTest, data )
	{
		type::Handler::init();
		DataTuple tup;
		tup.addRaw( false );
		tup.addRaw( static_cast<short_t>( -54 ) );
		tup.addRaw( static_cast<int_t>( -54 ) );
		tup.addName( name_active );
		tup.addRaw( string( "I am string!" ) );
		tup.finalize();
		REQUIRE_NO_EXCEPT( tup.addRaw( 3.14 ) );

		EonType exp_type( name_data );
		WANT_EQ( exp_type.str(), tup.type().str() );

		REQUIRE_EQ( 6, tup.numAttributes() );
		REQUIRE_EQ( name_bool, tup[ 0 ].type().name() );
		REQUIRE_EQ( name_int, tup[ 1 ].type().name() );
		REQUIRE_EQ( name_int, tup[ 2 ].type().name() );
		REQUIRE_EQ( name_name, tup[ 3 ].type().name() );
		REQUIRE_EQ( name_string, tup[ 4 ].type().name() );
		REQUIRE_EQ( name_float, tup[ 5 ].type().name() );
		WANT_FALSE( tup.value<bool>( 0 ) );
		WANT_EQ( -54, tup.value<int_t>( 1 ) );
		WANT_EQ( -54, tup.value<int_t>( 2 ) );
		WANT_EQ( name_active, tup.value<name_t>( 3 ) );
		WANT_EQ( "I am string!", tup.value<string>( 4 ) );
		WANT_EQ( 3.14, tup.value<double>( 5 ) );
	}
	TEST( TupleTest, data_named )
	{
		type::Handler::init();
		DataTuple tup;
		tup.addRaw( true );
		tup.addRaw( eon::name( "two" ), static_cast<int_t>( -54 ) );
		tup.addName( name_active );
		tup.addRaw( eon::name( "four" ), string( "I am string!" ) );
		tup.finalize();

		REQUIRE_EQ( 4, tup.numAttributes() );
		REQUIRE_EQ( name_bool, tup[ 0 ].type().name() );
		REQUIRE_EQ( name_int, tup[ 1 ].type().name() );
		REQUIRE_EQ( name_name, tup[ 2 ].type().name() );
		REQUIRE_EQ( name_string, tup[ 3 ].type().name() );
		WANT_TRUE( tup.value<bool>( 0 ) );
		WANT_EQ( -54, tup.value<int_t>( eon::name( "two" ) ) );
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

		Stringifier str;
		REQUIRE_NO_EXCEPT( tup.str( str ) );
		string act = str.generateString();
		string exp{
			"data(\"unnamed\", named=\"Name\",\n"
			"  deep:\n"
			"    deeper:\n"
			"      \"Rock\", \"bottom\")" };
		WANT_EQ( exp, act );
	}
	TEST( TupleTest, data_complex )
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

		Stringifier str;
		REQUIRE_NO_EXCEPT( tup.str( str ) );
		string act = str.generateString();
		string exp{
			"data(\"unnamed\", named=\"Name\",\n"
			"  deep:\n"
			"    deeper:\n"
			"      \"Rock\", \"bottom\")" };
		WANT_EQ( exp, act );
	}
	TEST( TupleTest, data_compare )
	{
		type::Handler::init();
		DataTuple base, eq, ne, lt, gt;

		base.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		base.addName( name_active );
		base.addRaw( eon::name( "three" ), string( "Str" ) );
		base.finalize();

		eq.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		eq.addName( name_active );
		eq.addRaw( eon::name( "three" ), string( "Str" ) );
		eq.finalize();

		ne.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		ne.addName( name_active );
		ne.addRaw( eon::name( "two" ), string( "Str" ) );	// "two" vs. "three"
		ne.finalize();

		lt.addRaw( eon::name( "one" ), static_cast<int_t>( -55 ) );	// -55 vs. -54
		lt.addName( name_active );
		lt.addRaw( eon::name( "three" ), string( "Str" ) );
		lt.finalize();

		gt.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		gt.addName( name_active );
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
		base.addName( name_active );
		base.addRaw( eon::name( "three" ), string( "Str" ) );
		base.finalize();

		// Names in different places
		c1.addRaw( eon::name( "three" ), string( "Str" ) );
		c1.addName( name_active );
		c1.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		c1.finalize();

		// Unnamed, incomplete
		c2.addRaw( static_cast<int_t>( -54 ) );
		c2.addName( name_active );
		c2.finalize();

		// Wrong type
		n1.addRaw( eon::name( "one" ), string( "Str" ) );
		n1.addName( name_active );
		n1.addRaw( eon::name( "three" ), static_cast<int_t>( -55 ) );
		n1.finalize();

		// Too many attributes
		n2.addRaw( eon::name( "one" ), static_cast<int_t>( -54 ) );
		n2.addName( name_active );
		n2.addRaw( eon::name( "three" ), string( "str" ) );	// "str" vs. "Str"
		n2.addRaw( std::string( "bytes" ) );
		n2.finalize();

		WANT_TRUE( c1.compatibleWith( base ) );
		WANT_TRUE( c2.compatibleWith( base ) );
		WANT_FALSE( n1.compatibleWith( base ) );
		WANT_FALSE( n2.compatibleWith( base ) );
	}


	TEST( TupleStrTest, rule_1 )
	{
		type::Handler::init();
		DynamicTuple dynamic;
		DataTuple data;
		Tuple plain;
		data.addRaw( 1 );
		data.finalize();
		plain.addRaw( 2 );
		plain.finalize();
		dynamic.addRaw( std::move( data ) );
		dynamic.addRaw( std::move( plain ) );
		dynamic.finalize();

		Stringifier str;
		REQUIRE_NO_EXCEPT( dynamic.str( str ) );
		string act = str.generateString();
		string exp{ "dynamic(data(1), p(2))" };
		WANT_EQ( exp, act );
	}
	TEST( TupleStrTest, rule_2 )
	{
		type::Handler::init();
		DataTuple dt, sub;
		sub.addRaw( eon::name( "one" ) );
		sub.addRaw( eon::name( "two" ) );
		sub.finalize();
		dt.addRaw( eon::name( "value" ), std::move( sub ) );
		dt.finalize();

		Stringifier str;
		REQUIRE_NO_EXCEPT( dt.str( str ) );
		string act = str.generateString();
		string exp{
			"data(\n"
			"  value:\n"
			"    one, two)" };
		WANT_EQ( exp, act );
	}
	TEST( TupleStrTest, rule_3 )
	{
		type::Handler::init();
		DynamicTuple dt, sub1;
		DataTuple sub2, sub3;
		sub3.addRaw( 1 );
		sub3.finalize();
		sub2.addRaw( eon::name( "value" ), std::move( sub3 ) );
		sub2.finalize();
		sub1.addRaw( std::move( sub2 ) );
		sub1.finalize();
		dt.add( std::move( sub1 ) );
		dt.finalize();

		Stringifier str;
		REQUIRE_NO_EXCEPT( dt.str( str ) );
		string act = str.generateString();
		string exp{
			"dynamic(dynamic(data(\n"
			"  value:\n"
			"    data(1))))" };
		WANT_EQ( exp, act );
	}
	TEST( TupleStrTest, rule_4_1a )
	{
		type::Handler::init();
		DataTuple dt, sub1, sub2, sub3;
		sub1.addRaw( "Element 1.1" );
		sub1.addRaw( "Element 1.2" );
		sub1.finalize();
		sub2.addRaw( "Element 2.1" );
		sub2.addRaw( "Element 2.2" );
		sub2.finalize();
		sub3.addRaw( "Element 3.1" );
		sub3.addRaw( "Element 3.2" );
		sub3.finalize();
		dt.addRaw( std::move( sub1 ) );
		dt.addRaw( std::move( sub2 ) );
		dt.addRaw( std::move( sub3 ) );
		dt.finalize();

		Stringifier str;
		str.softLineWidth( 40 );
		REQUIRE_NO_EXCEPT( dt.str( str ) );
		string act = str.generateString();
		string exp{
			"data((\"Element 1.1\", \"Element 1.2\"),\n"
			"  (\"Element 2.1\", \"Element 2.2\"),\n"
			"  (\"Element 3.1\", \"Element 3.2\"))" };
		WANT_EQ( exp, act );
	}
	TEST( TupleStrTest, rule_4_1b )
	{
		type::Handler::init();
		DataTuple dt, sub2, sub3;
		sub2.addRaw( "Element 2.1" );
		sub2.addRaw( "Element 2.2" );
		sub2.finalize();
		sub3.addRaw( "Element 3.1" );
		sub3.addRaw( "Element 3.2" );
		sub3.finalize();
		dt.addRaw( "Just long enough to consume a line." );
		dt.addRaw( std::move( sub2 ) );
		dt.addRaw( std::move( sub3 ) );
		dt.finalize();

		Stringifier str;
		str.softLineWidth( 40 );
		REQUIRE_NO_EXCEPT( dt.str( str ) );
		string act = str.generateString();
		string exp{
			"data(\n"
			"  \"Just long enough to consume a line.\",\n"
			"  (\"Element 2.1\", \"Element 2.2\"),\n"
			"  (\"Element 3.1\", \"Element 3.2\"))" };
		WANT_EQ( exp, act );
	}
	TEST( TupleStrTest, rule_4_2 )
	{
		type::Handler::init();
		DataTuple dt;
		dt.addRaw( eon::name( "value" ), "Just long enough to consume a line." );
		dt.finalize();

		Stringifier str;
		str.softLineWidth( 40 );
		REQUIRE_NO_EXCEPT( dt.str( str ) );
		string act = str.generateString();
		string exp{
			"data(value=\n"
			"  \"Just long enough to consume a line.\")" };
		WANT_EQ( exp, act );
	}
	TEST( TupleStrTest, rule_4_3 )
	{
		type::Handler::init();
		DataTuple dt;
		dt.addRaw( eon::name( "value" ), "Just long enough to consume a line." );
		dt.finalize();

		Stringifier str;
		str.softLineWidth( 40 );
		REQUIRE_NO_EXCEPT( dt.str( str ) );
		string act = str.generateString();
		string exp{
			"data(value=\n"
			"  \"Just long enough to consume a line.\")" };
		WANT_EQ( exp, act );
	}
	TEST( TupleStrTest, rule_4_4a )
	{
		type::Handler::init();
		DataTuple dt;
		dt.addRaw( "An element that is too long to fit on a single line!" );
		dt.finalize();

		Stringifier str;
		str.softLineWidth( 40 );
		REQUIRE_NO_EXCEPT( dt.str( str ) );
		string act = str.generateString();
		string exp{
			"data(\n"
			"  \"An element that is too long to fit on a single line!\"\n"
			"  )" };
		WANT_EQ( exp, act );
	}
	TEST( TupleStrTest, rule_4_4b )
	{
		type::Handler::init();
		DataTuple dt;
		dt.addRaw( "An element that is too long to fit on a single line!" );
		dt.finalize();

		Stringifier str;
		str.hardLineWidth( 40 );
		REQUIRE_NO_EXCEPT( dt.str( str ) );
		string act = str.generateString();
		string exp{
			"data(\n"
			"  \"An element that is too long to fit \\\n"
			"    on a single line!\"\n"
			"  )" };
		WANT_EQ( exp, act );
	}
	TEST( TupleStrTest, misc_1 )
	{
		type::Handler::init();
		DynamicTuple dt, dynamic;
		Tuple plain;
		dt.addRaw( eon::name( "bool_val" ), true );
		dt.addRaw( eon::name( "byte_val" ), byte_t( 'b' ) );
		dt.addRaw( eon::name( "char_val" ), char_t( 'c' ) );
		dt.addRaw( eon::name( "int_val" ), int_t( -99 ) );
		dt.addRaw( eon::name( "float_val" ), flt_t( -99.99 ) );
		dt.addRaw( eon::name( "index_val" ), index_t( 5635466241234LL ) );
		dt.addRaw( eon::name( "name_val" ), eon::name( "test" ) );
		dynamic.addRaw( eon::name( "short_val" ), short_t( 123 ) );
		dynamic.addRaw( eon::name( "long_val" ), long_t( -45764735234 ) );
		dynamic.addRaw( eon::name( "low_val" ), low_t( 9.11 ) );
		dynamic.addRaw( eon::name( "high_val" ), high_t( 47676476.0 ) );
		dynamic.finalize();
		dt.addRaw( eon::name( "tuple_val" ), std::move( dynamic ) );
		dt.addRaw( eon::name( "string_val" ), eon::string( "alpha beta" ) );
		dt.addRaw( "some value" );
		plain.addRaw( eon::name( "one" ) );
		plain.addRaw( eon::name( "two" ) );
		plain.addRaw( eon::name( "three" ) );
		plain.finalize();
		dt.addRaw( std::move( plain ) );
		dt.finalize();

		Stringifier str;
		REQUIRE_NO_EXCEPT( dt.str( str ) );
		string act = str.generateString();
		string exp{
			"dynamic(\n"
			"  bool_val=true,\n"
			"  byte_val=b'b',\n"
			"  char_val='c',\n"
			"  int_val=-99,\n"
			"  float_val=-99.99,\n"
			"  index_val=5635466241234,\n"
			"  name_val=test,\n"
			"  tuple_val=dynamic(short_val=123S, long_val=-45764735234L, low_val=9.11L, high_val=47676476.0H),\n"
			"  string_val=\"alpha beta\",\n"
			"  \"some value\",\n"
			"  p(one, two, three))" };
		WANT_EQ( exp, act );
	}



	TEST( TypeTest, str1 )
	{
		EonType t1( name_bool ), t2( name_byte, name_active );
		EonType t3( { name_char, name_int } );
		EonType t4( { EonType( EonType( { name_short, name_long } ), name( "one" ) ),
			EonType( { name_float, name_low, name_high } ) } );

		WANT_EQ( "T(bool)", t1.str() );
		WANT_EQ( "T(active=byte)", t2.str() );
		WANT_EQ( "T(char, int)", t3.str() );
		WANT_EQ( "T(one=(short, long), (float, low, high))", t4.str() );
	}
	TEST( TypeTest, str2 )
	{
		type::Handler::init();
		BoolInstance b1( true, source::Ref() );
		BoolInstance b2( false, source::Ref() );
		Tuple t1, t2;
		t1.addRaw( name( "one" ), false );
		t1.addRaw( static_cast<int_t>( -54 ) );
		t1.addName( name( "three" ), name_active );
		t1.addRaw( "I am string!" );
		t1.finalize();
		t2.addName( name( "three" ), name_active );
		t2.addRaw( static_cast<int_t>( 9 ) );
		t2.finalize();

		WANT_EQ( "T(bool)", b1.type().str() );
		WANT_EQ( "T(one=bool, int, three=name, string)", t1.type().str() );
		WANT_EQ( "T(three=name, int)", t2.type().str() );
	}
	TEST( TypeTest, compare )
	{
		EonType t1( name_bool ), t2( name_bool ), t3( name_char );

		WANT_TRUE( t1 == t2 );
		WANT_TRUE( t2 == t1 );
		WANT_FALSE( t1 == t3 );
	}
	TEST( TypeTest, compatible )
	{
		EonType t1( name_bool ), t2( name_bool );
		EonType t3( { EonType( name_bool, name( "one" ) ), EonType( name_int ), EonType( name_name, name( "three" ) ),
			EonType( name_string ) } );
		EonType t4( { EonType( name_name, name( "three" ) ), EonType( name_int ) } );

		WANT_TRUE( t1.compatibleWith( t2 ) );
		WANT_TRUE( t2.compatibleWith( t1 ) );
		WANT_TRUE( t1.compatibleWith( t3 ) );
		WANT_TRUE( t4.compatibleWith( t3 ) );
		WANT_FALSE( t3.compatibleWith( t4 ) );
	}
	
	
	
	TEST( GlobalTest, cache )
	{
		type::Handler::init();
		scope::Global global;

		source::String testsrc( "test", "dummy text" );
		source::Ref src( testsrc );

		cache::Locked lock( src );
		lock.addRaw( name_test, "Locked!" );
		lock.finalize();
		global.add( name_locked, &lock );

		cache::Static stat( src );
		stat.addRaw( name_test, "Static!" );
		stat.finalize();
		global.add( name_static, &stat );

		cache::Protected prot( src );
		prot.addRaw( name_test, "Protected!" );
		prot.finalize();
		global.add( name_protected, &prot );

		cache::Restricted rest( src );
		rest.addRaw( name_test, "Restricted!" );
		rest.finalize();
		global.add( name_restricted, &rest );

		cache::Open open( src );
		open.addRaw( name_test, "Open!" );
		open.finalize();
		global.add( name_open, &open );

		REQUIRE_TRUE( global.find( name_locked ) );
		auto l = (const cache::Locked*)global.find( name_locked );
		REQUIRE_TRUE( l->exists( name_test ) );
		REQUIRE_EQ( name_string, l->at( name_test ).type().name() );
		WANT_EQ( "Locked!", ( (StringInstance*)l->at( name_test ).value() )->value() );

		REQUIRE_TRUE( global.find( name_static ) );
		auto s = (const cache::Static*)global.find( name_static );
		REQUIRE_TRUE( s->exists( name_test ) );
		REQUIRE_EQ( name_string, s->at( name_test ).type().name() );
		WANT_EQ( "Static!", ( (StringInstance*)s->at( name_test ).value() )->value() );

		REQUIRE_TRUE( global.find( name_protected ) );
		auto p = (const cache::Protected*)global.find( name_protected );
		REQUIRE_TRUE( p->exists( name_test ) );
		REQUIRE_EQ( name_string, p->at( name_test ).type().name() );
		WANT_EQ( "Protected!", ( (StringInstance*)p->at( name_test ).value() )->value() );

		REQUIRE_TRUE( global.find( name_restricted ) );
		auto r = (const cache::Restricted*)global.find( name_restricted );
		REQUIRE_TRUE( r->exists( name_test ) );
		REQUIRE_EQ( name_string, r->at( name_test ).type().name() );
		WANT_EQ( "Restricted!", ( (StringInstance*)r->at( name_test ).value() )->value() );

		REQUIRE_TRUE( global.find( name_open ) );
		auto o = (const cache::Open*)global.find( name_open );
		REQUIRE_TRUE( o->exists( name_test ) );
		REQUIRE_EQ( name_string, o->at( name_test ).type().name() );
		WANT_EQ( "Open!", ( (StringInstance*)o->at( name_test ).value() )->value() );
	}
}
