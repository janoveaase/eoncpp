#include "Substring.h"
#include <eoninlinetest/InlineTest.h>
#include <cctype>


namespace eon
{
	EON_TEST( substring, startsWith, substr_empty_empty,
		EON_FALSE( substring( "" ).startsWith( substring( "" ) ) ) );
	EON_TEST( substring, startsWith, substr_empty_nonempty,
		EON_FALSE( substring( "" ).startsWith( substring( "a" ) ) ) );
	EON_TEST( substring, startsWith, substr_nonempty_empty,
		EON_FALSE( substring( "abcdef" ).startsWith( substring( "" ) ) ) );
	EON_TEST( substring, startsWith, substr_true,
		EON_TRUE( substring( "abcdef" ).startsWith( substring( "abc" ) ) ) );
	EON_TEST( substring, startsWith, substr_false,
		EON_FALSE( substring( "abcdef" ).startsWith( substring( "def" ) ) ) );
	EON_TEST( substring, startsWith, substr_case,
		EON_FALSE( substring( "abcdef" ).startsWith( substring( "ABC" ) ) ) );
	EON_TEST( substring, startsWith, substr_icase,
		EON_TRUE( substring( "abcdef" ).startsWith( substring( "ABC" ), strcmp::icase_utf8::Cmp ) ) );

	EON_TEST( substring, startsWith, char_t_empty_nonempty,
		EON_FALSE( substring( "" ).startsWith( char_t( 'a' ) ) ) );
	EON_TEST( substring, startsWith, char_t_true,
		EON_TRUE( substring( "abcdef" ).startsWith( char_t( 'a' ) ) ) );
	EON_TEST( substring, startsWith, char_t_false,
		EON_FALSE( substring( "abcdef" ).startsWith( char_t( 'f' ) ) ) );
	EON_TEST( substring, startsWith, char_t_case,
		EON_FALSE( substring( "abcdef" ).startsWith( char_t( 'A' ) ) ) );
	EON_TEST( substring, startsWith, char_t_icase,
		EON_TRUE( substring( "abcdef" ).startsWith( char_t( 'A' ), strcmp::icase_chr::Cmp ) ) );


	EON_TEST( substring, endsWith, substr_empty_empty,
		EON_FALSE( substring( "" ).endsWith( substring( "" ) ) ) );
	EON_TEST( substring, endsWith, substr_empty_nonempty,
		EON_FALSE( substring( "" ).endsWith( substring( "a" ) ) ) );
	EON_TEST( substring, endsWith, substr_nonempty_empty,
		EON_FALSE( substring( "abcdef" ).endsWith( substring( "" ) ) ) );
	EON_TEST( substring, endsWith, substr_false,
		EON_FALSE( substring( "abcdef" ).endsWith( substring( "abc" ) ) ) );
	EON_TEST( substring, endsWith, substr_true,
		EON_TRUE( substring( "abcdef" ).endsWith( substring( "def" ) ) ) );
	EON_TEST( substring, endsWith, substr_case,
		EON_FALSE( substring( "abcdef" ).endsWith( substring( "DEF" ) ) ) );
	EON_TEST( substring, endsWith, substr_icase,
		EON_TRUE( substring( "abcdef" ).endsWith( substring( "DEF" ), strcmp::icase_utf8::Cmp ) ) );

	EON_TEST( substring, endsWith, char_t_empty_nonempty,
		EON_FALSE( substring( "" ).endsWith( char_t( 'a' ) ) ) );
	EON_TEST( substring, endsWith, char_t_false,
		EON_FALSE( substring( "abcdef" ).endsWith( char_t( 'a' ) ) ) );
	EON_TEST( substring, endsWith, char_t_true,
		EON_TRUE( substring( "abcdef" ).endsWith( char_t( 'f' ) ) ) );
	EON_TEST( substring, endsWith, char_t_case,
		EON_FALSE( substring( "abcdef" ).endsWith( char_t( 'F' ) ) ) );
	EON_TEST( substring, endsWith, char_t_icase,
		EON_TRUE( substring( "abcdef" ).endsWith( char_t( 'F' ), strcmp::icase_chr::Cmp ) ) );


	EON_TEST( substring, compare, void,
		EON_EQ( 0, substring().compare( substring() ) ) );
	EON_TEST( substring, compare, empty,
		EON_EQ( 0, substring( "" ).compare( substring( "" ) ) ) );
	EON_TEST( substring, compare, lt,
		EON_LT( substring( "Abcdef" ).compare( substring( "abcdef" ) ), 0 ) );
	EON_TEST( substring, compare, gt,
		EON_GT( substring( "abcdef" ).compare( substring( "Abcdef" ) ), 0 ) );

	EON_TEST( substring, operator_eq, void,
		EON_EQ( substring(), substring() ) );
	EON_TEST( substring, operator_eq, empty,
		EON_EQ( substring( "" ), substring( "" ) ) );
	EON_TEST( substring, operator_lt, basic,
		EON_LT( substring( "Abcdef" ), substring( "abcdef" ) ) );
	EON_TEST( substring, operator_le, basic,
		EON_LE( substring( "Abcdef" ), substring( "Abcdef" ) ) );
	EON_TEST( substring, operator_gt, basic,
		EON_GT( substring( "abcdef" ), substring( "Abcdef" ) ) );
	EON_TEST( substring, operator_ge, basic,
		EON_GE( substring( "Abcdef" ), substring( "Abcdef" ) ) );
	EON_TEST( substring, operator_ne, lt,
		EON_NE( substring( "Abcdef" ), substring( "abcdef" ) ) );
	EON_TEST( substring, operator_ne, gt,
		EON_NE( substring( "abcdef" ), substring( "Abcdef" ) ) );

	EON_TEST( substring, operator_eq, stdstring_void,
		EON_EQ( substring(), std::string() ) );
	EON_TEST( substring, operator_eq, stdstring_empty,
		EON_EQ( substring( "" ), std::string( "" ) ) );
	EON_TEST( substring, operator_lt, stdstring_basic,
		EON_LT( substring( "Abcdef" ), std::string( "abcdef" ) ) );
	EON_TEST( substring, operator_le, stdstring_basic,
		EON_LE( substring( "Abcdef" ), std::string( "Abcdef" ) ) );
	EON_TEST( substring, operator_gt, stdstring_basic,
		EON_GT( substring( "abcdef" ), std::string( "Abcdef" ) ) );
	EON_TEST( substring, operator_ge, stdstring_basic,
		EON_GE( substring( "Abcdef" ), std::string( "Abcdef" ) ) );
	EON_TEST( substring, operator_ne, stdstring_lt,
		EON_NE( substring( "Abcdef" ), std::string( "abcdef" ) ) );
	EON_TEST( substring, operator_ne, stdstring_gt,
		EON_NE( substring( "abcdef" ), std::string( "Abcdef" ) ) );

	EON_TEST( substring, operator_eq, stdstring2_void,
		EON_EQ( std::string(), substring() ) );
	EON_TEST( substring, operator_eq, stdstring2_empty,
		EON_EQ( std::string( "" ), substring( "" ) ) );
	EON_TEST( substring, operator_lt, stdstring2_basic,
		EON_LT( std::string( "Abcdef" ), substring( "abcdef" ) ) );
	EON_TEST( substring, operator_le, stdstring2_basic,
		EON_LE( std::string( "Abcdef" ), substring( "Abcdef" ) ) );
	EON_TEST( substring, operator_gt, stdstring2_basic,
		EON_GT( std::string( "abcdef" ), substring( "Abcdef" ) ) );
	EON_TEST( substring, operator_ge, stdstring2_basic,
		EON_GE( std::string( "Abcdef" ), substring( "Abcdef" ) ) );
	EON_TEST( substring, operator_ne, stdstring2_lt,
		EON_NE( std::string( "Abcdef" ), substring( "abcdef" ) ) );
	EON_TEST( substring, operator_ne, stdstring2_gt,
		EON_NE( std::string( "abcdef" ), substring( "Abcdef" ) ) );

	EON_TEST( substring, operator_eq, cstring_empty,
		EON_EQ( substring( "" ), "" ) );
	EON_TEST( substring, operator_lt, cstring_basic,
		EON_LT( substring( "Abcdef" ), "abcdef" ) );
	EON_TEST( substring, operator_le, cstring_basic,
		EON_LE( substring( "Abcdef" ), "Abcdef" ) );
	EON_TEST( substring, operator_gt, cstring_basic,
		EON_GT( substring( "abcdef" ), "Abcdef" ) );
	EON_TEST( substring, operator_ge, cstring_basic,
		EON_GE( substring( "Abcdef" ), "Abcdef" ) );
	EON_TEST( substring, operator_ne, cstring_lt,
		EON_NE( substring( "Abcdef" ), "abcdef" ) );
	EON_TEST( substring, operator_ne, cstring_gt,
		EON_NE( substring( "abcdef" ), "Abcdef" ) );

	EON_TEST( substring, operator_eq, cstring2_empty,
		EON_EQ( "", substring( "" ) ) );
	EON_TEST( substring, operator_lt, cstring2_basic,
		EON_LT( "Abcdef", substring( "abcdef" ) ) );
	EON_TEST( substring, operator_le, cstring2_basic,
		EON_LE( "Abcdef", substring( "Abcdef" ) ) );
	EON_TEST( substring, operator_gt, cstring2_basic,
		EON_GT( "abcdef", substring( "Abcdef" ) ) );
	EON_TEST( substring, operator_ge, cstring2_basic,
		EON_GE( "Abcdef", substring( "Abcdef" ) ) );
	EON_TEST( substring, operator_ne, cstring2_lt,
		EON_NE( "Abcdef", substring( "abcdef" ) ) );
	EON_TEST( substring, operator_ne, cstring2_gt,
		EON_NE( "abcdef", substring( "Abcdef" ) ) );


	EON_TEST( substring, compareSub, none,
		EON_EQ( 0, substring( "abcdef" ).compareSub( substring( "abcfed" ), 0 ) ) );
	EON_TEST( substring, compareSub, eq,
		EON_EQ( 0, substring( "abcdef" ).compareSub( substring( "abcfed" ), 3 ) ) );
	EON_TEST( substring, compareSub, le,
		EON_EQ( -3, substring( "abcdef" ).compareSub( substring( "abfedc" ), 3 ) ) );
	EON_TEST( substring, compareSub, gt,
		EON_EQ( 3, substring( "abfedc" ).compareSub( substring( "abcfed" ), 3 ) ) );


	EON_TEST_3STEP( substring, comparePos, same,
		string source( "aaaaaa" ),
		substring obj( source.begin() + 2, source.end() - 2 ),
		EON_EQ( 0, obj.comparePos( substring( source.begin() + 2, source.end() - 2 ) ) ) );
	EON_TEST_3STEP( substring, comparePos, lt_front,
		string source( "aaaaaa" ),
		substring obj( source.begin() + 2, source.end() - 2 ),
		EON_EQ( -1, obj.comparePos( substring( source.begin() + 3, source.end() - 2 ) ) ) );
	EON_TEST_3STEP( substring, comparePos, lt_end,
		string source( "aaaaaa" ),
		substring obj( source.begin() + 2, source.end() - 2 ),
		EON_EQ( -1, obj.comparePos( substring( source.begin() + 2, source.end() - 1 ) ) ) );
	EON_TEST_3STEP( substring, comparePos, gt_front,
		string source( "aaaaaa" ),
		substring obj( source.begin() + 2, source.end() - 2 ),
		EON_EQ( 1, obj.comparePos( substring( source.begin() + 1, source.end() - 2 ) ) ) );
	EON_TEST_3STEP( substring, comparePos, gt_end,
		string source( "aaaaaa" ),
		substring obj( source.begin() + 2, source.end() - 2 ),
		EON_EQ( 1, obj.comparePos( substring( source.begin() + 2, source.end() - 3 ) ) ) );
}
