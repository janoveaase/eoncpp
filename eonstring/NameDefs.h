#pragma once
#include <cstdint>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* The name type
#ifdef _DEBUG
	struct name_t
	{
		name_t() = default;
		inline name_t( size_t value ) { Value = static_cast<std::uint32_t>( value ); }

		inline operator size_t() const noexcept { return static_cast<size_t>( Value ); }

		inline bool operator<( const name_t& other ) const noexcept { return Value < other.Value; }
		inline bool operator<=( const name_t& other ) const noexcept { return Value <= other.Value; }
		inline bool operator>( const name_t& other ) const noexcept { return Value > other.Value; }
		inline bool operator>=( const name_t& other ) const noexcept { return Value >= other.Value; }
		inline bool operator==( const name_t& other ) const noexcept { return Value == other.Value; }
		inline bool operator!=( const name_t& other ) const noexcept { return Value != other.Value; }

		std::uint32_t Value{ 0 };
	};
#else
	using name_t = std::uint32_t;
#endif

	//* The null name
	static const name_t no_name{ 0 };
}
