#include "Substring.h"
#include <eoninlinetest/InlineTest.h>
#include <cctype>


namespace eon
{
	bool substring::numeralsOnly() const noexcept
	{
		bool is_numeral = false;
		for( auto chr : *this )
		{
			if( !eon::isNumeral( chr ) )
				return false;
			else
				is_numeral = true;
		}
		return is_numeral;
	}
	EON_TEST( substring, numeralsOnly, empty,
		EON_FALSE( substring().numeralsOnly() ) );
	EON_TEST( substring, numeralsOnly, letters,
		EON_FALSE( substring( "abc" ).numeralsOnly() ) );
	EON_TEST( substring, numeralsOnly, partial,
		EON_FALSE( substring( "12abc" ).numeralsOnly() ) );
	EON_TEST( substring, numeralsOnly, ASCII,
		EON_TRUE( substring( "1234" ).numeralsOnly() ) );
	EON_TEST( substring, numeralsOnly, ASCII_plus,
		EON_FALSE( substring( "+1234" ).numeralsOnly() ) );
	EON_TEST( substring, numeralsOnly, ASCII_minus,
		EON_FALSE( substring( "-1234" ).numeralsOnly() ) );
	EON_TEST( substring, numeralsOnly, UTF8,
		EON_TRUE( substring( u8"١۲߃४" ).numeralsOnly() ) );
	EON_TEST( substring, numeralsOnly, UTF8_minus,
		EON_FALSE( substring( u8"-١۲߃४" ).numeralsOnly() ) );
	EON_TEST( substring, numeralsOnly, float,
		EON_FALSE( substring( "12.34" ).numeralsOnly() ) );
	EON_TEST( substring, numeralsOnly, float_minus,
		EON_FALSE( substring( "-12.34" ).numeralsOnly() ) );

	bool substring::isUInt() const noexcept
	{
		bool is_uint = false;
		for( auto chr : *this )
		{
			if( !eon::isDigit( chr ) )
				return false;
			else
				is_uint = true;
		}
		return is_uint;
	}
	EON_TEST( substring, isUInt, empty,
		EON_FALSE( substring().isUInt() ) );
	EON_TEST( substring, isUInt, letters,
		EON_FALSE( substring( "abc" ).isUInt() ) );
	EON_TEST( substring, isUInt, partial,
		EON_FALSE( substring( "12abc" ).isUInt() ) );
	EON_TEST( substring, isUInt, ASCII,
		EON_TRUE( substring( "1234" ).isUInt() ) );
	EON_TEST( substring, isUInt, ASCII_plus,
		EON_FALSE( substring( "+1234" ).isUInt() ) );
	EON_TEST( substring, isUInt, ASCII_minus,
		EON_FALSE( substring( "-1234" ).isUInt() ) );
	EON_TEST( substring, isUInt, UTF8,
		EON_FALSE( substring( u8"١۲߃४" ).isUInt() ) );
	EON_TEST( substring, isUInt, UTF8_minus,
		EON_FALSE( substring( u8"-١۲߃४" ).isUInt() ) );
	EON_TEST( substring, isUInt, float,
		EON_FALSE( substring( "12.34" ).isUInt() ) );
	EON_TEST( substring, isUInt, signed_float,
		EON_FALSE( substring( "-12.34" ).isUInt() ) );
	EON_TEST( substring, isUInt, UTF8_float_minus,
		EON_FALSE( substring( u8"-١۲.߃४" ).isUInt() ) );

	EON_TEST( substring, isSignedInteger, empty,
		EON_FALSE( substring().isSignedInteger() ) );
	EON_TEST( substring, isSignedInteger, letters,
		EON_FALSE( substring( "abc" ).isSignedInteger() ) );
	EON_TEST( substring, isSignedInteger, partial,
		EON_FALSE( substring( "12abc" ).isSignedInteger() ) );
	EON_TEST( substring, isSignedInteger, ASCII,
		EON_FALSE( substring( "1234" ).isSignedInteger() ) );
	EON_TEST( substring, isSignedInteger, ASCII_plus,
		EON_TRUE( substring( "+1234" ).isSignedInteger() ) );
	EON_TEST( substring, isSignedInteger, ASCII_minus,
		EON_TRUE( substring( "-1234" ).isSignedInteger() ) );
	EON_TEST( substring, isSignedInteger, UTF8,
		EON_FALSE( substring( u8"١۲߃४" ).isSignedInteger() ) );
	EON_TEST( substring, isSignedInteger, UTF8_minus,
		EON_TRUE( substring( u8"-١۲߃४" ).isSignedInteger() ) );
	EON_TEST( substring, isSignedInteger, float,
		EON_FALSE( substring( "12.34" ).isSignedInteger() ) );
	EON_TEST( substring, isSignedInteger, signed_float,
		EON_FALSE( substring( "-12.34" ).isSignedInteger() ) );
	EON_TEST( substring, isSignedInteger, UTF8_kloat_minus,
		EON_FALSE( substring( u8"-١۲.߃४" ).isSignedInteger() ) );

	EON_TEST( substring, isInt, empty,
		EON_FALSE( substring().isInt() ) );
	EON_TEST( substring, isInt, letters,
		EON_FALSE( substring( "abc" ).isInt() ) );
	EON_TEST( substring, isInt, partial,
		EON_FALSE( substring( "12abc" ).isInt() ) );
	EON_TEST( substring, isInt, ASCII,
		EON_FALSE( substring( "1234" ).isInt() ) );
	EON_TEST( substring, isInt, ASCII_plus,
		EON_TRUE( substring( "+1234" ).isInt() ) );
	EON_TEST( substring, isInt, ASCII_minus,
		EON_TRUE( substring( "-1234" ).isInt() ) );
	EON_TEST( substring, isInt, UTF8,
		EON_FALSE( substring( u8"١۲߃४" ).isInt() ) );
	EON_TEST( substring, isInt, UTF8_minus,
		EON_FALSE( substring( u8"-١۲߃४" ).isInt() ) );
	EON_TEST( substring, isInt, float,
		EON_FALSE( substring( "12.34" ).isInt() ) );
	EON_TEST( substring, isInt, signed_float,
		EON_FALSE( substring( "-12.34" ).isInt() ) );
	EON_TEST( substring, isInt, UTF8_float_minus,
		EON_FALSE( substring( u8"-١۲.߃४" ).isInt() ) );

	bool substring::isFloatingPoint( const locale* custom_locale ) const noexcept
	{
		if( empty() )
			return false;
		const locale& loc = custom_locale != nullptr ? *custom_locale : locale::get();
		string_iterator i = begin();
		if( isPlus( *i ) || isMinus( *i ) )
			++i;
		int sep = 0;
		for( ; i != end() && sep < 2; ++i )
		{
			if( *i == loc.decimalSep() )
				++sep;
			else if( !isNumeral( *i ) )
				return false;
		}
		return sep > 0 && sep < 2;
	}
	EON_TEST( substring, isFloatingPoint, empty,
		EON_FALSE( substring().isFloatingPoint() ) );
	EON_TEST( substring, isFloatingPoint, letters,
		EON_FALSE( substring( "abc" ).isFloatingPoint() ) );
	EON_TEST( substring, isFloatingPoint, partial,
		EON_FALSE( substring( "12abc" ).isFloatingPoint() ) );
	EON_TEST( substring, isFloatingPoint, ASCII,
		EON_FALSE( substring( "1234" ).isFloatingPoint() ) );
	EON_TEST( substring, isFloatingPoint, ASCII_plus,
		EON_FALSE( substring( "+1234" ).isFloatingPoint() ) );
	EON_TEST( substring, isFloatingPoint, ASCII_minus,
		EON_FALSE( substring( "-1234" ).isFloatingPoint() ) );
	EON_TEST( substring, isFloatingPoint, UTF8,
		EON_FALSE( substring( u8"١۲߃४" ).isFloatingPoint() ) );
	EON_TEST( substring, isFloatingPoint, UTF8_minus,
		EON_FALSE( substring( u8"-١۲߃४" ).isFloatingPoint() ) );
	EON_TEST( substring, isFloatingPoint, float,
		EON_TRUE( substring( "12.34" ).isFloatingPoint() ) );
	EON_TEST( substring, isFloatingPoint, signed_float,
		EON_TRUE( substring( "-12.34" ).isFloatingPoint() ) );
	EON_TEST( substring, isFloatingPoint, UTF8_float_minus,
		EON_TRUE( substring( u8"-١۲.߃४" ).isFloatingPoint() ) );

	bool substring::isFloat( const locale* custom_locale ) const noexcept
	{
		if( empty() )
			return false;
		const locale& loc = custom_locale != nullptr ? *custom_locale : locale::get();
		string_iterator i = begin();
		if( *i == '+' || *i == '-' )
			++i;
		int sep = 0;
		for( ; i != end() && sep < 2; ++i )
		{
			if( *i == loc.decimalSep() )
				++sep;
			else if( !isDigit( *i ) )
				return false;
		}
		return sep > 0 && sep < 2;
	}
	EON_TEST( substring, isFloat, empty,
		EON_FALSE( substring().isFloat() ) );
	EON_TEST( substring, isFloat, letters,
		EON_FALSE( substring( "abc" ).isFloat() ) );
	EON_TEST( substring, isFloat, partial,
		EON_FALSE( substring( "12abc" ).isFloat() ) );
	EON_TEST( substring, isFloat, ASCII,
		EON_FALSE( substring( "1234" ).isFloat() ) );
	EON_TEST( substring, isFloat, ASCII_plus,
		EON_FALSE( substring( "+1234" ).isFloat() ) );
	EON_TEST( substring, isFloat, ASCII_minus,
		EON_FALSE( substring( "-1234" ).isFloat() ) );
	EON_TEST( substring, isFloat, UTF8,
		EON_FALSE( substring( u8"١۲߃४" ).isFloat() ) );
	EON_TEST( substring, isFloat, UTF8_minus,
		EON_FALSE( substring( u8"-١۲߃४" ).isFloat() ) );
	EON_TEST( substring, isFloat, float,
		EON_TRUE( substring( "12.34" ).isFloat() ) );
	EON_TEST( substring, isFloat, signed_float,
		EON_TRUE( substring( "-12.34" ).isFloat() ) );
	EON_TEST( substring, isFloat, UTF8_float_minus,
		EON_FALSE( substring( u8"-١۲.߃४" ).isFloat() ) );


	template<typename T>
	struct ToNum
	{
		inline ToNum( string_iterator i ) noexcept
		{
			I = i;
			if( I )
			{
				Sign = isMinus( *I ) ? -1 : isPlus( *i ) ? 1 : 0;
				if( Sign == 0 )
					Sign = 1;
				else
					++I;
			}
		}
		inline void grow( T num ) noexcept { Value *= 10; Value += num; }

		T Value{ 0 }, Sign{ 0 };
		string_iterator I;
	};

	long_t substring::toLongT() const noexcept
	{
		ToNum<long_t> to_num( begin() );
		if( empty() )
			return to_num.Value;
		for( ; to_num.I != end(); ++to_num.I )
		{
			int num = numeralValue( *to_num.I );
			if( num >= 0 )
				to_num.grow( num );
			else
				return 0;
		}
		return to_num.Value * to_num.Sign;
	}
	EON_TEST( substring, toLongT, empty,
		EON_EQ( 0, substring( "" ).toLongT() ) );
	EON_TEST( substring, toLongT, ASCII,
		EON_EQ( 1234, substring( "1234" ).toLongT() ) );
	EON_TEST( substring, toLongT, UTF8,
		EON_EQ( 1234, substring( u8"١۲߃४" ).toLongT() ) );

	EON_TEST( substring, toIntT, empty,
		EON_EQ( 0, substring( "" ).toIntT() ) );
	EON_TEST( substring, toIntT, ASCII,
		EON_EQ( 1234, substring( "1234" ).toIntT() ) );
	EON_TEST( substring, toIntT, UTF8,
		EON_EQ( 1234, substring( u8"١۲߃४" ).toIntT() ) );

	EON_TEST( substring, toShortT, empty,
		EON_EQ( 0, substring( "" ).toShortT() ) );
	EON_TEST( substring, toShortT, ASCII,
		EON_EQ( 1234, substring( "1234" ).toShortT() ) );
	EON_TEST( substring, toShortT, UTF8,
		EON_EQ( 1234, substring( u8"١۲߃४" ).toShortT() ) );

	high_t substring::toHighT( const locale* custom_locale ) const noexcept
	{
		ToNum<high_t> to_num( begin() );
		if( empty() )
			return to_num.Value;
		const locale& loc = custom_locale != nullptr ? *custom_locale : locale::get();
		bool before_sep{ true };
		high_t decimals{ 0.0 }, decimal_power{ 1.0 };
		for( ; to_num.I != end(); ++to_num.I )
		{
			if( before_sep )
			{
				if( *to_num.I == loc.decimalSep() )
					before_sep = false;
				else
					to_num.grow( static_cast<high_t>( numeralValue( *to_num.I ) ) );
			}
			else
			{
				decimals *= 10.0;
				decimal_power *= 10.0;
				decimals += static_cast<high_t>( numeralValue( *to_num.I ) );
			}
		}
		return to_num.Sign * ( to_num.Value + ( decimals / decimal_power ) );
	}
	EON_TEST( substring, toHighT, empty,
		EON_EQ( 0, substring( "" ).toHighT() ) );
	EON_TEST( substring, toHighT, ASCII,
		EON_RANGE( 12.339, substring( "12.34" ).toHighT(), 12.341 ) );
	EON_TEST( substring, toHighT, UTF8,
		EON_RANGE( 12.339, substring( u8"١۲.߃४" ).toHighT(), 12.341 ) );

	EON_TEST( substring, toFltT, empty,
		EON_EQ( 0, substring( "" ).toFltT() ) );
	EON_TEST( substring, toFltT, ASCII,
		EON_EQ( 12.34, substring( "12.34" ).toFltT() ) );
	EON_TEST( substring, toFltT, UTF8,
		EON_EQ( 12.34, substring( u8"١۲.߃४" ).toFltT() ) );

	EON_TEST( substring, toLowT, empty,
		EON_EQ( 0, substring( "" ).toLowT() ) );
	EON_TEST( substring, toLowT, ASCII,
		EON_EQ( low_t( 12.34 ), substring( "12.34" ).toLowT() ) );
	EON_TEST( substring, toLowT, UTF8,
		EON_EQ( low_t( 12.34 ), substring( u8"١۲.߃४" ).toLowT() ) );

	index_t substring::toIndex() const noexcept
	{
		index_t value = 0;
		for( auto chr : *this )
		{
			value *= 10;
			value += numeralValue( chr );
		}
		return value;
	}
	EON_TEST( substring, toIndex, empty,
		EON_EQ( 0, substring( "" ).toIndex() ) );
	EON_TEST( substring, toIndex, ASCII,
		EON_EQ( 1234, substring( "1234" ).toIndex() ) );
	EON_TEST( substring, toIndex, UTF8,
		EON_EQ( 1234, substring( u8"١۲߃४" ).toIndex() ) );

	EON_TEST( substring, toInt32, empty,
		EON_EQ( 0, substring( "" ).toInt32() ) );
	EON_TEST( substring, toInt32, ASCII,
		EON_EQ( 1234, substring( "1234" ).toInt32() ) );
	EON_TEST( substring, toInt32, UTF8,
		EON_EQ( 1234, substring( u8"١۲߃४" ).toInt32() ) );

	EON_TEST( substring, toInt64, empty,
		EON_EQ( 0, substring( "" ).toInt64() ) );
	EON_TEST( substring, toInt64, ASCII,
		EON_EQ( 1234, substring( "1234" ).toInt64() ) );
	EON_TEST( substring, toInt64, UTF8,
		EON_EQ( 1234, substring( u8"١۲߃४" ).toInt64() ) );

	EON_TEST( substring, toDouble, empty,
		EON_EQ( 0, substring( "" ).toDouble() ) );
	EON_TEST( substring, toDouble, ASCII,
		EON_EQ( 12.34, substring( "12.34" ).toDouble() ) );
	EON_TEST( substring, toDouble, UTF8,
		EON_EQ( 12.34, substring( u8"١۲.߃४" ).toDouble() ) );

	EON_TEST( substring, toLongDouble, empty,
		EON_EQ( 0, substring( "" ).toLongDouble() ) );
	EON_TEST( substring, toLongDouble, ASCII,
		EON_RANGE( 12.339, substring( "12.34" ).toLongDouble(), 12.341 ) );
	EON_TEST( substring, toLongDouble, UTF8,
		EON_RANGE( 12.339, substring( u8"١۲.߃४" ).toLongDouble(), 12.341 ) );

	EON_TEST( substring, toUInt32, empty,
		EON_EQ( 0, substring( "" ).toUInt32() ) );
	EON_TEST( substring, toUInt32, ASCII,
		EON_EQ( 1234, substring( "1234" ).toUInt32() ) );
	EON_TEST( substring, toUInt32, UTF8,
		EON_EQ( 1234, substring( u8"١۲߃४" ).toUInt32() ) );

	EON_TEST( substring, toUInt64, empty,
		EON_EQ( 0, substring( "" ).toUInt64() ) );
	EON_TEST( substring, toUInt64, ASCII,
		EON_EQ( 1234, substring( "1234" ).toUInt64() ) );
	EON_TEST( substring, toUInt64, UTF8,
		EON_EQ( 1234, substring( u8"١۲߃४" ).toUInt64() ) );

	EON_TEST( substring, toSize, empty,
		EON_EQ( 0, substring( "" ).toSize() ) );
	EON_TEST( substring, toSize, ASCII,
		EON_EQ( 1234, substring( "1234" ).toSize() ) );
	EON_TEST( substring, toSize, UTF8,
		EON_EQ( 1234, substring( u8"١۲߃४" ).toSize() ) );


	struct NumTrimmer
	{
		inline NumTrimmer( string_iterator begin, string_iterator end, char_t decimal_sep ) noexcept
		{
			Begin = begin;
			End = end;
			FirstNonZero = begin;
			while( FirstNonZero != End && *FirstNonZero == ZeroChr )
				++FirstNonZero;
			DecimalSep = decimal_sep;
		}
		inline bool allZeros() const noexcept { return FirstNonZero == End; }
		inline bool firstNoneZeroIsPoint() const noexcept { return *FirstNonZero == DecimalSep && FirstNonZero != Begin; }
		inline void findDecimalSep() noexcept
		{
			Separator = FirstNonZero;
			while( Separator != End && *Separator != DecimalSep )
				++Separator;
		}

		inline bool isInteger() const noexcept { return Separator == End; }
		inline void findLastNonZero() noexcept
		{
			LastNonZero = End - 1;
			while( LastNonZero != Separator && *LastNonZero == ZeroChr )
				--LastNonZero;
		}
		inline bool hasTrailingFractionalZeros() const noexcept { return LastNonZero > Separator; }

		inline substring integerPartOnly()
		{
			if( FirstNonZero == LastNonZero )
			{
				if( FirstNonZero > Begin )
					return substring( Begin, Begin + 1 );
				else if( LastNonZero < End )
					return substring( LastNonZero + 1, LastNonZero + 2 );
				else
					return substring( FirstNonZero, LastNonZero );
			}
			return substring( FirstNonZero, Separator );
		}
		inline substring integerPartForFloat()
		{
			if( LastNonZero == Separator )
			{
				// If one or more trailing zeros, include one - otherwise just include the separator.
				if( LastNonZero + 1 != End )
					return substring( FirstNonZero, LastNonZero + 2 );
				else
					return substring( FirstNonZero, LastNonZero + 1 );
			}
			return substring( FirstNonZero, LastNonZero + 1 );
		}

		string_iterator Begin, End, FirstNonZero, LastNonZero, Separator;
		char_t DecimalSep{ '.' };
	};

	substring substring::trimNumber( const locale* custom_locale ) const
	{
		const locale& loc = custom_locale != nullptr ? *custom_locale : locale::get();
		NumTrimmer trimmer( begin(), end(), loc.decimalSep() );
		if( trimmer.allZeros() )					// Return first zero!
			return substring( trimmer.FirstNonZero - 1, end() );
		trimmer.findDecimalSep();
		if( trimmer.isInteger() )
			return substring( trimmer.FirstNonZero, end() );
		trimmer.findLastNonZero();
		if( !trimmer.hasTrailingFractionalZeros() )
			return trimmer.integerPartOnly();
		return substring( trimmer.FirstNonZero, trimmer.LastNonZero + 1 );
	}
	EON_TEST( substring, trimNumber, empty,
		EON_EQ( "", substring( "" ).trimNumber() ) );
	EON_TEST( substring, trimNumber, nothing_to_trim,
		EON_EQ( "123.45", substring( "123.45" ).trimNumber() ) );
	EON_TEST( substring, trimNumber, leading_int_zeros,
		EON_EQ( "123.45", substring( "00123.45" ).trimNumber() ) );
	EON_TEST( substring, trimNumber, all_int_zeros,
		EON_EQ( "0", substring( "0000" ).trimNumber() ) );
	EON_TEST( substring, trimNumber, trailing_fractional_zeros,
		EON_EQ( "123.45", substring( "123.4500" ).trimNumber() ) );
	EON_TEST( substring, trimNumber, all_fractional_zeros,
		EON_EQ( "123", substring( "123.0000" ).trimNumber() ) );
	EON_TEST( substring, trimNumber, all_int_and_fractional_zeros,
		EON_EQ( "0", substring( "000.00" ).trimNumber() ) );

	substring substring::trimFloat( const locale* custom_locale ) const
	{
		const locale& loc = custom_locale != nullptr ? *custom_locale : locale::get();
		NumTrimmer trimmer( begin(), end(), loc.decimalSep() );
		if( trimmer.allZeros() )					// Return empty!
			return substring( end().getEnd() );
		if( trimmer.firstNoneZeroIsPoint() )		// Include previous zero (if any)
			--trimmer.FirstNonZero;
		trimmer.findDecimalSep();
		if( trimmer.isInteger() )
			return substring( trimmer.FirstNonZero, end() );
		trimmer.findLastNonZero();
		if( !trimmer.hasTrailingFractionalZeros() )
			return trimmer.integerPartForFloat();
		return substring( trimmer.FirstNonZero, trimmer.LastNonZero + 1 );
	}
	EON_TEST( substring, trimFloat, empty,
		EON_EQ( "", substring( "" ).trimFloat() ) );
	EON_TEST( substring, trimFloat, nothing_to_trim,
		EON_EQ( "123.45", substring( "123.45" ).trimFloat() ) );
	EON_TEST( substring, trimFloat, leading_int_zeros,
		EON_EQ( "123.45", substring( "00123.45" ).trimFloat() ) );
	EON_TEST( substring, trimFloat, all_int_zeros,
		EON_EQ( "", substring( "0000" ).trimFloat() ) );
	EON_TEST( substring, trimFloat, trailing_fractional_zeros,
		EON_EQ( "123.45", substring( "123.4500" ).trimFloat() ) );
	EON_TEST( substring, trimFloat, all_fractional_zeros,
		EON_EQ( "123.0", substring( "123.0000" ).trimFloat() ) );
	EON_TEST( substring, trimFloat, all_int_and_fractional_zeros,
		EON_EQ( "0.0", substring( "000.00" ).trimFloat() ) );
}
