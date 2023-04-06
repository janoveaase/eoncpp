#pragma once
#include "StringIterator.h"
#include "Locale.h"
#include "Compare.h"


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// A substring is a part of a larger string, but rather than containing a
	// full copy of the section from the larger string, it merely references it
	// using iterators. (See [eon::string_iterator].)
	//
	// The substring class can be used to reference [eon::string],
	// [std::string], and [const char*].
	//
	// [eon::string] relies heavily on methods implemented for substring.
	//
	// A substring can be ordered 'low-to-high' (normal) and 'high-to-low'
	// (reverse). Unless explicitly stated otherwise, methods required that the
	// substring be in 'low-to-high' ordering.
	//
	// Substrings can have some flags set. For example, if the 'icase' flag is
	// set, all comparison and searching involving the substring in question
	// will ignore case differences.
	//
	// WARNING: A substring is only valid for as long as the source string
	//          remains unchanged! Use of a substring with an invalid source
	//          may result in undefined behavior!
	//
	class substring
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		// Default construction, results in a 'false', empty substring
		substring() = default;

		// Construct as a copy of another substring
		inline substring( const substring& other ) noexcept { *this = other; }

		// Construct from a pair of [eon::string_iterator]s
		inline substring( const string_iterator& begin, const string_iterator& end ) {
			begin.assertSameSource( end ); Beg = begin, End = end; }

		// Construct an empty substring for an actual [eon::string_iterator]
		inline explicit substring( const string_iterator& itr ) noexcept { Beg = itr; End = itr; }

		// Construct for an 'std::string' (the entire string)
		inline explicit substring( const std::string& stdstr ) noexcept {
			Beg = string_iterator( stdstr ); End = Beg.getEnd(); }

		// Construct for a 'C-string' (the entire string)
		inline explicit substring( const char* cstr ) noexcept {
			Beg = string_iterator( cstr, strlen( cstr ) ); End = Beg + Beg.numSourceChars(); }

		// Default destruction
		~substring() = default;




		///////////////////////////////////////////////////////////////////////
		//
		// General Modifier Methods
		//
		// Due to the nature of UTF-8 strings in general (and since substring is
		// specifically (but not exclusively)
		//
	public:

		// Clear the substring (make it empty and 'false')
		inline void clear() noexcept { Beg.reset(); End.reset(); }




		///////////////////////////////////////////////////////////////////////
		//
		// Read-only Methods
		//
	public:

		// Check if the specified source is the same as referenced by the substring.
		// NOTE: Must be the same exact memory address!
		inline bool sameSource( const char* source ) const noexcept {
			return Beg.sameSource( source ) && End.sameSource( source ); }

		// Assert that the specified source is the exact same as referenced by the substring.
		// Throws [eon::WrongSource] if not.
		inline void assertSameSource( const char* source ) const { if( !sameSource( source ) ) throw WrongSource(); }

		// Check if the 'other' substring covers the exact same area of the
		// source string as 'this' (with same ordering).
		inline bool sameArea( const substring& other ) const noexcept { return Beg == other.Beg && End == other.End; }


		// Check if ordered 'low-to-high'.
		// NOTE: Source-less and empty substrings have no ordering!
		inline bool isLowToHigh() const noexcept { return Beg < End; }

		// Check if ordered 'high-to-low' (reverse).
		// NOTE: Source-less and empty substrings have no ordering!
		inline bool isHighToLow() const noexcept { return Beg > End; }

		// Get a copy of the substring, ensured to be low-to-high ordered.
		substring lowToHigh() const noexcept;

		// Get a copy of the substring, ensured to be high-to-low ordered.
		substring highToLow() const noexcept;


		// Check if substring is for valid 'UTF-8' iterators.
		inline bool validUTF8() const noexcept { return Beg.ValidUTF8; }


		// Check if the substring iterators have a source and are not both at the end of that source.
		inline operator bool() const noexcept { return !Beg.isVoid() && !End.isVoid() && ( Beg || End ); }

		// Check if the substring is empty.
		// NOTE: A source-less substring is by definition empty!
		inline bool empty() const noexcept { return numChars() == 0; }

		// Check if there is a source.
		// NOTE: A substring with source can still be empty!
		inline bool hasSource() const noexcept { return !Beg.isVoid() && !End.isVoid(); }

		// Get number of characters in substring.
		inline index_t numChars() const noexcept { return static_cast<index_t>( End >= Beg ? End - Beg : Beg - End ); }

		// Get number of bytes in substring.
		index_t numBytes() const noexcept;


		// Get a (new) std::string (bytes) from substring.
		// NOTE: If the substring is 'high-to-low', the std::string will be in reverse.
		inline std::string stdstr() const noexcept {
			return isHighToLow() ? _getReverse() : std::string( Beg.byteData(), numBytes() ); }

		// Read a byte inside the substring.
		// Returns zero if source-less substring or position is out of bounds!
		byte_t byte( index_t pos ) const noexcept;


		// Check if the substring is enclosed by a specific pair of characters, such as quotation marks or parenthesis.
		// NOTE: if 'end_char' is 'same_char' it will be the same as 'start_char'!
		inline bool enclosed( char_t start_char, char_t end_char = same_char ) const noexcept {
			return numChars() > 1 && *begin() == start_char && *last() == (
				end_char == same_char ? start_char : end_char ); }


		// Check if the substring is blank (is empty or contains characters from
		// the 'Separator, Space' or 'Other, Control' categories only).
		bool blank() const noexcept;


		// Given a character number, get string iterator for that position.
		// Will count from end if that is closer than start.
		// Returns 'end' iterator if invalid character number.
		// NOTE: The 'num_char' argument is relative to the substring, not the source string!
		// NOTE: If 'high-to-low' ordering, counting is reversed!
		string_iterator iterator( index_t num_char ) const noexcept;


		// Get number of indentations for the first (and possibly only) line of text in the string.
		index_t indentationLevel( char_t indentation_char = TabChr ) const noexcept;


		// Get characters as a sequential container of codepoint elements.
		// High-to-low range will give characters in reverse!
		// NOTE: Container must support 'push_back( char_t )'!
		template<typename container>
		inline container characters() const
		{
			auto output = container();
			if( isLowToHigh() )
			{
				for( auto chr : *this )
					output.push_back( chr );
			}
			else
			{
				for( auto chr = begin(); chr != end(); --chr )
					output.push_back( *chr );
			}
			return output;
		}


		// Get iterator for first character not in the 'Separator, Space' category nor in 'Other, Control'.


		// Get a substring narrowed in front and end, bypassing all characters
		// in the 'Separator, Space' and 'Other, Control' categories.
		// NOTE: Ordering is ignored (always performed in 'low-to-high')!
		substring trim() const;

		// Get a substring narrowed in front, bypassing all characters in the
		// 'Separator, Space' and 'Other, Control' categories.
		// NOTE: Ordering is ignored (always performed in 'low-to-high')!
		substring trimLeading() const;

		// Get a substring narrowed at end, bypassing all characters in the
		// 'Separator, Space' and 'Other, Control' categories.
		// NOTE: Ordering is ignored (always performed in 'low-to-high')!
		substring trimTrailing() const;




		///////////////////////////////////////////////////////////////////////
		//
		// Strings as number
		//
	public:

		// Check if the substring contains only characters in the 'Number, Decimal Digit' category.
		bool numeralsOnly() const noexcept;

		// Check if the substring contains only ASCII digits. (Slightly cheaper than [numeralsOnly].)
		bool isUInt() const noexcept;

		// Check if substring starts with a plus or minus sign that is followed by numerals only!
		inline bool isSignedInteger() const noexcept {
			return numChars() > 1 && ( isPlus( *begin() ) || isMinus( *begin() ) )
				&& substring( begin() + 1, end() ).numeralsOnly(); }

		// Check if the substring starts with ASCII '+' or '-' that is followed by ASCII digits only!
		// (Slightly cheaper than [isSignedInteger].)
		inline bool isInt() const noexcept {
			return numChars() > 1 && ( *begin() == '+' || *begin() == '-' )
				&& substring( begin() + 1, end() ).isUInt(); }

		// Check if substring optionally starts with (any) plus or (any) minus sign and
		// only (any) numerals with a single decimal separator after that.
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		bool isFloatingPoint( const locale* custom_locale = nullptr ) const noexcept;

		// Check if substring optionally starts with '+' or '-' and contains
		// only ASCII digits and a single decimal separator after that.
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		// NOTE: Decimal separator may occur before the first numeral or after the last!
		// (Slightly cheaper than [isFloatingPoint].)
		bool isFloat( const locale* custom_locale = nullptr ) const noexcept;


		// Convert unsigned or signed integer string into long_t (signed 64-bit integer).
		// Assumes [numeralsOnly] or [isSignedInteger] is true. (Will not check!)
		long_t toLongT() const noexcept;

		// Convert unsigned or signed integer string into int_t (signed 32-bit integer).
		// Assumes [numeralsOnly] or [isSignedInteger] is true. (Will not check!)
		inline int_t toIntT() const noexcept { return static_cast<int_t>( toLongT() ); }

		// Convert unsigned or signed integer string into short_t (signed 16-bit integer).
		// Assumes [numeralsOnly] or [isSignedInteger] is true. (Will not check!)
		inline int_t toShortT() const noexcept { return static_cast<short_t>( toLongT() ); }

		// Convert unsigned or signed floating point string into high_t (64- or 96- or 128-bit float).
		// Assumes [numeralsOnly] or [isFloatingPoint] is true. (Will not check!)
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		high_t toHighT( const locale* custom_locale = nullptr ) const noexcept;

		// Convert unsigned or signed floating point string into flt_t (64-bit float).
		// Assumes [numeralsOnly] or [isFloatingPoint] is true. (Will not check!)
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		inline high_t toFltT( const locale* custom_locale = nullptr ) const noexcept {
			return static_cast<flt_t>( toHighT( custom_locale ) ); }

		// Convert unsigned or signed floating point string into low_t (32-bit float).
		// Assumes [numeralsOnly] or [isFloatingPoint] is true. (Will not check!)
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		inline low_t toLowT( const locale* custom_locale = nullptr ) const noexcept {
			return static_cast<low_t>( toHighT( custom_locale ) ); }

		// Convert unsigned integer string into index_t.
		// Assumes [numeralsOnly] or [isUInt] is true. (Will not check!)
		inline index_t toIndex() const noexcept { return static_cast<index_t>( toUInt64() ); }


		// Convert to int32_t by casting from int_t.
		inline int32_t toInt32() const noexcept { return static_cast<int32_t>( toIntT() ); }

		// Convert to int64_t by casting from long_t.
		inline int64_t toInt64() const noexcept { return static_cast<int64_t>( toLongT() ); }

		// Convert to uint64_t.
		// Assumes [numeralsOnly] or [isUInt] is true. (Will not check!)
		uint64_t toUInt64() const noexcept;

		// Convert to double by casting from flt_t.
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		inline double toDouble( const locale* custom_locale = nullptr ) const {
			return static_cast<double>( toFltT( custom_locale ) ); }

		// Convert to long double by casting from high_t.
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		long double toLongDouble( const locale* custom_locale = nullptr ) const {
			return static_cast<long double>( toHighT( custom_locale ) ); }

		// Convert to uint32_t by casting from index_t.
		inline uint32_t toUInt32() const { return static_cast<uint32_t>( toIndex() ); }

		// Convert to size_t by casting from index_t.
		inline size_t toSize() const { return static_cast<size_t>( toIndex() ); }


		// Assuming the entire substring is a number, get a reduced substring
		// where leading integer zeros and trailing fractional zeros are
		// removed - including the decimal separator if all fractional zeros.
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		// NOTE: If the number ends in separator, the separator will be excluded!
		// NOTE: If the number is signed, leading integer zeros will be included to keep the sign!
		// Example: 0001.0000 -> 1
		// Example: -001.0000 -> -001
		substring trimNumber( const locale* custom_locale = nullptr ) const;

		// Assuming the entire substring is a number, get a reduced substring
		// where leading integer zeros and trailing fractional zeros are
		// removed - leave the decimal separator and one fractional zero if
		// all fractional zeros. (If the number ends with the separator, it
		// will be included.)
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		// NOTE: If the number is signed, leading integer zeros will be
		//       included to keep the sign.
		// Example: 0001.0000 -> 1.0
		substring trimFloat( const locale* custom_locale = nullptr ) const;




		///////////////////////////////////////////////////////////////////////
		//
		// String hashing
		//
	public:

		// TODO: Use locale!

		// FNV-1a hash algorithm used for producing hash values from the
		// source string that is covered by the substring.
#define FNV_PRIME32 16777619
#define FNV_PRIME64 1099511628211LLU
#define FNV_OFFSET32 2166136261
#define FNV_OFFSET64 14695981039346656037LLU
		inline uint32_t hash32() const noexcept { return hash32( Beg.byteData(), End.byteData() ); }
		inline uint64_t hash64() const noexcept { return hash64( Beg.byteData(), End.byteData() ); }
#if defined(_WIN64) || defined(__x86_64__)
		inline size_t hash() const noexcept { return hash64( Beg.byteData(), End.byteData() ); }
#else
		inline size_t hash() const noexcept { return hash32( Beg.byteData(), End.byteData() ); }
#endif

		// Static hash method for raw bytes
		static inline uint32_t hash32( const char* begin, const char* end, uint32_t h = FNV_OFFSET32 ) noexcept {
			for( auto c = begin; c != end; ++c ) { h ^= static_cast<unsigned char>( *c ); h *= FNV_PRIME32; } return h; }
		static inline uint64_t hash64( const char* begin, const char* end, uint64_t h = FNV_OFFSET64 ) noexcept {
			for( auto c = begin; c != end; ++c ) { h ^= static_cast<unsigned char>( *c ); h *= FNV_PRIME64; } return h; }




		///////////////////////////////////////////////////////////////////////
		//
		// Iteration
		//
	public:

		// Get iterator for the start of the substring
		inline string_iterator& begin() noexcept { return Beg; }
		inline const string_iterator& begin() const noexcept { return Beg; }
		inline const string_iterator& cbegin() const noexcept { return Beg; }

		// Get iterator for the end of the substring
		inline string_iterator& end() noexcept { return End; }
		inline const string_iterator& end() const noexcept { return End; }
		inline const string_iterator& cend() const noexcept { return End; }

		// Get iterator for the last character in the substring
		// Returns 'end()' if 'high-to-low' ordering or empty substring.
		inline const string_iterator last() const noexcept { return Beg < End ? End - 1 : End; }




		///////////////////////////////////////////////////////////////////////
		//
		// Splitting into multiple substrings
		//
	public:

		// Split substring on every occurrence of 'delimiter' substring into a
		// 'sequential container' (typically list<substring> or
		// vector<substring>) - requires the 'push_back' method!
		// If a max number of elements is specified, the last element will
		// reference the unsplit remainder.
		template<typename container_t>
		container_t splitSequential( const substring& delimiter, index_t max_elements = INDEX_MAX ) const
		{
			auto elms = container_t();
			string_iterator start = begin();
			bool got_trailing_element = false;
			for( auto found = findFirst( delimiter );
				found || elms.size() == max_elements - 1;
				found = substring( start, end() ).findFirst( delimiter ) )
			{
				elms.push_back( substring( start, found.begin() ) );
				start = found.end();
				got_trailing_element = true;
			}
			if( start )
				elms.push_back( substring( start, end() ) );
			else if( got_trailing_element )
				elms.push_back( substring( end(), end() ) );
			return elms;
		}

		// Split substring on every occurrence of 'delimiter' character into a
		// 'sequential container' (typically list<substring> or
		// vector<substring>) - requires the 'push_back' method!
		// If a max number of elements is specified, the last element will
		// reference the unsplit remainder.
		template<typename container_t>
		container_t splitSequential( char_t delimiter, index_t max_elements = INDEX_MAX ) const
		{
			auto elms = container_t();
			string_iterator start = begin();
			bool got_trailing_element = false;
			for( auto found = findFirst( delimiter );
				found || elms.size() == max_elements - 1;
				found = substring( start, end() ).findFirst( delimiter ) )
			{
				elms.push_back( substring( start, found.begin() ) );
				start = found.end();
				got_trailing_element = true;
			}
			if( start )
				elms.push_back( substring( start, end() ) );
			else if( got_trailing_element )
				elms.push_back( substring( end(), end() ) );
			return elms;
		}

		// Split substring on every occurrence of 'delimiter' substring into a
		// 'non-sequential container' (typically set<substring>,
		// multiset<substring>, unordered_set<substring>, or
		// unordered_multiset<substring>) - requires the insert method!
		template<typename container_t>
		container_t splitNonSequential( const substring& delimiter ) const
		{
			auto elms = container_t();
			string_iterator start = begin();
			bool got_trailing_element = false;
			for( auto found = findFirst( delimiter );
				found;
				found = substring( start, end() ).findFirst( delimiter ) )
			{
				elms.insert( substring( start, found.begin() ) );
				start = found.begin() + delimiter.numChars();
				got_trailing_element = true;
			}
			if( start )
				elms.insert( substring( start, end() ) );
			else if( got_trailing_element )
				elms.insert( substring( end(), end() ) );
			return elms;
		}

		// Split substring on every occurrence of 'delimiter' character into a
		// 'non-sequential container' (typically set<substring>,
		// multiset<substring>, unordered_set<substring>, or
		// unordered_multiset<substring>) - requires the insert method!
		template<typename container_t>
		container_t splitNonSequential( char_t delimiter ) const
		{
			auto elms = container_t();
			string_iterator start = begin();
			bool got_trailing_element = false;
			for( auto found = findFirst( delimiter );
				found;
				found = substring( start, end() ).findFirst( delimiter ) )
			{
				elms.insert( substring( start, found.begin() ) );
				start = found.begin() + 1;
				got_trailing_element = true;
			}
			if( start )
				elms.insert( substring( start, end() ) );
			else if( got_trailing_element )
				elms.insert( substring( end(), end() ) );
			return elms;
		}




		///////////////////////////////////////////////////////////////////////
		//
		// Comparing
		//
		// All compare methods require all substrings to be 'low-to-high'!
		//
		// The C++ std::locale (std::collate) is not suited for comparing UTF-8
		// strings and therefore not suited for Eon strings - which are pre-
		// dominantly UTF-8. Instead, comparing is done using binary compare
		// predicates that users requiring custom sorting can provide, while
		// default predicates are used otherwise.
		//
		// NOTE: All compare methods work on UTF-8 characters and none involved
		//       direct byte comparison!
		//
	public:

		// Check if substring starts with a specific substring 'value'.
		// If the 'value' is empty, the return value will always be 'false'!
		// Comparison is done using a binary compare predicate.
		template<typename compare_predicate = strcmp::utf8>
		inline bool startsWith( const substring& value, const compare_predicate& cmp = strcmp::utf8::Cmp ) const noexcept {
			return value.empty() || numChars() < value.numChars() ? false
				: cmp( begin(), begin() + value.numChars(), value.begin(), value.end() ) == 0; }

		// Check if substring starts with a specific character.
		// Comparison is done using a binary compare predicate.
		template<typename compare_predicate = strcmp::chr>
		inline bool startsWith( char_t value, const compare_predicate& cmp = strcmp::chr::Cmp ) const noexcept {
			return cmp( *begin(), value ) == 0; }

		// Check if substring ends with a specific substring.
		// If the 'value' is empty, the return value will always be 'false'!
		// Comparison is done using a binary compare predicate.
		template<typename compare_predicate = strcmp::utf8>
		inline bool endsWith( const substring& value, const compare_predicate& cmp = strcmp::utf8::Cmp ) const noexcept {
			return value.empty() || numChars() < value.numChars() ? false
				: cmp( end() - value.numChars(), end(), value.begin(), value.end() ) == 0; }

		// Check if substring ends with a specific character.
		// Comparison is done using a binary compare predicate for single character.
		template<typename compare_predicate = strcmp::chr>
		inline bool endsWith( char_t value, const compare_predicate& cmp = strcmp::chr::Cmp ) const noexcept {
			return cmp( *last(), value ) == 0; }


		// UTF-8 characater comparison.
		// Compares using string_iterator objects.
		// NOTE: Will compare UTF-8 characters, can be locale aware (depending on compare predicate).
		// Returns: Zero if equal or number of equal characters from start if different,
		//          negative if 'this' is less than 'other'.
		// Template argument:
		//   A compare predicate accepting four iterators (a_start, a_end, b_start, b_end).
		// Default predicates are:
		//   - [eon::strcmp::utf8]: Will not ignore case.
		//   - [eon::strcmp::icase_utf8]: Will ignore case.
		// NOTE: To compare strings differently, implement and use a custom compare predicate matching:
		//       int operator()(
		//           const string_iterator& a_start,
		//           const string_iterator& a_end,
		//           const string_iterator& b_start,
		//           const string_iterator& b_end ) const noexcept;
		template<typename compare_predicate = strcmp::utf8>
		inline int compare( const substring& other, const compare_predicate& cmp = strcmp::utf8::Cmp ) const noexcept {
			return cmp( begin(), end(), other.begin(), other.end() ); }

		// Check if 'this' substring sorts before 'other' using [eon::strcmp::utf8].
		inline bool operator<( const substring& other ) const noexcept { return compare( other, strcmp::utf8::Cmp ) < 0; }

		// Check if 'this' substring sorts before or same as 'other' using [eon::strcmp::utf8].
		inline bool operator<=( const substring& other ) const noexcept { return compare( other, strcmp::utf8::Cmp ) <= 0; }

		// Check if 'this' substring sorts after 'other' using [eon::strcmp::utf8].
		inline bool operator>( const substring& other ) const noexcept { return compare( other, strcmp::utf8::Cmp ) > 0; }

		// Check if 'this' substring sorts after or same as 'other' using [eon::strcmp::utf8].
		inline bool operator>=( const substring& other ) const noexcept { return compare( other, strcmp::utf8::Cmp ) >= 0; }

		// Check if 'this' substring sorts same as 'other' using [eon::strcmp::utf8].
		inline bool operator==( const substring& other ) const noexcept { return compare( other, strcmp::utf8::Cmp ) == 0; }

		// Check if 'this' substring sorts before or after 'other' using [eon::strcmp::utf8].
		inline bool operator!=( const substring& other ) const noexcept { return compare( other, strcmp::utf8::Cmp ) != 0; }


		// Check if 'this' substring sorts before std::string 'other' using [eon::strcmp::utf8].
		inline bool operator<( const std::string& other ) const noexcept { return compare( substring( other ) ) < 0; }

		// Check if 'this' substring sorts before or same as std::string 'other' using [eon::strcmp::utf8].
		inline bool operator<=( const std::string& other ) const noexcept { return compare( substring( other ) ) <= 0; }

		// Check if 'this' substring sorts after std::string 'other' using [eon::strcmp::utf8].
		inline bool operator>( const std::string& other ) const noexcept { return compare( substring( other ) ) > 0; }

		// Check if 'this' substring sorts after or same as std::string 'other' using [eon::strcmp::utf8].
		inline bool operator>=( const std::string& other ) const noexcept { return compare( substring( other ) ) >= 0; }

		// Check if 'this' substring sorts same as std::string 'other' using [eon::strcmp::utf8].
		inline bool operator==( const std::string& other ) const noexcept { return compare( substring( other ) ) == 0; }

		// Check if 'this' substring sorts before or after std::string 'other' using [eon::strcmp::utf8].
		inline bool operator!=( const std::string& other ) const noexcept { return compare( substring( other ) ) != 0; }

		inline friend bool operator<( const std::string& a, const substring& b ) noexcept {
			return b.compare( substring( a ) ) > 0; }
		inline friend bool operator<=( const std::string& a, const substring& b ) noexcept {
			return b.compare( substring( a ) ) >= 0; }
		inline friend bool operator>( const std::string& a, const substring& b ) noexcept {
			return b.compare( substring( a ) ) < 0; }
		inline friend bool operator>=( const std::string& a, const substring& b ) noexcept {
			return b.compare( substring( a ) ) <= 0; }
		inline friend bool operator==( const std::string& a, const substring& b ) noexcept {
			return b.compare( substring( a ) ) == 0; }
		inline friend bool operator!=( const std::string& a, const substring& b ) noexcept {
			return b.compare( substring( a ) ) != 0; }


		// Check if 'this' substring sorts before C-string 'other' using [eon::strcmp::utf8].
		inline bool operator<( const char* other ) const noexcept { return compare( substring( other ) ) < 0; }

		// Check if 'this' substring sorts before or same as C-string 'other' using [eon::strcmp::utf8].
		inline bool operator<=( const char* other ) const noexcept { return compare( substring( other ) ) <= 0; }

		// Check if 'this' substring sorts after C-string 'other' using [eon::strcmp::utf8].
		inline bool operator>( const char* other ) const noexcept { return compare( substring( other ) ) > 0; }

		// Check if 'this' substring sorts after or same as C-string 'other' using [eon::strcmp::utf8].
		inline bool operator>=( const char* other ) const noexcept { return compare( substring( other ) ) >= 0; }

		// Check if 'this' substring sorts same as C-string 'other' using [eon::strcmp::utf8].
		inline bool operator==( const char* other ) const noexcept { return compare( substring( other ) ) == 0; }

		// Check if 'this' substring sorts before or after C-string 'other' using [eon::strcmp::utf8].
		inline bool operator!=( const char* other ) const noexcept { return compare( substring( other ) ) != 0; }

		inline friend bool operator<( const char* a, const substring& b ) noexcept {
			return b.compare( substring( a ) ) > 0; }
		inline friend bool operator<=( const char* a, const substring& b ) noexcept {
			return b.compare( substring( a ) ) >= 0; }
		inline friend bool operator>( const char* a, const substring& b ) noexcept {
			return b.compare( substring( a ) ) < 0; }
		inline friend bool operator>=( const char* a, const substring& b ) noexcept {
			return b.compare( substring( a ) ) <= 0; }
		inline friend bool operator==( const char* a, const substring& b ) noexcept {
			return b.compare( substring( a ) ) == 0; }
		inline friend bool operator!=( const char* a, const substring& b ) noexcept {
			return b.compare( substring( a ) ) != 0; }


		// Do partial comparison from start of substrings.
		// Assumes both 'this' and 'other' are 'low-to-high'!
		// Comparison is done using a binary compare predicate for single character.
		template<typename compare_predicate = strcmp::chr>
		int compareSub( const substring& other, index_t num_chars, const compare_predicate& cmp = strcmp::chr::Cmp ) const noexcept
		{
			if( !*this )
				return other ? -1 : 0;
			else if( !other )
				return 1;

			string_iterator i = Beg, other_i = other.Beg;
			int pos{ 1 }, cmp_val{ 0 };
			for( ; i != End && other_i != other.end() && static_cast<index_t>( pos ) <= num_chars; ++i, ++other_i, ++pos )
			{
				cmp_val = cmp( *i, *other_i );
				if( cmp_val != 0 )
					return cmp_val < 0 ? -pos : pos;
			}
			return static_cast<index_t>( pos - 1 ) == num_chars ? 0 : i != End ? pos : -pos;
		}


		// Compare by iterator positions, rather than characters.
		// Returns -1 if 'this' starts before 'other' or same start but ends before.
		// Returns 0 if start and end positions are identical.
		// Returns 1 if 'this' starts after 'other' of same start but ends after.
		inline int comparePos( const substring& other ) const noexcept {
			return Beg < other.Beg ? -1 : other.Beg < Beg ? 1 : End < other.End ? -1 : other.End < End ? 1 : 0; }




		///////////////////////////////////////////////////////////////////////
		//
		// Searching
		//
	public:

		// Check if substring contains the specified substring.
		// Comparison is done using a binary compare predicate for single character.
		// NOTE: ASCII-only optimization is only done if strcmp::byte::Cmp is used!
		template<typename compare_predicate = strcmp::byte>
		inline bool contains( const substring& sub, const compare_predicate& cmp = strcmp::byte::Cmp ) const noexcept {
			return static_cast<bool>( findFirst( sub, cmp ) ); }

		// Check if substring contains the specified codepoint.
		// Comparison is done using a binary compare predicate for single character.
		// NOTE: ASCII-only optimization is only done if strcmp::byte::Cmp is used!
		template<typename compare_predicate = strcmp::byte>
		inline bool contains( char_t codepoint, const compare_predicate& cmp = strcmp::byte::Cmp ) const noexcept {
			return static_cast<bool>( findFirst( codepoint, cmp ) ); }

		// Check if substring contains any of the characters in the specified substring.
		// Comparison is done using a binary compare predicate for single character.
		// NOTE: ASCII-only optimization is only done if strcmp::byte::Cmp is used!
		template<typename compare_predicate = strcmp::byte>
		inline bool containsAnyOf( const substring& sub, const compare_predicate& cmp = strcmp::byte::Cmp ) const noexcept {
			return static_cast<bool>( findFirstOf( sub, cmp ) ); }

		// Check if substring contains none of the 'characters' in the specified substring.
		// Comparison is done using a binary compare predicate for single character.
		// NOTE: ASCII-only optimization is only done if strcmp::byte::Cmp is used!
		// NOTE: If 'chararcters' is empty the return value will always be 'true'!
		template<typename compare_predicate = strcmp::byte>
		inline bool containsNoneOf( const substring& chararcters, const compare_predicate& cmp = strcmp::byte::Cmp ) const noexcept {
			return !static_cast<bool>( findFirstOf( chararcters, cmp ) ); }


		// Find first occurrence of 'to_find' substring in 'this'.
		// NOTE: Search is done in 'low-to-high' ordering regardless of current settings!
		// Returns the found substring ('low-to-high' ordering) within 'this' - ''false' substring if not found.
		// Comparison is done using a binary compare predicate for single character.
		// NOTE: ASCII-only optimization is only done if strcmp::byte::Cmp is used!
		template<typename compare_predicate = strcmp::byte>
		substring findFirst( const substring& to_find, const compare_predicate& cmp = strcmp::byte::Cmp ) const noexcept
		{
			if( empty() || to_find.empty() )
				return substring( End.getEnd() );
			else if( isHighToLow() )
				return lowToHigh().findFirst( to_find.isHighToLow() ? to_find.lowToHigh() : to_find, cmp );
			else if( to_find.isHighToLow() )
				return findFirst( to_find.lowToHigh(), cmp );
			if( Beg.bytesOnly() && typeid( cmp ) == typeid( strcmp::byte ) )
				return _optimizedFindFirst( to_find );
			for( string_iterator i = begin(); i != end(); ++i )
			{
				string_iterator i_beg = i, j = to_find.begin();
				for( ; j != to_find.end() && i != end() && cmp( *i, *j ) == 0; ++j, ++i )
					;
				if( j == to_find.end() )
					return substring( i_beg, i );
				else if( i != i_beg )
					i = i_beg;
			}
			return substring( End.getEnd() );
		}

		// Find first occurrence of 'to_find' codepoint in 'this'.
		// NOTE: Search is done in 'low-to-high' ordering regardless of current settings!
		// Returns the found codepoint as a substring - 'false' substring if not found.
		// Comparison is done using a binary compare predicate for single character.
		// NOTE: ASCII-only optimization is only done if strcmp::byte::Cmp is used!
		template<typename compare_predicate = strcmp::byte>
		substring findFirst( char_t to_find, const compare_predicate& cmp = strcmp::byte::Cmp ) const noexcept
		{
			if( empty() )
				return substring( End.getEnd() );
			else if( isHighToLow() )
				return lowToHigh().findFirst( to_find, cmp );
			if( Beg.bytesOnly() && typeid( cmp ) == typeid( strcmp::byte ) )
				return _optimizedFindFirst( to_find );
			for( string_iterator i = begin(); i != end(); ++i )
			{
				if( cmp( *i, to_find ) == 0 )
					return substring( i, i + 1 );
			}
			return substring( End.getEnd() );
		}


		// Find the first number within the substring.
		// NOTE: Search is done in 'low-to-high' ordering regardless of current settings!
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		// Returns a substring wrapping only the found number, 'false' if not found.
		// If the 'separator' iterator argument is specified, it will contain
		// the position of the decimal separator - if it exists.
		// NOTE: If the decimal separator appears before the first numeral, it will not be counted as part of the number!
		substring findFirstNumber( string_iterator* separator = nullptr, const locale* custom_locale = nullptr );


		// Find last occurrence of 'to_find' substring in 'this'.
		// NOTE: Search is done in 'high-to-low' ordering regardless of current settings!
		// Returns the found substring ('low-to-high' ordering) - 'false' substring if not found.
		// Comparison is done using a binary compare predicate for single character.
		// NOTE: ASCII-only optimization is only done if strcmp::byte::Cmp is used!
		template<typename compare_predicate = strcmp::byte>
		substring findLast( const substring& to_find, const compare_predicate& cmp = strcmp::byte::Cmp ) const noexcept
		{
			if( empty() || to_find.empty() )
				return substring( End.getEnd() );
			else if( isLowToHigh() )
				return highToLow().findLast( to_find.isLowToHigh() ? to_find.highToLow() : to_find, cmp );
			else if( to_find.isLowToHigh() )
				return findLast( to_find.highToLow(), cmp );
			if( Beg.bytesOnly() && typeid( cmp ) == typeid( strcmp::byte ) )
				return _optimizedFindLast( to_find );
			for( string_iterator pos = begin(); pos != end(); --pos )
			{
				string_iterator i = pos, j = to_find.begin();
				for( ; j != to_find.end() && i != end() && cmp( *i, *j ) == 0; --j, --i )
					;
				if( j.atREnd() )
				{
					if( i.atREnd() )
						int g = 0;
					return substring( pos, i ).lowToHigh();
				}
			}
			return substring( End.getEnd() );
		}

		// Find last occurrence of 'to_find' codepoint in 'this'.
		// NOTE: Search is done in 'low-to-high' ordering regardless of current settings!
		// Returns the found codepoint as a substring ('low-to-high' ordering) - 'false' substring if not found.
		// Comparison is done using a binary compare predicate for single character.
		// NOTE: ASCII-only optimization is only done if strcmp::byte::Cmp is used!
		template<typename compare_predicate = strcmp::byte>
		substring findLast( char_t to_find, const compare_predicate& cmp = strcmp::byte::Cmp ) const noexcept
		{
			if( empty() )
				return substring( End.getEnd() );
			else if( isLowToHigh() )
				return highToLow().findLast( to_find, cmp );
			if( Beg.bytesOnly() && typeid( cmp ) == typeid( strcmp::byte ) )
				return _optimizedFindLast( to_find );
			for( string_iterator i = Beg; i != End; --i )
			{
				if( cmp( *i, to_find ) == 0 )
					return substring( i, i + 1 );
			}
			return substring( End.getEnd() );
		}


		// Find first occurrence of any of the specified 'characters'.
		// NOTE: Search is done in 'low-to-high' ordering regardless of current settings!
		// Returns iterator for the found character - 'false' iterator if not found.
		// Comparison is done using a binary compare predicate for single character.
		template<typename compare_predicate = strcmp::chr>
		string_iterator findFirstOf(
			const substring& characters, const compare_predicate& cmp = strcmp::chr::Cmp ) const noexcept
		{
			if( empty() || characters.empty() )
				return string_iterator( End.getEnd() );
			else if( isHighToLow() )
				return lowToHigh().findFirstOf( characters.isHighToLow() ? characters.lowToHigh() : characters, cmp );
			else if( characters.isHighToLow() )
				return findFirstOf( characters.lowToHigh(), cmp );
			for( string_iterator i = begin(); i != end(); ++i )
			{
				if( characters.contains( *i, cmp ) )
					return i;
			}
			return End.getEnd();
		}

		// Find first occurrence of any character in the specified character class(es).
		// NOTE: Search is done in 'low-to-high' ordering regardless of current settings!
		// NOTE: Multiple classes can be or'ed together!
		// Returns iterator for the found character - 'false' iterator if not found.
		string_iterator findFirstOf( charcat category ) const noexcept;


		// Find last occurrence of any of the specified 'characters'.
		// NOTE: Search is done in 'high-to-low' ordering regardless of current settings!
		// Returns iterator for the found character - 'false' iterator if not found.
		// Comparison is done using a binary compare predicate for single character.
		template<typename compare_predicate = strcmp::chr>
		string_iterator findLastOf(
			const substring& characters, const compare_predicate& cmp = strcmp::chr::Cmp ) const noexcept
		{
			if( empty() || characters.empty() )
				return End.getEnd();
			else if( isLowToHigh() )
				return highToLow().findLastOf( characters.isHighToLow() ? characters.lowToHigh() : characters, cmp );
			else if( characters.isHighToLow() )
				return findLastOf( characters.lowToHigh(), cmp );
			for( string_iterator i = begin(); i != end(); --i )
			{
				if( characters.contains( *i, cmp ) )
					return i;
			}
			return End.getEnd();
		}

		// Find last occurrence of any character in the specified character class(es).
		// NOTE: Search is done in 'high-to-low' ordering regardless of current settings!
		// NOTE: Multiple classes can be or'ed together!
		// Returns iterator for the found character - 'false' iterator if not found.
		string_iterator findLastOf( charcat category ) const noexcept;


		// Find first occurrence of any character not among the specified 'characters'.
		// NOTE: Search is done in 'low-to-high' ordering regardless of current settings!
		// Returns iterator for the found character - 'false' iterator if not found.
		// Comparison is done using a binary compare predicate for single character.
		template<typename compare_predicate = strcmp::chr>
		string_iterator findFirstNotOf(
			const substring& characters, const compare_predicate& cmp = strcmp::chr::Cmp ) const noexcept
		{
			if( empty() || characters.empty() )
				return End.getEnd();
			else if( isHighToLow() )
				return lowToHigh().findFirstNotOf( characters.isHighToLow() ? characters.lowToHigh() : characters, cmp );
			else if( characters.isHighToLow() )
				return findFirstNotOf( characters.lowToHigh(), cmp );
			for( string_iterator i = begin(); i != end(); ++i )
			{
				if( !characters.contains( *i, cmp ) )
					return i;
			}
			return End.getEnd();
		}

		// Find first occurrence of any character not in the specified character class(es).
		// NOTE: Search is done in 'low-to-high' ordering regardless of current settings!
		// NOTE: Multiple classes can be or'ed together!
		// Returns iterator for the found character - 'false' iterator if not found.
		string_iterator findFirstNotOf( charcat category ) const noexcept;


		// Find last occurrence of any character not among the specified 'characters'.
		// NOTE: Search is done in 'high-to-low' ordering regardless of current settings!
		// Returns iterator for the found character - 'false' iterator if not found.
		// Comparison is done using a binary compare predicate for single character.
		template<typename compare_predicate = strcmp::chr>
		string_iterator findLastNotOf( const substring& characters, const compare_predicate& cmp = strcmp::chr::Cmp ) const noexcept
		{
			if( empty() || characters.empty() )
				return End.getEnd();
			else if( isLowToHigh() )
				return highToLow().findLastNotOf( characters.isLowToHigh() ? characters.highToLow() : characters, cmp );
			else if( characters.isLowToHigh() )
				return findLastNotOf( characters.highToLow(), cmp );
			for( string_iterator i = begin(); i != end(); --i )
			{
				if( !characters.contains( *i, cmp ) )
					return i;
			}
			return End.getEnd();
		}

		// Find last occurrence of any character not in the specified character class(es).
		// NOTE: Search is done in 'high-to-low' ordering regardless of current settings!
		// NOTE: Multiple classes can be or'ed together!
		// Returns iterator for the found character - 'false' iterator if not found.
		string_iterator findLastNotOf( charcat category ) const noexcept;


		// Find the first character in 'this' substring that differs from 'other',
		// 'false' iterator if none (equal substrings).
		// NOTE: Search is done in 'low-to-high' ordering regardless of current settings!
		// Comparison is done using a binary compare predicate for single character.
		template<typename compare_predicate = strcmp::chr>
		string_iterator findFirstDiff( const substring& other, const compare_predicate& cmp = strcmp::chr::Cmp ) const noexcept
		{
			if( empty() || other.empty() )
				return End.getEnd();
			else if( isHighToLow() )
				return lowToHigh().findFirstDiff( other.isHighToLow() ? other.lowToHigh() : other, cmp );
			else if( other.isHighToLow() )
				return findFirstDiff( other.lowToHigh(), cmp );
			string_iterator i = begin(), j = other.begin();
			for( ; i != end() && j != other.end(); ++i, ++j )
			{
				if( cmp( *i, *j ) != 0 )
					return i;
			}
			if( i != end() )
				return i;
			else
				return End.getEnd();
		}


		// Similar to [findFirst] for substring argument but sections enclosed
		// by matching 'start_sect' and 'end_sect' characters will be skipped.
		// For example, quoted sections, parenthesized sections, etc.
		// NOTE: If 'start_sect' is different from 'end_sect', nested occurrences will be accounted for!
		// NOTE: If 'start_sect' is identical to 'end_sect', escaped versions of the character within or
		//       outside an enclosed section will not be interpreted as neither 'start_sect' nor 'end_sect'!
		// NOTE: Search is done in 'low-to-high' ordering regardless of current settings!
		// NOTE: if 'end_sect' is 'same_char', it will be the same as 'start_sect'!
		// Comparison is done using a binary substring predicate.
		template<typename compare_predicate = strcmp::chr>
		substring findFirstIgnoreSections(
			const substring& other,
			char_t start_sect,
			char_t end_sect = same_char,
			const compare_predicate& cmp = strcmp::chr::Cmp ) const noexcept
		{
			if( empty() || other.empty() )
				return substring( End.getEnd() );
			else if( isHighToLow() )
				return lowToHigh().findFirstIgnoreSections( other.isHighToLow() ? other.lowToHigh() : other, start_sect, end_sect, cmp );
			else if( other.isHighToLow() )
				return findFirstIgnoreSections( other.lowToHigh(), start_sect, end_sect, cmp );
			end_sect = end_sect == same_char ? start_sect : end_sect;
			bool escaped = false;
			for( string_iterator i = begin(); i < end(); ++i )
			{
				if( escaped )
					escaped = false;
				else
				{
					if( start_sect == end_sect && *i == BackSlashChr )
						escaped = true;
					else if( *i == start_sect )
						_bypassSection( start_sect, end_sect, i );
					else
					{
						string_iterator j = other.begin(), k = i;
						for( ; j != other.end() && k != end() && cmp( *k, *j ) == 0; ++j, ++k )
							;
						if( j == other.end() )
							return substring( i, k );
					}
				}
			}
			return substring( End.getEnd() );
		}

		// Similar to [findFirst] for character argument but sections enclosed
		// by matching 'start_sect' and 'end_sect' characters will be skipped.
		// For example, quoted sections, parenthesized sections, etc.
		// NOTE: If 'start_sect' is different from 'end_sect', nested occurrences will be accounted for!
		// NOTE: If 'start_sect' is identical to 'end_sect', escaped versions of the character within or
		//       outside an enclosed section will not be interpreted as neither 'start_sect' nor 'end_sect'!
		// NOTE: Search is done in 'low-to-high' ordering regardless of current settings!
		// NOTE: if 'end_sect' is 'same_char', it will be the same as 'start_sect'!
		// Comparison is done using a binary compare predicate for single character.
		template<typename compare_predicate = strcmp::chr>
		substring findFirstIgnoreSections(
			char_t codepoint,
			char_t start_sect,
			char_t end_sect = same_char,
			const compare_predicate& cmp = strcmp::chr::Cmp ) const noexcept
		{
			if( empty() )
				return substring( End.getEnd() );
			else if( isHighToLow() )
				return lowToHigh().findFirstIgnoreSections( codepoint, start_sect, end_sect, cmp );
			end_sect = end_sect == same_char ? start_sect : end_sect;
			bool escaped = false;
			for( string_iterator i = begin(); i != end(); ++i )
			{
				if( escaped )
					escaped = false;
				else
				{
					if( start_sect == end_sect && *i == BackSlashChr )
						escaped = true;
					else if( *i == start_sect )
						_bypassSection( start_sect, end_sect, i );
					else if( cmp( *i, codepoint ) == 0 )
						return substring( i, i + 1 );
				}
			}
			return substring( End.getEnd() );
		}


		// Get as new substring everything inside 'this' substring that appears
		// before the first occurrence of 'delimiter' substring.
		// Comparison is done using a binary compare predicate for single character.
		// NOTE: Search is done in 'low-to-high' ordering regardless of current settings!
		// NOTE: ASCII-only optimization is only done if strcmp::byte::Cmp is used!
		template<typename compare_predicate = strcmp::byte>
		substring beforeFirst( const substring& delimiter, const compare_predicate& cmp = strcmp::byte::Cmp ) const noexcept
		{
			auto found = findFirst( delimiter, cmp );
			if( found )
				return substring( begin(), found.begin() );
			else
				return substring( End.getEnd() );
		}

		// Get as new substring everything inside 'this' substring that appears
		// before the first occurrence of 'delimiter' character.
		// Comparison is done using a binary compare predicate for single character.
		// NOTE: Search is done in 'low-to-high' ordering regardless of current settings!
		// NOTE: ASCII-only optimization is only done if strcmp::byte::Cmp is used!
		template<typename compare_predicate = strcmp::byte>
		substring beforeFirst( char_t delimiter, const compare_predicate& cmp = strcmp::byte::Cmp ) const noexcept
		{
			auto found = findFirst( delimiter, cmp );
			if( found )
				return substring( begin(), found.begin() );
			else
				return substring( End.getEnd() );
		}


		// Get as new substring everything inside 'this' substring appearing
		// before the last occurrence of 'delimiter' substring.
		// Comparison is done using a binary compare predicate for single character.
		// NOTE: Search is done in 'high-to-low' ordering regardless of current settings!
		// NOTE: ASCII-only optimization is only done if strcmp::byte::Cmp is used!
		template<typename compare_predicate = strcmp::byte>
		substring beforeLast( const substring& delimiter, const compare_predicate& cmp = strcmp::byte::Cmp ) const noexcept
		{
			auto found = findLast( delimiter, cmp );
			if( found )
				return isHighToLow() ? substring( End, found.begin() ) : substring( Beg, found.begin() );
			else
				return substring( End.getEnd() );
		}

		// Get as new substring everything inside 'this' substring appearing
		// before the last occurrence of 'delimiter' character.
		// Comparison is done using a binary compare predicate for single character.
		// NOTE: Search is done in 'high-to-low' ordering regardless of current settings!
		// NOTE: ASCII-only optimization is only done if strcmp::byte::Cmp is used!
		template<typename compare_predicate = strcmp::byte>
		substring beforeLast( char_t delimiter, const compare_predicate& cmp = strcmp::byte::Cmp ) const noexcept
		{
			auto found = findLast( delimiter, cmp );
			if( found )
				return substring( begin(), found.begin() );
			else
				return substring( End.getEnd() );
		}


		// Get as new substring everything inside 'this' substring appearing
		// after the first occurrence of 'delimiter' substring.
		// Comparison is done using a binary compare predicate for single character.
		// NOTE: Search is done in 'low-to-high' ordering regardless of current settings!
		// NOTE: ASCII-only optimization is only done if strcmp::byte::Cmp is used!
		template<typename compare_predicate = strcmp::byte>
		substring afterFirst( const substring& delimiter, const compare_predicate& cmp = strcmp::byte::Cmp ) const noexcept
		{
			auto found = findFirst( delimiter, cmp );
			if( found )
				return substring( found.end(), end() );
			else
				return substring( End.getEnd() );
		}

		// Get as new substring everything inside 'this' substring appearing
		// after the first occurrence of 'delimiter' character.
		// Comparison is done using a binary compare predicate for single character.
		// NOTE: Search is done in 'low-to-high' ordering regardless of current settings!
		// NOTE: ASCII-only optimization is only done if strcmp::byte::Cmp is used!
		template<typename compare_predicate = strcmp::byte>
		substring afterFirst( char_t delimiter, const compare_predicate& cmp = strcmp::byte::Cmp ) const noexcept
		{
			auto found = findFirst( delimiter );
			if( found )
				return substring( found.end(), end() );
			else
				return substring( End.getEnd() );
		}


		// Get as new substring everything inside 'this' substring appearing
		// after the last occurrence of 'delimiter' substring.
		// Comparison is done using a binary compare predicate for single character.
		// NOTE: Search is done in 'high-to-low' ordering regardless of current settings!
		// NOTE: ASCII-only optimization is only done if strcmp::byte::Cmp is used!
		template<typename compare_predicate = strcmp::byte>
		substring afterLast( const substring& delimiter, const compare_predicate& cmp = strcmp::byte::Cmp ) const noexcept
		{
			auto found = highToLow().findLast( delimiter, cmp );
			if( found )
				return substring( found.end(), end() );
			else
				return substring( End.getEnd() );
		}

		// Get as new substring everything inside 'this' substring appearing
		// after the last occurrence of 'delimiter' character.
		// Comparison is done using a binary compare predicate for single character.
		// NOTE: Search is done in 'high-to-low' ordering regardless of current settings!
		// NOTE: ASCII-only optimization is only done if strcmp::byte::Cmp is used!
		template<typename compare_predicate = strcmp::byte>
		substring afterLast( char_t delimiter, const compare_predicate& cmp = strcmp::byte::Cmp ) const noexcept
		{
			auto found = highToLow().findLast( delimiter, cmp );
			if( found )
				return substring( found.end(), end() );
			else
				return substring( End.getEnd() );
		}




		///////////////////////////////////////////////////////////////////////
		//
		// Counting
		//
	public:

		// Count occurrences of a sub-string.
		// NOTE: Search is done in 'low-to-high' ordering regardless of current settings!
		// NOTE: All occurrences are counted, including overlaps!
		// Comparison is done using a binary compare predicate for single character.
		template<typename compare_predicate = strcmp::byte>
		index_t count( const substring& to_count, const compare_predicate& cmp = strcmp::byte::Cmp ) const noexcept
		{
			if( numBytes() > 0 && numBytes() < to_count.numChars() )
				return 0;
			else if( isHighToLow() )
				return lowToHigh().count( to_count.isHighToLow() ? to_count.lowToHigh() : to_count, cmp );
			else if( to_count.isHighToLow() )
				return count( to_count.lowToHigh(), cmp );
			index_t cnt = 0;
			auto found = findFirst( to_count, cmp );
			for( ; found; ++cnt )
				found = substring( found.begin() + 1, end() ).findFirst( to_count, cmp );
			return cnt;
		}

		// Count occurrences of a character.
		// NOTE: Search is done in 'low-to-high' ordering regardless of current settings!
		// Comparison is done using a binary compare predicate for single character.
		template<typename compare_predicate = strcmp::byte>
		index_t count( char_t to_count, const compare_predicate& cmp = strcmp::byte::Cmp ) const noexcept
		{
			if( numBytes() == 0 )
				return 0;
			else if( isHighToLow() )
				return lowToHigh().count( to_count, cmp );
			index_t cnt = 0;
			for( string_iterator i = begin(); i != end(); ++i )
			{
				if( cmp( *i, to_count ) == 0 )
					++cnt;
			}
			return cnt;
		}




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		substring _optimizedFindFirst( const substring& to_find ) const noexcept;
		substring _optimizedFindFirst( char_t to_find ) const noexcept;
		substring _optimizedFindLast( const substring& to_find ) const noexcept;
		substring _optimizedFindLast( char_t to_find ) const noexcept;

		void _bypassSection( char_t start_sect, char_t end_sect, string_iterator& i ) const;

		static inline const char* _findFirst( const char* str, index_t str_size, char c ) noexcept {
			return (char*)memchr( str, c, str_size ); }
		const char* _findFirst(
			const char* source, index_t source_size, const char* substr, index_t substr_size ) const noexcept;

		static const char* _findLast( const char* str, index_t str_size, char chr ) noexcept;
		const char* _findLast(
			const char* source, index_t source_size, const char* substr, index_t substr_size ) const noexcept;

		std::string _getReverse() const;




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		string_iterator Beg, End;

		enum class Flag : uint8_t
		{
			none = 0x00,
			icase = 0x01,
			force_favor_speed = 0x02,
			force_favor_humans = 0x04,
			force_favor_diff = force_favor_speed | force_favor_humans
		};
		Flag Flags{ Flag::none };
	};
}
