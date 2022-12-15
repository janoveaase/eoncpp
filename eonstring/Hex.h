#pragma once
#include "String.h"
#include <eonexcept/Exception.h>
#include <eoninlinetest/InlineTest.h>
#include <set>
#include <mutex>


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// Eon Hex Class - eon::hex
	//
	// This class can convert bytes to and from hex digits, but also store
	// binary in the form of hex digits.
	//
	class hex
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Definitions
		//
	public:
		
		// Exception thrown when trying to set an invalid hex value
		EONEXCEPT( Invalid );


		// Pair of hex digits
		struct digits
		{
			digits() = default;
			inline digits( byte_t first, byte_t second ) { this->first = first; this->second = second; }

			inline std::string stdstr() const { std::string s; s += first; s += second; return s; }
			EON_CMPTEST( digits, stdstr, basic, "F2", EON_EQ, digits( 'F', '2' ).stdstr() );

			inline operator std::string() const { return stdstr(); }
			EON_CMPTEST( digits, str, basic, "F2", EON_EQ, static_cast<eon::string>( digits( 'F', '2' ) ) );

			byte_t first{ '0' }, second{ '0' };
		};




		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		hex() = default;
		inline hex( const hex& other ) { Value = other.Value; }
		inline hex( hex&& other ) noexcept { Value = std::move( other.Value ); }
		inline hex( const std::string& binary_data ) { *this = binary_data; }
		inline hex( const char* binary_data ) { *this = binary_data; }


		virtual ~hex() = default;




		///////////////////////////////////////////////////////////////////////
		//
		// Modifier Methods
		//
	public:

		inline hex& operator=( const hex& other ) { Value = other.Value; return *this; }
		EON_CMPTEST( hex, operator_assign, copy, "4139", EON_EQ, ( hex() = *(const hex*)&hex( "A9" ) ).Value );

		inline hex& operator=( hex&& other ) noexcept { Value = std::move( other.Value ); return *this; }
		EON_CMPTEST( hex, operator_assign, move, "4139", EON_EQ, ( hex() = hex( "A9" ) ).Value );

		hex& operator=( const std::string& binary_data );
		hex& operator=( const char* binary_data );

		inline hex& operator+=( const hex& other ) { Value += other.Value; return *this; }
		EON_CMPTEST( hex, operator_plusassign, basic, "413962", EON_EQ, ( hex( "A9" ) += "b" ).Value );

		inline friend hex operator+( const hex& a, const hex& b ) { hex c{ a }; return c += b; }
		EON_CMPTEST( hex, operator_plus, basic, "4139", EON_EQ, ( hex( "A" ) + hex( "9" ) ).Value );

		// Set hex value
		hex& setHex( const std::string& hex );

		// Clear value
		inline void clear() noexcept { Value.clear(); }
		EON_CMPTEST_3STEP( hex, clear, basic, hex x( "A0" ), x.clear(), "", EON_EQ, x.Value );




		///////////////////////////////////////////////////////////////////////
		//
		// Read-only Methods
		//
	public:

		// Check if empty
		inline bool empty() const noexcept { return Value.empty(); }
		EON_CMPTEST( hex, empty, empty, true, EON_EQ, hex().empty() );
		EON_CMPTEST( hex, empty, nonempty, false, EON_EQ, hex( "A0" ).empty() );

		// Use as boolean
		// Returns true if not empty!
		inline operator bool() const noexcept { return !Value.empty(); }
		EON_CMPTEST( hex, operator_bool, empty, false, EON_EQ, static_cast<bool>( hex() ) );
		EON_CMPTEST( hex, operator_bool, nonempty, true, EON_EQ, static_cast<bool>( hex( "A0" ) ) );

		// Get number of hex digits
		// (Number of bytes can be obtained by dividing by 2!)
		inline index_t size() const noexcept { return Value.size(); }
		EON_CMPTEST( hex, size, empty, 0, EON_EQ, hex().size() );
		EON_CMPTEST( hex, size, nonempty, 8, EON_EQ, hex( "abcd" ).size() );


		// Get value as binary data
		// This involves converting from hex to binary!
		std::string binary() const noexcept;

		// Access the hex data
		inline const std::string& value() const noexcept { return Value; }

		// Access byte data for given position
		inline byte_t byte( index_t pos ) const {
			if( pos * 2 >= Value.size() ) throw std::out_of_range( "eon::hex::byte position out of range!" );
			return toByte( digits( Value[ pos * 2 ], Value[ pos * 2 + 1 ] ) ); }
		EON_CMPTEST( hex, byte, basic, "9", EON_EQ, hex( "F29E32" ).byte( 2 ) );
		EON_XTEST( hex, byte, throw, std::out_of_range, hex( "F29E32" ).byte( 6 ) );


		// Compare against another hex, returns negative if 'this' < 'other', 0 if same, and positive if 'this' > 'other'.
		inline int compare( const hex& other ) const noexcept { return Value.compare( other.Value ); }
		EON_CMPTEST( hex, compare, lt, 0, EON_GT, hex( "A" ).compare( hex( "B" ) ) );
		EON_CMPTEST( hex, compare, eq, 0, EON_EQ, hex( "A" ).compare( hex( "A" ) ) );
		EON_CMPTEST( hex, compare, gt, 0, EON_LT, hex( "B" ).compare( hex( "A" ) ) );

		// Check if 'this' is less than 'other'.
		inline bool operator<( const hex& other ) const noexcept { return Value < other.Value; }




		///////////////////////////////////////////////////////////////////////
		//
		// Static Methods
		//
	public:

		// Convert a byte into two hex digits (represented by a 16 bit
		// unsigned integer)
		static digits toHex( byte_t byte ) noexcept;

		// Convert two hex digits (represented by a 16 bit unsigned integer)
		// into a byte
		static byte_t toByte( digits hex ) noexcept;

		// Check if the given byte is a hex digit
		static inline bool isDigit( byte_t byte ) noexcept {
			return ( byte >= '0' && byte <= '9' ) || ( byte >= 'A' && byte <= 'F' ) || ( byte >= 'a' && byte <= 'f' ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	PRIVATE:

		inline void _assertValidHex( const std::string& hex ) {
			if( hex.size() % 2 == 1 ) throw Invalid( "Odd number of digits" ); }
		EON_NOXTEST( hex, _assertValidHex, no_throw, hex()._assertValidHex( "B2" ) );
		EON_XTEST( hex, _assertValidHex, throw, Invalid, hex()._assertValidHex( "B20" ) );

		std::string _parseHex( const std::string& hex );
		std::string _parseNonEmptyHex( const std::string& hex );



		
		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	PRIVATE:

		std::string Value;
	};
}
