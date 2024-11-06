#include "Substring.h"
#include <eoninlinetest/InlineTest.h>
#include <cctype>


namespace eon
{
	EON_TEST( substring, contains, substr_void_void,
		EON_FALSE( substring().contains( substring() ) ) );
	EON_TEST( substring, contains, substr_void_empty,
		EON_FALSE( substring().contains( substring( "" ) ) ) );
	EON_TEST( substring, contains, substr_empty_void,
		EON_FALSE( substring( "" ).contains( substring() ) ) );
	EON_TEST( substring, contains, substr_true,
		EON_TRUE( substring( "abcdef" ).contains( substring( "cd" ) ) ) );
	EON_TEST( substring, contains, substr_false,
		EON_FALSE( substring( "abcdef" ).contains( substring( "ce" ) ) ) );

	EON_TEST( substring, contains, char_t_true,
		EON_TRUE( substring( "abcdef" ).contains( char_t( 'c' ) ) ) );
	EON_TEST( substring, contains, char_t_false,
		EON_FALSE( substring( "abcdef" ).contains( char_t( 'g' ) ) ) );

	EON_TEST( substring, containsAnyOf, empty,
		EON_FALSE( substring( "abcdef" ).containsAnyOf( substring( "" ) ) ) );
	EON_TEST( substring, containsAnyOf, one,
		EON_TRUE( substring( "abcdef" ).containsAnyOf( substring( "cgh" ) ) ) );
	EON_TEST( substring, containsAnyOf, several,
		EON_TRUE( substring( "abcdef" ).containsAnyOf( substring( "ghfe" ) ) ) );

	EON_TEST( substring, containsNoneOf, empty,
		EON_TRUE( substring( "abcdef" ).containsNoneOf( substring( "" ) ) ) );
	EON_TEST( substring, containsNoneOf, false,
		EON_FALSE( substring( "abcdef" ).containsNoneOf( substring( "cgh" ) ) ) );
	EON_TEST( substring, containsNoneOf, true,
		EON_TRUE( substring( "abcdef" ).containsNoneOf( substring( "ghij" ) ) ) );


	EON_TEST( substring, findFirst, substr_void_void,
		EON_FALSE( substring().findFirst( substring() ) ) );
	EON_TEST( substring, findFirst, substr_void_empty,
		EON_FALSE( substring().findFirst( substring( "" ) ) ) );
	EON_TEST( substring, findFirst, substr_empty_void,
		EON_FALSE( substring( "" ).findFirst( substring() ) ) );
	EON_TEST_2STEP( substring, findFirst, substr_ASCII_at_start,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.substr( 0, 2 ), obj.substr().findFirst( substring( "ab" ) ) ) );
	EON_TEST_2STEP( substring, findFirst, substr_ASCII_at_middle,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.substr( 2, 2 ), obj.substr().findFirst( substring( "cd" ) ) ) );
	EON_TEST_2STEP( substring, findFirst, substr_ASCII_at_end,
		string obj( "abcdef" ),
		EON_EQ( obj.substr( 4, 2 ), obj.substr().findFirst( substring( "ef" ) ) ) );
	EON_TEST_2STEP( substring, findFirst, substr_UTF8_at_start,
		string obj( u8"Ø€Œ™©µØ€Œ™©µ" ),
		EON_EQ( obj.substr( 0, 2 ), obj.substr().findFirst( substring( u8"Ø€" ) ) ) );
	EON_TEST_2STEP( substring, findFirst, substr_UTF8_at_middle,
		string obj( u8"Ø€Œ™©µØ€Œ™©µ" ),
		EON_EQ( obj.substr( 2, 2 ), obj.substr().findFirst( substring( u8"Œ™" ) ) ) );
	EON_TEST_2STEP( substring, findFirst, substr_UTF8_at_end,
		string obj( u8"Ø€Œ™©µ" ),
		EON_EQ( obj.substr( 4, 2 ), obj.substr().findFirst( substring( u8"©µ" ) ) ) );
	EON_TEST_2STEP( substring, findFirst, substr_beyond_end,
		string obj( "abcdef" ),
		EON_FALSE( obj.substr().findFirst( substring( "efg" ) ) ) );

	EON_TEST_2STEP( substring, findFirst, char_t_ASCII_at_start,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.substr( 0, 1 ), obj.substr().findFirst( char_t( 'a' ) ) ) );
	EON_TEST_2STEP( substring, findFirst, char_t_ASCII_at_middle,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.substr( 3, 1 ), obj.substr().findFirst( char_t( 'd' ) ) ) );
	EON_TEST_2STEP( substring, findFirst, char_t_ASCII_at_end,
		string obj( "abcdef" ),
		EON_EQ( obj.substr( 5, 1 ), obj.substr().findFirst( char_t( 'f' ) ) ) );
	EON_TEST_2STEP( substring, findFirst, char_t_UTF8_at_start,
		string obj( u8"Ø€Œ™©µØ€Œ™©µ" ),
		EON_EQ( obj.substr( 0, 1 ), obj.substr().findFirst( char_t( 216 ) ) ) );
	EON_TEST_2STEP( substring, findFirst, char_t_UTF8_at_middle,
		string obj( u8"Ø€Œ™©µØ€Œ™©µ" ),
		EON_EQ( obj.substr( 3, 1 ), obj.substr().findFirst( char_t( 8482 ) ) ) );
	EON_TEST_2STEP( substring, findFirst, char_t_UTF8_at_end,
		string obj( u8"Ø€Œ™©µ" ),
		EON_EQ( obj.substr( 5, 1 ), obj.substr().findFirst( char_t( 181 ) ) ) );
	EON_TEST_2STEP( substring, findFirst, char_t_beyond_end,
		string obj( "abcdef" ),
		EON_EQ( obj.end(), obj.substr().findFirst( char_t( 'g' ) ).begin() ) );


	substring substring::findFirstNumber( string_iterator* separator, const locale* custom_locale )
	{
		if( isHighToLow() )
			return lowToHigh().findFirstNumber( separator, custom_locale );
		auto& chars = Characters::get();
		const locale& loc = custom_locale != nullptr ? *custom_locale : locale::get();
		substring number;
		for( string_iterator i = begin(); i != end(); ++i )
		{
			if( !number.begin() )
			{
				if( chars.isNumber( *i ) )
					number.begin() = i; number.end() = i + 1;
			}
			else
			{
				if( chars.isNumber( *i ) )
					++number.end();
				else if( separator != nullptr && *i == loc.decimalSep() && !*separator )
					*separator = i;
				else
					break;
			}
		}
		return number;
	}
	EON_TEST( substring, findFirstNumber, substr_void,
		EON_FALSE( substring().findFirstNumber() ) );
	EON_TEST( substring, findFirstNumber, substr_empty,
		EON_FALSE( substring( "" ).findFirstNumber() ) );
	EON_TEST( substring, findFirstNumber, substr_no_numbers,
		EON_FALSE( substring( "abcdef" ).findFirstNumber() ) );
	EON_TEST_2STEP( substring, findFirstNumber, substr_at_start,
		string obj( "123abcdef123abcdef" ),
		EON_EQ( obj.substr( 0, 3 ), obj.substr().findFirstNumber() ) );
	EON_TEST_2STEP( substring, findFirstNumber, substr_at_middle,
		string obj( "abc456def123abcdef" ),
		EON_EQ( obj.substr( 3, 3 ), obj.findFirstNumber() ) );
	EON_TEST_2STEP( substring, findFirstNumber, substr_at_end,
		string obj( "abcdef789" ),
		EON_EQ( obj.substr( 6, 3 ), obj.findFirstNumber() ) );


	EON_TEST( substring, findLast, substr_void_void,
		EON_FALSE( substring().findLast( substring() ) ) );
	EON_TEST( substring, findLast, substr_void_empty,
		EON_FALSE( substring().findLast( substring( "" ) ) ) );
	EON_TEST( substring, findLast, substr_empty_void,
		EON_FALSE( substring( "" ).findLast( substring() ) ) );
	EON_TEST_2STEP( substring, findLast, substr_ASCII_at_start,
		string obj( "abcdef" ),
		EON_EQ( obj.substr( 0, 2 ), obj.substr().findLast( substring( "ab" ) ) ) );
	EON_TEST_2STEP( substring, findLast, substr_ASCII_at_middle,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.substr( 8, 2 ), obj.substr().findLast( substring( "cd" ) ) ) );
	EON_TEST_2STEP( substring, findLast, substr_ASCII_at_end,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.substr( 10, 2 ), obj.substr().findLast( substring( "ef" ) ) ) );
	EON_TEST_2STEP( substring, findLast, substr_UTF8_at_start,
		string obj( u8"Ø€Œ™©µ" ),
		EON_EQ( obj.substr( 0, 2 ), obj.substr().findLast( substring( u8"Ø€" ) ) ) );
	EON_TEST_2STEP( substring, findLast, substr_UTF8_at_middle,
		string obj( u8"Ø€Œ™©µØ€Œ™©µ" ),
		EON_EQ( obj.substr( 8, 2 ), obj.substr().findLast( substring( u8"Œ™" ) ) ) );
	EON_TEST_2STEP( substring, findLast, substr_UTF8_at_end,
		string obj( u8"Ø€Œ™©µØ€Œ™©µ" ),
		EON_EQ( obj.substr( 10, 2 ), obj.substr().findLast( substring( u8"©µ" ) ) ) );
	EON_TEST_2STEP( substring, findLast, substr_beyond_end,
		string obj( "abcdefabcdef" ),
		EON_FALSE( obj.substr().findLast( substring( "efg" ) ) ) );

	EON_TEST_2STEP( substring, findLast, char_t_ASCII_at_start,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.substr( 6, 1 ), obj.substr().findLast( char_t( 'a' ) ) ) );
	EON_TEST_2STEP( substring, findLast, char_t_ASCII_at_middle,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.substr( 9, 1 ), obj.substr().findLast( char_t( 'd' ) ) ) );
	EON_TEST_2STEP( substring, findLast, char_t_ASCII_at_end,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.substr( 11, 1 ), obj.substr().findLast( char_t( 'f' ) ) ) );
	EON_TEST_2STEP( substring, findLast, char_t_UTF8_at_start,
		string obj( u8"Ø€Œ™©µØ€Œ™©µ" ),
		EON_EQ( obj.substr( 6, 1 ), obj.substr().findLast( char_t( 216 ) ) ) );
	EON_TEST_2STEP( substring, findLast, char_t_UTF8_at_middle,
		string obj( u8"Ø€Œ™©µØ€Œ™©µ" ),
		EON_EQ( obj.substr( 9, 1 ), obj.substr().findLast( char_t( 8482 ) ) ) );
	EON_TEST_2STEP( substring, findLast, char_t_UTF8_at_end,
		string obj( u8"Ø€Œ™©µØ€Œ™©µ" ),
		EON_EQ( obj.substr( 11, 1 ), obj.substr().findLast( char_t( 181 ) ) ) );
	EON_TEST_2STEP( substring, findLast, char_t_beyond_end,
		string obj( "abcdefabcdef" ),
		EON_FALSE( obj.substr().findLast( char_t( 'g' ) ) ) );


	EON_TEST_2STEP( substring, findFirstOf, chars_found,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.begin() + 2, obj.substr().findFirstOf( substring( "ghc" ) ) ) );
	EON_TEST_2STEP( substring, findFirstOf, chars_not_found,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.end(), obj.substr().findFirstOf( substring( "ghi" ) ) ) );

	string_iterator substring::findFirstOf( charcat category ) const noexcept
	{
		if( empty() )
			return End.getEnd();
		else if( isHighToLow() )
			return lowToHigh().findFirstOf( category );
		auto& chars = Characters::get();
		for( auto i = begin(); i != end(); ++i )
		{
			if( chars.is( *i, category ) )
				return i;
		}
		return End.getEnd();
	}
	EON_TEST_2STEP( substring, findFirstOf, cat_found,
		string obj( "ab12cd34ef" ),
		EON_EQ( obj.begin() + 2, obj.substr().findFirstOf( charcat::number ) ) );
	EON_TEST_2STEP( substring, findFirstOf, cat_not_found,
		string obj( "abcdef" ),
		EON_EQ( obj.end(), obj.substr().findFirstOf( charcat::number ) ) );


	EON_TEST_2STEP( substring, findLastOf, chars_found,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.begin() + 8, obj.substr().highToLow().findLastOf( substring( "ghc" ) ) ) );
	EON_TEST_2STEP( substring, findLastOf, chars_not_found,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.end(), obj.substr().highToLow().findLastOf( substring( "ghi" ) ) ) );

	string_iterator substring::findLastOf( charcat category ) const noexcept
	{
		if( empty() )
			return End.getEnd();
		else if( isLowToHigh() )
			return highToLow().findLastOf( category );
		auto& chars = Characters::get();
		for( auto i = begin(); i != end(); --i )
		{
			if( chars.is( *i, category ) )
				return i;
		}
		return End.getEnd();
	}
	EON_TEST_2STEP( substring, findLastOf, cat_found,
		string obj( "ab12cd34ef" ),
		EON_EQ( obj.begin() + 7, obj.substr().highToLow().findLastOf( charcat::number ) ) );
	EON_TEST_2STEP( substring, findLastOf, cat_not_found,
		string obj( "abcdef" ),
		EON_EQ( obj.end(), obj.substr().highToLow().findLastOf( charcat::number ) ) );


	EON_TEST_2STEP( substring, findFirstNotOf, chars_found,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.begin() + 2, obj.substr().findFirstNotOf( substring( "abefgh" ) ) ) );
	EON_TEST_2STEP( substring, findFirstNotOf, chars_not_found,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.end(), obj.substr().findFirstNotOf( substring( "abcdef" ) ) ) );

	string_iterator substring::findFirstNotOf( charcat category ) const noexcept
	{
		if( empty() )
			return End.getEnd();
		else if( isHighToLow() )
			return lowToHigh().findFirstNotOf( category );
		auto& chars = Characters::get();
		for( string_iterator i = begin(); i != end(); ++i )
		{
			if( !chars.is( *i, category ) )
				return i;
		}
		return End.getEnd();
	}
	EON_TEST_2STEP( substring, findFirstNotOf, cat_found,
		string obj( "12abcdef34abcdef56" ),
		EON_EQ( obj.begin() + 2, obj.substr().findFirstNotOf( charcat::number ) ) );
	EON_TEST_2STEP( substring, findFirstNotOf, cat_not_found,
		string obj( "1234567890987654321" ),
		EON_EQ( obj.end(), obj.substr().findFirstNotOf( charcat::number ) ) );


	EON_TEST_2STEP( substring, findLastNotOf, chars_found,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.begin() + 9, obj.substr().highToLow().findLastNotOf( substring( "abefgh" ) ) ) );
	EON_TEST_2STEP( substring, findLastNotOf, chars_not_found,
		string obj( "abcdefabcdef" ),
		EON_EQ( obj.end(), obj.substr().highToLow().findLastNotOf( substring( "abcdef" ) ) ) );

	string_iterator substring::findLastNotOf( charcat category ) const noexcept
	{
		if( empty() )
			return End.getEnd();
		else if( isLowToHigh() )
			return highToLow().findLastNotOf( category );
		auto& chars = Characters::get();
		for( string_iterator i = begin(); i != end(); --i )
		{
			if( !chars.is( *i, category ) )
				return i;
		}
		return End.getEnd();
	}
	EON_TEST_2STEP( substring, findLastNotOf, cat_found,
		string obj( "12abcdef34abcdef56" ),
		EON_EQ( obj.begin() + 15, obj.substr().highToLow().findLastNotOf( charcat::number ) ) );
	EON_TEST_2STEP( substring, findLastNotOf, cat_not_found,
		string obj( "1234567890987654321" ),
		EON_EQ( obj.end(), obj.substr().highToLow().findLastNotOf( charcat::number ) ) );


	EON_TEST_2STEP( substring, findFirstDiff, void_void,
		string obj,
		EON_EQ( obj.end(), obj.substr().findFirstDiff( substring() ) ) );
	EON_TEST_2STEP( substring, findFirstDiff, void_empty,
		string obj,
		EON_EQ( obj.end(), obj.substr().findFirstDiff( substring( "" ) ) ) );
	EON_TEST_2STEP( substring, findFirstDiff, empty_void,
		string obj( "" ),
		EON_EQ( obj.end(), obj.substr().findFirstDiff( substring() ) ) );
	EON_TEST_2STEP( substring, findFirstDiff, empty_empty,
		string obj( "" ),
		EON_EQ( obj.end(), obj.substr().findFirstDiff( substring( "" ) ) ) );
	EON_TEST_2STEP( substring, findFirstDiff, begin,
		string obj( "abcdef" ),
		EON_EQ( obj.begin(), obj.substr().findFirstDiff( substring( "Abcdef" ) ) ) );
	EON_TEST_2STEP( substring, findFirstDiff, middle,
		string obj( "abcdef" ),
		EON_EQ( obj.begin() + 2, obj.substr().findFirstDiff( substring( "abCdef" ) ) ) );
	EON_TEST_2STEP( substring, findFirstDiff, last,
		string obj( "abcdef" ),
		EON_EQ( obj.last(), obj.substr().findFirstDiff( substring( "abcdeF" ) ) ) );


	EON_TEST_2STEP( substring, findFirstIgnoreSections, substr_at_start,
		string obj( "(abc)defabcdef" ),
		EON_EQ( obj.substr( 8, 2 ), obj.substr().findFirstIgnoreSections( substring( "ab" ), '(', ')' ) ) );
	EON_TEST_2STEP( substring, findFirstIgnoreSections, substr_at_middle,
		string obj( "(abc)defabcdef" ),
		EON_EQ( obj.substr( 10, 2 ), obj.substr().findFirstIgnoreSections( substring( "cd" ), '(', ')' ) ) );

	EON_TEST_2STEP( substring, findFirstIgnoreSections, char_t_at_start,
		string obj( "(abcd)efabcdef" ),
		EON_EQ( obj.substr( 8, 1 ), obj.substr().findFirstIgnoreSections( char_t( 'a' ), '(', ')' ) ) );
	EON_TEST_2STEP( substring, findFirstIgnoreSections, char_t_at_middle,
		string obj( "(abcd)efabcdef" ),
		EON_EQ( obj.substr( 11, 1 ), obj.substr().findFirstIgnoreSections( char_t( 'd' ), '(', ')' ) ) );


	EON_TEST( substring, beforeFirst, substr_ASCII_first,
		EON_EQ( "", substring( "abcdefabcdef" ).beforeFirst( substring( "ab" ) ) ) );
	EON_TEST( substring, beforeFirst, substr_ASCII_middle,
		EON_EQ( "ab", substring( "abcdefabcdef" ).beforeFirst( substring( "cd" ) ) ) );
	EON_TEST( substring, beforeFirst, substr_ASCII_last,
		EON_EQ( "abcd", substring( "abcdefabcdef" ).beforeFirst( substring( "ef" ) ) ) );
	EON_TEST( substring, beforeFirst, substr_UTF8_first,
		EON_EQ( u8"", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).beforeFirst( substring( u8"Ø€" ) ) ) );
	EON_TEST( substring, beforeFirst, substr_UTF8_middle,
		EON_EQ( u8"Ø€", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).beforeFirst( substring( u8"Œ™" ) ) ) );
	EON_TEST( substring, beforeFirst, substr_UTF8_end,
		EON_EQ( u8"Ø€Œ™", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).beforeFirst( substring( u8"©µ" ) ) ) );

	EON_TEST( substring, beforeFirst, char_t_ASCII_first,
		EON_EQ( "", substring( "abcdefabcdef" ).beforeFirst( char_t( 'a' ) ) ) );
	EON_TEST( substring, beforeFirst, char_t_ASCII_middle,
		EON_EQ( "abc", substring( "abcdefabcdef" ).beforeFirst( char_t( 'd' ) ) ) );
	EON_TEST( substring, beforeFirst, char_t_ASCII_last,
		EON_EQ( "abcde", substring( "abcdefabcdef" ).beforeFirst( char_t( 'f' ) ) ) );
	EON_TEST( substring, beforeFirst, char_t_UTF8_at_start,
		EON_EQ( u8"", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).beforeFirst( char_t( 216 ) ) ) );
	EON_TEST( substring, beforeFirst, char_t_UTF8_at_middle,
		EON_EQ( u8"Ø€Œ", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).beforeFirst( char_t( 8482 ) ) ) );
	EON_TEST( substring, beforeFirst, char_t_UTF8_at_end,
		EON_EQ( u8"Ø€Œ™©", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).beforeFirst( char_t( 181 ) ) ) );


	EON_TEST( substring, beforeLast, substr_ASCII_first,
		EON_EQ( "abcdef", substring( "abcdefabcdef" ).beforeLast( substring( "ab" ) ) ) );
	EON_TEST( substring, beforeLast, substr_ASCII_middle,
		EON_EQ( "abcdefab", substring( "abcdefabcdef" ).beforeLast( substring( "cd" ) ) ) );
	EON_TEST( substring, beforeLast, substr_ASCII_end,
		EON_EQ( "abcdefabcd", substring( "abcdefabcdef" ).beforeLast( substring( "ef" ) ) ) );
	EON_TEST( substring, beforeLast, substr_UTF8_first,
		EON_EQ( u8"Ø€Œ™©µ", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).beforeLast( substring( u8"Ø€" ) ) ) );
	EON_TEST( substring, beforeLast, substr_UTF8_middle,
		EON_EQ( u8"Ø€Œ™©µØ€", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).beforeLast( substring( u8"Œ™" ) ) ) );
	EON_TEST( substring, beforeLast, substr_UTF8_end,
		EON_EQ( u8"Ø€Œ™©µØ€Œ™", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).beforeLast( substring( u8"©µ" ) ) ) );

	EON_TEST( substring, beforeLast, char_t_ASCII_first,
		EON_EQ( "abcdef", substring( "abcdefabcdef" ).beforeLast( char_t( 'a' ) ) ) );
	EON_TEST( substring, beforeLast, char_t_ASCII_middle,
		EON_EQ( "abcdefab", substring( "abcdefabcdef" ).beforeLast( char_t( 'c' ) ) ) );
	EON_TEST( substring, beforeLast, char_t_ASCII_last,
		EON_EQ( "abcdefabcde", substring( "abcdefabcdef" ).beforeLast( char_t( 'f' ) ) ) );
	EON_TEST( substring, beforeLast, char_t_UTF8_at_start,
		EON_EQ( u8"Ø€Œ™©µ", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).beforeLast( char_t( 216 ) ) ) );
	EON_TEST( substring, beforeLast, char_t_UTF8_at_middle,
		EON_EQ( u8"Ø€Œ™©µØ€Œ", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).beforeLast( char_t( 8482 ) ) ) );
	EON_TEST( substring, beforeLast, char_t_UTF8_at_end,
		EON_EQ( u8"Ø€Œ™©µØ€Œ™©", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).beforeLast( char_t( 181 ) ) ) );


	EON_TEST( substring, afterFirst, substr_ASCII_first,
		EON_EQ( "cdefabcdef", substring( "abcdefabcdef" ).afterFirst( substring( "ab" ) ) ) );
	EON_TEST( substring, afterFirst, substr_ASCII_middle,
		EON_EQ( "efabcdef", substring( "abcdefabcdef" ).afterFirst( substring( "cd" ) ) ) );
	EON_TEST( substring, afterFirst, substr_ASCII_last,
		EON_EQ( "abcdef", substring( "abcdefabcdef" ).afterFirst( substring( "ef" ) ) ) );
	EON_TEST( substring, afterFirst, substr_UTF8_first,
		EON_EQ( u8"Œ™©µØ€Œ™©µ", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).afterFirst( substring( u8"Ø€" ) ) ) );
	EON_TEST( substring, afterFirst, substr_UTF8_middle,
		EON_EQ( u8"©µØ€Œ™©µ", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).afterFirst( substring( u8"Œ™" ) ) ) );
	EON_TEST( substring, afterFirst, substr_UTF8_end,
		EON_EQ( u8"Ø€Œ™©µ", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).afterFirst( substring( u8"©µ" ) ) ) );

	EON_TEST( substring, afterFirst, char_t_ASCII_first,
		EON_EQ( "bcdefabcdef", substring( "abcdefabcdef" ).afterFirst( char_t( 'a' ) ) ) );
	EON_TEST( substring, afterFirst, char_t_ASCII_middle,
		EON_EQ( "efabcdef", substring( "abcdefabcdef" ).afterFirst( char_t( 'd' ) ) ) );
	EON_TEST( substring, afterFirst, char_t_ASCII_last,
		EON_EQ( "abcdef", substring( "abcdefabcdef" ).afterFirst( char_t( 'f' ) ) ) );
	EON_TEST( substring, afterFirst, char_t_UTF8_at_start,
		EON_EQ( u8"€Œ™©µØ€Œ™©µ", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).afterFirst( char_t( 216 ) ) ) );
	EON_TEST( substring, afterFirst, char_t_UTF8_at_middle,
		EON_EQ( u8"©µØ€Œ™©µ", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).afterFirst( char_t( 8482 ) ) ) );
	EON_TEST( substring, afterFirst, char_t_UTF8_at_end,
		EON_EQ( u8"Ø€Œ™©µ", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).afterFirst( char_t( 181 ) ) ) );


	EON_TEST( substring, afterLast, substr_ASCII_first,
		EON_EQ( "cdef", substring( "abcdefabcdef" ).afterLast( substring( "ab" ) ) ) );
	EON_TEST( substring, afterLast, substr_ASCII_middle,
		EON_EQ( "ef", substring( "abcdefabcdef" ).afterLast( substring( "cd" ) ) ) );
	EON_TEST( substring, afterLast, substr_ASCII_end,
		EON_EQ( "", substring( "abcdefabcdef" ).afterLast( substring( "ef" ) ) ) );
	EON_TEST( substring, afterLast, substr_UTF8_first,
		EON_EQ( u8"Œ™©µ", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).afterLast( substring( u8"Ø€" ) ) ) );
	EON_TEST( substring, afterLast, substr_UTF8_middle,
		EON_EQ( u8"©µ", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).afterLast( substring( u8"Œ™" ) ) ) );
	EON_TEST( substring, afterLast, substr_UTF8_end,
		EON_EQ( u8"", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).afterLast( substring( u8"©µ" ) ) ) );

	EON_TEST( substring, afterLast, char_t_ASCII_first,
		EON_EQ( "bcdef", substring( "abcdefabcdef" ).afterLast( char_t( 'a' ) ) ) );
	EON_TEST( substring, afterLast, char_t_ASCII_middle,
		EON_EQ( "def", substring( "abcdefabcdef" ).afterLast( char_t( 'c' ) ) ) );
	EON_TEST( substring, afterLast, char_t_ASCII_last,
		EON_EQ( "", substring( "abcdefabcdef" ).afterLast( char_t( 'f' ) ) ) );
	EON_TEST( substring, afterLast, char_t_UTF8_at_start,
		EON_EQ( u8"€Œ™©µ", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).afterLast( char_t( 216 ) ) ) );
	EON_TEST( substring, afterLast, char_t_UTF8_at_middle,
		EON_EQ( u8"©µ", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).afterLast( char_t( 8482 ) ) ) );
	EON_TEST( substring, afterLast, char_t_UTF8_at_end,
		EON_EQ( u8"", substring( u8"Ø€Œ™©µØ€Œ™©µ" ).afterLast( char_t( 181 ) ) ) );
}
