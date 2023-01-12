#include "String.h"
#include <eoninlinetest/InlineTest.h>
#include <cctype>
#include <regex>
#include <unordered_map>


namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// Strings as Numbers
	//

	EON_TEST( string, numeralsOnly, no,
		EON_FALSE( string( "1b2" ).numeralsOnly() ) );
	EON_TEST( string, numeralsOnly, yes,
		EON_TRUE( string( "123" ).numeralsOnly() ) );
	EON_TEST( string, numeralsOnly, decimal_sep,
		EON_FALSE( string( "1.3" ).numeralsOnly() ) );
	EON_TEST( string, numeralsOnly, UTF8,
		EON_TRUE( string( u8"1١" ).numeralsOnly() ) );

	EON_TEST_2STEP( string, numeralsOnly, substr,
		string obj( "ab12cd" ),
		EON_TRUE( obj.numeralsOnly( obj.substr( obj.begin() + 2, obj.end() - 2 ) ) ) );


	EON_TEST( string, isUInt, yes,
		EON_TRUE( string( "12345" ).isUInt() ) );
	EON_TEST( string, isUInt, no,
		EON_FALSE( string( "-12345" ).isUInt() ) );

	EON_TEST_2STEP( string, isUInt, substr,
		string obj( "-12345.0" ),
		EON_TRUE( obj.isUInt( obj.substr( obj.begin() + 1, obj.end() - 2 ) ) ) );


	EON_TEST( string, isInt, basic,
		EON_TRUE( string( "-12345" ).isInt() ) );

	EON_TEST_2STEP( string, isInt, substr,
		string obj( "-12345.0" ),
		EON_TRUE( obj.isInt( obj.substr( obj.begin(), obj.end() - 2 ) ) ) );

	EON_TEST( string, isFloat, no,
		EON_FALSE( string( "-12345" ).isFloat() ) );
	EON_TEST( string, isFloat, yes,
		EON_TRUE( string( "-123.45" ).isFloat() ) );

	EON_TEST_2STEP( string, isFloat, substr,
		string obj( "a-123.45b" ),
		EON_TRUE( obj.isFloat( obj.substr( obj.begin() + 1, obj.end() - 1 ) ) ) );


	EON_TEST( string, isNumerical, uint,
		EON_TRUE( string( "123" ).isNumerical() ) );
	EON_TEST( string, isNumerical, int,
		EON_TRUE( string( "-123" ).isNumerical() ) );
	EON_TEST( string, isNumerical, float,
		EON_TRUE( string( "-12.3" ).isNumerical() ) );

	EON_TEST_2STEP( string, isNumerical, substr_uint,
		string obj( "a123b" ),
		EON_TRUE( obj.isNumerical( obj.substr( obj.begin() + 1, obj.end() - 1 ) ) ) );
	EON_TEST_2STEP( string, isNumerical, substr_int,
		string obj( "a-123b" ),
		EON_TRUE( obj.isNumerical( obj.substr( obj.begin() + 1, obj.end() - 1 ) ) ) );;
	EON_TEST_2STEP( string, isNumerical, substr_float,
		string obj( "a-12.3b" ),
		EON_TRUE( obj.isNumerical( obj.substr( obj.begin() + 1, obj.end() - 1 ) ) ) );


	EON_TEST( string, toIntT, proper,
		EON_EQ( 123, string( "123" ).toIntT() ) );
	EON_TEST( string, toIntT, invalid,
		EON_EQ( 0, string( "12b3" ).toIntT() ) );


	string string::separateThousands( char_t thousands_sep, const eon::locale* custom_locale ) const
	{
		iterator separator;
		auto area = substr().findFirstNumber( &separator, custom_locale );
		if( !area )
			return *this;
		if( separator )
			area.end() = separator;
		auto diff = ( area.end() - area.begin() ) % 3;
		if( diff == 0 )
			diff = 3;
		area.begin() += diff;
		string output = _prepOutput( area );
		for( auto pos = area.begin(); pos != area.end(); pos += 3 )
			output << thousands_sep << substr( pos, pos + 3 );
		if( area.end() != end() )
			output += substr( area.end(), end() );
		return output;
	}
	EON_TEST( string, separateThousands, empty,
		EON_EQ( "", string().separateThousands() ) );
	EON_TEST( string, separateThousands, non_numerical,
		EON_EQ( "abcdef", string( "abcdef" ).separateThousands() ) );
	EON_TEST( string, separateThousands, all_integer_short,
		EON_EQ( "123", string( "123" ).separateThousands() ) );
	EON_TEST( string, separateThousands, all_integer_even,
		EON_EQ( "123,456,789", string( "123456789" ).separateThousands() ) );
	EON_TEST( string, separateThousands, all_integer_odd,
		EON_EQ( "12,345,678", string( "12345678" ).separateThousands() ) );
	EON_TEST( string, separateThousands, all_float_even,
		EON_EQ( "123,456.789", string( "123456.789" ).separateThousands() ) );
	EON_TEST( string, separateThousands, all_float_odd,
		EON_EQ( "1,234,567.89", string( "1234567.89" ).separateThousands() ) );
	EON_TEST( string, separateThousands, all_float_no_int,
		EON_EQ( ".123,456,789", string( ".123456789" ).separateThousands() ) );
	EON_TEST( string, separateThousands, part_float_short,
		EON_EQ( "abc123.456def", string( "abc123.456def" ).separateThousands() ) );
	EON_TEST( string, separateThousands, part_float_even,
		EON_EQ( "abc123,456.789", string( "abc123456.789" ).separateThousands() ) );
	EON_TEST( string, separateThousands, part_float_odd,
		EON_EQ( "abc1,234,567.89", string( "abc1234567.89" ).separateThousands() ) );


	string string::roundNumber( index_t max_decimals, const eon::locale* custom_locale ) const
	{
		const eon::locale& loc = custom_locale != nullptr ? *custom_locale : locale::get();
		if( !isFloat( &loc ) )
			return *this;

		auto digits = characters<std::vector<char_t>>();
		auto sep = _findDecimalSeparator( digits, loc.decimalSep() );
		if( sep == digits.size() )
			return *this;

		auto num_decimals = digits.size() - sep - 1;
		if( num_decimals <= max_decimals )
			return *this;

		auto value = numeralValue( digits[ sep + 1 + max_decimals ] );
		if( max_decimals == 0 )
			digits.erase( digits.begin() + sep, digits.end() );
		else
			digits.erase( digits.begin() + sep + 1 + max_decimals, digits.end() );
		if( value >= 5 )
			_roundUp( digits, digits.end() - 1, loc.decimalSep()  );

		return string( digits );
	}
	EON_TEST( string, roundNumber, empty,
		EON_EQ( "", string().roundNumber( 2 ) ) );
	EON_TEST( string, roundNumber, non_numerical,
		EON_EQ( "abcd", string( "abcd" ).roundNumber( 2 ) ) );
	EON_TEST( string, roundNumber, integer,
		EON_EQ( "12345", string( "12345" ).roundNumber( 2 ) ) );
	EON_TEST( string, roundNumber, integer_default,
		EON_EQ( "12345", string( "12345" ).roundNumber() ) );
	EON_TEST( string, roundNumber, rounded_float,
		EON_EQ( "123.45", string( "123.45" ).roundNumber( 2 ) ) );
	EON_TEST( string, roundNumber, round_down,
		EON_EQ( "12.34", string( "12.344" ).roundNumber( 2 ) ) );
	EON_TEST( string, roundNumber, round_up,
		EON_EQ( "12.35", string( "12.345" ).roundNumber( 2 ) ) );
	EON_TEST( string, roundNumber, round_down_zero_dec,
		EON_EQ( "12", string( "12.345" ).roundNumber( 0 ) ) );
	EON_TEST( string, roundNumber, round_up_zero_dec,
		EON_EQ( "13", string( "12.543" ).roundNumber( 0 ) ) );
}
