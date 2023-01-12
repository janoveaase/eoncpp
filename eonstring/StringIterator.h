#pragma once
#include "UniChar.h"
#include <memory>
#include <codecvt>
#include <iostream>
#include <functional>
#include <exception>
#include <string.h>


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
#ifndef _DEBUG
	using charsize_t = uint8_t;
#else
	using charsize_t = uint16_t;
#endif

#define UTF8_ACCEPT 0
#define UTF8_REJECT 1



	///////////////////////////////////////////////////////////////////////////
	//
	// Exception used when two iterators that are supposed to be for the same
	// source string are not, or an interator that is supposed to be for a
	// specific source string, is not.
	//
	class WrongSource : public std::exception
	{
	public:
		inline WrongSource() : std::exception() {}
		inline WrongSource( const char* message ) : std::exception() { Message = message; }
		inline WrongSource( const std::string& message ) : std::exception() { Message = message; }
		inline WrongSource( std::string&& message ) noexcept : std::exception() { Message = std::move( message ); }
		inline const char* what() const noexcept override { return Message.c_str(); }
	private:
		std::string Message;
	};




	///////////////////////////////////////////////////////////////////////////
	//
	// Exception used when UTF-8 is required, but the source string contains
	// one or more invalid characters.
	//
	class InvalidUTF8 : public std::exception
	{
	public:
		inline InvalidUTF8() : std::exception() {}
		inline InvalidUTF8( const char* message ) : std::exception() { Message = message; }
		inline InvalidUTF8( const std::string& message ) : std::exception() { Message = message; }
		inline InvalidUTF8( std::string&& message ) noexcept : std::exception() { Message = std::move( message ); }
		inline const char* what() const noexcept override { return Message.c_str(); }
	private:
		std::string Message;
	};




	///////////////////////////////////////////////////////////////////////////
	//
	// Eon UTF-8 Capable String Iterator Class - eon::string_iterator.
	//
	// While the iterator is primarily purposed for use with [eon::string], it
	// can be applied to both 'std::string' and pure 'C-strings' (const char*).
	// On its own, it does not require the source string to be UTF-8 (while
	// [eon::string] does!).
	//
	// NOTE: If you intend to iterate a non-UTF-8 string, you are normally
	//       better off using 'std::string::iterator' or pointer iteration!
	//
	// WARNING: A string_iterator is only valid for as long as the source
	//          string remains unchanged! Use of a string_iterator with an
	//          invalid source may result in undefined behavior!
	//
	class string_iterator
	{
	public:

		using iterator_category = std::input_iterator_tag;
		using value_type = char_t;
		using difference_type = char_t;
		using pointer = const char_t*;
		using reference = char_t;




		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		// Construct an unattached iterator that is equivalent to 'false'.
		// (Typically as a place-holder.)
		string_iterator() = default;

		// Construct as a copy of another iterator.
		inline string_iterator( const string_iterator& other ) { *this = other; }


		// Construct for a 'std::string'.
		// Will count number of UTF-8 characters and optimize for UTF-8 handling.
		// NOTE: If the string contains invalid UTF-8 characters, it will be
		//       treated as a pure C string with byte characters only!
		inline explicit string_iterator( const std::string& stdstr ) noexcept {
			_prep( stdstr.c_str(), stdstr.c_str() + stdstr.size(), stdstr.c_str() ); }

		// Construct for a 'std::string', but set to a specific character position.
		// Will count number of UTF-8 characters and optimize for UTF-8 handling.
		// NOTE: If the string contains invalid UTF-8 characters, it will be
		//       treated as a pure C string with byte characters only!
		inline string_iterator( const std::string& stdstr, index_t pos ) noexcept {
			_prep( stdstr.c_str(), stdstr.c_str() + stdstr.size(), stdstr.c_str() ); *this += pos; }


		// Construct for a 'C-string'.
		// Will count number of UTF-8 characters and optimize for UTF-8 handling.
		// NOTE: If the string contains invalid UTF-8 characters, it will be
		//       treated as a pure C string with byte characters only!
		inline explicit string_iterator( const char* cstr ) noexcept { _prep( cstr, cstr + strlen( cstr ), cstr ); }

		// Construct for a 'C-string'.
		// Will count number of UTF-8 characters and optimize for UTF-8 handling.
		// NOTE: If the string contains invalid UTF-8 characters, it will be
		//       treated as a pure C string with byte characters only!
		inline explicit string_iterator( const unsigned char* cstr ) noexcept {
			_prep( (const char*)cstr, (const char*)cstr + strlen( (const char*)cstr ), (const char*)cstr ); }

		// Construct for a 'C-string', but set to a specific byte length.
		// Will count number of UTF-8 characters and optimize for UTF-8 handling.
		// NOTE: If the string contains invalid UTF-8 characters, it will be
		//       treated as a pure C string with byte characters only!
		inline string_iterator( const char* cstr, index_t size ) noexcept { _prep( cstr, cstr + size, cstr ); }


		// Construct from already known details, position at start of source.
		// Fast, does not have to count any UTF-8 characters!
		// WARNING: Setting invalid details may likely result in undefined behavior!
		// Parameters:
		//   begin            : Start of source string.
		//   num_bytes        : Total number of bytes in source string.
		//   num_source_chars : Total number of UTF-8 characters in source string.
		inline string_iterator( const char* begin, index_t num_bytes, index_t num_source_chars ) noexcept {
			NumSourceChars = num_source_chars; _prep( begin, begin + num_bytes, begin ); }

		// Construct from already known details, position specified.
		// Will count number of UTF-8 characters and optimize for UTF-8 handling.
		// WARNING: Setting invalid details may likely result in undefined behavior!
		// Parameters:
		//   begin     : Start of source string.
		//   num_bytes : Total number of bytes in source string.
		//   pos       : Iterator position in source string.
		inline string_iterator( const char* begin, index_t num_bytes, const char* pos ) noexcept {
			_prep( begin, begin + num_bytes, pos ); }

		// Construct from already known details, position specified.
		// Does not have to count all UTF-8 characters, only up to 'pos'!
		// WARNING: Setting invalid details may likely result in undefined behavior!
		// Parameters:
		//   begin            : Start of source string.
		//   num_bytes        : Total number of bytes in source string.
		//   num_source_chars : Total number of UTF-8 characters in source string.
		//   pos              : Iterator position in source string.
		inline string_iterator(
			const char* begin, index_t num_bytes, index_t num_source_chars, const char* pos ) noexcept {
			NumSourceChars = num_source_chars; _prep( begin, begin + num_bytes, pos ); }

		// Construct from already known details, position specified.
		// Fast, does not have to count any UTF-8 characters!
		// WARNING: Setting invalid details may likely result in undefined behavior!
		// Parameters:
		//   begin            : Start of source string.
		//   num_bytes        : Total number of bytes in source string.
		//   num_source_chars : Total number of UTF-8 characters in source string.
		//   pos              : Iterator position in source string.
		//   num_char         : UTF-8 character number of iterator position.
		inline string_iterator(
			const char* begin, index_t num_bytes, index_t num_source_chars, const char* pos, index_t num_char ) noexcept {
			NumSourceChars = num_source_chars; NumChar = num_char; _prep( begin, begin + num_bytes, pos ); }

		// Construct using another 'string_iterator' as 'source', but set a specific position.
		// Fast, does not have to count any UTF-8 characters!
		// WARNING: Setting invalid details may likely result in undefined behavior!
		// Parameters:
		//   source   : 'string_iterator' to copy source details from.
		//   pos      : New iterator position.
		//   num_char : UTF-8 character number of new iterator position.
		string_iterator( const string_iterator& source, const char* pos, index_t num_char ) noexcept;


		// Default destructor
		virtual ~string_iterator() = default;




		///////////////////////////////////////////////////////////////////////
		//
		// String encoding
		//
	public:

		// Get iterator as an std::string formatted as "<byte pos>:<char pos>".
		inline std::string encode() const {
			return hasSource() ? std::to_string( numByte() ) + ":" + std::to_string( numChar() ) : std::string(); }

		// Set iterator from std::string formatted as "<byte pos>:<char pos>" for the specified source.
		// Parameters:
		//   iterator_format : String formatted as "<byte pos>:<char pos>".
		//   source_start    : Start of source to set for the decoded iterator.
		//   source_end      : End of iterator source. If nullptr, then assume null-terminated.
		//   num_source_chars: Number of UTF-8 characters in source.
		//                     If this is no_index, then the characters will be counted.
		// Returns: true if set, false if invalid format!
		// Throws [eon::WrongSource] if invalid format or <byte pos> or <char pos> doesn't fit the source!
		// Throws [eon::InvalidUTF8] if num_source_chars is [eon::no_index] and source turns out to be invalid!
		string_iterator& decode(
			const std::string& iterator_format,
			const char* source_start,
			const char* source_end = nullptr,
			index_t num_source_chars = no_index );




		///////////////////////////////////////////////////////////////////////
		//
		// Modifier Methods
		//
	public:

		// Set from 'std::string'.
		// Will use UTF-8 iteration if no invalid UTF-8 characters, byte-sized characters otherwise.
		inline string_iterator& operator=( const std::string& stdstr ) noexcept {
			_prep( stdstr.c_str(), stdstr.c_str() + stdstr.size(), stdstr.c_str() ); return *this; }

		// Reset string_iterator to default constructor state. (No source.)
		string_iterator& reset() noexcept;

		// Reset string_iterator to beginning (start position).
		string_iterator& resetToBegin() noexcept;

		// Reset string_iterator to end position.
		string_iterator& resetToEnd() noexcept;

		// Reset string_iterator to reverse end position.
		string_iterator& resetToREnd() noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Read methods
		//
	public:

		// Check if the iterator has a source and is not at the (r)end of that source.
		inline operator bool() const noexcept { return CodepointSize > 0; }

		// Check if the iterator has a source, regardless of position.
		inline bool hasSource() const noexcept { return Source != nullptr; }

		// Check if the iterator is at reverse end of the source string.
		// Returns false if not at end or there is no source!
		inline bool atREnd() const noexcept { return CodepointSize == 0 && Pos == Source; }

		// Check if the iterator is at the end of the source string.
		// Returns false if not at end or there is no source!
		inline bool atEnd() const noexcept { return CodepointSize == 0 && Pos > Source; }


		// Check if the specified source is the same as referenced by the iterator.
		// NOTE: Must be the same exact memory address!
		inline bool sameSource( const char* source ) const noexcept { return Source == source; }

		// Check if compatible with other iterator. (Both must have the exact same source!)
		inline bool compatibleWith( const string_iterator& other ) const noexcept { return sameSource( other.Source ); }

		// Assert that the 'other' iterator has the exact same source as 'this'.
		// Throws [eon::WrongSource] if not.
		inline void assertSameSource( const string_iterator& other ) const {
			if( Source != other.Source ) throw WrongSource(); }

		// Assert that the specified source is the exact same as referenced by the iterator.
		// Throws [eon::WrongSource] if not.
		inline void assertSameSource( const char* source ) const { if( Source != source ) throw WrongSource(); }


		// Get the codepoint for the position in the source string referenced by the iterator.
		// Will be [eon::nochar] if no source or at the end of the source.
		inline char_t operator*() const noexcept { return Codepoint; }

		// Get which character (counted from the start) the iterator refers to in the source.
		// For UTF-8 strings, this will be the number of UTF-8 characters, number of bytes otherwise.
		inline index_t numChar() const noexcept { return NumChar; }

		// Get which byte (counted from the start) the iterator refers to in the source.
		// This may be different from [numChar] if there are non-ASCII UTF-8 characters in the source.
		inline index_t numByte() const noexcept { return Pos - Source; }

		// Get the number of bytes making up the code point the iterator refers to in the source.
		// For ASCII characters, this will always be 1, otherwise > 1.
		inline index_t codepointSize() const noexcept { return CodepointSize; }


		// Check if the entire source string referenced by the iterator is ASCII- (byte-) characters only.
		// (If it is, certain optimizations for pure byte-strings can be utilizied.)
		inline bool bytesOnly() const noexcept { return NumSourceChars == SourceEnd - Source; }

		// Check if the string contains only valid UTF-8 characters.
		// Returns 'true' if ASCII/bytes only, otherwise all characters must be valid UTF-8!
		inline bool validUTF8() const noexcept { return ValidUTF8; }


		// Get the address of the raw source byte data starting at the position referenced by the iterator.
		// WARNING: The returned C-string may end at any arbitrary chracter, null is not guaranteed!
		inline const char* byteData() const noexcept { return Pos; }

		// Get total number of raw bytes in the source string.
		inline index_t numSourceBytes() const noexcept { return SourceEnd - Source; }

		// Get total number of (UTF-8 or byte-sized) characters in the source string.
		inline index_t numSourceChars() const noexcept { return NumSourceChars; }

		// Get a copy of the iterator, reset to end of source.
		inline string_iterator getEnd() const noexcept {
			if( !hasSource() ) return *this; string_iterator end = *this; return end.resetToEnd(); }

		// Get a copy of the iterator, reset to start of source.
		inline string_iterator getBegin() const noexcept {
			if( !*this ) return *this; auto beg = *this; return beg.resetToBegin(); }




		///////////////////////////////////////////////////////////////////////
		//
		// Iteration
		//
	public:

		// Prefix iteration by increment. Fast!
		string_iterator& operator++() noexcept;

		// Prefix iteration by decrement. Fast!
		inline string_iterator& operator--() noexcept { return *this -= 1; }

		// Postfix iteration by increment. Involves copying!
		inline string_iterator operator++( int ) noexcept { auto _this = *this; ++*this; return _this; }

		// Postfix iteration by decrement. Involves copying!
		inline string_iterator operator--( int ) noexcept { auto _this = *this; *this -= 1; return _this; }


		// Move forward a specific number of (UTF-8) characters.
		// Uses the prefix increment operator.
		// Optimized for pure ASCII strings!
		inline string_iterator& operator+=( index_t num_chars ) noexcept {
			if( bytesOnly() ) _advanceBytes( num_chars );
				else { for( index_t i = 0; i < num_chars && *this; ++i, ++*this ); } return *this; }

		// Move backward a specific number of characaters.
		// Uses the prefix decrement operator.
		string_iterator& operator-=( index_t num_chars ) noexcept;


		// Move forward (positive) or backward (negative) a specific number of characters.
		inline string_iterator& operator+=( long_t num_chars ) noexcept {
			return num_chars >= 0 ? *this += static_cast<index_t>( num_chars )
				: *this -= static_cast<index_t>( -num_chars ); }

		// Move forward (positive) or backward (negative) a specific number of characters.
		inline string_iterator& operator+=( int num_chars ) noexcept {
			return num_chars >= 0 ? *this += static_cast<index_t>( num_chars )
				: *this -= static_cast<index_t>( -num_chars ); }

		// Move backward (positive) or forward (negative) a specific number of characters.
		inline string_iterator& operator-=( long_t num_chars ) noexcept {
			return num_chars >= 0 ? *this -= static_cast<index_t>( num_chars )
				: *this += static_cast<index_t>( -num_chars ); }

		// Move backward (positive) or forward (negative) a specific number of characters.
		inline string_iterator& operator-=( int num_chars ) noexcept {
			return num_chars >= 0 ? *this -= static_cast<index_t>( num_chars )
				: *this += static_cast<index_t>( -num_chars ); }

		// Get a copy of the specified iterator moved forward by the specified number of characters.
		inline friend string_iterator operator+( const string_iterator& itr, index_t num ) noexcept {
			return string_iterator( itr ) += num; }

		// Get a copy of the specified iterator moved forward (or backward if negative)
		// by the specified number of characters.
		inline friend string_iterator operator+( const string_iterator& itr, long_t num ) noexcept {
			return string_iterator( itr ) += num; }

		// Get a copy of the specified iterator moved forward (or backward if negative)
		// by the specified number of characters.
		inline friend string_iterator operator+( const string_iterator& itr, int num ) noexcept {
			return string_iterator( itr ) += num; }


		// Get number of characters between two iterators of the same source.
		// Throws [eon::WrongSource] if different sources!
		friend long_t operator-( const string_iterator& lhs, const string_iterator& rhs );

		// Get a copy of the specified iterator moved backward by the specified number of characters.
		inline friend string_iterator operator-( const string_iterator& itr, index_t num ) noexcept {
			return string_iterator( itr ) -= num; }

		// Get a copy of the specified iterator moved backward (or forward if negative)
		// by the specified number of characters.
		inline friend string_iterator operator-( const string_iterator& itr, long_t num ) noexcept {
			return string_iterator( itr ) -= num; }

		// Get a copy of the specified iterator moved backward (or forward if negative)
		// by the specified number of characters.
		inline friend string_iterator operator-( const string_iterator& itr, int num ) noexcept {
			return string_iterator( itr ) -= num; }




		///////////////////////////////////////////////////////////////////////
		//
		// Comparison
		//
	public:

		// Compare against another 'string_iterator' by comparing source positions.
		// Returns -1 if 'this' occurs before 'other', 0 if same, and 1 if after.
		// NOTE: If different sources, then source (start) addresses are compared!
		// NOTE: End positions are accounted for!
		int compare( const string_iterator& other ) const noexcept;

		// Check if 'this' comes before 'other' using [compare].
		inline bool operator<( const string_iterator& other ) const noexcept { return compare( other ) < 0; }

		// Check if 'this' comes before or same as 'other' using [compare].
		inline bool operator<=( const string_iterator& other ) const noexcept { return compare( other ) <= 0; }

		// Check if 'this' comes after 'other' using [compare].
		inline bool operator>( const string_iterator& other ) const noexcept { return compare( other ) > 0; }

		// Check if 'this' comes after or same as 'other' using [compare].
		inline bool operator>=( const string_iterator& other ) const noexcept { return compare( other ) >= 0; }

		// Check if 'this' is at same position as 'other' using [compare].
		inline bool operator==( const string_iterator& other ) const noexcept { return compare( other ) == 0; }

		// Check if 'this' is at different position from 'other' using [compare].
		inline bool operator!=( const string_iterator& other ) const noexcept { return compare( other ) != 0; }




		///////////////////////////////////////////////////////////////////////
		//
		// Static utility methods
		//
	public:

		// Given a raw byte string with a 'start' and an 'end' position, set
		// 'codepoint' to the first codepoint at or after 'start' and return
		// the number of bytes it occupies. Zero if not invalid unicode.
		static index_t bytesToUnicode( const char* start, const char* end, char_t& codepoint );

		// Given a codepoint, convert it into bytes.
		// The bytes are stored in the 'bytes' uint32_t, up to 4 of them.
		// Returns the number of bytes used (1-4).
		// Throws [eon::InvalidUTF8] if not a valid code point.
		static index_t unicodeToBytes( char_t codepoint, uint32_t& bytes );

		// Run as part of a UTF-8 decoding algorithm.
		// WARNING: Only use this if you really know what you are doing!
		// See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details about
		// the UTF-8 decoder algorithm used here.
		static char32_t utf8Decode( char32_t& state, char32_t& codep, char32_t byte ) noexcept;

		// Count the number of UTF-8 characters in the given string.
		// Throws [eon::InvalidUTF8] if not valid!
		static index_t countUtf8Chars( const char* str, index_t size );




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		void _prep( const char* begin, const char* end, const char* pos ) noexcept;

		void _utf8CharacterCount() noexcept;

		void _translateCodepoint() noexcept;

		void _backtrack( index_t num_chars ) noexcept;

		void _advanceBytes( index_t bytes ) noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		const char* Pos{ nullptr };			// Address of character in string.
		const char* Source{ nullptr };		// Start of source string.
		const char* SourceEnd{ nullptr };	// End of source string.
		char_t Codepoint{ nochar };			// Translated code point at 'Pos'.
		charsize_t CodepointSize{ 0 };		// Number of bytes in code point.
		index_t NumChar{ 0 };				// Character position from start
		index_t NumSourceChars{ 0 };		// Total number of characters in source.
		bool ValidUTF8{ true };				// All valid UTF-8 characters?

		// Best friends with [eon::substring]!
		friend class substring;
	};
}
