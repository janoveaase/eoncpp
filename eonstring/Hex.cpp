#include "Hex.h"


namespace eon
{
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
	EON_CMPTEST( hex, operator_assign, binary_stdstr, EON_EQ, "4139", ( hex() = std::string( "A9" ) ).Value );

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
	EON_CMPTEST( hex, operator_assign, binary_char, EON_EQ, "4139", ( hex() = "A9" ).Value );

	hex& hex::setHex( const std::string& hex )
	{
		_assertValidHex( hex );
		Value = _parseHex( hex );
		return *this;
	}
	EON_CMPTEST( hex, setHex, empty, EON_EQ, "", hex().setHex( "" ).Value );
	EON_CMPTEST( hex, setHex, nonempty, EON_EQ, "10DE", hex().setHex( "10DE" ).Value );




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
	EON_CMPTEST( hex, binary, basic, EON_EQ, "a1Z", hex( "a1Z" ).binary() );




	hex::digits hex::toHex( byte_t byte ) noexcept
	{
		digits hex;
		hex.first = byte / 16;
		hex.second = byte % 16;
		hex.first += hex.first < 10 ? 0x30 : 0x37;
		hex.second += hex.second < 10 ? 0x30 : 0x37;
		return hex;
	}
	EON_CMPTEST( hex, toHex, low, EON_EQ, "48", hex::toHex( 'H' ) );
	EON_CMPTEST( hex, toHex, high, EON_EQ, "EC", hex::toHex( 236 ) );

	byte_t hex::toByte( digits hex ) noexcept
	{
		hex.first -= hex.first >= 0x41 ? 0x37 : 0x30;
		hex.second -= hex.second >= 0x41 ? 0x37 : 0x30;
		return hex.first * 16 + hex.second;
	}
	EON_CMPTEST( hex, toByte, low, EON_EQ, byte_t( 'H' ), hex::toByte( hex::digits( '4', '8' ) ) );
	EON_CMPTEST( hex, toByte, high, EON_EQ, byte_t( 236 ), hex::toByte( hex::digits( 'E', 'C' ) ) );




	std::string hex::_parseHex( const std::string& hex )
	{
		if( hex.size() == 0 )
			return std::string();
		else
			return _parseNonEmptyHex( hex );
	}
	EON_CMPTEST( hex, _parseHex, empty, EON_EQ, "", hex()._parseHex( "" ) );
	EON_CMPTEST( hex, _parseHex, nonempty, EON_EQ, "10DE", hex()._parseHex( "10DE" ) );

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
	EON_CMPTEST( hex, _parseNonEmptyHex, basic, EON_EQ, "0EFF32", hex()._parseNonEmptyHex( "0EFF32" ) );
}
