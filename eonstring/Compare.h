#pragma once
#include "StringIterator.h"
#include "Locale.h"


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// The strcmp' namespace encloses all public compare funcionality
	//
	// Under this namespace, binary predicates for comparing strings and
	// characters are defined, for use by [eon::string] and [eon::substring],
	// as well as independently, when working with Unicode characters, etc.
	//
	namespace strcmp
	{
		// Predicate for comparing two bytes as fast as possible.
		// Pro: Very fast.
		// Con: Not locale aware.
		// Returns: -1 if 'a' is less than 'b', 0 if equal, and 1 if 'a' is greater than 'b'.
		// Includes default object 'Cmp'.
		// NOTE: This predicate is identical to [eon::strcmp::chr] except it also
		//       supports call operator with [eon::byte_t] arguments.
		struct byte
		{
			inline int operator()( char_t a, char_t b ) const noexcept { return a < b ? -1 : b < a ? 1 : 0; }
			inline int operator()( char a, char b ) const noexcept { return a < b ? -1 : b < a ? 1 : 0; }
			inline int operator()( byte_t a, byte_t b ) const noexcept { return a < b ? -1 : b < a ? 1 : 0; }
			static const byte Cmp;
		};


		// Predicate for comparing two Unicode characters as fast as possible.
		// Pro: Very fast.
		// Con: Not locale aware.
		// Returns: -1 if 'a' is less than 'b', 0 if equal, and 1 if 'a' is greater than 'b'.
		// Includes default object 'Cmp'.
		struct chr
		{
			inline int operator()( char_t a, char_t b ) const noexcept { return a < b ? -1 : b < a ? 1 : 0; }
			static const chr Cmp;
		};


		// Predicate for comparing two Unicode characters while ignoring case.
		// Pro: Fast.
		// Con: Partially locale aware (for converting case).
		// Returns: -1 if 'a' is less than 'b', 0 if equal, and 1 if 'a' is greater than 'b'.
		// Includes default object 'Cmp'.
		class icase_chr
		{
		public:
			inline icase_chr( const eon::locale* custom_locale = nullptr ) {
				Loc = custom_locale != nullptr ? custom_locale : &eon::locale::get(); }
			inline int operator()( char_t a, char_t b ) const noexcept {
				char_t lw_a = Loc->toLower( static_cast<wchar_t>( a ) ), lw_b = Loc->toLower( static_cast<wchar_t>( b ) );
				return lw_a < lw_b ? -1 : lw_a == lw_b ? 0 : 1; }
			static const icase_chr Cmp;
		private:
			const eon::locale* Loc{ nullptr };
		};



		// Predicate for comparing two byte sequences as fast as possible.
		// Pro: Very fast.
		// Con: Byte comparison, simple comparison only (-1|0|1 returned).
		// Returns: -1 if a is less than b, 0 if equal, and 1 if a is greater than b.
		struct bytes
		{
			int operator()( const char* a_start, size_t a_numbutes, const char* b_start, size_t b_numbytes ) const noexcept;
			static const bytes Cmp;
		};


		// Predicate for comparing two strings lexiographically.
		// Pro: UTF-8 comparison, returns number of equal characters when different.
		// Con: Relatively slow.
		// Returns: Negative value if a is less than b, positive if a is greater than b, zero if equal.
		struct utf8
		{
			int operator()(
				const string_iterator& a_start,
				const string_iterator& a_end,
				const string_iterator& b_start,
				const string_iterator& b_end ) const noexcept;
			static const utf8 Cmp;
		};


		// Predicate for compare two strings lexiographically while ignoring case differences.
		// Pro: UTF-comparison, ignore case, returns number of equal characters when different.
		// Con: Relatively slow.
		// Returns: Negative value if a is less than b, positive if a is greater than b, zero if equal.
		// NOTE: A custom locale can be provided if the default Eon locale currently set (for the thread) should be overridden.
		class icase_utf8
		{
		public:
			inline icase_utf8( const eon::locale* custom_locale = nullptr ) {
				Loc = custom_locale != nullptr ? custom_locale : &eon::locale::get(); }
			int operator()(
				const string_iterator& a_start,
				const string_iterator& a_end,
				const string_iterator& b_start,
				const string_iterator& b_end ) const noexcept;
			static const icase_utf8 Cmp;
		private:
			const eon::locale* Loc{ nullptr };
		};
	};
}
