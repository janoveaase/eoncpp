#include "Regression.h"



namespace eon
{
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
}
