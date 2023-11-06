#pragma once
#include "String.h"
#include <eonexcept/Exception.h>
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
		inline hex& operator=( hex&& other ) noexcept { Value = std::move( other.Value ); return *this; }

		hex& operator=( const std::string& binary_data );
		hex& operator=( const char* binary_data );

		inline hex& operator+=( const hex& other ) { Value += other.Value; return *this; }
		inline friend hex operator+( const hex& a, const hex& b ) { hex c{ a }; return c += b; }

		// Set hex value
		hex& setHex( const std::string& hex );

		// Clear value
		inline void clear() noexcept { Value.clear(); }




		///////////////////////////////////////////////////////////////////////
		//
		// Read-only Methods
		//
	public:

		// Check if empty
		inline bool empty() const noexcept { return Value.empty(); }

		// Use as boolean
		// Returns true if not empty!
		inline operator bool() const noexcept { return !Value.empty(); }

		// Get number of hex digits
		// (Number of bytes can be obtained by dividing by 2!)
		inline index_t size() const noexcept { return Value.size(); }


		// Get value as binary data
		// This involves converting from hex to binary!
		std::string binary() const noexcept;

		// Access the hex data
		inline const std::string& value() const noexcept { return Value; }

		// Access byte data for given position
		inline byte_t byte( index_t pos ) const {
			if( pos * 2 >= Value.size() ) throw std::out_of_range( "eon::hex::byte position out of range!" );
			return toByte( digits( Value[ pos * 2 ], Value[ pos * 2 + 1 ] ) ); }


		// Compare against another hex, returns negative if 'this' < 'other', 0 if same, and positive if 'this' > 'other'.
		inline int compare( const hex& other ) const noexcept { return Value.compare( other.Value ); }

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
	EON_PRIVATE:

		inline void _assertValidHex( const std::string& hex ) {
			if( hex.size() % 2 == 1 ) throw Invalid( "Odd number of digits" ); }

		std::string _parseHex( const std::string& hex );
		std::string _parseNonEmptyHex( const std::string& hex );




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	EON_PRIVATE:

		std::string Value;
	};
}
