#include "Substring.h"
#include <eoninlinetest/InlineTest.h>
#include <cctype>


namespace eon
{
	const substring::fast_compare substring::FastCompare;

	const substring::diff_compare substring::DiffCompare;
	const substring::icase_compare substring::ICaseCompare;

	const substring::fast_chr_compare substring::FastChrCompare;
	EON_TEST( substring, fast_chr_compare, lt,
		EON_EQ( -1, substring::FastChrCompare( char_t( 'a' ), char_t( 'b' ) ) ) );
	EON_TEST( substring, fast_chr_compare, eq,
		EON_EQ( 0, substring::FastChrCompare( char_t( 'a' ), char_t( 'a' ) ) ) );
	EON_TEST( substring, fast_chr_compare, gt,
		EON_EQ( 1, substring::FastChrCompare( char_t( 'b' ), char_t( 'a' ) ) ) );

	const substring::icase_chr_compare substring::ICaseChrCompare;
	EON_TEST( substring, icase_chr_compare, lt,
		EON_EQ( -1, substring::ICaseChrCompare( char_t( 'a' ), char_t( 'B' ) ) ) );
	EON_TEST( substring, icase_chr_compare, eq,
		EON_EQ( 0, substring::ICaseChrCompare( char_t( 'a' ), char_t( 'A' ) ) ) );
	EON_TEST( substring, icase_chr_compare, gt,
		EON_EQ( 1, substring::ICaseChrCompare( char_t( 'b' ), char_t( 'A' ) ) ) );

	int substring::fast_compare::operator()( const substring& a, const substring& b ) const noexcept
	{
		if( a.empty() || b.empty() )
			return !b.empty() ? -1 : !a.empty() ? 1 : 0;
		auto cmp = memcmp( a.Beg.Pos, b.Beg.Pos, a.numBytes() < b.numBytes() ? a.numBytes() : b.numBytes() );
		return cmp != 0 || a.numBytes() == b.numBytes() ? cmp : a.numBytes() < b.numBytes() ? -1 : 1;
	}
	EON_TEST( substring, fast_compare, void_void,
		EON_EQ( 0, substring::FastCompare( substring(), substring() ) ) );
	EON_TEST( substring, fast_compare, void_empty,
		EON_EQ( 0, substring::FastCompare( substring(), substring( "" ) ) ) );
	EON_TEST( substring, fast_compare, empty_void,
		EON_EQ( 0, substring::FastCompare( substring( "" ), substring() ) ) );
	EON_TEST( substring, fast_compare, empty_empty,
		EON_EQ( 0, substring::FastCompare( substring( "" ), substring( "" ) ) ) );
	EON_TEST( substring, fast_compare, empty_nonempty,
		EON_EQ( -1, substring::FastCompare( substring( "" ), substring( "abc" ) ) ) );
	EON_TEST( substring, fast_compare, nonempty_empty,
		EON_EQ( 1, substring::FastCompare( substring( "abc" ), substring( "" ) ) ) );
	EON_TEST( substring, fast_compare, lt,
		EON_EQ( -1, substring::FastCompare( substring( "abc" ), substring( "abd" ) ) ) );
	EON_TEST( substring, fast_compare, eq,
		EON_EQ( 0, substring::FastCompare( substring( "abc" ), substring( "abc" ) ) ) );
	EON_TEST( substring, fast_compare, gt,
		EON_EQ( 1, substring::FastCompare( substring( "abd" ), substring( "abc" ) ) ) );

	int substring::diff_compare::operator()( const substring& a, const substring& b ) const noexcept
	{
		if( a.empty() || b.empty() )
			return !b.empty() ? -1 : !a.empty() ? 1 : 0;
		string_iterator a_i = a.Beg, b_i = b.Beg;
		int pos = 1;
		for( ; a_i != a.End && b_i != b.end(); ++a_i, ++b_i, ++pos )
		{
			if( *a_i < *b_i )
				return -pos;
			else if( *b_i < *a_i )
				return pos;
		}
		return a_i == a.End && b_i == b.End ? 0 : a_i != a.End ? pos : -pos;
	}
	EON_TEST( substring, diff_compare, void_void,
		EON_EQ( 0, substring::DiffCompare( substring(), substring() ) ) );
	EON_TEST( substring, diff_compare, void_empty,
		EON_EQ( 0, substring::DiffCompare( substring(), substring( "" ) ) ) );
	EON_TEST( substring, diff_compare, empty_void,
		EON_EQ( 0, substring::DiffCompare( substring( "" ), substring() ) ) );
	EON_TEST( substring, diff_compare, empty_empty,
		EON_EQ( 0, substring::DiffCompare( substring( "" ), substring( "" ) ) ) );
	EON_TEST( substring, diff_compare, empty_nonempty,
		EON_EQ( -1, substring::DiffCompare( substring( "" ), substring( "abc" ) ) ) );
	EON_TEST( substring, diff_compare, nonempty_empty,
		EON_EQ( 1, substring::DiffCompare( substring( "abc" ), substring( "" ) ) ) );
	EON_TEST( substring, diff_compare, lt,
		EON_EQ( -3, substring::DiffCompare( substring( "abc" ), substring( "abd" ) ) ) );
	EON_TEST( substring, diff_compare, eq,
		EON_EQ( 0, substring::DiffCompare( substring( "abc" ), substring( "abc" ) ) ) );
	EON_TEST( substring, diff_compare, gt,
		EON_EQ( 3, substring::DiffCompare( substring( "abd" ), substring( "abc" ) ) ) );

	int substring::icase_compare::operator()( const substring& a, const substring& b ) const noexcept
	{
		if( a.empty() || b.empty() )
			return !b.empty() ? -1 : !a.empty() ? 1 : 0;
		string_iterator a_i = a.Beg, b_i = b.Beg;
		int pos = 1;
		for( ; a_i != a.End && b_i != b.end(); ++a_i, ++b_i, ++pos )
		{
			if( *a_i != *b_i )
			{
				auto lw_a = Loc->toLower(
					static_cast<wchar_t>( *a_i ) ), lw_b = Loc->toLower( static_cast<wchar_t>( *b_i ) );
				if( lw_a != lw_b )
					return lw_a < lw_b ? -pos : pos;
			}
		}
		return a_i == a.End && b_i == b.End ? 0 : a_i != a.End ? pos : -pos;
	}
	EON_TEST( substring, icase_compare, void_void,
		EON_EQ( 0, substring::ICaseCompare( substring(), substring() ) ) );
	EON_TEST( substring, icase_compare, void_empty,
		EON_EQ( 0, substring::ICaseCompare( substring(), substring( "" ) ) ) );
	EON_TEST( substring, icase_compare, empty_void,
		EON_EQ( 0, substring::ICaseCompare( substring( "" ), substring() ) ) );
	EON_TEST( substring, icase_compare, empty_empty,
		EON_EQ( 0, substring::ICaseCompare( substring( "" ), substring( "" ) ) ) );
	EON_TEST( substring, icase_compare, empty_nonempty,
		EON_EQ( -1, substring::ICaseCompare( substring( "" ), substring( "abc" ) ) ) );
	EON_TEST( substring, icase_compare, nonempty_empty,
		EON_EQ( 1, substring::ICaseCompare( substring( "abc" ), substring( "" ) ) ) );
	EON_TEST( substring, icase_compare, lt,
		EON_EQ( -3, substring::ICaseCompare( substring( "abc" ), substring( "Abd" ) ) ) );
	EON_TEST( substring, icase_compare, eq,
		EON_EQ( 0, substring::ICaseCompare( substring( "abc" ), substring( "Abc" ) ) ) );
	EON_TEST( substring, icase_compare, gt,
		EON_EQ( 3, substring::ICaseCompare( substring( "abd" ), substring( "Abc" ) ) ) );


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
		EON_TRUE( substring( "abcdef" ).startsWith( substring( "ABC" ), substring::ICaseCompare ) ) );

	EON_TEST( substring, startsWith, char_t_empty_nonempty,
		EON_FALSE( substring( "" ).startsWith( char_t( 'a' ) ) ) );
	EON_TEST( substring, startsWith, char_t_true,
		EON_TRUE( substring( "abcdef" ).startsWith( char_t( 'a' ) ) ) );
	EON_TEST( substring, startsWith, char_t_false,
		EON_FALSE( substring( "abcdef" ).startsWith( char_t( 'f' ) ) ) );
	EON_TEST( substring, startsWith, char_t_case,
		EON_FALSE( substring( "abcdef" ).startsWith( char_t( 'A' ) ) ) );
	EON_TEST( substring, startsWith, char_t_icase,
		EON_TRUE( substring( "abcdef" ).startsWith( char_t( 'A' ), substring::ICaseChrCompare ) ) );


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
		EON_TRUE( substring( "abcdef" ).endsWith( substring( "DEF" ), substring::ICaseCompare ) ) );

	EON_TEST( substring, endsWith, char_t_empty_nonempty,
		EON_FALSE( substring( "" ).endsWith( char_t( 'a' ) ) ) );
	EON_TEST( substring, endsWith, char_t_false,
		EON_FALSE( substring( "abcdef" ).endsWith( char_t( 'a' ) ) ) );
	EON_TEST( substring, endsWith, char_t_true,
		EON_TRUE( substring( "abcdef" ).endsWith( char_t( 'f' ) ) ) );
	EON_TEST( substring, endsWith, char_t_case,
		EON_FALSE( substring( "abcdef" ).endsWith( char_t( 'F' ) ) ) );
	EON_TEST( substring, endsWith, char_t_icase,
		EON_TRUE( substring( "abcdef" ).endsWith( char_t( 'F' ), substring::ICaseChrCompare ) ) );


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
