#pragma once
#include "StringIterator.h"

/*
	A substring is a part of a larger string, but rather than using a full copy
	of the section from the larger string, sub_string objects uses iterators.

	Since a substring can be from zero character to the entire larger string,
	we can emply a lot of common functionality directly on substring rather
	than the larger string. Thus, many common string operations are implemented
	for substring (which can also deal with std::string and const char* (C
	strings).

	A substring can be ordered low-to-high (normal) and high-to-low (reverse).
	Unless explicitly stated otherwise, methods required that the substring be
	in low-to-high order.
*/



namespace eon
{
	// All string comparison is done by the substring class.
	// The 'faster' compare type will cause compare to run as fast as possible,
	// but only return -1, 0 and 1 for less than, equal to or grater than.
	// The 'diff_pos' compare type will cause compare to run somewhat slower,
	// but return position of first diff (absolute of return value).
	enum class CompareType
	{
		faster,			// Return only -1, 0 or 1
		diff_pos		// Return position of first diff
	};

	using eon_byte = char;
	using eon_pos = size_t;



	class substring
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//		
	public:
		substring() = default;
		inline substring( const substring& other ) noexcept { *this = other; }
		inline substring( const string_iterator& begin,
			const string_iterator& end ) { begin.assertCompatibleWith(
				end ); Beg = begin, End = end; }
		inline explicit substring( const string_iterator& itr ) noexcept {
			Beg = itr; End = itr; }
		~substring() = default;
		inline explicit substring( const std::string& stdstr ) noexcept {
			Beg = string_iterator( stdstr ); End = Beg.getEnd(); }
		inline explicit substring( const char* cstr ) noexcept {
			Beg = string_iterator( cstr, strlen( cstr ) ); End = Beg
				+ Beg.numChars(); }
		
		inline substring& operator=( const substring& other ) noexcept {
			Beg = other.Beg; End = other.End; return *this; }




		///////////////////////////////////////////////////////////////////////
		//
		// Write Methods
		//
	public:
		inline void clear() noexcept { Beg.reset(); End.reset(); }




		///////////////////////////////////////////////////////////////////////
		//
		// Read methods
		//
	public:

		// Check if this substring is for a string starting at the specified
		// raw buffer address
		inline bool sameBuffer( const char* buffer_start ) const noexcept {
			return Beg.sameBuffer( buffer_start )
				&& End.sameBuffer( buffer_start ); }

		// Assert substring is for a string starting at the specified raw
		// buffer address.
		// Throws WrongSource if not
		inline void assertSameBuffer( const char* buffer_start ) const {
			if( !sameBuffer( buffer_start ) ) throw WrongSource(); }

		// Check ordering
		// NOTE: Empty substring has no ordering!
		inline bool isLowToHigh() const noexcept { return Beg < End; }
		inline bool isHighToLow() const noexcept { return Beg > End; }

		// Get a copy of the substring, ensured to be low-to-high ordered
		substring lowToHigh() const noexcept;

		// Get a copy of the substring, ensured to be high-to-low ordered
		substring highToLow() const noexcept;

		// A 'true' substring has one or both of begin and end less than
		// end-of-string. The two can be equal (empty substring)!
		// Returns false if both begin and end are at end-of-string (invalid
		// substring).
		inline operator bool() const noexcept {
			return static_cast<bool>( Beg ) || static_cast<bool>( End ); }

		// An empty substring is one where begin and end are the same.
		inline bool empty() const noexcept { return numChars() == 0; }

		// Get number of characters in substring
		inline size_t numChars() const noexcept {
			return End >= Beg ? End - Beg : Beg - End; }

		// Get number of bytes in substring
		inline size_t numBytes() const noexcept;

		// Check if 'this' substring is for the same area as the 'other'
		inline bool sameArea( const substring& other ) const
			noexcept { return Beg == other.Beg && End == other.End; }

		// Check if substring is for valid UTF-8 iterators
		inline bool validUTF8() const noexcept { return Beg.ValidUTF8; }

		// Get std::string from substring (bytes)
		inline std::string stdstr() const noexcept { return isHighToLow()
			? _getReverse() : std::string( Beg.byteData(), numBytes() ); }

		// Access a byte inside the substring
		// Returns 0 if the substring is invalid or position is out of bounds!
		eon_byte byte( eon_pos pos ) const noexcept;

		// Check if the substring is enclosed by a specific pair of characters
		// - such as quotation marks or parenthesis.
		inline bool enclosed( eon_char start_char,
			eon_char end_char = same_char ) const noexcept {
			return numChars() > 1 && *begin() == start_char && *last() == (
				end_char == same_char ? start_char : end_char ); }

		// Check if the substring is blank (empty or space characters only)
		bool blank() const noexcept;

		// Given a character number, get the iterator for that position
		// Will count from start or end, depending on which is closest.
		// Returns an invalid (end) iterator if invalid character number.
		// NOTE: The character number is relative to the substring, not the
		//       source string!
		string_iterator iterator( size_t num_char ) const noexcept;

		// Get number of indentations for the first (possibly only) line of
		// text in the string.
		eon_pos indentationLevel( eon_char indentation_char = TabChr )
			const noexcept;

		// Get characters as a vector of eon_char elements
		// High-to-low range will give characters in reverse!
		std::vector<eon_char> chars() const;


		// Get a substring with leading and/or trailing spaces excluded
		substring trim() const;
		substring trimLeading() const;
		substring trimTrailing() const;




		///////////////////////////////////////////////////////////////////////
		//
		// String as number
		//
	public:

		// Check if the substring contains only numeral characters
		bool numeralsOnly() const noexcept;

		// Check if the substring contains only ASCII digits
		bool isUInt() const noexcept;

		// Check if the substring starts with '+' or '-' and contains only
		// ASCII digits after that
		inline bool isInt() const noexcept {
			return numChars() > 1 && ( *begin() == '+' || *begin() == '-' )
				&& substring( begin() + 1, end() ).isUInt(); }

		// Check if substring optionally starts with '+' or '-' and contains
		// only ASCII digits and a single decimal separator (which may occur
		// first or last).
		bool isFloat( eon_char decimal_separator = PointChr ) const noexcept;
		

		// Convert String to number
		// Non-numbers (including non-ASCII numerals) will be returned as zero!
		inline int32_t toInt32() const {
			return static_cast<int32_t>( toInt64() ); }
		int64_t toInt64() const;
		inline uint32_t toUInt32() const {
			return static_cast<uint32_t>( toUInt64() ); }
		int64_t toUInt64() const;
		inline size_t toSize() const { return (size_t)toUInt64(); }
		double toDouble( eon_char decimal_separator = PointChr ) const;


		// Assuming the entire substring is a number, get a reduced substring
		// where leading integer zeros and trailing fractional zeros are
		// removed - including the decimal separator if all fractional zeros.
		// (If the number ends with the separator, it will be excluded.)
		// NOTE: If the number is signed, leading integer zeros will be
		//       included to keep the sign.
		// Example: 0001.0000 -> 1
		substring trimNumber( eon_char decimal_separator = PointChr ) const;

		// Same as trimNumber, but if all fractional zeros, leave the decimal
		// separator and one zero. (If the number ends with the separator, it
		// will be included.)
		// Example: 0001.0000 -> 1.0
		substring trimFloat( eon_char decimal_separator = PointChr ) const;




		///////////////////////////////////////////////////////////////////////
		//
		// String hashing
		//
	public:

		// FNV-1a hash algorithm
#define FNV_PRIME32 16777619;
#define FNV_PRIME64 1099511628211;
#define FNV_OFFSET32 2166136261;
#define FNV_OFFSET64 14695981039346656037;
		uint32_t hash32() const noexcept;
		uint64_t hash64() const noexcept;
		inline size_t hash() const noexcept {
			return sizeof( size_t ) == 4 ? hash32() : hash64(); }




		///////////////////////////////////////////////////////////////////////
		//
		// Iteration
		//
	public:
		inline string_iterator& begin() noexcept { return Beg; }
		inline const string_iterator& begin() const noexcept { return Beg; }
		inline string_iterator& end() noexcept { return End; }
		inline const string_iterator& end() const noexcept { return End; }

		inline const string_iterator last() const noexcept {
			return Beg < End ? End - 1 : End; }




		///////////////////////////////////////////////////////////////////////
		//
		// Searching
		//
	public:

		// Check if the substring contains the given substring or codepoint
		inline bool contains( const substring& sub ) const noexcept {
			return static_cast<bool>( findFirst( sub ) ); }
		inline bool contains( eon_char codepoint ) const noexcept {
			return static_cast<bool>( findFirst( codepoint ) ); }

		// Check if the substring contains any of the charcters in the given
		// substring
		inline bool containsAnyOf( const substring& sub ) const noexcept {
			return static_cast<bool>( findFirstOf( sub ) ); }

		// Find first occurrence of the sub-string
		// Both substrings must be low-to-high! (No checking is done!)
		// Returns the found substring - 'false' if not found.
		substring findFirst( const substring& to_find ) const noexcept;

		// Find first occurrence of the codepoint
		// 'this' substring must be low-to-high! (No checking is done!)
		// Returns the found codepoint - 'false' if not found.
		substring findFirst( eon_char to_find ) const noexcept;


		// Find last occurrence of the sub-string
		// Both substrings must be high-to-low! (No checking is done!)
		// Returns the found substring - 'false' if not found.
		substring findLast( const substring& to_find ) const noexcept;

		// Find last occurrence of the codepoint
		// 'this' substring must be high-to-low! (No checking is done!)
		// Returns the found codepoint - 'false' if not found.
		substring findLast( eon_char to_find ) const noexcept;


		// Find first occurrence of any of the specified characters
		// If the substring is high-to-low, this is the same as findLastOf on a
		// low-to-high substring!
		// Returns iterator for the found character - 'false' if not found.
		string_iterator findFirstOf( const substring& characters )
			const noexcept;

		// Find last occurrence of any of the specified characters
		// If the substring is high-to-low, this is the same as findLastOf on a
		// low-to-high substring!
		// Returns iterator for the found character - 'false' if not found.
		string_iterator findLastOf( const substring& characters )
			const noexcept;


		// Find first occurrence of any character not among the specified
		// If the substring is high-to-low, this is the same as findLastNotOf
		// on a low-to-high substring!
		// Returns iterator for the found character - 'false' if not found.
		string_iterator findFirstNotOf( const substring& characters )
			const noexcept;

		// Find last occurrence of any character not among the specified
		// If the substring is high-to-low, this is the same as findFirstNotOf
		// on a low-to-high substring!
		// Returns iterator for the found character - 'false' if not found.
		string_iterator findLastNotOf( const substring& characters )
			const noexcept;


		// Find the first character in 'this' substring that differs from the
		// other, 'false' if none (equal substrings).
		// Both substrings must be low-to-high!
		inline string_iterator findFirstDiff( const substring& other )
			const noexcept;


		// Find first, but skip sections of matching start_sect and end_sect
		// characters. For example, quoted sections, parenthesized sections,
		// etc.
		// If start_sect and end_sect are different, then nested occurrences
		// will be accounted for.
		// If start_sect and end_sect are the same, then backslashed section
		//  characters will be ignored (not taken as section start or end).
		// Both substrings must be low-to-high!
		substring findFirstIgnoreSections( const substring& other,
			eon_char start_sect, eon_char end_sect = same_char )
			const noexcept;
		substring findFirstIgnoreSections( eon_char codepoint,
			eon_char start_sect, eon_char end_sect = same_char )
			const noexcept;


		// Find first occurrence of a character from a character category
		// The substring must be low-to-high!
		string_iterator findFirst( charcat category ) const noexcept;

		// Find last occurrence of a character from a character category
		// The substring must be high-to-low!
		string_iterator findLast( charcat category ) const noexcept;


		// Find first occurrence of a character not from a character category
		// The substring must be low-to-high!
		string_iterator findFirstOtherThan( charcat category ) const noexcept;

		// Find last occurrence of a character not from a character category
		// The substring must be high-to-low!
		string_iterator findLastOtherThan( charcat category ) const noexcept;

		// Get substring before/after first/last occurrence of the delimiter
		substring beforeFirst( const substring& delimiter ) const noexcept;
		substring beforeFirst( eon_char delimiter ) const noexcept;
		substring beforeLast( const substring& delimiter ) const noexcept;
		substring beforeLast( eon_char delimiter ) const noexcept;
		substring afterFirst( const substring& delimiter ) const noexcept;
		substring afterFirst( eon_char delimiter ) const noexcept;
		substring afterLast( const substring& delimiter ) const noexcept;
		substring afterLast( eon_char delimiter ) const noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Counting
		//
	public:

		// Count occurrences of a character
		// The substring must be low-to-high
		size_t count( eon_char to_count ) const noexcept;

		// Count occurrences of a sub-string
		// NOTE: All occurrences are counted, even overlaps!
		// Both substrings must be low-to-high
		size_t count( const substring& to_count ) const noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Splitting into multiple substrings
		//
	public:

		// Split substring on every occurrence of the delimiter character or
		// substring into a sequential container (typically list<substring> or
		// vector<substring>) - requires the push_back method!
		// If a max number of elements is specified, the last element will
		// reference the unsplit remainder
		template<typename container_t>
		container_t splitSequential( const substring& delimiter,
			size_t max_elements = SIZE_MAX ) const
		{
			container_t elms;
			auto start = begin();
			for( auto found = findFirst( delimiter );
				found || elms.size() == max_elements - 1;
				found = substring( start, end() ).findFirst( delimiter ) )
			{
				elms.push_back( substring( start, found.begin() ) );
				start = found.end();
			}
			if( start )
				elms.push_back( substring( start, end() ) );
			return elms;
		}
		template<typename container_t>
		container_t splitSequential( eon_char delimiter,
			size_t max_elements = SIZE_MAX ) const
		{
			container_t elms;
			auto start = begin();
			for( auto found = findFirst( delimiter );
				found || elms.size() == max_elements - 1;
				found = substring( start, end() ).findFirst( delimiter ) )
			{
				elms.push_back( substring( start, found.begin() ) );
				start = found.end();
			}
			if( start )
				elms.push_back( substring( start, end() ) );
			return elms;
		}

		// Split substring on every occurrence of the delimiter character or
		// substring into a non-sequential container (typically set<substring>,
		// multiset<substring>, unordered_set<substring>, or
		// unordered_multiset<substring>) - requires the insert method!
		template<typename container_t>
		container_t splitNonsequential( const substring& delimiter ) const
		{
			container_t elms;
			auto start = begin();
			for( auto found = findFirst( delimiter );
				found;
				found = substring( start, end() ).findFirst( delimiter ) )
			{
				elms.insert( substring( start, found.begin() ) );
				start = found.begin() + delimiter.numChars();
			}
			if( start )
				elms.insert( substring( start, end() ) );
			return elms;
		}
		template<typename container_t>
		container_t splitNonSequential( eon_char delimiter ) const
		{
			container_t elms;
			auto start = begin();
			for( auto found = findFirst( delimiter );
				found;
				found = substring( start, end() ).findFirst( delimiter ) )
			{
				elms.insert( substring( start, found.begin() ) );
				start = found.begin() + 1;
			}
			if( start )
				elms.insert( substring( start, end() ) );
			return elms;
		}




		///////////////////////////////////////////////////////////////////////
		//
		// Comparing
		//
		// If CompareType::faster is specified, then compare as fast as
		// possible and only return -1, 0 or 1. If CompareType::diff_pos is
		// specified, return -pos, 0 or pos - where pos is the first character
		// that is different between the two.
		//
		// All compare methods required all substrings to be low-to-high!
		//
	public:

		// Check if the substring starts with a specific value
		inline bool startsWith( const substring& value ) const noexcept {
			return memcmp( begin().byteData(), value.begin().byteData(),
				value.numBytes() ) == 0; }
		inline bool startsWith( eon_char value ) const noexcept {
			return *begin() == value; }

		// Check if string ends with a specific value
		inline bool endsWith( const substring& value ) const noexcept {
			return numChars() < value.numChars() ? false : memcmp(
				begin().byteData() + ( numBytes() - value.numBytes() ),
				value.begin().byteData(), value.numBytes() ) == 0; }
		inline bool endsWith( eon_char value ) const noexcept {
			return *last() == value; }

		// Do normal comparison
		// Assumes both 'this' and 'other' are low-to-high!
		int compare( const substring& other,
			CompareType type = CompareType::faster ) const noexcept;

		// Comparison operators - all using CompareType::faster
		inline bool operator<( const substring& other ) const noexcept {
			return compare( other ) < 0; }
		inline bool operator<=( const substring& other ) const noexcept {
			return compare( other ) <= 0; }
		inline bool operator>( const substring& other ) const noexcept {
			return compare( other ) > 0; }
		inline bool operator>=( const substring& other ) const noexcept {
			return compare( other ) >= 0; }
		inline bool operator==( const substring& other ) const noexcept {
			return compare( other ) == 0; }
		inline bool operator!=( const substring& other ) const noexcept {
			return compare( other ) != 0; }

		// Compare while ignoring case
		// Assumes both 'this' and 'other' are low-to-high!
		int iCompare( const substring& other ) const noexcept;


		// Compare by position, rather than characters.
		// Returns -1, 0 or 1.
		inline int comparePos( const substring& other ) const noexcept {
			return Beg < other.Beg ? -1 : other.Beg < Beg
				? 1 : End < other.End ? -1 : other.End < End ? 1 : 0; }





		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		static inline const char* _findFirst( const char* str, size_t str_size,
			char c ) noexcept { return (char*)memchr( str, c, str_size ); }
		const char* _findFirst( const char* source, size_t source_size,
			const char* substr, size_t substr_size ) const noexcept;
		
		static const char* _findLast( const char* str, size_t str_size,
			char chr ) noexcept;
		const char* _findLast( const char* source, size_t source_size,
			const char* substr, size_t substr_size ) const noexcept;

		std::string _getReverse() const;



		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:
		string_iterator Beg, End;
	};
}
