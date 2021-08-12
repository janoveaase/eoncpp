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

	void hex::setHex( const std::string& hex )
	{
		if( hex.size() % 2 == 1 )
			throw Invalid( "Odd number of digits" );
		if( hex.size() == 0 )
		{
			clear();
			return;
		}
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
		Value = std::move( value );
	}




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




	hex::digits hex::toHex( byte_t byte ) noexcept
	{
		digits hex;
		hex.first = byte / 16;
		hex.second = byte % 16;
		hex.first += hex.first < 10 ? 0x30 : 0x37;
		hex.second += hex.second < 10 ? 0x30 : 0x37;
		return hex;
	}
	byte_t hex::toByte( digits hex ) noexcept
	{
		hex.first -= hex.first >= 0x41 ? 0x37 : 0x30;
		hex.second -= hex.second >= 0x41 ? 0x37 : 0x30;
		return hex.first * 16 + hex.second;
	}
}
