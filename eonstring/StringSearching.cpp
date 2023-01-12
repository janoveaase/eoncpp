#include "String.h"
#include <eoninlinetest/InlineTest.h>


namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// Searching
	//

	EON_TEST( string, contains, str_empty_empty,
		EON_FALSE( string().contains( "" ) ) );
	EON_TEST( string, contains, str_nonempty_empty,
		EON_FALSE( string( "abcdef" ).contains( "" ) ) );
	EON_TEST( string, contains, str_empty_nonempty,
		EON_FALSE( string().contains( "cd" ) ) );
	EON_TEST( string, contains, str_true,
		EON_TRUE( string( "abcdef" ).contains( "cd" ) ) );
	EON_TEST( string, contains, str_false,
		EON_FALSE( string( "abcdef" ).contains( "dc" ) ) );

	EON_TEST( string, contains, char_t_true,
		EON_TRUE( string( "abcdef" ).contains( char_t( 'c' ) ) ) );
	EON_TEST( string, contains, char_t_false,
		EON_FALSE( string( "abcdef" ).contains( char_t( 'g' ) ) ) );

	EON_TEST( string, containsAnyOf, empty_empty,
		EON_FALSE( string().containsAnyOf( "" ) ) );
	EON_TEST( string, containsAnyOf, empty_nonempty,
		EON_FALSE( string().containsAnyOf( "cdgh" ) ) );
	EON_TEST( string, containsAnyOf, nonempty_empty,
		EON_FALSE( string( "abcdef" ).containsAnyOf( "" ) ) );
	EON_TEST( string, containsAnyOf, true,
		EON_TRUE( string( "abcdef" ).containsAnyOf( "cdgh" ) ) );
	EON_TEST( string, containsAnyOf, false,
		EON_FALSE( string( "abcdef" ).containsAnyOf( "gh" ) ) );

	EON_TEST( string, containsNoneOf, empty_empty,
		EON_TRUE( string().containsNoneOf( "" ) ) );
	EON_TEST( string, containsNoneOf, empty_nonempty,
		EON_TRUE( string().containsNoneOf( "cdgh" ) ) );
	EON_TEST( string, containsNoneOf, nonempty_empty,
		EON_TRUE( string( "abcdef" ).containsNoneOf( "" ) ) );
	EON_TEST( string, containsNoneOf, false,
		EON_FALSE( string( "abcdef" ).containsNoneOf( "cdgh" ) ) );
	EON_TEST( string, containsNoneOf, true,
		EON_TRUE( string( "abcdef" ).containsNoneOf( "gh" ) ) );


	EON_TEST( string, findFirst, str_true,
		EON_TRUE( string( "abcdefabcdef" ).findFirst( "ef" ) ) );
	EON_TEST( string, findFirst, str_false,
		EON_FALSE( string( "abcdefabcdef" ).findFirst( "fe" ) ) );

	EON_TEST_2STEP( string, findFirst, in_substr_true,
		string obj( "abcdefabcdef" ),
		EON_TRUE( obj.findFirst( "fa", obj.substr( 4, 4 ) ) ) );
	EON_TEST_2STEP( string, findFirst, in_substr_false,
		string obj( "abcdefabcdef" ),
		EON_FALSE( obj.findFirst( "fa", obj.substr( 6, 4 ) ) ) );

	EON_TEST( string, findFirst, char_t_true,
		EON_TRUE( string( "abcdefabcdef" ).findFirst( char_t( 'f' ) ) ) );
	EON_TEST( string, findFirst, char_t_false,
		EON_FALSE( string( "abcdefabcdef" ).findFirst( char_t( 'g' ) ) ) );

	EON_TEST_2STEP( string, findFirst, char_t_substr_true,
		string obj( "abcdefabcdef" ),
		EON_TRUE( obj.findFirst( char_t( 'f' ), obj.substr( 4, 4 ) ) ) );
	EON_TEST_2STEP( string, findFirst, char_t_substr_false,
		string obj( "abcdefabcdef" ),
		EON_FALSE( obj.findFirst( char_t( 'f' ), obj.substr( 6, 4 ) ) ) );

	EON_TEST( string, findFirst, char_true,
		EON_TRUE( string( "abcdefabcdef" ).findFirst( 'f' ) ) );
	EON_TEST( string, findFirst, char_false,
		EON_FALSE( string( "abcdefabcdef" ).findFirst( 'g' ) ) );

	EON_TEST_2STEP( string, findFirst, char_substr_true,
		string obj( "abcdefabcdef" ),
		EON_TRUE( obj.findFirst( 'f', obj.substr( 4, 4 ) ) ) );
	EON_TEST_2STEP( string, findFirst, char_substr_false,
		string obj( "abcdefabcdef" ),
		EON_FALSE( obj.findFirst( 'f', obj.substr( 6, 4 ) ) ) );


	EON_TEST( string, findFirstNumber, empty,
		EON_FALSE( string().findFirstNumber() ) );
	EON_TEST( string, findFirstNumber, non_number,
		EON_FALSE( string( "abcdef" ).findFirstNumber() ) );
	EON_TEST( string, findFirstNumber, all_number,
		EON_TRUE( string( "123456" ).findFirstNumber() ) );
	EON_TEST( string, findFirstNumber, part_number,
		EON_TRUE( string( "ab34ef" ).findFirstNumber() ) );


	EON_TEST( string, findFirstOf, str_empty,
		EON_FALSE( string( "abcdef" ).findFirstOf( string() ) ) );
	EON_TEST( string, findFirstOf, str_true,
		EON_TRUE( string( "abcdef" ).findFirstOf( string( "ce" ) ) ) );
	EON_TEST( string, findFirstOf, str_false,
		EON_FALSE( string( "abcdef" ).findFirstOf( string( "gh" ) ) ) );

	EON_TEST( string, findFirstOf, cat_true,
		EON_TRUE( string( "ab23ef" ).findFirstOf( charcat::number ) ) );
	EON_TEST( string, findFirstOf, cat_false,
		EON_FALSE( string( "abcdef" ).findFirstOf( charcat::number ) ) );

	EON_TEST_2STEP( string, findFirstOf, str_sub_true,
		string obj( "abcdefabcdef" ),
		EON_TRUE( obj.findFirstOf( string( "ef" ), obj.substr( 3, 4 ) ) ) );
	EON_TEST_2STEP( string, findFirstOf, str_sub_false,
		string obj( "abcdefabcdef" ),
		EON_FALSE( obj.findFirstOf( string( "ef" ), obj.substr( 6, 4 ) ) ) );

	EON_TEST_2STEP( string, findFirstOf, cat_sub_true,
		string obj( "ab23cdef" ),
		EON_TRUE( obj.findFirstOf( charcat::number, obj.substr( 1, 4 ) ) ) );
	EON_TEST_2STEP( string, findFirstOf, cat_sub_false,
		string obj( "ab23cdef" ),
		EON_FALSE( obj.findFirstOf( charcat::number, obj.substr( 4, 4 ) ) ) );


	EON_TEST_2STEP( string, findFirstNotOf, str_empty,
		string obj( "abcdef" ),
		EON_EQ( obj.end(), obj.findFirstNotOf( string() ) ) );
	EON_TEST_2STEP( string, findFirstNotOf, str_begin,
		string obj( "abcdef" ),
		EON_EQ( obj.begin(), obj.findFirstNotOf( string( "ce" ) ) ) );
	EON_TEST_2STEP( string, findFirstNotOf, str_mid,
		string obj( "abcdef" ),
		EON_EQ( obj.begin() + 2, obj.findFirstNotOf( string( "ba" ) ) ) );
	EON_TEST_2STEP( string, findFirstNotOf, str_none,
		string obj( "abcdef" ),
		EON_EQ( obj.end(), obj.findFirstNotOf( string( "fedcba" ) ) ) );

	EON_TEST_2STEP( string, findFirstNotOf, cat_begin,
		string obj( "ab23ef" ),
		EON_EQ( obj.begin(), obj.findFirstNotOf( charcat::number ) ) );
	EON_TEST_2STEP( string, findFirstNotOf, cat_mid,
		string obj( "12cd56" ),
		EON_EQ( obj.begin() + 2, obj.findFirstNotOf( charcat::number ) ) );
	EON_TEST_2STEP( string, findFirstNotOf, cat_none,
		string obj( "123456" ),
		EON_EQ( obj.end(), obj.findFirstNotOf( charcat::number ) ) );

	EON_TEST_2STEP( string, findFirstNotOf, str_sub_mid,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.begin() + 6, obj.findFirstNotOf( string( "ef" ), obj.substr( 4, 4 ) ) ) );
	EON_TEST_2STEP( string, findFirstNotOf, str_sub_none,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.end(), obj.findFirstNotOf( string( "abcd" ), obj.substr( 6, 4 ) ) ) );

	EON_TEST_2STEP( string, findFirstNotOf, cat_sub_mid,
		string obj( "ab23ef67" ),
		EON_EQ( obj.begin() + 4, obj.findFirstNotOf( charcat::number, obj.substr( 2, 4 ) ) ) );
	EON_TEST_2STEP( string, findFirstNotOf, cat_sub_none,
		string obj( "ab2345ef" ),
		EON_EQ( obj.end(), obj.findFirstNotOf( charcat::number, obj.substr( 2, 4 ) ) ) );


	EON_TEST_2STEP( string, findFirstNotDoubleQuoted, str_found,
		string obj( "ab\"cdef\"cdef" ),
		EON_EQ( obj.substr( 8, 2 ), obj.findFirstNotDoubleQuoted( string( "cd" ) ) ) );
	EON_TEST_2STEP( string, findFirstNotDoubleQuoted, str_notfound,
		string obj( "ab\"cdef\"ef" ),
		EON_FALSE( obj.findFirstNotDoubleQuoted( string( "cd" ) ) ) );

	EON_TEST_2STEP( string, findFirstNotDoubleQuoted, char_t_found,
		string obj( "ab\"cdef\"cdef" ),
		EON_EQ( obj.substr( 8, 1 ), obj.findFirstNotDoubleQuoted( char_t( 'c' ) ) ) );
	EON_TEST_2STEP( string, findFirstNotDoubleQuoted, char_t_notfound,
		string obj( "ab\"cdef\"ef" ),
		EON_FALSE( obj.findFirstNotDoubleQuoted( char_t( 'c' ) ) ) );

	EON_TEST_2STEP( string, findFirstNotSingleQuoted, str_found,
		string obj( "ab'cdef'cdef" ),
		EON_EQ( obj.substr( 8, 2 ), obj.findFirstNotSingleQuoted( string( "cd" ) ) ) );
	EON_TEST_2STEP( string, findFirstNotSingleQuoted, str_notfound,
		string obj( "ab'cdef'ef" ),
		EON_FALSE( obj.findFirstNotSingleQuoted( string( "cd" ) ) ) );

	EON_TEST_2STEP( string, findFirstNotSingleQuoted, char_t_found,
		string obj( "ab'cdef'cdef" ),
		EON_EQ( obj.substr( 8, 1 ), obj.findFirstNotSingleQuoted( char_t( 'c' ) ) ) );
	EON_TEST_2STEP( string, findFirstNotSingleQuoted, char_t_notfound,
		string obj( "ab'cdef'ef" ),
		EON_FALSE( obj.findFirstNotSingleQuoted( char_t( 'c' ) ) ) );

	EON_TEST_2STEP( string, findFirstNotBraced, str_found,
		string obj( "ab(cdef)cdef" ),
		EON_EQ( obj.substr( 8, 2 ), obj.findFirstNotBraced( string( "cd" ) ) ) );
	EON_TEST_2STEP( string, findFirstNotBraced, str_notfound,
		string obj( "ab(cdef)ef" ),
		EON_FALSE( obj.findFirstNotBraced( string( "cd" ) ) ) );

	EON_TEST_2STEP( string, findFirstNotBraced, char_t_found,
		string obj( "ab(cdef)cdef" ),
		EON_EQ( obj.substr( 8, 1 ), obj.findFirstNotBraced( char_t( 'c' ) ) ) );
	EON_TEST_2STEP( string, findFirstNotBraced, char_t_notfound,
		string obj( "ab(cdef)ef" ),
		EON_FALSE( obj.findFirstNotBraced( char_t( 'c' ) ) ) );


	EON_TEST_2STEP( string, findFirstDiff, str_none,
		string obj( "abcdef" ),
		EON_EQ( obj.end(), obj.findFirstDiff( string( "abcdef" ) ) ) );
	EON_TEST_2STEP( string, findFirstDiff, str_found,
		string obj( "abcdef" ),
		EON_EQ( obj.begin() + 3, obj.findFirstDiff( string( "abcDEF" ) ) ) );




	EON_TEST( string, findLast, str_true,
		EON_TRUE( string( "abcdefabcdef" ).findLast( "ef" ) ) );
	EON_TEST( string, findLast, str_false,
		EON_FALSE( string( "abcdefabcdef" ).findLast( "fe" ) ) );

	EON_TEST_2STEP( string, findLast, in_substr_true,
		string obj( "abcdefabcdef" ),
		EON_TRUE( obj.findLast( "fa", obj.substr( 4, 4 ) ) ) );
	EON_TEST_2STEP( string, findLast, in_substr_false,
		string obj( "abcdefabcdef" ),
		EON_FALSE( obj.findLast( "fa", obj.substr( 6, 4 ) ) ) );

	EON_TEST( string, findLast, char_t_true,
		EON_TRUE( string( "abcdefabcdef" ).findLast( char_t( 'f' ) ) ) );
	EON_TEST( string, findLast, char_t_false,
		EON_FALSE( string( "abcdefabcdef" ).findLast( char_t( 'g' ) ) ) );

	EON_TEST_2STEP( string, findLast, char_t_substr_true,
		string obj( "abcdefabcdef" ),
		EON_TRUE( obj.findLast( char_t( 'f' ), obj.substr( 4, 4 ) ) ) );
	EON_TEST_2STEP( string, findLast, char_t_substr_false,
		string obj( "abcdefabcdef" ),
		EON_FALSE( obj.findLast( char_t( 'f' ), obj.substr( 6, 4 ) ) ) );

	EON_TEST( string, findLast, char_true,
		EON_TRUE( string( "abcdefabcdef" ).findLast( 'f' ) ) );
	EON_TEST( string, findLast, char_false,
		EON_FALSE( string( "abcdefabcdef" ).findLast( 'g' ) ) );

	EON_TEST_2STEP( string, findLast, char_substr_true,
		string obj( "abcdefabcdef" ),
		EON_TRUE( obj.findLast( 'f', obj.substr( 4, 4 ) ) ) );
	EON_TEST_2STEP( string, findLast, char_substr_false,
		string obj( "abcdefabcdef" ),
		EON_FALSE( obj.findLast( 'f', obj.substr( 6, 4 ) ) ) );


	EON_TEST( string, findLastOf, str_empty,
		EON_FALSE( string( "abcdef" ).findLastOf( string() ) ) );
	EON_TEST( string, findLastOf, str_true,
		EON_TRUE( string( "abcdef" ).findLastOf( string( "ce" ) ) ) );
	EON_TEST( string, findLastOf, str_false,
		EON_FALSE( string( "abcdef" ).findLastOf( string( "gh" ) ) ) );

	EON_TEST( string, findLastOf, cat_true,
		EON_TRUE( string( "ab23ef" ).findLastOf( charcat::number ) ) );
	EON_TEST( string, findLastOf, cat_false,
		EON_FALSE( string( "abcdef" ).findLastOf( charcat::number ) ) );

	EON_TEST_2STEP( string, findLastOf, str_sub_true,
		string obj( "abcdefabcdef" ),
		EON_TRUE( obj.findLastOf( string( "ef" ), obj.substr( 3, 4 ) ) ) );
	EON_TEST_2STEP( string, findLastOf, str_sub_false,
		string obj( "abcdefabcdef" ),
		EON_FALSE( obj.findLastOf( string( "ef" ), obj.substr( 6, 4 ) ) ) );

	EON_TEST_2STEP( string, findLastOf, cat_sub_true,
		string obj( "ab23cdef" ),
		EON_TRUE( obj.findLastOf( charcat::number, obj.substr( 1, 4 ) ) ) );
	EON_TEST_2STEP( string, findLastOf, cat_sub_false,
		string obj( "ab23cdef" ),
		EON_FALSE( obj.findLastOf( charcat::number, obj.substr( 4, 4 ) ) ) );


	EON_TEST_2STEP( string, findLastNotOf, str_empty,
		string obj( "abcdef" ),
		EON_EQ( obj.end(), obj.findLastNotOf( string() ) ) );
	EON_TEST_2STEP( string, findLastNotOf, str_last,
		string obj( "abcdef" ),
		EON_EQ( obj.begin() + 5, obj.findLastNotOf( string( "ce" ) ) ) );
	EON_TEST_2STEP( string, findLastNotOf, str_mid,
		string obj( "abcdef" ),
		EON_EQ( obj.begin() + 3, obj.findLastNotOf( string( "ef" ) ) ) );
	EON_TEST_2STEP( string, findLastNotOf, str_none,
		string obj( "abcdef" ),
		EON_EQ( obj.end(), obj.findLastNotOf( string( "fedcba" ) ) ) );

	EON_TEST_2STEP( string, findLastNotOf, cat_last,
		string obj( "ab23ef" ),
		EON_EQ( obj.begin() + 5, obj.findLastNotOf( charcat::number ) ) );
	EON_TEST_2STEP( string, findLastNotOf, cat_mid,
		string obj( "12cd56" ),
		EON_EQ( obj.begin() + 3, obj.findLastNotOf( charcat::number ) ) );
	EON_TEST_2STEP( string, findLastNotOf, cat_none,
		string obj( "123456" ),
		EON_EQ( obj.end(), obj.findLastNotOf( charcat::number ) ) );

	EON_TEST_2STEP( string, findLastNotOf, str_sub_mid,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.begin() + 7, obj.findLastNotOf( string( "ef" ), obj.substr( 4, 4 ) ) ) );
	EON_TEST_2STEP( string, findLastNotOf, str_sub_none,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.end(), obj.findLastNotOf( string( "abcd" ), obj.substr( 6, 4 ) ) ) );

	EON_TEST_2STEP( string, findLastNotOf, cat_sub_mid,
		string obj( "abcd23ef" ),
		EON_EQ( obj.begin() + 3, obj.findLastNotOf( charcat::number, obj.substr( 2, 4 ) ) ) );
	EON_TEST_2STEP( string, findLastNotOf, cat_sub_none,
		string obj( "ab2345ef" ),
		EON_EQ( obj.end(), obj.findLastNotOf( charcat::number, obj.substr( 2, 4 ) ) ) );
}
