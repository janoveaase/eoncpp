#include "Vector.h"
#include <eoninlinetest/InlineTest.h>


namespace eon
{
	EON_TEST( vector, vector, init,
		EON_EQ( "a,b,c,d", string( "," ).join( vector<string>( { "a", "b", "c", "d" } ) ) ) );

	EON_TEST( vector, vector, duplicat_element,
		EON_EQ( "a,a,a,a", string( "," ).join( vector<string>( size_t( 4 ), "a" ) ) ) );

	EON_TEST_2STEP( vector, vector, iterators,
		std::vector<string> input( { "a", "b", "c", "d" } ),
		EON_EQ( "abcd", string().join( vector<string>( input.begin(), input.end() ) ) ) );




	EON_NO_TEST( vector, reserve );

	EON_TEST_3STEP( vector, clear, basic,
		vector<int> obj( { 1, 2, 3, 4 } ),
		obj.clear(),
		EON_EQ( 0, obj.numElements() ) );

	EON_TEST_3STEP( vector, append, from_empty,
		vector<int> obj,
		obj.append( 1 ),
		EON_EQ( 1, obj.numElements() ) );
	EON_TEST_3STEP( vector, append, from_something,
		vector<int> obj( { 1, 2, 3 } ),
		obj.append( 4 ),
		EON_EQ( 4, obj.numElements() ) );

	EON_TEST_3STEP( vector, insert, empty,
		vector<string> obj,
		obj.insert( 0, "a" ),
		EON_EQ( "a", string().join( obj ) ) );
	EON_TEST_3STEP( vector, insert, front,
		vector<string> obj( { "a", "b", "c", "d" } ),
		obj.insert( 0, "x" ),
		EON_EQ( "xabcd", string().join( obj ) ) );
	EON_TEST_3STEP( vector, insert, mid,
		vector<string> obj( { "a", "b", "c", "d" } ),
		obj.insert( 2, "x" ),
		EON_EQ( "abxcd", string().join( obj ) ) );

	EON_TEST_3STEP( vector, pop, empty,
		vector<int> obj,
		obj.pop(),
		EON_EQ( 0, obj.numElements() ) );
	EON_TEST_3STEP( vector, pop, singleton,
		vector<int> obj( { 1 } ),
		obj.pop(),
		EON_EQ( 0, obj.numElements() ) );
	EON_TEST_3STEP( vector, pop, multiple,
		vector<int> obj( { 1, 2, 3, 4 } ),
		obj.pop(),
		EON_EQ( 3, obj.numElements() ) );

	EON_TEST_3STEP( vector, remove, empty,
		vector<int> obj,
		obj.remove( 0 ),
		EON_EQ( 0, obj.numElements() ) );
	EON_TEST_3STEP( vector, remove, singleton,
		vector<int> obj( { 1 } ),
		obj.remove( 0 ),
		EON_EQ( 0, obj.numElements() ) );
	EON_TEST_3STEP( vector, remove, multiple,
		vector<string> obj( { "a", "b", "c", "d", "e" } ),
		obj.remove( 2 ),
		EON_EQ( "abde", string().join( obj ) ) );

	EON_TEST_2STEP( vector, at, invalid_empty,
		vector<int> obj,
		EON_RAISE( obj.at( 0 ), std::out_of_range ) );
	EON_TEST_2STEP( vector, at, invalid_nonempty,
		vector<int> obj( { 1, 2, 3, 4 } ),
		EON_RAISE( obj.at( 4 ), std::out_of_range ) );
	EON_TEST_3STEP( vector, at, valid,
		vector<int> obj( { 1, 2, 3, 4 } ),
		obj.at( 2 ) = 6,
		EON_EQ( 6, obj.at( 2 ) ) );

	EON_TEST_3STEP( vector, operator_subscr, basic,
		vector<int> obj( { 2, 4, 6, 8, 10 } ),
		obj[ 2 ] = 5,
		EON_EQ( 5, obj[ 2 ] ) );




	EON_TEST( vector, operator_bool, false,
		EON_FALSE( vector<int>() ) );
	EON_TEST( vector, operator_bool, true,
		EON_TRUE( vector<int>( { 1, 2, 3 } ) ) );

	EON_TEST( vector, empty, true,
		EON_TRUE( vector<int>().empty() ) );
	EON_TEST_2STEP( vector, empty, false,
		vector<int> obj( { 1, 2, 3 } ),
		EON_FALSE( obj.empty() ) );

	EON_TEST( vector, full, true_empty,
		EON_TRUE( vector<int>().full() ) );
	EON_TEST_2STEP( vector, full, true_nonempty,
		vector<int> obj( size_t( 3 ), 5 ),
		EON_TRUE( obj.full() ) );
	EON_TEST_3STEP( vector, full, false_nonempty,
		vector<int> obj( size_t( 3 ), 5 ),
		obj.append( 6 ),
		EON_FALSE( obj.full() ) );

	EON_TEST( vector, numElements, empty,
		EON_EQ( 0, vector<int>().numElements() ) );
	EON_TEST( vector, numElements, singleton,
		EON_EQ( 1, vector<int>( size_t( 1 ), 5 ).numElements() ) );
	EON_TEST( vector, numElements, many,
		EON_EQ( 5, vector<int>( size_t( 5 ), 5 ).numElements() ) );

	EON_TEST_2STEP( vector, lastPos, empty,
		vector<int> obj,
		EON_EQ( 0, obj.lastPos() ) );
	EON_TEST_2STEP( vector, lastPos, singleton,
		vector<int> obj( { 1 } ),
		EON_EQ( 0, obj.lastPos() ) );
	EON_TEST_2STEP( vector, lastPos, many,
		vector<int> obj( { 1, 2, 3, 4, 5 } ),
		EON_EQ( 4, obj.lastPos() ) );

	EON_TEST( vector, capacity, empty,
		EON_EQ( 0, vector<int>().capacity() ) );
	EON_TEST( vector, capacity, fixed,
		EON_EQ( 5, vector<int>( size_t( 5 ), 5 ).capacity() ) );


	EON_TEST_2STEP( vector, begin, empty,
		vector<int> obj,
		EON_EQ( obj.end(), obj.begin() ) );
	EON_TEST_2STEP( vector, begin, nonempty,
		vector<int> obj( { 1, 2, 3, 4 } ),
		EON_EQ( 1, *obj.begin() ) );
	EON_TEST_3STEP( vector, begin, modify,
		vector<int> obj( { 1, 2, 3, 4 } ),
		*obj.begin() = 5,
		EON_EQ( 5, obj[ 0 ] ) );

	EON_TEST_2STEP( vector, cbegin, nonempty,
		vector<int> obj( { 1, 2, 3, 4 } ),
		EON_EQ( 1, *obj.cbegin() ) );

	EON_TEST_3STEP( vector, end, nonempty_modify,
		vector<int> obj( { 1, 2, 3, 4 } ),
		*( obj.end() - 1 ) = 6,
		EON_EQ( 6, obj.at( 3 ) ) );
}
