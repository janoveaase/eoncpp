#include "Substring.h"
#include <eoninlinetest/InlineTest.h>
#include <cctype>


namespace eon
{
	EON_TEST_3STEP( substring, sameSource, true,
		const char* source = "abcdef",
		substring obj( source ),
		EON_TRUE( obj.sameSource( source ) ) );
	EON_TEST_3STEP( substring, sameSource, false,
		const char* source = "abcdef",
		substring obj( "bcdefa" ),
		EON_FALSE( obj.sameSource( source ) ) );

	EON_TEST_3STEP( substring, assertSameSource, true,
		const char* source = "abcdef",
		substring obj( source ),
		EON_NO_X( obj.assertSameSource( source ) ) );
	EON_TEST_3STEP( substring, assertSameSource, false,
		const char* source = "abcdef",
		substring obj( "bcdefa" ),
		EON_RAISE( obj.assertSameSource( source ), WrongSource ) );

	EON_TEST_3STEP( substring, sameArea, true,
		const char* source = "abcdef",
		substring obj( source ),
		EON_TRUE( obj.sameArea( substring( source ) ) ) );
	EON_TEST_3STEP( substring, sameArea, false,
		const char* source = "abcdef",
		substring obj( "bcdefa" ),
		EON_FALSE( obj.sameArea( substring( source + 1 ) ) ) );

	EON_TEST( substring, isLowToHigh, true_default,
		EON_TRUE( substring( "abcdef" ).isLowToHigh() ) );
	EON_TEST( substring, isLowToHigh, true_flipped,
		EON_TRUE( substring( "abcdef" ).highToLow().lowToHigh().isLowToHigh() ) );
	EON_TEST( substring, isLowToHigh, false,
		EON_FALSE( substring( "abcdef" ).highToLow().isLowToHigh() ) );

	EON_TEST( substring, isHighToLow, false_default,
		EON_FALSE( substring( "abcdef" ).isHighToLow() ) );
	EON_TEST( substring, isHighToLow, true_flipped,
		EON_TRUE( substring( "abcdef" ).highToLow().isHighToLow() ) );

	substring substring::lowToHigh() const noexcept
	{
		if( !isHighToLow() )
			return *this;
		if( End.atREnd() )
			return substring( string_iterator( End ).resetToFirst(), Beg + 1 );
		else
			return substring( End + 1, Beg + 1 );
	}
	EON_NO_TEST( substring, lowToHigh );

	substring substring::highToLow() const noexcept
	{
		if( !isLowToHigh() )
			return *this;
		return substring( End - 1, Beg - 1 );
	}
	EON_NO_TEST( substring, lowToHigh );

	EON_NO_TEST( substring, validUTF8 );

	EON_TEST( substring, operator_bool, true,
		EON_TRUE( substring( "abcdef" ) ) );
	EON_TEST( substring, operator_bool, false_empty,
		EON_FALSE( substring( "" ) ) );
	EON_TEST( substring, operator_bool, false_no_source,
		EON_FALSE( substring() ) );

	EON_TEST( substring, empty, true_no_source,
		EON_TRUE( substring().empty() ) );
	EON_TEST( substring, empty, true_source,
		EON_TRUE( substring( "" ).empty() ) );
	EON_TEST( substring, empty, false,
		EON_FALSE( substring( "abcdef" ).empty() ) );

	EON_TEST( substring, hasSource, true_empty,
		EON_TRUE( substring( "" ).hasSource() ) );
	EON_TEST( substring, hasSource, true_source,
		EON_TRUE( substring( "abcdef" ).hasSource() ) );
	EON_TEST( substring, hasSource, false,
		EON_FALSE( substring().hasSource() ) );

	EON_TEST( substring, numChars, no_source,
		EON_EQ( 0, substring().numChars() ) );
	EON_TEST( substring, numChars, empty,
		EON_EQ( 0, substring( "" ).numChars() ) );
	EON_TEST( substring, numChars, ASCII,
		EON_EQ( 6, substring( "abcdef" ).numChars() ) );
	EON_TEST( substring, numChars, UTF8,
		EON_EQ( 6, substring( u8"Ø€Œ™©µ" ).numChars() ) );

	index_t substring::numBytes() const noexcept
	{
		if( isHighToLow() )
			return lowToHigh().numBytes();
		auto beg_bytes = Beg.atREnd() ? 0 : Beg.atEnd() ? Beg.numSourceBytes() : Beg.numByte();
		auto end_bytes = End.atREnd() ? 0 : End.atEnd() ? End.numSourceBytes() : End.numByte();
		return end_bytes - beg_bytes;
	}
	EON_TEST( substring, numBytes, no_source,
		EON_EQ( 0, substring().numBytes() ) );
	EON_TEST( substring, numBytes, empty,
		EON_EQ( 0, substring( "" ).numBytes() ) );
	EON_TEST( substring, numBytes, ASCII,
		EON_EQ( 6, substring( "abcdef" ).numBytes() ) );
	EON_TEST( substring, numBytes, ASCII_HighToLow,
		EON_EQ( 6, substring( "abcdef" ).highToLow().numBytes() ) );
	EON_TEST( substring, numBytes, UTF8,
		EON_EQ( 14, substring( u8"Ø€Œ™©µ" ).numBytes() ) );
	EON_TEST( substring, numBytes, UTF8_HighToLow,
		EON_EQ( 14, substring( u8"Ø€Œ™©µ" ).highToLow().numBytes() ) );

	EON_TEST( substring, stdstr, empty,
		EON_EQ( "", substring( "" ).stdstr() ) );
	EON_TEST( substring, stdstr, all,
		EON_EQ( "abcdef", substring( "abcdef" ).stdstr() ) );
	EON_TEST_2STEP( substring, stdstr, part,
		const char* source = "abcdef",
		EON_EQ( "bcde", substring( string_iterator( source ) + 1, string_iterator( source ) + 5 ).stdstr() ) );
	EON_TEST_2STEP( substring, stdstr, part_HighToLow,
		const char* source = "abcdef",
		EON_EQ( "edcb", substring( string_iterator( source ) + 1, string_iterator( source ) + 5 ).highToLow().stdstr() ) );

	byte_t substring::byte( index_t pos ) const noexcept
	{
		if( empty() || pos >= numBytes() )
			return 0;
		if( isLowToHigh() )
			return *( Beg.Pos + pos );
		else
			return *( Beg.Pos - pos );
	}
	EON_TEST( substring, byte, no_source,
		EON_EQ( 0, static_cast<int>( substring().byte( 0 ) ) ) );
	EON_TEST( substring, byte, empty,
		EON_EQ( 0, static_cast<int>( substring( "" ).byte( 0 ) ) ) );
	EON_TEST( substring, byte, out_of_bounds,
		EON_EQ( 0, static_cast<int>( substring( "abcdef" ).byte( 7 ) ) ) );
	EON_TEST( substring, byte, ASCII_full,
		EON_EQ( 99, static_cast<int>( substring( "abcdef" ).byte( 2 ) ) ) );
	EON_TEST_2STEP( substring, byte, ASCII_sub,
		const char* source = "abcdefg",
		EON_EQ( 100, static_cast<int>( substring(
			string_iterator( source ) + 1, string_iterator( source ) + 5 ).byte( 2 ) ) ) );
	EON_TEST( substring, byte, ASCII_full_HighToLow,
		EON_EQ( 100, static_cast<int>( substring( "abcdef" ).highToLow().byte( 2 ) ) ) );
	EON_TEST_2STEP( substring, byte, ASCII_sub_HighToLow,
		const char* source = "abcdefg",
		EON_EQ( 101, static_cast<int>( substring(
			string_iterator( source ) + 6, string_iterator( source ) + 1 ).byte( 2 ) ) ) );

	EON_TEST( substring, enclosed, no_source,
		EON_FALSE( substring().enclosed( '"' ) ) );
	EON_TEST( substring, enclosed, empty,
		EON_FALSE( substring( "" ).enclosed( '"' ) ) );
	EON_TEST( substring, enclosed, false,
		EON_FALSE( substring( "abcdef" ).enclosed( '"' ) ) );
	EON_TEST( substring, enclosed, half,
		EON_FALSE( substring( "\"abcdef" ).enclosed( '"' ) ) );
	EON_TEST( substring, enclosed, same_char,
		EON_TRUE( substring( "\"abcdef\"" ).enclosed( '"' ) ) );
	EON_TEST( substring, enclosed, diff_char,
		EON_TRUE( substring( "(abcdef)" ).enclosed( '(', ')' ) ) );

	bool substring::blank() const noexcept
	{
		auto& chars = Characters::get();
		for( auto cp : *this )
		{
			if( !chars.isSeparatorSpace( cp ) && !chars.isOtherControl( cp ) )
				return false;
		}
		return true;
	}
	EON_TEST( substring, blank, no_source,
		EON_TRUE( substring().blank() ) );
	EON_TEST( substring, blank, empty,
		EON_TRUE( substring( "" ).blank() ) );
	EON_TEST( substring, blank, ASCII_true,
		EON_TRUE( substring( " \t" ).blank() ) );
	EON_TEST( substring, blank, ASCII_false,
		EON_FALSE( substring( " _" ).blank() ) );
	EON_TEST( substring, blank, UTF8_true,
		EON_TRUE( substring( u8" \u00A0\u2000\u3000" ).blank() ) );
	EON_TEST( substring, blank, UTF8_false,
		EON_FALSE( substring( u8" \u00A0\u2000_\u3000" ).blank() ) );

	string_iterator substring::iterator( index_t num_char ) const noexcept
	{
		auto num_chars = numChars();
		if( num_char > num_chars )
			return end().getEnd();

		if( num_chars >= 10 )
		{
			// Only optimize counting if the substring is of a certain size.
			// NOTE: Counting backward is slightly more costly so skew the middle-point.
			auto mid_point = ( num_chars / 3 ) * 2;
			if( num_char >= mid_point )
			{
				if( isLowToHigh() )
					return end() - ( num_chars - num_char );
				else
					lowToHigh().iterator( num_chars - num_char );
			}
		}
		return isLowToHigh() ? begin() + num_char : begin() - num_char;
	}
	EON_TEST( substring, iterator, no_source,
		EON_TRUE( substring().iterator( 0 ).isVoid() ) );
	EON_TEST( substring, iterator, empty,
		EON_FALSE( substring( "" ).iterator( 0 ).isVoid() ) );
	EON_TEST( substring, iterator, lowToHigh,
		EON_EQ( char_t( 'c' ), *substring( "abcdef" ).iterator( 2 ) ) );
	EON_TEST( substring, iterator, highToLow,
		EON_EQ( char_t( 'd' ), *substring( "abcdef" ).highToLow().iterator( 2 ) ) );
	EON_TEST( substring, iterator, lowToHigh_long,
		EON_EQ( char_t( 'u' ), *substring( "abcdefghijklmnopqrstuvwxyz" ).iterator( 20 ) ) );
	EON_TEST( substring, iterator, highToLow_long,
		EON_EQ( char_t( 'f' ), *substring( "abcdefghijklmnopqrstuvwxyz" ).highToLow().iterator( 20 ) ) );

	index_t substring::indentationLevel( char_t indentation_char ) const noexcept
	{
		index_t level = 0;
		for( auto cp : *this )
		{
			if( cp != indentation_char )
				break;
			++level;
		}
		return level;
	}
	EON_TEST( substring, indentationLevel, no_source,
		EON_EQ( 0, substring().indentationLevel( ' ' ) ) );
	EON_TEST( substring, indentationLevel, empty,
		EON_EQ( 0, substring( "" ).indentationLevel( ' ' ) ) );
	EON_TEST( substring, indentationLevel, ASCII_all_indents,
		EON_EQ( 2, substring( "  " ).indentationLevel( ' ' ) ) );
	EON_TEST( substring, indentationLevel, ASCII,
		EON_EQ( 2, substring( "  abc" ).indentationLevel( ' ' ) ) );
	EON_TEST( substring, indentationLevel, UTF8_all_indents,
		EON_EQ( 2, substring( u8"™™" ).indentationLevel( char_t( 8482 ) ) ) );
	EON_TEST( substring, indentationLevel, UTF8,
		EON_EQ( 2, substring( u8"™™abc" ).indentationLevel( char_t( 8482 ) ) ) );

	EON_TEST( substring, characters, no_source,
		EON_EQ( 0, substring().characters<std::vector<char_t>>().size() ) );
	EON_TEST( substring, characters, empty,
		EON_EQ( 0, substring( "" ).characters<std::vector<char_t>>().size() ) );
	EON_TEST( substring, characters, ASCII,
		EON_EQ( 6, substring( "abcdef" ).characters<std::vector<char_t>>().size() ) );
	EON_TEST( substring, characters, UTF8,
		EON_EQ( 6, substring( u8"Ø€Œ™©µ" ).characters<std::vector<char_t>>().size() ) );

	substring substring::trim() const
	{
		if( empty() )
			return *this;
		else if( isHighToLow() )
			return lowToHigh().trim();
		auto beg = findFirstNotOf( charcat::separator_space | charcat::other_control );
		auto end = highToLow().findLastNotOf( charcat::separator_space | charcat::other_control );
		return substring( beg ? beg : End, end ? end + 1 : End );
	}
	EON_TEST_2STEP( substring, trim, ASCII,
		const char* source = "  abcdefg  ",
		EON_EQ( "abcdefg", substring( source ).trim().stdstr() ) );
	EON_TEST_2STEP( substring, trim, UTF8,
		const char* source = u8"\u00A0\u2008Ø€Œ™©µ\u200A\u3000",
		EON_EQ( u8"Ø€Œ™©µ", substring( source ).trim().stdstr() ) );
	EON_TEST_2STEP( substring, trim, all,
		const char* source = " ",
		EON_EQ( "", substring( source ).trim().stdstr() ) );

	substring substring::trimLeading() const
	{
		if( empty() )
			return *this;
		else if( isHighToLow() )
			return lowToHigh().trimLeading();
		auto beg = findFirstNotOf( charcat::separator_space | charcat::other_control );
		return substring( beg ? beg : End, End );
	}
	EON_TEST_2STEP( substring, trimLeading, ASCII,
		const char* source = "  abcdefg  ",
		EON_EQ( "abcdefg  ", substring( source ).trimLeading().stdstr() ) );
	EON_TEST_2STEP( substring, trimLeading, UTF8,
		const char* source = u8"\u00A0\u2008Ø€Œ™©µ\u200A\u3000",
		EON_EQ( u8"Ø€Œ™©µ\u200A\u3000", substring( source ).trimLeading().stdstr() ) );
	EON_TEST_2STEP( substring, trimLeading, all,
		const char* source = " ",
		EON_EQ( "", substring( source ).trimLeading().stdstr() ) );

	substring substring::trimTrailing() const
	{
		if( empty() )
			return *this;
		else if( isHighToLow() )
			return lowToHigh().trimTrailing();
		auto end = highToLow().findLastNotOf( charcat::separator_space | charcat::other_control );
		return substring( Beg, end ? end + 1 : End );
	}
	EON_TEST_2STEP( substring, trimTrailing, ASCII,
		const char* source = "  abcdefg  ",
		EON_EQ( "  abcdefg", substring( source ).trimTrailing().stdstr() ) );
	EON_TEST_2STEP( substring, trimTrailing, UTF8,
		const char* source = u8"\u00A0\u2008Ø€Œ™©µ\u200A\u3000",
		EON_EQ( u8"\u00A0\u2008Ø€Œ™©µ", substring( source ).trimTrailing().stdstr() ) );
}
