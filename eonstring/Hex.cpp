#include "Hex.h"
#include <eoninlinetest/InlineTest.h>


namespace eon
{
	EON_TEST( digits, stdstr, basic,
		EON_EQ( "F2", hex::digits( 'F', '2' ).stdstr() ) );
	EON_TEST( digits, str, basic,
		EON_EQ( eon::string( "F2" ), static_cast<eon::string>( hex::digits( 'F', '2' ) ) ) );




	EON_TEST_2STEP( hex, operator_assign, copy,
		hex obj( "A9" ),
		EON_EQ( "4139", ( hex() = *(const hex*)&obj ).Value ) );
	EON_TEST( hex, operator_assign, move,
		EON_EQ( "4139", ( hex() = hex( "A9" ) ).Value ) );

	hex& hex::operator=( const std::string& binary_data )
	{
		clear();
		Value.reserve( binary_data.size() * 2 );
		for( auto c : binary_data )
		{
			auto digits = toHex( c );
			Value += static_cast<char>( digits.first );
			Value += static_cast<char>( digits.second );
		}
		return *this;
	}
	EON_TEST( hex, operator_asgn, binary_stdstr,
		EON_EQ( "4139", ( hex() = std::string( "A9" ) ).Value ) );

	hex& hex::operator=( const char* binary_data )
	{
		clear();
		for( auto c = binary_data; *c != '\0'; ++c )
		{
			auto digits = toHex( *c );
			Value += static_cast<char>( digits.first );
			Value += static_cast<char>( digits.second );
		}
		return *this;
	}
	EON_TEST( hex, operator_asgn, binary_char,
		EON_EQ( "4139", ( hex() = "A9" ).Value ) );

	EON_TEST( hex, operator_plusassign, basic,
		EON_EQ( "413962", ( hex( "A9" ) += "b" ).Value ) );
	EON_TEST( hex, operator_plus, basic,
		EON_EQ( "4139", ( hex( "A" ) + hex( "9" ) ).Value ) );

	hex& hex::setHex( const std::string& hex )
	{
		_assertValidHex( hex );
		Value = _parseHex( hex );
		return *this;
	}
	EON_TEST( hex, setHex, emtpy,
		EON_EQ( "", hex().setHex( "" ).Value ) );
	EON_TEST( hex, setHex, nonempty,
		EON_EQ( "10DE", hex().setHex( "10DE" ).Value ) );

	EON_TEST_3STEP( hex, clear, basic,
		hex x( "A0" ),
		x.clear(),
		EON_EQ( "", x.Value ) );




	EON_TEST( hex, empty, empty,
		EON_TRUE( hex().empty() ) );
	EON_TEST( hex, empty, nonempty,
		EON_FALSE( hex( "A0" ).empty() ) );

	EON_TEST( hex, operator_bool, empty,
		EON_FALSE( hex() ) );
	EON_TEST( hex, operator_bool, nonempty,
		EON_TRUE( hex( "A0" ) ) );

	EON_TEST( hex, size, empty,
		EON_EQ( 0, hex().size() ) );
	EON_TEST( hex, size, nonempty,
		EON_EQ( 8, hex( "abcd" ).size() ) );

	std::string hex::binary() const noexcept
	{
		std::string bin;
		digits hx;
		for( auto c = Value.c_str(), end = c + Value.size(); c != end; c += 2 )
		{
			hx.first = *c;
			hx.second = *( c + 1 );
			bin += static_cast<char>( toByte( hx ) );
		}
		return bin;
	}
	EON_TEST( hex, binary, basic,
		EON_EQ( "a1Z", hex( "a1Z" ).binary() ) );

	EON_NO_TEST( hex, value );

	EON_TEST( hex, byte, basic,
		EON_EQ( '9', hex( "F29E32" ).byte( 2 ) ) );
	EON_TEST( hex, byte, throw,
		EON_RAISE( hex( "F29E32" ).byte( 6 ), std::out_of_range ) );

	EON_TEST( hex, compare, lt,
		EON_LT( hex( "A" ).compare( hex( "B" ) ), 0 ) );
	EON_TEST( hex, compare, eq,
		EON_EQ( hex( "A" ).compare( hex( "A" ) ), 0 ) );
	EON_TEST( hex, compare, gt,
		EON_GT( hex( "B" ).compare( hex( "A" ) ), 0 ) );




	hex::digits hex::toHex( byte_t byte ) noexcept
	{
		digits hex;
		hex.first = byte / 16;
		hex.second = byte % 16;
		hex.first += hex.first < 10 ? 0x30 : 0x37;
		hex.second += hex.second < 10 ? 0x30 : 0x37;
		return hex;
	}
	EON_TEST( hex, toHex, low,
		EON_EQ( "48", hex::toHex( 'H' ).stdstr() ) );
	EON_TEST( hex, toHex, high,
		EON_EQ( "EC", hex::toHex( 236 ).stdstr() ) );

	byte_t hex::toByte( digits hex ) noexcept
	{
		hex.first -= hex.first >= 0x41 ? 0x37 : 0x30;
		hex.second -= hex.second >= 0x41 ? 0x37 : 0x30;
		return hex.first * 16 + hex.second;
	}
	EON_TEST( hex, toByte, low,
		EON_EQ( byte_t( 'H' ), hex::toByte( hex::digits( '4', '8' ) ) ) );
	EON_TEST( hex, toByte, high,
		EON_EQ( byte_t( 236 ), hex::toByte( hex::digits( 'E', 'C' ) ) ) );



	EON_TEST( hex, _assertValidHex, no_throw,
		EON_NO_X( hex()._assertValidHex( "B2" ) ) );
	EON_TEST( hex, _assertValidHex, throw,
		EON_RAISE( hex()._assertValidHex( "B20" ), hex::Invalid ) );

	std::string hex::_parseHex( const std::string& hex )
	{
		if( hex.size() == 0 )
			return std::string();
		else
			return _parseNonEmptyHex( hex );
	}
	EON_TEST( hex, _parseHex, empty,
		EON_EQ( "", hex()._parseHex( "" ) ) );
	EON_TEST( hex, _parseHex, nonempty,
		EON_EQ( "10DE", hex()._parseHex( "10DE" ) ) );

	std::string hex::_parseNonEmptyHex( const std::string& hex )
	{
		std::string value;
		digits hx;
		for( auto c = hex.c_str(), end = c + hex.size(); c != end; c += 2 )
		{
			hx.first = *c;
			hx.second = *( c + 1 );
			if( !isDigit( hx.first ) || !isDigit( hx.second ) )
				throw Invalid( "Contains non-hex digits" );
			value += static_cast<char>( hx.first );
			value += static_cast<char>( hx.second );
		}
		return value;
	}
	EON_TEST( hex, _parseNonEmptyHex, basic,
		EON_EQ( "0EFF32", hex()._parseNonEmptyHex( "0EFF32" ) ) );
}
