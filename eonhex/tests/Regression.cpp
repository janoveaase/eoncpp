#include "Regression.h"



namespace eon
{
	TEST( HexTest, toHex )
	{
		WANT_EQ( "00", hex::toHex( 0 ).stdstr() );
		WANT_EQ( "30", hex::toHex( '0' ).stdstr() );
		WANT_EQ( "41", hex::toHex( 'A' ).stdstr() );
		WANT_EQ( "FF", hex::toHex( 255 ).stdstr() );
	}
	TEST( HexTest, toByte )
	{
		WANT_EQ( 0, hex::toByte( hex::digits( '0', '0' ) ) );
		WANT_EQ( '0', hex::toByte( hex::digits( '3', '0' ) ) );
		WANT_EQ( 'A', hex::toByte( hex::digits( '4', '1' ) ) );
		WANT_EQ( 255, hex::toByte( hex::digits( 'F', 'F' ) ) );
	}

	TEST( HexTest, binary1 )
	{
		std::string bin{ "This is binary!" };
		bin += '\0'; bin += static_cast<char>( 0xC6 );
		hex h( bin );
		WANT_EQ( "546869732069732062696E6172792100C6", h.value() );
		WANT_EQ( bin, h.binary() );
	}
	TEST( HexTest, binary2 )
	{
		std::string hx{ "546869732069732062696E6172792100C6" };
		std::string bin{ "This is binary!" };
		bin += '\0'; bin += static_cast<char>( 0xC6 );
		hex h;
		REQUIRE_NO_EXCEPT( h.setHex( hx ) );
		WANT_EQ( "546869732069732062696E6172792100C6", h.value() );
		WANT_EQ( bin, h.binary() );
	}
	TEST( HexTest, binary3 )
	{
		std::string hx1{ "546869732069732062696E6172792100C" };
		std::string hx2{ "546869732069732062696E6172792100Ct" };
		hex h;
		WANT_EXCEPT( h.setHex( hx1 ), hex::Invalid )
			<< "Failed to detect odd number of digits";
		WANT_EXCEPT( h.setHex( hx2 ), hex::Invalid )
			<< "Failed to detect invalid digit";
	}
}
