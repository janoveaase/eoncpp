#include "Regression.h"
#include <string>



namespace eon
{
	TEST( VectorTest, basic )
	{
		vector<std::string> strings{ "alpha", "beta", "gamma", "delta" };
		REQUIRE_EQ( 4, strings.size() );
		WANT_EQ( "alpha", strings[ 0 ] );
		WANT_EQ( "beta", strings[ 1 ] );
		WANT_EQ( "gamma", strings[ 2 ] );
		WANT_EQ( "delta", strings[ 3 ] );
	}
	TEST( VectorTest, negative_positions )
	{
		vector<std::string> strings{ "alpha", "beta", "gamma", "delta" };
		REQUIRE_EQ( 4, strings.size() );
		WANT_EQ( "delta", strings[ -1 ] );
		WANT_EQ( "gamma", strings[ -2 ] );
		WANT_EQ( "beta", strings[ -3 ] );
		WANT_EQ( "alpha", strings[ -4 ] );
	}

	TEST( VectorTest, slice )
	{
		vector<std::string> strings{ "alpha", "beta", "gamma", "delta" };

		auto norm = strings.slice( 1, 2 );
		REQUIRE_EQ( 2, norm.size() );
		WANT_EQ( "beta", norm[ 0 ] );
		WANT_EQ( "gamma", norm[ 1 ] );

		auto revs = strings.slice( -2, -3 );
		REQUIRE_EQ( 2, revs.size() );
		WANT_EQ( "gamma", revs[ 0 ] );
		WANT_EQ( "beta", revs[ 1 ] );
	}

	TEST( VectorTest, contains )
	{
		vector<std::string> strings{ "alpha", "beta", "gamma", "delta" };
		REQUIRE_EQ( 4, strings.size() );
		WANT_TRUE( strings.contains( "alpha" ) );
		WANT_TRUE( strings.contains( "beta" ) );
		WANT_TRUE( strings.contains( "gamma" ) );
		WANT_TRUE( strings.contains( "delta" ) );
		WANT_FALSE( strings.contains( "epsilon" ) );
	}


	TEST( StackTest, basic )
	{
		stack<std::string> strings{ "alpha", "beta", "gamma", "delta" };
		REQUIRE_EQ( 4, strings.size() );
		WANT_EQ( "delta", strings.top() );
		WANT_EQ( "delta", strings.at( 0 ) );
		WANT_EQ( "gamma", strings.at( 1 ) );
		WANT_EQ( "beta", strings.at( 2 ) );
		WANT_EQ( "alpha", strings.at( 3 ) );
	}
	TEST( StackTest, negative_positions )
	{
		stack<std::string> strings{ "alpha", "beta", "gamma", "delta" };
		REQUIRE_EQ( 4, strings.size() );
		WANT_EQ( "alpha", strings.at( -1 ) );
		WANT_EQ( "beta", strings.at( -2 ) );
		WANT_EQ( "gamma", strings.at( -3 ) );
		WANT_EQ( "delta", strings.at( -4 ) );
	}
}
