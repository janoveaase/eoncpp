#include "Regression.h"



namespace eon
{
	TEST( RegExTest, parse_ok_01 )
	{
		regex expr;
		WANT_NO_EXCEPT( expr = R"(^\w+|\d+alpha{2}$)" );
	}
	TEST( RegExTest, parse_error_01 )
	{
		regex expr;
		WANT_EXCEPT( expr = R"(^\w+|\d+alpha{2$)", rx::InvalidExpression );
	}

	TEST( RegExTest, parse_ok_02 )
	{
		regex expr;
		WANT_NO_EXCEPT( expr = R"(@<spaces>(\s*?)..@:<spaces>)" );
	}
	TEST( RegExTest, parse_error_02 )
	{
		regex expr;
		WANT_EXCEPT( expr = R"(@<spaces>(\s*?)..@:<spa!ces>)", rx::InvalidExpression );
	}

	TEST( RegExTest, removeDuplicates )
	{
		regex ex{ R"(a+a+)" };
		WANT_EQ( "a{2,}", ex.strStruct().stdstr() );
		ex = regex{ R"(a+a*)" };
		WANT_EQ( "a+", ex.strStruct().stdstr() );
		ex = regex{ R"(a*a+)" };
		WANT_EQ( "a+", ex.strStruct().stdstr() );
		ex = regex{ R"(a+a+?)" };
		WANT_EQ( "a{2,}", ex.strStruct().stdstr() );
		ex = regex{ R"(a+?a+)" };
		WANT_EQ( "a{2,}", ex.strStruct().stdstr() );
		ex = regex{ R"(a+?a+?)" };
		WANT_EQ( "a{2,}?", ex.strStruct().stdstr() );
		ex = regex{ R"((a+a+))" };
		WANT_EQ( "(a{2,})", ex.strStruct().stdstr() );
		ex = regex{ R"(a+a+a+a+)" };
		WANT_EQ( "a{4,}", ex.strStruct().stdstr() );
		ex = regex{ R"(a+?a+?a+?a+?)" };
		WANT_EQ( "a{4,}?", ex.strStruct().stdstr() );
		ex = regex{ R"((a+a+)(a+a+))" };
		WANT_EQ( "(a{2,}){2}", ex.strStruct().stdstr() );
		ex = regex{ R"(a{1,5}a{1,5})" };
		WANT_EQ( "a{2,10}", ex.strStruct().stdstr() );
		ex = regex{ R"(a{1,5}a{2,6})" };
		WANT_EQ( "a{3,11}", ex.strStruct().stdstr() );
		ex = regex{ R"(a{2,5}a{1,5})" };
		WANT_EQ( "a{3,10}", ex.strStruct().stdstr() );
		ex = regex{ R"(\d\d+:)" };
		WANT_EQ( "\\d{2,}:", ex.strStruct().stdstr() );
		ex = regex{ R"((x+x+)+y)" };
		WANT_EQ( "(x{2,})+y", ex.strStruct().stdstr() );
		ex = regex{ R"((x+?x+?)+?y)" };
		WANT_EQ( "(x{2,}?)+?y", ex.strStruct().stdstr() );
	}


	TEST( RegExTest, match_any_single )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(.)" ) << "Failed to parse";

		WANT_TRUE( expr.match( "8" ) ) << "Didn't match digit";
		WANT_TRUE( expr.match( "b" ) ) << "Didn't match letter";
		WANT_TRUE( expr.match( "β" ) ) << "Didn't match unicode letter";
		WANT_TRUE( expr.match( "_" ) ) << "Didn't match underscore";
	}
	TEST( RegExTest, match_any_two )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(..)" ) << "Failed to parse";

		WANT_TRUE( expr.match( "82" ) ) << "Didn't match digits";
		WANT_TRUE( expr.match( "bx" ) ) << "Didn't match letters";
		WANT_TRUE( expr.match( "βγ" ) ) << "Didn't match unicode letters";
		WANT_TRUE( expr.match( "__" ) ) << "Didn't match underscores";
		WANT_FALSE( expr.match( "a" ) ) << "Matched single";
	}
	TEST( RegExTest, match_any_three )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(.{3})" ) << "Failed to parse";

		WANT_TRUE( expr.match( "8bβ" ) ) << "Didn't match 3";
		WANT_FALSE( expr.match( "8β" ) ) << "Matched 2";
		WANT_FALSE( expr.match( "β" ) ) << "Matched 1";
	}
	TEST( RegExTest, match_any_range )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(.{2,3})" ) << "Failed to parse";

		WANT_TRUE( expr.match( "8bβ" ) ) << "Didn't match 3";
		WANT_TRUE( expr.match( "8b" ) ) << "Didn't match 2";
		WANT_FALSE( expr.match( "β" ) ) << "Matched 1";
	}
	TEST( RegExTest, match_any_range_fixed )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(^.{2,3}$)" ) << "Failed to parse";

		WANT_TRUE( expr.match( "8bβ" ) ) << "Didn't match 3";
		WANT_TRUE( expr.match( "8b" ) ) << "Didn't match 2";
		WANT_FALSE( expr.match( "β" ) ) << "Matched 1";
		WANT_FALSE( expr.match( "8bβ_" ) ) << "Matched 4";
	}

	TEST( RegExTest, match_digit_single )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(\d)" ) << "Failed to parse";

		WANT_TRUE( expr.match( "8" ) ) << "Didn't match digit";
		WANT_FALSE( expr.match( "b" ) ) << "Matched letter";
	}
	TEST( RegExTest, match_digit_single_not )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(!\d)" ) << "Failed to parse";

		WANT_TRUE( expr.match( "b" ) ) << "Didn't match letter";
		WANT_FALSE( expr.match( "8" ) ) << "Matched digit";
	}
	TEST( RegExTest, match_digit_two )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(\d\d)" ) << "Failed to parse";

		WANT_TRUE( expr.match( "82" ) ) << "Didn't match digit";
		WANT_FALSE( expr.match( "6b" ) ) << "Match with letter";
		WANT_FALSE( expr.match( "3" ) ) << "Matched single";
	}

	TEST( RegExTest, match_wordchar_single )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(\w)" ) << "Failed to parse";

		WANT_TRUE( expr.match( "8" ) ) << "Didn't match digit";
		WANT_TRUE( expr.match( "b" ) ) << "Didn't match letter";
		WANT_TRUE( expr.match( "β" ) ) << "Didn't match unicode letter";
		WANT_TRUE( expr.match( "_" ) ) << "Didn't match underscore";
		WANT_FALSE( expr.match( "." ) ) << "Matched punctuation";
	}
	TEST( RegExTest, match_wordchar_two )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(\w\w)" ) << "Failed to parse";

		WANT_TRUE( expr.match( "8b" ) ) << "Didn't match digit + letter";
		WANT_TRUE( expr.match( "_8" ) ) << "Didn't match underscore + digit";
		WANT_TRUE( expr.match( "βγ" ) ) << "Didn't match unicode letters";
		WANT_TRUE( expr.match( "__" ) ) << "Didn't match underscores";
		WANT_FALSE( expr.match( ".d" ) ) << "Matched punctuation";
		WANT_FALSE( expr.match( "3" ) ) << "Matched single";
	}

	TEST( RegExTest, match_nonwordchar_single )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(\W)" ) << "Failed to parse";

		WANT_FALSE( expr.match( "8") )<< "Matched digit";
		WANT_FALSE( expr.match( "b") )<< "Matched letter";
		WANT_FALSE( expr.match( "β") )<< "Matched unicode letter";
		WANT_FALSE( expr.match( "_") )<< "Matched underscore";
		WANT_TRUE( expr.match( ".") )<< "Didn't match punctuation";
	}

	TEST( RegExTest, match_chargroup_single )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"([aβc])" ) << "Failed to parse";

		WANT_TRUE( expr.match( "a" ) ) << "Didn't match 'a'";
		WANT_TRUE( expr.match( "β" ) ) << "Didn't match unicode letter";
		WANT_TRUE( expr.match( "c" ) ) << "Didn't match 'c'";
		WANT_FALSE( expr.match( "d" ) ) << "Matched 'd'";
	}

	TEST( RegExTest, match_capture_greedy_1 )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(.*@<key>(\w+): @<value>(\w+).*)" ) << "Failed to parse";
		rx::match result;

		WANT_TRUE( result = expr.match( "one: two" ) ) << "Didn't match basic";
		WANT_EQ( "e", result.group( name_key ).stdstr() ) << "Wrong key for basic";
		WANT_EQ( 2, result.group( name_key ).begin().numChar() ) << "Wrong start position for basic";
		WANT_EQ( "two", result.group( name_value ).stdstr() ) << "Wrong value for basic";

		WANT_TRUE( result = expr.match( "  alpha: beta  " ) ) << "Didn't match centered";
		WANT_EQ( "a", result.group( name_key ).stdstr() ) << "Wrong key for centered";
		WANT_EQ( "beta", result.group( name_value ).stdstr() ) << "Wrong value for centered";

		WANT_TRUE( result = expr.match( "βγέ: ΨΩΣ" ) ) << "Didn't match unicode";
		WANT_EQ( "έ", result.group( name_key ).stdstr() ) << "Wrong key for unicode";
		WANT_EQ( "ΨΩΣ", result.group( name_value ).stdstr() ) << "Wrong value for unicode";
	}
	TEST( RegExTest, match_capture_greedy_2 )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(\W*@<key>(\w+): @<value>(\w+)\W*)" ) << "Failed to parse";
		rx::match result;

		WANT_TRUE( result = expr.match( "one: two" ) ) << "Didn't match basic";
		WANT_EQ( "one", result.group( name_key ).stdstr() ) << "Wrong key for basic";
		WANT_EQ( "two", result.group( name_value ).stdstr() ) << "Wrong value for basic";

		WANT_TRUE( result = expr.match( "  alpha: beta  " ) ) << "Didn't match centered";
		WANT_EQ( "alpha", result.group( name_key ).stdstr() ) << "Wrong key for centered";
		WANT_EQ( "beta", result.group( name_value ).stdstr() ) << "Wrong value for centered";

		WANT_TRUE( result = expr.match( "βγέ: ΨΩΣ" ) ) << "Didn't match unicode";
		WANT_EQ( "βγέ", result.group( name_key ).stdstr() ) << "Wrong key for unicode";
		WANT_EQ( "ΨΩΣ", result.group( name_value ).stdstr() ) << "Wrong value for unicode";
	}
	TEST( RegExTest, match_capture_nongreedy )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(.*?@<key>(\w+): @<value>(\w+).*?)" ) << "Failed to parse";
		rx::match result;

		WANT_TRUE( result = expr.match( "one: two" ) ) << "Didn't match basic";
		WANT_EQ( "one", result.group( name_key ).stdstr() ) << "Wrong key for basic";
		WANT_EQ( "two", result.group( name_value ).stdstr() ) << "Wrong value for basic";

		WANT_TRUE( result = expr.match( "  alpha: beta  " ) ) << "Didn't match centered";
		WANT_EQ( "alpha", result.group( name_key ).stdstr() ) << "Wrong key for centered";
		WANT_EQ( "beta", result.group( name_value ).stdstr() ) << "Wrong value for centered";

		WANT_TRUE( result = expr.match( "βγέ: ΨΩΣ" ) ) << "Didn't match unicode";
		WANT_EQ( "βγέ", result.group( name_key ).stdstr() ) << "Wrong key for unicode";
		WANT_EQ( "ΨΩΣ", result.group( name_value ).stdstr() ) << "Wrong value for unicode";
	}
	TEST( RegExTest, match_capture_issue1 )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = "^.*?@<m>(p.*i).*$" ) << "Failed to parse";
		rx::match result;

		WANT_FALSE( result = expr.match( "alpha" ) ) << "Matched alpha";
		WANT_TRUE( result = expr.match( "pi" ) ) << "Didn't match pi";
		if( result )
			WANT_EQ( "pi", string( result.group( eon::name::get( "m" ) ) ).stdstr() ) << "Wrong capture for pi";
		WANT_TRUE( result = expr.match( "spring" ) ) << "Didn't match spring";
		if( result )
			WANT_EQ( "pri", string( result.group( eon::name::get( "m" ) ) ).stdstr() ) << "Wrong capture for spring";
	}

	TEST( RegExTest, match_newline_tab )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(one\n\ttwo)" ) << "Failed to parse";
		rx::match result;

		WANT_TRUE( result = expr.match( "one\n	two" ) );
		WANT_FALSE( result = expr.match( "one\n  two" ) );
	}

	TEST( RegExTest, match_name1 )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"((\w+){name})" ) << "Failed to parse";
		rx::match result;

		WANT_TRUE( result = expr.match( "12_" ) );
		WANT_FALSE( result = expr.match( "123" ) );
	}
	TEST( RegExTest, match_name2 )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(\s+(\w+){name}\s+)" ) << "Failed to parse";
		rx::match result;

		WANT_TRUE( result = expr.match( "   12_   " ) );
		WANT_FALSE( result = expr.match( " 123     " ) );
	}

	TEST( MatchOrTest, simple_binary )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(a|b)" ) << "Failed to parse";
		rx::match result;

		WANT_TRUE( result = expr.match( "a" ) ) << "Didn't match 'a'";
		WANT_TRUE( result = expr.match( "b" ) ) << "Didn't match 'b'";
		WANT_FALSE( result = expr.match( "c" ) ) << "Didn't fail to match 'c'";
	}
	TEST( MatchOrTest, simple_multi )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(a|b|c|d)" ) << "Failed to parse";
		rx::match result;

		WANT_TRUE( result = expr.match( "a" ) ) << "Didn't match 'a'";
		WANT_TRUE( result = expr.match( "b" ) ) << "Didn't match 'b'";
		WANT_TRUE( result = expr.match( "c" ) ) << "Didn't match 'c'";
		WANT_TRUE( result = expr.match( "d" ) ) << "Didn't match 'd'";
		WANT_FALSE( result = expr.match( "e" ) ) << "Didn't fail to match 'e'";
	}
	TEST( MatchOrTest, complex_binary )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(alpha|beta)" ) << "Failed to parse";
		rx::match result;

		WANT_TRUE( result = expr.match( "alpha" ) ) << "Didn't match 'alpha'";
		WANT_TRUE( result = expr.match( "beta" ) ) << "Didn't match 'beta'";
		WANT_FALSE( result = expr.match( "gamma" ) ) << "Didn't fail to match 'gamma'";
	}
	TEST( MatchOrTest, complex_multi )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(alpha|beta|gamma|delta)" ) << "Failed to parse";
		rx::match result;

		WANT_TRUE( result = expr.match( "alpha" ) ) << "Didn't match 'alpha'";
		WANT_TRUE( result = expr.match( "beta" ) ) << "Didn't match 'beta'";
		WANT_TRUE( result = expr.match( "gamma" ) ) << "Didn't match 'gamma'";
		WANT_TRUE( result = expr.match( "delta" ) ) << "Didn't match 'delta'";
		WANT_FALSE( result = expr.match( "epsilon" ) ) << "Didn't fail to match 'epsilon'";
	}
	TEST( MatchOrTest, group_binary )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"((alpha|beta){1,2})" ) << "Failed to parse";
		rx::match result;

		WANT_TRUE( result = expr.match( "alpha" ) ) << "Didn't match 'alpha'";
		WANT_TRUE( result = expr.match( "beta" ) ) << "Didn't match 'beta'";
		WANT_TRUE( result = expr.match( "alphabeta" ) ) << "Didn't match 'alphabeta'";
		WANT_TRUE( result = expr.match( "alphaalpha" ) ) << "Didn't match 'alphaalpha'";
		WANT_TRUE( result = expr.match( "betabeta" ) ) << "Didn't match 'betabeta'";
		WANT_FALSE( result = expr.match( "gamma" ) ) << "Didn't fail to match 'gamma'";
	}


	TEST( MiscTests, basic_match )
	{
		string str_good{ "Eon is great!" };
		string str_bad{ "Eon is not great!" };
		regex expr{ R"(^(\w+) is (\w+)(.)$)" };
		WANT_TRUE( expr.match( str_good ) );
		WANT_FALSE( expr.match( str_bad ) );
	}
	TEST( MiscTests, group_match )
	{
		string str_good{ "Eon is great!" };
		regex expr{ R"(@<one>(\w+) is @<2_>(\w+)@<three>(.))" };
		
		rx::match match;
		REQUIRE_NO_EXCEPT( match = expr.match( str_good ) );
		WANT_TRUE( match );
		REQUIRE_EQ( 4, match.size() ) << "Wrong number of groups";
		WANT_EQ( str_good.stdstr(), match.all().stdstr() ) << "Wrong complete match";
		WANT_EQ( "Eon", match.group( name::get( "one" ) ).stdstr() ) << "Wrong first group";
		WANT_EQ( "great", match.group( name::get( "2_" ) ).stdstr() ) << "Wrong second group";
		WANT_EQ( "!", match.group( name::get( "three" ) ).stdstr() ) << "Wrong third group";
	}

	TEST( MiscTests, utf8_match )
	{
		string str_good, str_bad;
		str_good += char_t( 0xC6 );
		str_good += "on is great!";
		str_bad += char_t( 0xC6 );
		str_bad += "Eon is not great!";
		regex expr{ R"(^(\w+) is (\w+)(.)$)" };
		WANT_TRUE( expr.match( str_good ) );
		WANT_FALSE( expr.match( str_bad ) );
	}

	TEST( MiscTests, horror1 )
	{
		regex expr{ R"((x+x+)+y)" };
		WANT_FALSE( expr.match( "xxxxxxxxxxxxxxxxxxxx" ) );
	}
	TEST( MiscTests, horror2 )
	{
		regex expr{ R"((x+?x+?)+?y)" };
		WANT_FALSE( expr.match( "xxxxxxxxxxxxxxxxxxxx" ) );
	}

	TEST( MiscTests, repetitions1 )
	{
		regex expr{ R"(\d\d+:)" };
		WANT_FALSE( expr.match( "9:" ) );
		WANT_TRUE( expr.match( "99:" ) );
		WANT_TRUE( expr.match( "999:" ) );
		WANT_TRUE( expr.match( "9999:" ) );
	}

	TEST( MiscTests, advanced1 )
	{
		string str{ "alpha: 1:99:7 beta ." };
		regex expr{ R"(^\w{2,6}: \d\d?:\d\d?:\d\d? (alpha|beta){1,2} \s*\.$)" };
		WANT_TRUE( expr.match( str ) );
	}


/*	TEST( OptimizeTests, basic )
	{
		std::chrono::steady_clock clock;
		string str{ "alpha: 1:99:7 beta ." };

		int iterations = 10000;
#ifdef _DEBUG
		iterations /= 10;
#endif
		std::chrono::steady_clock::time_point start, end;
		std::chrono::nanoseconds time1{ 0 }, time2{ 0 };

		regex expr1{ R"(^\w{2,6}: \d\d?:\d\d?:\d\d? (alpha|beta){1,2} \s*\.$)" };
		regex expr2( R"(^\w{2,6}: \d\d?:\d\d?:\d\d? (alpha|beta){1,2} \s*\.$)", "o" );
		int matches1 = 0, matches2 = 0;
		std::cout << "\n";

		for( auto i = 0; i < iterations; ++i )
		{
			start = clock.now();
			if( expr1.match( str ) )
				++matches1;
			end = clock.now();
			time1 += end - start;

			start = clock.now();
			if( expr2.match( str ) )
				++matches2;
			end = clock.now();
			time2 += end - start;
		}

		auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>( time1 );
		auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>( time2 );
		std::cout << "Non-optimized: " << string::toString( duration1.count() ).separateThousands() << " microsecs\n";
		std::cout << "    Optimized: " << string::toString( duration2.count() ).separateThousands() << " microsecs\n";

		WANT_EQ( iterations, matches1 );
		WANT_EQ( iterations, matches2 );
		WANT_TRUE( duration2 <= ( duration1 / 4 ) * 3 ) << "Optimized must be <= "
			<< string( ( ( duration1 / 4 ) * 3 ).count() ).separateThousands().stdstr() << " microsecs";
	}*/


	TEST( SpeedCmp, eon_std_parse )
	{
		eon::regex erx;
		std::regex srx;
		size_t count = 50000;
#ifdef EON_UNIX
		count /= 10;
#endif
#ifdef _DEBUG
		count /= 50;
#endif
		std::chrono::steady_clock clock;
		string e_pattern{ R"(^\w{2,6}: \d\d?:\d\d?:\d\d? (alpha|beta){1,2} \s*\.$)" };
		std::string s_pattern{ R"(^\w{2,6}: \d\d?:\d\d?:\d\d? (alpha|beta){1,2} \s*\.$)" };
		std::vector<eon::regex> e_list;
		std::vector<std::regex> s_list;

		auto e_start = clock.now();
		for( size_t i = 0; i < count; ++i )
		{
			REQUIRE_NO_EXCEPT( erx = e_pattern ) << "Failed to parse #" << string::toString( i );
			e_list.push_back( std::move( erx ) );
		}
		auto e_end = clock.now();

		auto s_start = clock.now();
		for( size_t i = 0; i < count; ++i )
		{
			REQUIRE_NO_EXCEPT( srx.assign( s_pattern ) ) << "Failed to parse #" << string::toString( i );
			s_list.push_back( std::move( srx ) );
		}
		auto s_end = clock.now();

		auto e_time = e_end - e_start;
		auto s_time = s_end - s_start;
		auto e_ms = std::chrono::duration_cast<std::chrono::milliseconds>( e_time );
		auto s_ms = std::chrono::duration_cast<std::chrono::milliseconds>( s_time );

		std::cout << "\nEon regex (" + string::toString( e_list.size() ) << "): " << string::toString( e_ms.count() )
			<< "ms\n";
		std::cout << "Std regex (" + string::toString( s_list.size() ) << "): " << string::toString( s_ms.count() )
			<< "ms\n";
	}
	TEST( SpeedCmp, eon_std_match )
	{
		eon::regex erx;
		REQUIRE_NO_EXCEPT( erx = R"(^\w{2,6}: \d\d?:\d\d?:\d\d? (alpha|beta){1,2} \s*\.$)" ) << "Failed to parse Eon regex";
		std::regex srx;
		REQUIRE_NO_EXCEPT( srx = std::regex( R"(^\w{2,6}: \d\d?:\d\d?:\d\d? (alpha|beta){1,2} \s*\.$)" ) )
			<< "Failed to parse std regex";
		size_t count = 5000;
#ifdef _DEBUG
		count /= 10;
#endif
		std::map<std::string, bool> s_text{
			{ "alpha: 1:99:7 beta .", true },
			{ "beta: 01:3:18 alphabeta       .", true },
			{ "gamma: 123:1:1 alpha  .", false },
			{ "delta: 1:1:1 gamma  .", false },
			{ "epsilon: 1:1:1 alpha .", false },
			{ "zeta: 00:00:00 betaalpha      .", true },
			{ "eta: 0::1 alpha .", false },
			{ "theta: :: beta .", false },
			{ "iota: 1:2:3 alpha.", false },
			{ "kappa: 11:22:33 beta          .", true },
			{ "lambda: 9:8:7 alphaalpha      .", true },
			{ "mu: 1:2 alpha .", false },
			{ "nu: 1.2:3 alpha .", false },
			{ "xi: 1:2.3 alpha .", false },
			{ "omikron: 2:2:2 alpha .", false },
			{ "pi:: 1:2:3 alpha .", false },
			{ "rho 1:2:3 alpha .", false },
			{ "sigma: 1:1:1 alpha                                                                                          .", true },
			{ "tau: 1,2,3 alpha .", false },
			{ "upsilon: 1:2:3 beta .", false },
			{ "phi:  1:2:3 alpha .", false },
			{ "chi: 1 :2 :3 alpa .", false },
			{ "psi: 1.:2.:3. alpha .", false },
			{ "omega: 44:55:66 beta                                                                                                .", true }
		};
		std::map<string, bool> e_text;
		for( auto& details : s_text )
			e_text[ string( details.first ) ] = details.second;
		std::chrono::steady_clock clock;
		size_t e_successes = 0, s_successes = 0;

		auto e_start = clock.now();
		for( size_t i = 0; i < count; ++i )
		{
			for( auto& details : e_text )
			{
				bool match = false;
				REQUIRE_NO_EXCEPT( match = static_cast<bool>( erx.match( details.first ) ) )
					<< "Eon failed match on round #" << string::toString( i + 1 ) << ", text \"" << details.first << "\"";
				WANT_TRUE( match == details.second )
					<< "Eon bad match on round #" << string::toString( i + 1 ) << ", text \"" << details.first + "\"";
				if( match == details.second )
					++e_successes;
			}
		}
		auto e_end = clock.now();

		auto s_start = clock.now();
		for( size_t i = 0; i < count; ++i )
		{
			for( auto& details : s_text )
			{
				bool match = false;
				REQUIRE_NO_EXCEPT( match = static_cast<bool>(
					std::regex_match( details.first, srx ) ) )
					<< "Std failed match on round #"
					<< string::toString( i + 1 ) << ", text \""
					<< details.first << "\"";
				WANT_TRUE( match == details.second )
					<< "Std bad match on round #" << string::toString( i + 1 )
					<< ", text \"" << details.first + "\"";
				if( match == details.second )
					++s_successes;
			}
		}
		auto s_end = clock.now();

		auto e_time = e_end - e_start;
		auto s_time = s_end - s_start;
		auto e_ms = std::chrono::duration_cast<std::chrono::milliseconds>( e_time );
		auto s_ms = std::chrono::duration_cast<std::chrono::milliseconds>( s_time );

		std::cout << "\nEon regex (" << string::toString( e_successes ) << "): " << string::toString( e_ms.count() )
			<< "ms\n";
		std::cout << "Std regex (" << string::toString( s_successes ) << "): " << string::toString( s_ms.count() )
			<< "ms\n";
	}
	TEST( SpeedCmp, eon_match_only )
	{
		eon::regex rx;
		REQUIRE_NO_EXCEPT( rx = R"(^\w{2,6}: \d\d?:\d\d?:\d\d? (alpha|beta){1,2} \s*\.$)" ) << "Failed to parse Eon regex";
		size_t count = 5000;
#ifdef _DEBUG
		count /= 10;
#endif
		std::map<string, bool> text{
			{ "alpha: 1:99:7 beta .", true },
			{ "beta: 01:3:18 alphabeta       .", true },
			{ "gamma: 123:1:1 alpha  .", false },
			{ "delta: 1:1:1 gamma  .", false },
			{ "epsilon: 1:1:1 alpha .", false },
			{ "zeta: 00:00:00 betaalpha      .", true },
			{ "eta: 0::1 alpha .", false },
			{ "theta: :: beta .", false },
			{ "iota: 1:2:3 alpha.", false },
			{ "kappa: 11:22:33 beta          .", true },
			{ "lambda: 9:8:7 alphaalpha      .", true },
			{ "mu: 1:2 alpha .", false },
			{ "nu: 1.2:3 alpha .", false },
			{ "xi: 1:2.3 alpha .", false },
			{ "omikron: 2:2:2 alpha .", false },
			{ "pi:: 1:2:3 alpha .", false },
			{ "rho 1:2:3 alpha .", false },
			{ "sigma: 1:1:1 alpha                                                                                          .", true },
			{ "tau: 1,2,3 alpha .", false },
			{ "upsilon: 1:2:3 beta .", false },
			{ "phi:  1:2:3 alpha .", false },
			{ "chi: 1 :2 :3 alpa .", false },
			{ "psi: 1.:2.:3. alpha .", false },
			{ "omega: 44:55:66 beta                                                                                                .", true }
		};
		std::chrono::steady_clock clock;
		size_t successes = 0;

		auto start = clock.now();
		for( size_t i = 0; i < count; ++i )
		{
			for( auto& details : text )
			{
				bool match = false;
				REQUIRE_NO_EXCEPT( match = static_cast<bool>( rx.match( details.first ) ) )
					<< "Eon failed match on round #" << string::toString( i + 1 ) << ", text \"" << details.first << "\"";
				WANT_TRUE( match == details.second )
					<< "Eon bad match on round #" << string::toString( i + 1 ) << ", text \"" << details.first + "\"";
				if( match == details.second )
					++successes;
			}
		}
		auto end = clock.now();

		auto e_time = end - start;
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>( e_time );

		std::cout << "\nEon regex (" << string::toString( successes )
			<< "): " << string::toString( ms.count() ) << "ms\n";
	}
}
