#pragma once

#include <eonexcept/Exception.h>
#include <typeindex>



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Eon primitives (not defined elsewhere)
	using int_t = long int;
	using short_t = int16_t;
	using long_t = int64_t;
	using index_t = size_t;
	static const index_t no_index{ SIZE_MAX };
	using flt_t = double;
	using low_t = float;
	using high_t = long double;
	using handle_t = void*;
	static const handle_t no_handle{ nullptr };




	/**************************************************************************
	  The 'eon::type' namespace encloses all hidden type functionality
	**************************************************************************/
	namespace type
	{
		// Exception thrown when trying to set a null value
		EONEXCEPT( NullValue );

		// Exception thrown when trying to access an item that doesn't exists
		EONEXCEPT( NotFound );

		// Exception thrown when trying to access an item of the wrong type
		EONEXCEPT( WrongType );

		// Exception thrown when trying to access attribute as mutable when
		// declared read-only
		EONEXCEPT( AccessDenied );

		// Exception thrown when trying to add a new attribute with the same
		// name as an exsisting
		EONEXCEPT( DuplicateName );

		// Exception thrown when trying to assign a value of type that is
		// different and incompatible with the attribute's type
		EONEXCEPT( IncompatibleType );

		// Exception thrown when a required argument is missing
		EONEXCEPT( MissingArgument );




		//* Actions (specifically operators) have precedence, determining how
		//* some are to be evaluated before others. Æon operates with the
		//* following levels:
		enum class Precedence
		{
			max_hi = 34, max_lo = 33,					// return bumpcall

			upper_ultra_hi = 32, upper_ultra_lo = 31,	// [] type uid call 's
			ultra_hi = 30, ultra_lo = 29,				// is -(unary) not ~
			lower_ultra_hi = 28, lower_ultra_lo = 27,	// [:] in swap

			upper_high_hi = 26, upper_high_lo = 25,		// * /
			high_hi = 24, high_lo = 23,					// + -
			lower_high_hi = 22, lower_high_lo = 21,		// << >>

			upper_mid_hi = 20, upper_mid_lo = 19,		// cmp
			mid_hi = 18, mid_lo = 17,					// < <= > >=
			lower_mid_hi = 16, lower_mid_lo = 15,		// == !=

			upper_low_hi = 14, upper_low_lo = 13,		// &
			low_hi = 12, low_lo = 11,					// ^
			lower_low_hi = 10, lower_low_lo = 9,		// |

			upper_micro_hi = 8, upper_micro_lo = 7,		// and
			micro_hi = 6, micro_lo = 5,					// xor
			lower_micro_hi = 4, lower_micro_lo = 3,		// or

			// += -= *= /= = raise if if-else for-each for-each-where while
			// until find continue break
			min_hi = 2, min_lo = 1,
			none = 0
		};
	}
}
