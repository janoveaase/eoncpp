#pragma once
#include <cstdint>
#include <cstdlib>
#include "UniChar.h"


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// Eon Name Type Class - eon::name_t
	//
	class name_t
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Constructors
		//
	public:

		// Construct a null-name (no_name)
		name_t() = default;

		// Construct a name from an unsigned 32 bit integer.
		inline explicit name_t( uint32_t value ) noexcept : Value( value ) {}

		// Construct a name from an int - which will be converted into an unsigned 32 bit integer.
		explicit inline name_t( int value ) noexcept : Value( static_cast<uint32_t>( value ) ) {}

#if INTPTR_MAX == INT64_MAX
		// Construct a name from a size_t - which will be converted into an unsigned 32 bit integer.
		explicit inline name_t( size_t value ) noexcept : Value( static_cast<uint32_t>( value ) ) {}
#endif

		// Copy another name
		name_t( const name_t& ) noexcept = default;




		///////////////////////////////////////////////////////////////////////
		//
		// Basic access
		//
	public:

		// Copy another name
		name_t& operator=( const name_t& ) noexcept = default;


		// Get the name as an unsigned 32 bit integer.
		inline uint32_t value() const noexcept { return Value; }

		// Allow the use of name_t as index.
		inline explicit operator size_t() const noexcept { return static_cast<size_t>( Value ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Comparison
		//
	public:

		// Check if 'this' name sorts before 'other'.
		inline friend bool operator<( const name_t& a, const name_t& b ) noexcept { return a.Value < b.Value; }

		// Check if 'this' name sorts before or same as 'other'.
		inline friend bool operator<=( const name_t& a, const name_t& b ) noexcept { return a.Value <= b.Value; }

		// Check if 'this' name sorts after 'other'.
		inline friend bool operator>( const name_t& a, const name_t& b ) noexcept { return a.Value > b.Value; }

		// Check if 'this' name sorts after or same as 'other'.
		inline friend bool operator>=( const name_t& a, const name_t& b ) noexcept { return a.Value >= b.Value; }

		// Check if 'this' name sorts same as 'other'.
		inline friend bool operator==( const name_t& a, const name_t& b ) noexcept { return a.Value == b.Value; }

		// Check if 'this' name sorts before or after 'other'.
		inline friend bool operator!=( const name_t& a, const name_t& b ) noexcept { return a.Value != b.Value; }




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		std::uint32_t Value{ 0 };
	};

	// The null name
	static const name_t no_name{ 0 };
	static const name_t nn{ 0 };
}




namespace std
{
	template<>
	struct hash<::eon::name_t> {
		std::hash<uint32_t> Hasher;
		inline size_t operator()( const ::eon::name_t& a ) const { return Hasher( a.value() ); } };
	template<>
	struct equal_to<::eon::name_t> {
		inline bool operator()( const ::eon::name_t& a, const ::eon::name_t& b ) const { return a == b; } };
}
