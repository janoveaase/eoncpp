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

	TEST( RegExTest, boundaries )
	{
		regex input_rx( R"(^\l+$)" );
		regex line_rx( R"(.+?^\l+$.+?)", "l" );
		regex word_rx{ R"(.+?\b.+?\B.+)" };
		regex spaces_rx{ R"(.+?\b.+?\B.+)", "b" };

		WANT_TRUE( input_rx.match( "one" ) );
		WANT_FALSE( input_rx.match( ".one" ) );
		WANT_FALSE( input_rx.match( "one." ) );

		WANT_TRUE( line_rx.match( " \none\n " ) );
		WANT_FALSE( line_rx.match( "one\n" ) );
		WANT_FALSE( line_rx.match( "one\n" ) );

		WANT_TRUE( word_rx.match( " one " ) );
		WANT_TRUE( word_rx.match( " .one. " ) );
		WANT_FALSE( word_rx.match( " ... " ) );
		WANT_FALSE( word_rx.match( "   " ) );

		WANT_TRUE( spaces_rx.match( " one " ) );
		WANT_TRUE( spaces_rx.match( " .one. " ) );
		WANT_TRUE( spaces_rx.match( " ... " ) );
		WANT_FALSE( spaces_rx.match( "   " ) );
	}

	TEST( RegExTest, prefix_not )
	{
		regex rx( R"(.\b!a\l)" );

		WANT_TRUE( rx.match( " b" ) );
		WANT_FALSE( rx.match( " a" ) );
	}

	TEST( RegExTest, ignore_case )
	{
		regex rx1( R"(aBc)", "i" );

		WANT_TRUE( rx1.match( "abc" ) );
		WANT_TRUE( rx1.match( "ABC" ) );
		WANT_TRUE( rx1.match( "AbC" ) );
	}

	TEST( RegExTest, removeDuplicates )
	{
		regex ex{ R"(a+a+)" };
		WANT_EQ( "a{2,}", ex.strStruct().stdstr() );
		ex = regex{ R"(a+a*)" };
		WANT_EQ( "aa*", ex.strStruct().stdstr() );
		ex = regex{ R"(a*a+)" };
		WANT_EQ( "aa*", ex.strStruct().stdstr() );
		ex = regex{ R"(a+a+?)" };
		WANT_EQ( "a{2,}", ex.strStruct().stdstr() );
		ex = regex{ R"(a+?a+)" };
		WANT_EQ( "a{2,}", ex.strStruct().stdstr() );
		ex = regex{ R"(a+?a+?)" };
		WANT_EQ( "a{2,}?", ex.strStruct().stdstr() );
		ex = regex{ R"((a+a+))" };
		WANT_EQ( "a{2,}", ex.strStruct().stdstr() );
		ex = regex{ R"(a+a+a+a+)" };
		WANT_EQ( "a{4,}", ex.strStruct().stdstr() );
		ex = regex{ R"(a+?a+?a+?a+?)" };
		WANT_EQ( "a{4,}?", ex.strStruct().stdstr() );
		ex = regex{ R"((a+a+)(a+a+))" };
		WANT_EQ( "a{4,}", ex.strStruct().stdstr() );
		ex = regex{ R"(a{1,5}a{1,5})" };
		WANT_EQ( "a{2,10}", ex.strStruct().stdstr() );
		ex = regex{ R"(a{1,5}a{2,6})" };
		WANT_EQ( "a{3,11}", ex.strStruct().stdstr() );
		ex = regex{ R"(a{2,5}a{1,5})" };
		WANT_EQ( "a{3,10}", ex.strStruct().stdstr() );
		ex = regex{ R"(\d\d+:)" };
		WANT_EQ( "\\d{2,}:", ex.strStruct().stdstr() );
		ex = regex{ R"((x+x+)+y)" };
		WANT_EQ( "x{2,}y", ex.strStruct().stdstr() );
		ex = regex{ R"((x+?x+?)+?y)" };
		WANT_EQ( "x{2,}?y", ex.strStruct().stdstr() );
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
		REQUIRE_NO_EXCEPT( expr = R"(!\d.)" ) << "Failed to parse";

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

	TEST( RegExTest, match_letter )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(\l)" ) << "Failed to parse";

		WANT_FALSE( expr.match( "8" ) ) << "Matched digit";
		WANT_TRUE( expr.match( "b" ) ) << "Didn't match lower case letter";
		WANT_TRUE( expr.match( "B" ) ) << "Didn't match upper case letter";
		WANT_TRUE( expr.match( "β" ) ) << "Didn't match unicode letter";
		WANT_FALSE( expr.match( "_" ) ) << "Matched underscore";
		WANT_FALSE( expr.match( "." ) ) << "Matched punctuation";
	}
	TEST( RegExTest, match_notletter )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(\L)" ) << "Failed to parse";

		WANT_TRUE( expr.match( "8" ) ) << "Didn't match digit";
		WANT_FALSE( expr.match( "b" ) ) << "Matched lower case letter";
		WANT_FALSE( expr.match( "B" ) ) << "Matched upper case letter";
		WANT_FALSE( expr.match( "β" ) ) << "Matched unicode letter";
		WANT_TRUE( expr.match( "_" ) ) << "Didn't match underscore";
		WANT_TRUE( expr.match( "." ) ) << "Didn't match punctuation";
	}
	TEST( RegExTest, match_lower)
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(\u)" ) << "Failed to parse";

		WANT_FALSE( expr.match( "8" ) ) << "Matched digit";
		WANT_TRUE( expr.match( "b" ) ) << "Didn't match lower case letter";
		WANT_FALSE( expr.match( "B" ) ) << "Matched upper case letter";
		WANT_TRUE( expr.match( "β" ) ) << "Didn't match unicode letter";
		WANT_FALSE( expr.match( "_" ) ) << "Matched underscore";
		WANT_FALSE( expr.match( "." ) ) << "Matched punctuation";
	}
	TEST( RegExTest, match_upper )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(\U)" ) << "Failed to parse";

		WANT_FALSE( expr.match( "8" ) ) << "Matched digit";
		WANT_FALSE( expr.match( "b" ) ) << "Matched lower case letter";
		WANT_TRUE( expr.match( "B" ) ) << "Didn't match upper case letter";
		WANT_FALSE( expr.match( "β" ) ) << "Matched unicode letter";
		WANT_FALSE( expr.match( "_" ) ) << "Matched underscore";
		WANT_FALSE( expr.match( "." ) ) << "Matched punctuation";
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
	TEST( RegExTest, match_chargroup_negate )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"([^aβc])" ) << "Failed to parse";

		WANT_FALSE( expr.match( "a" ) ) << "Matched 'a'";
		WANT_FALSE( expr.match( "β" ) ) << "Matched unicode letter";
		WANT_FALSE( expr.match( "c" ) ) << "Matched 'c'";
		WANT_TRUE( expr.match( "d" ) ) << "Didn't match 'd'";
	}
	TEST( RegExTest, match_chargroup_negate2 )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(![aβc]x)" ) << "Failed to parse";

		WANT_FALSE( expr.match( "a" ) ) << "Matched 'a'";
		WANT_FALSE( expr.match( "β" ) ) << "Matched unicode letter";
		WANT_FALSE( expr.match( "c" ) ) << "Matched 'c'";
		WANT_FALSE( expr.match( "d" ) ) << "Matched 'd'";
		WANT_TRUE( expr.match( "x" ) ) << "Didn't match 'x'";
	}
	TEST( RegExTest, match_chargroup_special )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"([\dAB\p])" ) << "Failed to parse";

		WANT_TRUE( expr.match( "1" ) ) << "Didn't match '1'";
		WANT_TRUE( expr.match( "A" ) ) << "Didn't match 'A'";
		WANT_TRUE( expr.match( "9" ) ) << "Didn't match '9'";
		WANT_TRUE( expr.match( "*" ) ) << "Didn't match '*'";
		WANT_FALSE( expr.match( "d" ) ) << "Matched 'd'";
	}

	TEST( RegExTest, match_substring )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(.*)" ) << "Failed to parse";
		string total{ "12345" };
		auto sub = total.slice( 2, -3 );

		auto match = expr.match( sub );
		REQUIRE_TRUE( match ) << "Didn't match!";

		string matched( match.group( name_complete ) );
		WANT_EQ( "3", matched ) << "Wrong part of sub-string matched";
	}

	TEST( RegExTest, match_capture_greedy_1 )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(.*@<key>(\w+): @<value>(\w+).*)" ) << "Failed to parse";
		rx::match result;

		WANT_TRUE( result = expr.match( "one: two" ) ) << "Didn't match basic";
		auto e = result.group( name_key );
		auto two = result.group( name_value );
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
			WANT_EQ( "pi", string( result.group( eon::name( "m" ) ) ).stdstr() ) << "Wrong capture for pi";
		WANT_TRUE( result = expr.match( "spring" ) ) << "Didn't match spring";
		if( result )
			WANT_EQ( "pri", string( result.group( eon::name( "m" ) ) ).stdstr() ) << "Wrong capture for spring";
	}
	TEST( RegExTest, match_capture_issue2 )
	{
		regex expr;
		REQUIRE_NO_EXCEPT( expr = R"(x@<m>(.+?)x)" ) << "Failed to parse";
		rx::match result;

		WANT_TRUE( result = expr.match( "xax" ) );
		if( result )
			WANT_EQ( "a", string( result.group( eon::name( "m" ) ) ) );
		WANT_TRUE( result = expr.match( "xaax" ) );
		if( result )
			WANT_EQ( "aa", string( result.group( eon::name( "m" ) ) ) );
	}

	TEST( RegExTest, backreference1 )
	{
		string str{ "v=\"a b\" " };
		regex expr{ R"(@<key>(\S+)=@<qt>(\")?@<value>(.+)@:<qt>)" };
		auto match = expr.match( str );
		REQUIRE_TRUE( match ) << "Failed to match";
		WANT_EQ( "v", eon::string( match.group( eon::name( "key" ) ) ) ) << "Wrong key";
		WANT_EQ( "a b", eon::string( match.group( eon::name( "value" ) ) ) ) << "Wrong value";
	}
	TEST( RegExTest, backreference2 )
	{
		string str{ "v=a" };
		regex expr{ R"(@<key>(\S+)=@<qt>(\")?@<value>(.+)@:<qt>)" };
		auto match = expr.match( str );
		REQUIRE_TRUE( match ) << "Failed to match";
		WANT_EQ( "v", eon::string( match.group( eon::name( "key" ) ) ) ) << "Wrong key";
		WANT_EQ( "a", eon::string( match.group( eon::name( "value" ) ) ) ) << "Wrong value";
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
		WANT_EQ( "Eon", match.group( name( "one" ) ).stdstr() ) << "Wrong first group";
		WANT_EQ( "great", match.group( name( "2_" ) ).stdstr() ) << "Wrong second group";
		WANT_EQ( "!", match.group( name( "three" ) ).stdstr() ) << "Wrong third group";
	}
	TEST( MiscTests, complex1 )
	{
		string str{ "this and that and this or that when that and this" };
		regex expr{ R"(@<key>(\w+) @<conj>(and|or|when) this|that @:<conj> @:<key> !@:<conj>\w+ \w+ and|or|when this|that @:<conj> this|that)" };
		WANT_TRUE( expr.match( str ) );
	}
	TEST( MiscTests, complex2 )
	{
		string str{ "this more than that" };
		regex expr{ R"(more|(than|that|extra)|this)" };
		WANT_TRUE( expr.match( str ) );
	}
	TEST( MiscTests, complex3 )
	{
		string str{ "thisthisthismorethanthatthatthatthatthat" };
		regex expr{ R"(this+m\w+?....that{3}that{2,})" };
		WANT_TRUE( expr.match( str ) );
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

	TEST( MiscTests, real_case1 )
	{
		string str{ "2022-04-27T08:13:28.391 INFO    : Log \"test\" opened, Details=aase015win;Windows;10.0.22000.434;+01:00 (+01:00 3M5W6D2H-10M5W6D3H);29720;8273120453202572832" };
		regex expr{ R"(2022\-04\-27T\d\d:\d\d:\d\d\.\d\d\d INFO    : Log "test" opened, Details=[^;]+;Windows[^;]*;[^;]+;[^;]*;\d+;\d+)" };
		WANT_TRUE( expr.match( str ) );
	}
	TEST( MiscTests, real_case2 )
	{
		string str{ "(E:\\levo_dev\\packages\\log\\test\\function\\unit_context\\eon_context.cpp:149" };
		regex expr{ R"(.+?[\/]log[\/]test[\/]function[\/]unit_context[\/]eon_context.cpp:149$)" };
		WANT_TRUE( expr.match( str ) );
	}
	TEST( MiscTests, real_case3 )
	{
		string str{ R"(C:\ProgramData)" };
		regex expr{ R"(C:\\ProgramData)" };
		WANT_TRUE( expr.match( str ) );
	}
	TEST( MiscTests, real_case4 )
	{
		string str{ "Flexible License Manager status on Wed 1/20/2010 11:00" };
		regex orig{ R"(Flexible License Manager status on \l+ @<month>(\d+)/@<day>(\d+)/@<year>(\d+) @<hour>(\d+):@<minute>(\d+))" };
		regex expr = orig;
		eon::term << "orig: " << orig.strStruct() << "\n";
		eon::term << "expr: " << expr.strStruct() << "\n";
		auto match = expr.match( str );
		REQUIRE_TRUE( match ) << "Failed to match";
		REQUIRE_EQ( 6, match.size() ) << "Wrong number of captures";
	}
	TEST( MiscTests, real_case5 )
	{
		string str{ "\"A\"5,b:B" };
		regex expr{ R"(\s*\"@<name1>([^"]+)\"@<v>(\d+[^,]*),b:@<name2>(\S+))" };
		auto match = expr.match( str );
		REQUIRE_TRUE( match ) << "Failed to match";
		REQUIRE_EQ( 4, match.size() ) << "Wrong number of captures";
		WANT_EQ( "A", eon::string( match.group( eon::name( "name1" ) ) ) );
		WANT_EQ( "5", eon::string( match.group( eon::name( "v" ) ) ) );
		WANT_EQ( "B", eon::string( match.group( eon::name( "name2" ) ) ) );
	}
	TEST( MiscTests, real_case6 )
	{
		string str{ "A (v17) (ls/5 2)," };
		regex expr{ R"(@<name1>([^(]+) \(v@<v>(\d+.*)\) \([^/]+/\d+ \d+\),)" };
		auto match = expr.match( str );
		REQUIRE_TRUE( match ) << "Failed to match";
		REQUIRE_TRUE( match.group( eon::name( "v" ) ) ) << "Didn't get version";
		WANT_EQ( "17", eon::string( match.group( eon::name( "v" ) ) ) );
	}
	TEST( MiscTests, real_case7 )
	{
		string str{ "a 36.0         -8.7            292.2          437.0              " };
		regex expr{ R"(@<name1>(\S+) @<name2>([^(]+) \(v@<v>(\d+.*)\) \(@<name3>([^/]+)/@<num1>(\d+) @<num1>(\d+)\))" };
		auto match = expr.match( str );
		REQUIRE_FALSE( match ) << "Didn't fail to match";
	}

	TEST( MiscTests, tricky_case1 )
	{
		string str{ "abc: xyz: " };
		regex expr{ R"(@<alpha>(\S+): @<beta>(\S+): )" };
		auto match = expr.match( str );
		REQUIRE_TRUE( match ) << "Failed to match";
		REQUIRE_EQ( 3, match.size() ) << "Wrong number of captures";
		WANT_EQ( "abc", eon::string( match.group( eon::name( "alpha" ) ) ) ) << "Wrong capture";
		WANT_EQ( "xyz", eon::string( match.group( eon::name( "beta" ) ) ) ) << "Wrong capture";
	}
	TEST( MiscTests, tricky_case2 )
	{
		string str{ "(a 6)" };
		regex expr{ R"(\(a(\s+@<n2>(\d+))?\))" };
		auto match = expr.match( str );
		REQUIRE_TRUE( match ) << "Failed to match";
		REQUIRE_EQ( 2, match.size() ) << "Wrong number of captures";
		WANT_EQ( "6", eon::string( match.group( eon::name( "n2" ) ) ) ) << "Wrong capture";
	}
	TEST( MiscTests, tricky_case3 )
	{
		string str{ "(*A*)" };
		regex expr{ R"((!\*\p\*)@<m>([^*]+?)\*($|\s|\p))" };
		auto match = expr.match( str );
		REQUIRE_TRUE( match ) << "Failed to match";
		REQUIRE_EQ( 2, match.size() ) << "Wrong number of captures";
		WANT_EQ( "A", eon::string( match.group( eon::name( "m" ) ) ) ) << "Wrong capture";
	}

	TEST( MiscTests, quote_issue )
	{
		string str{ "\"  " };
		regex expr{ R"(\"[^"]+?\")" };
		auto match = expr.match( str );
		REQUIRE_FALSE( match ) << "Didn't fail to match.";
	}


	TEST( FindTests, findFirst )
	{
		string good{ "123caa456ba78" };
		string bad{ "123CAA456BA78" };
		regex rx1{ R"([abc]+)" };
		regex rx2{ R"(\d+)" };
		auto found = rx1.findFirst( good );
		REQUIRE_TRUE( found ) << "Failed to find good";
		WANT_EQ( "caa", eon::string( found.group( name_complete ) ) ) << "Wrong value found";
		found = rx1.findFirst( bad );
		REQUIRE_FALSE( found ) << "Found bad";
		found = rx2.findFirst( good );
		REQUIRE_TRUE( found ) << "Failed to find alternate";
		WANT_EQ( "123", eon::string( found.group( name_complete ) ) ) << "Wrong altenate value found";
	}
	TEST( FindTests, findLast )
	{
		string good{ "123caa456ba78" };
		string bad{ "123CAA456BA78" };
		regex rx1{ R"([abc]+)" };
		regex rx2{ R"(\d+)" };
		auto found = rx1.findLast( good );
		REQUIRE_TRUE( found ) << "Failed to find good";
		WANT_EQ( "ba", eon::string( found.group( name_complete ) ) ) << "Wrong value found";
		found = rx1.findLast( bad );
		REQUIRE_FALSE( found ) << "Found bad";
		found = rx2.findLast( good );
		REQUIRE_TRUE( found ) << "Failed to find alternate";
		WANT_EQ( "78", eon::string( found.group( name_complete ) ) ) << "Wrong altenate value found";
	}
	TEST( FindTests, findAll )
	{
		string good{ "123caa456ba78" };
		string bad{ "123CAA456BA78" };
		regex rx{ R"(\u+)" };
		auto found = rx.findAll( good );
		REQUIRE_EQ( 2, found.size() ) << "Failed to find good";
		WANT_EQ( "caa", eon::string( found[ 0 ].group( name_complete ) ) ) << "Wrong first value found";
		WANT_EQ( "ba", eon::string( found[ 1 ].group( name_complete ) ) ) << "Wrong second value found";
		
		found = rx.findAll( bad );
		WANT_EQ( 0, found.size() ) << "Found bad";
	}


	// Common function used by optimize tests
	void OptimizeTests::optimizeTest( regex& plain, regex& optimized, string& good_str, string& bad_str, int iterations )
	{
#ifdef _DEBUG
		if( iterations > 100 )
			iterations /= 10;
#endif

		std::chrono::steady_clock clock;
		std::chrono::steady_clock::time_point start, end;
		std::chrono::nanoseconds plain_good{ 0 }, plain_bad{ 0 }, optimized_good{ 0 }, optimized_bad{ 0 };

		int plain_matches = 0;
		start = clock.now();
		for( auto i = 0; i < iterations; ++i )
		{
			if( plain.match( good_str ) )
				++plain_matches;
		}
		end = clock.now();
		plain_good = end - start;
		start = clock.now();
		for( auto i = 0; i < iterations; ++i )
		{
			if( plain.match( bad_str ) )
				++plain_matches;
		}
		end = clock.now();
		plain_bad = end - start;

		int optimized_matches = 0;
		start = clock.now();
		for( auto i = 0; i < iterations; ++i )
		{
			if( optimized.match( good_str ) )
				++optimized_matches;
		}
		end = clock.now();
		optimized_good = end - start;
		start = clock.now();
		for( auto i = 0; i < iterations; ++i )
		{
			if( optimized.match( bad_str ) )
				++optimized_matches;
		}
		end = clock.now();
		optimized_bad = end - start;

		WANT_EQ( plain_matches, optimized_matches );

		auto plain_good_time = std::chrono::duration_cast<std::chrono::microseconds>( plain_good );
		auto plain_bad_time = std::chrono::duration_cast<std::chrono::microseconds>( plain_bad );
		auto plain_total_time = plain_good_time + plain_bad_time;
		auto optimized_good_time = std::chrono::duration_cast<std::chrono::microseconds>( optimized_good );
		auto optimized_bad_time = std::chrono::duration_cast<std::chrono::microseconds>( optimized_bad );
		auto optimized_total_time = optimized_good_time + optimized_bad_time;
		eon::term << "Plain total    : " << string::toString( plain_total_time.count() / 1000.0 ).separateThousands()
			<< " ms\n";
		eon::term << "Plain good     : " << string::toString( plain_good_time.count() / 1000.0 ).separateThousands()
			<< " ms\n";
		eon::term << "Plain bad      : " << string::toString( plain_bad_time.count() / 1000.0 ).separateThousands()
			<< " ms\n";
		eon::term << "Optimized total: " << string::toString( optimized_total_time.count() / 1000.0 ).separateThousands()
			<< " ms\n";
		eon::term << "Optimized good : " << string::toString( optimized_good_time.count() / 1000.0 ).separateThousands()
			<< " ms\n";
		eon::term << "Optimized bad  : " << string::toString( optimized_bad_time.count() / 1000.0 ).separateThousands()
			<< " ms\n";

		WANT_TRUE( optimized_total_time < plain_total_time ) << "No improvement";
	}

/*	TEST( OptimizeTests, expose_literals )			// VERY SMALL IMPROVEMENT! CONSIDER REMOVING!
	{
		regex plain{ R"(A+)", "!e" };
		regex optimized{ R"(A+)" };
//		regex optimized( R"(A(A*))" );
		string good{ "AAAAAAAAAAAAAAAAAAAAAAABB" };
		string bad{ "ABBBBBBBBBBBB" };
		optimizeTest( plain, optimized, good, bad );
	}	//*/
	TEST( OptimizeTests, removeSuperfluousGroups )	// CONSIDERABLE IMPROVEMENT! FINT OUT WHY!
	{
		regex plain{ R"(((A)|(B))*)", "!g" };
		regex optimized( R"(((A)|(B))*)" );
		string good{ "AAAABBBBABABABBBBBBCCC" };
		string bad{ "CCCCCCCCCCCCCCCCCCCCCCCCCCBABABA" };
		optimizeTest( plain, optimized, good, bad );
	}	//*/
/*	TEST( OptimizeTests, unroll_alternations )		// NO IMPROVEMENT! DO NOT ADD!
	{
		regex plain{ R"((\d|[aeiou])*)" };
		regex optimized( R"(\d*([aeiou]+\d*)*)" );
		string good{ "14e987eaie7e7e7e9876ei14ou" };
		string bad{ "CCCCCCCCCCCCCCCCCCCCCCCCCCBABABA" };
		optimizeTest( plain, optimized, good, bad );
	}	//*/
/*	TEST( OptimizeTests, expose_anchors )			// VERY SMALL IMPROVEMENT! DO NOT ADD!
	{
		regex plain{ R"((^A|^B))" };
		regex optimized( R"(^(A|B))" );
		string good{ "ACCCCCCCCCCCCCCCCCCCCCCCCCCC" };
		string bad{ "CCCCCCCCCCCCCCCCCCCCCCCCCCBA" };
		optimizeTest( plain, optimized, good, bad );
	}	//*/
	TEST( OptimizeTests, failfast_fixed_end )		// SIGNIFICANT IMPROVEMENT!
	{
		regex plain{ R"(.*A$)" };
		regex optimized( R"(.*A$)", "f" );
		string good{ "ACCCCCCCCCCCCCCCCCCCCCCCCCCCA" };
		string bad{ "ACCCCCCCCCCCCCCCCCCCCCCCCCCB" };
		optimizeTest( plain, optimized, good, bad );
	}	//*/


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
		std::string s_pattern{ R"(^\w{2,6}: \d\d?:\d\d?:\d\d? (?:alpha|beta){1,2} \s*\.$)" };
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

		eon::term << "Eon regex (" + string::toString( e_list.size() ) << "): " << string::toString( e_ms.count() )
			<< "ms\n";
		eon::term << "Std regex (" + string::toString( s_list.size() ) << "): " << string::toString( s_ms.count() )
			<< "ms\n";
	}
	TEST( SpeedCmp, eon_std_match )
	{
		eon::regex erx;
		REQUIRE_NO_EXCEPT( erx = R"(^\w{2,6}: \d\d?:\d\d?:\d\d? (alpha|beta){1,2} \s*\.$)" ) << "Failed to parse Eon regex";
		eon::term << "Optimized Eon regex: " << erx.strStruct() << "\n";
		std::regex srx;
		REQUIRE_NO_EXCEPT( srx = std::regex( R"(^\w{2,6}: \d\d?:\d\d?:\d\d? (?:alpha|beta){1,2} \s*\.$)" ) )
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
				REQUIRE_NO_EXCEPT( match = static_cast<bool>( std::regex_match( details.first, srx ) ) )
					<< "Std failed match on round #" << string::toString( i + 1 ) << ", text \"" << details.first << "\"";
				WANT_TRUE( match == details.second )
					<< "Std bad match on round #" << string::toString( i + 1 ) << ", text \"" << details.first + "\"";
				if( match == details.second )
					++s_successes;
			}
		}
		auto s_end = clock.now();

		auto e_time = e_end - e_start;
		auto s_time = s_end - s_start;
		auto e_ms = std::chrono::duration_cast<std::chrono::milliseconds>( e_time );
		auto s_ms = std::chrono::duration_cast<std::chrono::milliseconds>( s_time );

		eon::term << "Eon regex (" << string::toString( e_successes ) << "): " << string::toString( e_ms.count() )
			<< "ms\n";
		eon::term << "Std regex (" << string::toString( s_successes ) << "): " << string::toString( s_ms.count() )
			<< "ms\n";

		WANT_EQ( s_successes, e_successes );
	}
	TEST( SpeedCmp, eon_match_only )
	{
		eon::regex rx;
		REQUIRE_NO_EXCEPT( rx = R"(^\l{2,6}: a\-b\.c \d\d?:\d\d?:\d\d? (alpha|beta){1,2} \s*\.$)" ) << "Failed to parse Eon regex";
		size_t count = 5000;
#ifdef _DEBUG
		count /= 10;
#endif
		std::map<string, bool> text{
			{ "alpha: a-b.c 1:99:7 beta .", true },
			{ "beta: a-b.c 01:3:18 alphabeta       .", true },
			{ "gamma: a-b.c 123:1:1 alpha  .", false },
			{ "delta: a-b.c 1:1:1 gamma  .", false },
			{ "epsilon: a-b.c 1:1:1 alpha .", false },
			{ "zeta: a-b.c 00:00:00 betaalpha      .", true },
			{ "eta: a-b.c 0::1 alpha .", false },
			{ "theta: a-b.c :: beta .", false },
			{ "iota: a-b.c 1:2:3 alpha.", false },
			{ "kappa: a-b.c 11:22:33 beta          .", true },
			{ "lambda: a-b.c 9:8:7 alphaalpha      .", true },
			{ "mu: a-b.c 1:2 alpha .", false },
			{ "nu: a-b.c 1.2:3 alpha .", false },
			{ "xi: a-b.c 1:2.3 alpha .", false },
			{ "omikron: a-b.c 2:2:2 alpha .", false },
			{ "pi:: a-b.c 1:2:3 alpha .", false },
			{ "rho a-b.c 1:2:3 alpha .", false },
			{ "sigma: a-b.c 1:1:1 alpha                                                                                          .", true },
			{ "tau: a-b.c 1,2,3 alpha .", false },
			{ "upsilon: a-b.c 1:2:3 beta .", false },
			{ "phi: a-b.c  1:2:3 alpha .", false },
			{ "chi: a-b.c 1 :2 :3 alpa .", false },
			{ "psi: a-b.c 1.:2.:3. alpha .", false },
			{ "omega: a-b.c 44:55:66 beta                                                                                                .", true }
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

		eon::term << "Eon regex (" << string::toString( successes )
			<< "): " << string::toString( ms.count() ) << "ms\n";
	}

	/* The "bad" tests will always fail so only use for testing of WANT_MATCH/REQUIRE_MATCH!
	TEST( TestRegex, lines_good )
	{
		eon::string exp{ "\\d+\n\\w+" };
		eon::string act{ "1234\nabcd" };
		WANT_MATCH( exp, act );
	}
	TEST( TestRegex, lines_bad1 )
	{
		eon::string exp{ "\\d+\n\\d\\w+" };
		eon::string act{ "1234\nabcd" };
		WANT_MATCH( exp, act );
	}
	TEST( TestRegex, lines_bad2 )
	{
		eon::string exp{ "\\d+\n\\w+\n\\w\\d" };
		eon::string act{ "1234\nabcd" };
		WANT_MATCH( exp, act );
	}
	TEST( TestRegex, lines_bad3 )
	{
		eon::string exp{ "\\d+\n\\d\\w+" };
		eon::string act{ "1234\nabcd\na8" };
		WANT_MATCH( exp, act );
	}
	TEST( TestRegex, lines_bad4 )
	{
		eon::string exp{ "\\d+\n\\d\\w+" };
		eon::string act{ "1234\nabcd\na8\n:::" };
		WANT_MATCH( exp, act );
	}	//*/
}
