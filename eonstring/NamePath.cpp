#include "NamePath.h"


namespace eon
{
	EON_TEST( namepath, empty, empty,
		EON_TRUE( namepath().empty() ) );
	EON_TEST( namepath, empty, nonempty,
		EON_FALSE( namepath( { name_and, name_or } ).empty() ) );

	EON_TEST( namepath, operator_bool, empty,
		EON_FALSE( namepath() ) );
	EON_TEST( namepath, operator_bool, nonempty,
		EON_TRUE( namepath( { name_and, name_or } ) ) );

	EON_TEST( namepath, numElms, empty,
		EON_EQ( 0, namepath().numElms() ) );
	EON_TEST( namepath, numElms, size_one,
		EON_EQ( 1, namepath( { name_and } ).numElms() ) );
	EON_TEST( namepath, numElms, size_three,
		EON_EQ( 3, namepath( { name_and, name_or, name_as } ).numElms() ) );

	EON_TEST( namepath, at, empty,
		EON_EQ( no_name, namepath().at( 0 ) ) );
	EON_TEST( namepath, at, first,
		EON_EQ( name_and, namepath( { name_and, name_or, name_as } ).at( 0 ) ) );
	EON_TEST( namepath, at, middle,
		EON_EQ( name_or, namepath( { name_and, name_or, name_as } ).at( 1 ) ) );
	EON_TEST( namepath, at, last,
		EON_EQ( name_as, namepath( { name_and, name_or, name_as } ).at( 2 ) ) );
	EON_TEST( namepath, at, beyond,
		EON_EQ( no_name, namepath( { name_and, name_or, name_as } ).at( 3 ) ) );

	EON_TEST( namepath, last, empty,
		EON_EQ( no_name, namepath().last() ) );
	EON_TEST( namepath, last, nonempty,
		EON_EQ( name_as, namepath( { name_and, name_or, name_as } ).last() ) );

	EON_TEST( namepath, chopFirst, empty,
		EON_EQ( "@", namepath().chopFirst().str() ) );
	EON_TEST( namepath, chopFirst, singleton,
		EON_EQ( "@", namepath( { name_and } ).chopFirst().str() ) );
	EON_TEST( namepath, chopFirst, dual,
		EON_EQ( "@or", namepath( { name_and, name_or } ).chopFirst().str() ) );
	EON_TEST( namepath, chopFirst, trio,
		EON_EQ( "@or/as", namepath( { name_and, name_or, name_as } ).chopFirst().str() ) );

	EON_TEST( namepath, chopLast, empty,
		EON_EQ( "@", namepath().chopLast().str() ) );
	EON_TEST( namepath, chopLast, singleton,
		EON_EQ( "@", namepath( { name_and } ).chopLast().str() ) );
	EON_TEST( namepath, chopLast, dual,
		EON_EQ( "@and", namepath( { name_and, name_or } ).chopLast().str() ) );
	EON_TEST( namepath, chopLast, trio,
		EON_EQ( "@and/or", namepath( { name_and, name_or, name_as } ).chopLast().str() ) );

	EON_TEST( namepath, str, empty,
		EON_EQ( "@", namepath().str() ) );
	EON_TEST( namepath, str, singleton,
		EON_EQ( "@and", namepath( { name_and } ).str() ) );
	EON_TEST( namepath, str, full,
		EON_EQ( "@and/or/as", namepath( { name_and, name_or, name_as } ).str() ) );

	EON_TEST( namepath, hash, empty,
		EON_EQ( 0, namepath().hash() ) );
	EON_TEST( namepath, hash, nonempty,
		EON_NE( 0, namepath( { name_and, name_or, name_as } ).hash() ) );


	int namepath::compare( const namepath& other ) const noexcept
	{
		int i = 1;
		for( ; static_cast<index_t>( i ) <= Value.size() && static_cast<index_t>( i ) <= other.Value.size(); ++i )
		{
			auto cmp = eon::str( Value[ i - 1 ] ).compare( eon::str( other.Value[ i - 1 ] ) );
			if( cmp < 0 )
				return -i;
			else if( cmp > 0 )
				return i;
		}
		if( i == Value.size() + 1 && i == other.Value.size() + 1 )
			return 0;
		else
			return i == Value.size() + 1 ? -i : i;
	}
	EON_TEST( namepath, compare, empty,
		EON_EQ( 0, namepath().compare( namepath() ) ) );
	EON_TEST( namepath, compare, equal,
		EON_EQ( 0, namepath( { name_and, name_or, name_as } ).compare( namepath( { name_and, name_or, name_as } ) ) ) );
	EON_TEST( namepath, compare, lessthan,
		EON_EQ( -2, namepath( { name_and, name_as, name_or } ).compare( namepath( { name_and, name_or, name_as } ) ) ) );
	EON_TEST( namepath, compare, greaterthan,
		EON_EQ( 2, namepath( { name_and, name_or, name_as } ).compare( namepath( { name_and, name_as, name_or } ) ) ) );
	EON_TEST( namepath, compare, lessthan_contents,
		EON_EQ( -3, namepath( { name_and, name_or } ).compare( namepath( { name_and, name_or, name_as } ) ) ) );
	EON_TEST( namepath, compare, greaterthan_contents,
		EON_EQ( 3, namepath( { name_and, name_or, name_as } ).compare( namepath( { name_and, name_or } ) ) ) );

	EON_TEST( namepath, operator_lt, basic,
		EON_LT( namepath( { name_and, name_as } ), namepath( { name_and, name_or } ) ) );
	EON_TEST( namepath, operator_le, basic,
		EON_LE( namepath( { name_and, name_as } ), namepath( { name_and, name_or } ) ) );
	EON_TEST( namepath, operator_gt, basic,
		EON_GT( namepath( { name_and, name_or } ), namepath( { name_and, name_as } ) ) );
	EON_TEST( namepath, operator_ge, basic,
		EON_GE( namepath( { name_and, name_or } ), namepath( { name_and, name_as } ) ) );
	EON_TEST( namepath, operator_eq, basic,
		EON_EQ( namepath( { name_and, name_or } ), namepath( { name_and, name_or } ) ) );
	EON_TEST( namepath, operator_ne, less,
		EON_NE( namepath( { name_and, name_as } ), namepath( { name_and, name_or } ) ) );
	EON_TEST( namepath, operator_ne, greater,
		EON_NE( namepath( { name_and, name_or } ), namepath( { name_and, name_as } ) ) );




	namepath& namepath::operator=( const string& ref )
	{
		string name;
		for( auto c : ref )
		{
			if( c == ForwSlashChr )
			{
				if( !name.empty() && !name.isUInt() )
					add( eon::name( std::move( name ) ) );
			}
			else if( string::isLetter( c ) || string::isNumberAsciiDigit( c ) || c == '_' )
				name += c;
		}
		if( !name.empty() && !name.isUInt() )
			add( eon::name( std::move( name ) ) );
		return *this;
	}
	EON_TEST( namepath, operator_asgn_str, legal,
		EON_EQ( "@and/or/as", ( namepath() = "@and/or/as" ).str() ) );
	EON_TEST( namepath, operator_asgn_str, illegal,
		EON_EQ( "@and/or/as", ( namepath() = "@an#d/o!r/a,s" ).str() ) );
	EON_TEST( namepath, operator_asgn_str, empty_elements,
		EON_EQ( "@and/or/as", ( namepath() = "@and//or//as" ).str() ) );

	EON_TEST( namepath, operator_asgn_init, empty,
		EON_EQ( "@", ( namepath() = {} ).str() ) );
	EON_TEST( namepath, operator_asgn_init, singleton,
		EON_EQ( "@and", ( namepath() = { name_and } ).str() ) );
	EON_TEST( namepath, operator_asgn_init, triple,
		EON_EQ( "@and/or/as", ( namepath() = { name_and, name_or, name_as } ).str() ) );

	EON_TEST_3STEP( namepath, clear, empty,
		namepath obj,
		obj.clear(),
		EON_TRUE( obj.empty() ) );
	EON_TEST_3STEP( namepath, clear, nonempty,
		auto obj = namepath( { name_and, name_or, name_as } ),
		obj.clear(),
		EON_TRUE( obj.empty() ) );

	EON_TEST_3STEP( namepath, add, to_empty,
		namepath obj,
		obj.add( name_and ),
		EON_EQ( "@and", obj.str() ) );
	EON_TEST_3STEP( namepath, add, to_singleton,
		namepath obj{ name_and },
		obj.add( name_or ),
		EON_EQ( "@and/or", obj.str() ) );
	EON_TEST_3STEP( namepath, add, to_longer,
		auto obj = namepath( { name_and, name_or } ),
		obj.add( name_as ),
		EON_EQ( "@and/or/as", obj.str() ) );
}
