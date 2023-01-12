#include "Regression.h"
#include <eonstring/String.h>



namespace eon
{
	class dummy
	{
	public:
		dummy() = default;
		dummy( int x )
		{
			X = x;
		}
		dummy( const dummy& other ) noexcept
		{
			X = other.X;
		}
		~dummy()
		{
			X = 0;
		}

		int X = 0;
	};

	TEST( VectorTest, basic )
	{
		vector<dummy> vec;
		vec.append( dummy( 1 ) );
		vec.append( dummy( 3 ) );
		REQUIRE_EQ( 2, vec.numElements() );
		WANT_EQ( 1, vec[ 0 ].X );
		WANT_EQ( 3, vec[ 1 ].X );

		vec.insert( 1, dummy( 2 ) );
		REQUIRE_EQ( 3, vec.numElements() );
		WANT_EQ( 1, vec[ 0 ].X );
		WANT_EQ( 2, vec[ 1 ].X );
		WANT_EQ( 3, vec[ 2 ].X );

		vec.remove( 1 );
		REQUIRE_EQ( 2, vec.numElements() );
		WANT_EQ( 1, vec[ 0 ].X );
		WANT_EQ( 3, vec[ 1 ].X );
	}
	TEST( VectorTest, str )
	{
		vector<string> strings{ "alpha", "beta", "gamma", "delta" };
		REQUIRE_EQ( 4, strings.numElements() );
		WANT_EQ( "alpha", strings[ 0 ] );
		WANT_EQ( "beta", strings[ 1 ] );
		WANT_EQ( "gamma", strings[ 2 ] );
		WANT_EQ( "delta", strings[ 3 ] );
		strings.pop();
		REQUIRE_EQ( 3, strings.numElements() );
		WANT_EQ( "gamma", strings[ strings.lastPos() ] );
		strings.remove( 1 );
		REQUIRE_EQ( 2, strings.numElements() );
		WANT_EQ( "alpha", strings[ 0 ] );
		WANT_EQ( "gamma", strings[ 1 ] );
	}
	TEST( VectorTest, iterate )
	{
		vector<string> strings{ "alpha", "beta", "gamma", "delta" };
		REQUIRE_EQ( 4, strings.numElements() );
		string act;
		for( auto& elm : strings )
			act += elm;
		WANT_EQ( "alphabetagammadelta", act );
	}




	TEST( StackTest, basic )
	{
		stack<int> data;
		WANT_TRUE( data.empty() );
		data.push( 1 );
		WANT_FALSE( data.empty() );
		WANT_EQ( 1, data.size() );
		data.push( 2 );
		WANT_FALSE( data.empty() );
		WANT_EQ( 2, data.size() );
		data.pop();
		WANT_FALSE( data.empty() );
		WANT_EQ( 1, data.size() );
		data.pop();
		WANT_TRUE( data.empty() );
	}

	TEST( StackTest, literal )
	{
		stack<int> data{ 1, 2, 3, 4 };
		REQUIRE_EQ( 4, data.size() );
		WANT_EQ( 4, data.top() );
		data.pop();
		WANT_EQ( 3, data.top() );
		data.pop();
		WANT_EQ( 2, data.top() );
		data.pop();
		WANT_EQ( 1, data.top() );
	}

	TEST( StackTest, concatenate )
	{
		stack<int> data{ 1, 4 };
		stack<int> data2{ 2, 3 };
		data += data2;
		REQUIRE_EQ( 4, data.size() );
		WANT_EQ( 2, data2.size() );

		WANT_EQ( 3, data.top() );
		data.pop();
		WANT_EQ( 2, data.top() );
		data.pop();
		WANT_EQ( 4, data.top() );
		data.pop();
		WANT_EQ( 1, data.top() );
	}
	TEST( StackTest, transfer )
	{
		stack<int> data{ 1, 4 };
		stack<int> data2{ 2, 3 };
		data += std::move( data2 );
		REQUIRE_EQ( 4, data.size() );
		WANT_EQ( 0, data2.size() );

		WANT_EQ( 3, data.top() );
		data.pop();
		WANT_EQ( 2, data.top() );
		data.pop();
		WANT_EQ( 4, data.top() );
		data.pop();
		WANT_EQ( 1, data.top() );
	}




	TEST( ReflexiveMapTest, basic )
	{
		reflexive_map<int, char> data;
		WANT_TRUE( data.empty() );
		data.insert( 6, '6' );
		WANT_FALSE( data.empty() );
		WANT_EQ( 1, data.size() );
		data.insert( 9, '9' );
		WANT_FALSE( data.empty() );
		WANT_EQ( 2, data.size() );
		data.removeOne( 6 );
		WANT_FALSE( data.empty() );
		WANT_EQ( 1, data.size() );
		data.removeTwo( '9' );
		WANT_TRUE( data.empty() );
	}

	TEST( ReflexiveMapTest, literal )
	{
		reflexive_map<int, char> data{ { 1, '1' }, { 2, '2' }, { 3, '3' }, { 4, '4' } };
		REQUIRE_EQ( 4, data.size() );
		WANT_EQ( '4', data.one( 4 ) );
		WANT_EQ( 4, data.two( '4' ) );
		WANT_EQ( '3', data.one( 3 ) );
		WANT_EQ( 3, data.two( '3' ) );
		WANT_EQ( '2', data.one( 2 ) );
		WANT_EQ( 2, data.two( '2' ) );
		WANT_EQ( '1', data.one( 1 ) );
		WANT_EQ( 1, data.two( '1' ) );
	}
}
