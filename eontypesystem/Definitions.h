#pragma once

#include <eonexcept/Exception.h>
#include <eoncontainers/Stack.h>
#include <eoncontainers/ReflexiveMap.h>
#include <eonstring/Name.h>
#include <eonstring/Stringifier.h>
#include <typeindex>
#include <vector>
#include <unordered_set>
#include <limits.h>
#include <eoninlinetest/InlineTest.h>



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	namespace type
	{
		EONEXCEPT( AccessDenied );
		EONEXCEPT( WrongType );
		EONEXCEPT( NotFound );
		EONEXCEPT( DuplicateName );
		EONEXCEPT( NoName );

		EON_NAME( system );
		EON_NAME( copy );
		EON_NAME( variable );
		EON_NAME( attribute );

		enum class Qualifier : uint8_t
		{
			_none = 0x00,			// No qualifiers.
			_owned = 0x01,			// Value is owned.
			_borrowed = 0x02,		// Value is borrowed. (Can co-exist with _owned but takes precedence!)
			_optional = 0x04,		// Value is optional. (Can be null!)
			_pointer = 0x08,		// Value is a pointer to an optional value. (The optional value can be null!)
			_reference = 0x10,		// Value is a reference to another value.
			_literal = 0x20,		// Value originates as a literal. (This is also an rvalue!)
			_rvalue = 0x40,			// Value is an rvalue. (Assigning to it is pointless!)
		};
		inline Qualifier operator|( Qualifier a, Qualifier b ) noexcept {
			return static_cast<Qualifier>( static_cast<int>( a ) | static_cast<int>( b ) ); }
		inline Qualifier& operator|=( Qualifier& a, Qualifier b ) noexcept {
			return a = static_cast<Qualifier>( static_cast<int>( a ) | static_cast<int>( b ) ); }
		inline Qualifier operator&( Qualifier a, Qualifier b ) noexcept {
			return static_cast<Qualifier>( static_cast<int>( a ) & static_cast<int>( b ) ); }
		inline bool operator&&( Qualifier a, Qualifier b ) noexcept {
			return static_cast<bool>( static_cast<int>( a ) & static_cast<int>( b ) ); }
		inline Qualifier operator~( Qualifier a ) noexcept { return static_cast<Qualifier>( ~static_cast<int>( a ) ); }

#define EON_SHORT_MIN INT16_MIN
#define EON_SHORT_MAX INT16_MAX
#define EON_INT_MIN INT_MIN
#define EON_INT_MAX INT_MAX
#define EON_LONG_MIN INT64_MIN
#define EON_LONG_MAX INT64_MAX
#define EON_LOW_MIN std::numeric_limits<low_t>::min()
#define EON_LOW_MAX std::numeric_limits<low_t>::max()
#define EON_FLOAT_MIN std::numeric_limits<flt_t>::min()
#define EON_FLOAT_MAX std::numeric_limits<flt_t>::max()
#define EON_HIGH_MIN std::numeric_limits<high_t>::min()
#define EON_HIGH_MAX std::numeric_limits<high_t>::max()
	}

#define ptr_t void*
#if SIZE_MAX == UINT64_MAX
#	define val_t(type) (type)(uint64_t)
#	define to_ptr (ptr_t)(uint64_t)
#elif SIZE_MAX == UINT32_MAX
#	define val_t(type) (type)(int32_t)
#	define to_ptr (ptr_t)(uint32_t)
#else
#	error Unknown pointer size
#endif


	extern const eon::name_t name_ttypetuple;
	extern const eon::name_t name_value;


	using b8_t = uint8_t;
	using b16_t = uint16_t;
	using b32_t = uint32_t;
	using b64_t = uint64_t;


	using handle_t = void*;
	static const handle_t no_handle{ nullptr };


#define CPPTYPE( x ) std::type_index( typeid( x ) )
}
