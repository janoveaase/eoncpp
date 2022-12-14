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
		struct digits {
			digits() = default;
			inline digits( byte_t first, byte_t second ) { this->first = first; this->second = second; }
			inline std::string stdstr() const { std::string s; s += first; s += second; return s; }
			inline operator std::string() const { return stdstr(); }
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
		EON_CMPTEST( hex, operator_assign, copy, EON_EQ, "4139", ( hex() = *(const hex*)&hex( "A9" ) ).Value );

		inline hex& operator=( hex&& other ) noexcept { Value = std::move( other.Value ); return *this; }
		EON_CMPTEST( hex, operator_assign, move, EON_EQ, "4139", ( hex() = hex( "A9" ) ).Value );

		hex& operator=( const std::string& binary_data );
		hex& operator=( const char* binary_data );

		inline hex& operator+=( const hex& other ) { Value += other.Value; return *this; }
		EON_CMPTEST( hex, operator_plusassign, basic, EON_EQ, "413962", ( hex( "A9" ) += "b" ).Value );

		inline friend hex operator+( const hex& a, const hex& b ) { hex c{ a }; return c += b; }
		EON_CMPTEST( hex, operator_plus, basic, EON_EQ, "4139", ( hex( "A" ) + hex( "9" ) ).Value );

		// Set hex value
		hex& setHex( const std::string& hex );

		// Clear value
		inline void clear() noexcept { Value.clear(); }
		EON_CMPTEST_3STEP( hex, clear, basic, hex x( "A0" ), x.clear(), EON_EQ, "", x.Value );




		///////////////////////////////////////////////////////////////////////
		//
		// Read-only Methods
		//
	public:

		// Check if empty
		inline bool empty() const noexcept { return Value.empty(); }
		EON_CMPTEST( hex, empty, empty, EON_EQ, true, hex().empty() );
		EON_CMPTEST( hex, empty, nonempty, EON_EQ, false, hex( "A0" ).empty() );

		// Use as boolean
		// Returns true if not empty!
		inline operator bool() const noexcept { return !Value.empty(); }
		EON_CMPTEST( hex, operator_bool, empty, EON_EQ, false, static_cast<bool>( hex() ) );
		EON_CMPTEST( hex, operator_bool, nonempty, EON_EQ, true, static_cast<bool>( hex( "A0" ) ) );

		// Get number of hex digits
		// (Number of bytes can be obtained by dividing by 2!)
		inline index_t size() const noexcept { return Value.size(); }
		EON_CMPTEST( hex, size, empty, EON_EQ, 0, hex().size() );
		EON_CMPTEST( hex, size, nonempty, EON_EQ, 8, hex( "abcd" ).size() );


		// Get value as binary data
		// This involves converting from hex to binary!
		std::string binary() const noexcept;

		// Access the hex data
		inline const std::string& value() const noexcept { return Value; }

		// Access byte data for given position
		inline byte_t byte( index_t pos ) const { return toByte( digits( Value[ pos * 2 ], Value[ pos * 2 + 1 ] ) ); }


		// Comparison
		inline int compare( const hex& other ) const noexcept { return Value.compare( other.Value ); }
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
