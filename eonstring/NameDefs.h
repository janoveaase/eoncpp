#pragma once
#include <cstdint>
#include <cstdlib>
#include "UniChar.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* The name type
#if defined( _DEBUG ) || defined( EON_SUN )
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

#if defined( _DEBUG ) || defined( EON_SUN )
namespace std
{
	template<>
	struct hash<::eon::name_t> {
		std::hash<uint32_t> Hasher;
		inline size_t operator()( const ::eon::name_t& a ) const { return Hasher( a.Value ); } };
	template<>
	struct equal_to<::eon::name_t> {
		inline bool operator()( const ::eon::name_t& a, const ::eon::name_t& b ) const { return a.Value == b.Value; }
	};
}
#endif
