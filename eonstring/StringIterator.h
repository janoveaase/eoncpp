#pragma once
#include "UniChar.h"
#include <memory>
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

	constexpr int UTF8_ACCEPT = 0;
	constexpr int UTF8_REJECT = 1;



	///////////////////////////////////////////////////////////////////////////
	//
	// Exception used when two iterators that are supposed to be for the same
	// source string are not, or an interator that is supposed to be for a
	// specific source string, is not.
	//
	class WrongSource : public std::exception
	{
	public:
		using std::exception::exception;
		inline explicit WrongSource( const char* message ) : std::exception(), Message( message ) {}
		inline explicit WrongSource( const std::string& message ) : std::exception(), Message( message ) {}
		inline explicit WrongSource( std::string&& message ) noexcept : std::exception(), Message( std::move( message ) ) {}
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
		using std::exception::exception;
		inline explicit InvalidUTF8( const char* message ) : std::exception(), Message( message ) {}
		inline explicit InvalidUTF8( const std::string& message ) : std::exception(), Message( message ) {}
		inline explicit InvalidUTF8( std::string&& message ) noexcept : std::exception(), Message( std::move( message ) ) {}
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
	// A string iterator is aware of its source string, including where it
	// starts and ends. It can be used to iterate both forwards and backwards.
	// An iterator will always be in one of the following absolute states:
	//  - void   : No source!
	//  - end    : After last character in source.
	//             Using the -- operator will change it's state to 'last'.
	//  - rend   : Before first character in source.
	//             Using the ++ operator will change it's state to 'first'.
	//  - valid  : Somewhere after rend and before end.
	//
	// Additionally, the following virtual states are possible for 'valid' iterators:
	//  - first  : First character in source.
	//  - last   : Last character in source.
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

		// Construct a 'void' iterator.
		string_iterator() = default;

		// Construct as a copy of another iterator.
		inline string_iterator( const string_iterator& other ) { *this = other; }


		// Construct for a 'std::string' source.
		// Slow, will count number of UTF-8 characters in source and optimize for UTF-8 handling.
		// NOTE: If the source is not valid UTF-8, it will be treated as all bytes!
		inline explicit string_iterator( const std::string& stdstr ) noexcept {
			_prep( stdstr.c_str(), stdstr.c_str() + stdstr.size(), stdstr.c_str() ); }

		// Construct for a 'std::string' source, set to a specific character position.
		// Slow, will count number of UTF-8 characters in source and optimize for UTF-8 handling.
		// NOTE: If the source is not valid UTF-8, it will be treated as all bytes!
		inline string_iterator( const std::string& stdstr, index_t pos ) noexcept {
			_prep( stdstr.c_str(), stdstr.c_str() + stdstr.size(), stdstr.c_str() ); *this += pos; }


		// Construct for a 'C-string' source.
		// Slow, will count number of UTF-8 characters in source and optimize for UTF-8 handling.
		// NOTE: If the source is not valid UTF-8, it will be treated as all bytes!
		inline explicit string_iterator( const char* cstr ) noexcept { _prep( cstr, cstr + strlen( cstr ), cstr ); }

		// Construct for a 'C-string' source.
		// Slow, will count number of UTF-8 characters in source and optimize for UTF-8 handling.
		// NOTE: If the source is not valid UTF-8, it will be treated as all bytes!
		inline explicit string_iterator( const unsigned char* cstr ) noexcept {
			_prep( (const char*)cstr, (const char*)cstr + strlen( (const char*)cstr ), (const char*)cstr ); }

		// Construct for a 'C-string' source, set to a specific byte length.
		// Slow, will count number of UTF-8 characters in source and optimize for UTF-8 handling.
		// NOTE: If the source is not valid UTF-8, it will be treated as all bytes!
		inline string_iterator( const char* cstr, index_t size ) noexcept { _prep( cstr, cstr + size, cstr ); }


		// Construct from already known details, position at start of source.
		// Fast, will not count number of UTF-8 characters in source!
		// WARNING: Setting invalid details may result in undefined behavior!
		// Parameters:
		//   source           : Start of source.
		//   num_bytes        : Source size in bytes.
		//   num_source_chars : Source size in number of UTF-8 characters.
		inline string_iterator( const char* source, index_t num_bytes, index_t num_source_chars ) noexcept
			: NumSourceChars( num_source_chars ) { _prep( source, source + num_bytes, source ); }

		// Construct from already known details, for specified position in source.
		// Slow, will count number of UTF-8 characters in source and optimize for UTF-8 handling.
		// WARNING: Setting invalid details may result in undefined behavior!
		// Parameters:
		//   source    : Start of source.
		//   num_bytes : Source size in bytes.
		//   pos       : Iterator position in source in bytes.
		inline string_iterator( const char* source, index_t num_bytes, const char* pos ) noexcept {
			_prep( source, source + num_bytes, pos ); }

		// Construct from already known details, for specified position in source.
		// Will only count number of UTF-8 characters up to specified position!
		// WARNING: Setting invalid details may result in undefined behavior!
		// Parameters:
		//   source           : Start of source.
		//   num_bytes        : Source size in bytes.
		//   num_source_chars : Source size in number of UTF-8 characters.
		//   pos              : Iterator position in source in bytes.
		inline string_iterator(
			const char* source, index_t num_bytes, index_t num_source_chars, const char* pos ) noexcept
		: NumSourceChars( num_source_chars ) { _prep( source, source + num_bytes, pos ); }

		// Construct from already known details, for specified position in source.
		// Fast, will not count number of UTF-8 characters in source!
		// WARNING: Setting invalid details may result in undefined behavior!
		// Parameters:
		//   source           : Start of source.
		//   num_bytes        : Source size in bytes.
		//   num_source_chars : Source size in number of UTF-8 characters.
		//   pos              : Iterator position in source in bytes.
		//   num_char         : Iteration position in source in UTF-8 characters.
		inline string_iterator(
			const char* source, index_t num_bytes, index_t num_source_chars, const char* pos, index_t num_char ) noexcept
		: NumChar( num_char ), NumSourceChars( num_source_chars ) { _prep( source, source + num_bytes, pos ); }


		// Construct using another 'string_iterator' as 'source', but set specified position.
		// Fast, will not count number of UTF-8 characters in source!
		// WARNING: Setting invalid details may result in undefined behavior!
		// Parameters:
		//   other    : Other 'string_iterator' to copy source details from.
		//   pos      : Iterator position in source in bytes.
		//   num_char : Iterator position in source in UTF-8 characters.
		string_iterator( const string_iterator& other, const char* pos, index_t num_char ) noexcept;


		// Default destructor
		virtual ~string_iterator() = default;




		///////////////////////////////////////////////////////////////////////
		//
		// String encoding
		//
	public:

		// Get iterator as a std::string formatted as "<byte pos>:<char pos>".
		inline std::string encode() const {
			return isVoid() ? std::string() : std::to_string( numByte() ) + ":" + std::to_string( numChar() ); }

		// Set iterator from std::string formatted as "<byte pos>:<char pos>" for the specified source.
		// Parameters:
		//   iterator_format : String formatted as "<byte pos>:<char pos>".
		//   source_start    : Start of source to set for the decoded iterator.
		//   source_end      : End of iterator source. If nullptr, then assume null-terminated.
		//   num_source_chars: Source size in number of UTF-8 characters.
		//                     If this is no_index, then the characters will be counted (slow).
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

		// Discard existing details and set new 'std::string' as source, positioned at start.
		// Slow, will count number of UTF-8 characters in source and optimize for UTF-8 handling.
		// NOTE: If the source is not valid UTF-8, it will be treated as all bytes!
		inline string_iterator& operator=( const std::string& stdstr ) noexcept {
			_prep( stdstr.c_str(), stdstr.c_str() + stdstr.size(), stdstr.c_str() ); return *this; }

		// Reset to 'void' state.
		string_iterator& reset() noexcept;

		// Reset to 'first' state.
		string_iterator& resetToFirst() noexcept;

		// Reset to 'last' state.
		inline string_iterator& resetToLast() noexcept { resetToEnd(); return --( *this ); }

		// Reset to 'end' state.
		string_iterator& resetToEnd() noexcept;

		// Reset to 'rend' state.
		string_iterator& resetToREnd() noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Read methods
		//
	public:

		// Check that state is 'valid' (not 'void' or 'end' or 'rend).
		inline explicit operator bool() const noexcept { return CodepointSize > 0; }

		// Check if in 'void' state.
		inline bool isVoid() const noexcept { return Source == nullptr; }

		// Check if in 'first' state.
		inline bool atFirst() const noexcept { return CodepointSize > 0 && Pos == Source; }

		// Check if in 'last' state.
		inline bool atLast() const noexcept { return CodepointSize > 0 && NumChar == NumSourceChars - 1; }

		// Check if in 'end' state.
		inline bool atEnd() const noexcept { return CodepointSize == 0 && Pos > Source; }

		// Check if in 'rend' state.
		inline bool atREnd() const noexcept { return CodepointSize == 0 && Pos == Source; }


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
		// Returns [eon::nochar] if not in 'valid' state (if in 'void', 'end', or 'rend' state)!
		inline char_t operator*() const noexcept { return Codepoint; }

		// Get iteration position in source as number of UTF-8 characters from start.
		// For byte strings this will be identical to [numByte]!
		inline index_t numChar() const noexcept { return NumChar; }

		// Get iterator position in source as number of bytes from start.
		// For UTF-8 strings this will be >= [numChar]!
		inline index_t numByte() const noexcept { return Pos - Source; }

		// Get the number of bytes making up the code point at [numChar] in the source.
		// For UTF-8 strings this will be >= 1, for byte strings, always 1!
		inline index_t codepointSize() const noexcept { return CodepointSize; }


		// Check if source is all bytes (no UTF-8 characters).
		inline bool bytesOnly() const noexcept { return NumSourceChars == SourceEnd - Source; }

		// Check if source is valid UTF-8.
		inline bool validUTF8() const noexcept { return ValidUTF8; }


		// Get source address.
		// WARNING: Terminating null character is not guaranteed! Use [numSourceBytes] to determine end point.
		inline const char* byteData() const noexcept { return Pos; }

		// Get source size in number of bytes.
		inline index_t numSourceBytes() const noexcept { return SourceEnd - Source; }

		// Get source size in number of UTF-8 characters.
		inline index_t numSourceChars() const noexcept { return NumSourceChars; }


		// Get a new iterator that is in 'end' state.
		inline string_iterator getEnd() const noexcept
		{
			if( isVoid() )
				return *this;
			string_iterator other{ *this };
			return other.resetToEnd();
		}

		// Get a new iterator that is in 'rend' state.
		inline string_iterator getREnd() const noexcept
		{
			if( isVoid() )
				return *this;
			string_iterator other{ *this };
			return other.resetToREnd();
		}

		// Get a new iterator that is in 'first' state.
		inline string_iterator getFirst() const noexcept
		{
			if( isVoid() )
				return *this;
			string_iterator other{ *this };
			return other.resetToFirst();
		}

		// Get a new iterator that is in 'last' state.
		inline string_iterator getLast() const noexcept
		{
			if( isVoid() )
				return *this;
			string_iterator other{ *this };
			return other.resetToLast();
		}




		///////////////////////////////////////////////////////////////////////
		//
		// Iteration
		//
	public:

		// Prefix iteration by increment.
		// If in 'rend' state, state will change to 'first' if source is not empty.
		string_iterator& operator++() noexcept;

		// Prefix iteration by decrement.
		// If in 'end' state, state will change to 'last' if source is not empty.
		inline string_iterator& operator--() noexcept { return *this -= 1; }

		// Postfix iteration by increment. Involves copying!
		inline string_iterator operator++( int ) noexcept { string_iterator _this = *this; ++*this; return _this; }

		// Postfix iteration by decrement. Involves copying!
		inline string_iterator operator--( int ) noexcept { string_iterator _this = *this; *this -= 1; return _this; }


		// Move forward a specific number of UTF-8 characters.
		// Uses the prefix increment operator unless source is all bytes, in which case an optimized increment is used.
		inline string_iterator& operator+=( index_t num_chars ) noexcept {
			if( bytesOnly() ) _advanceBytes( num_chars );
				else { for( index_t i = 0; i < num_chars && *this; ++i, ++*this ); } return *this; }

		// Move backward a specific number of UTF-8 characaters.
		// Uses the prefix decrement operator.
		string_iterator& operator-=( index_t num_chars ) noexcept;


		// Move forward (positive) or backward (negative) a specific number of UTF-8 characters.
		inline string_iterator& operator+=( long_t num_chars ) noexcept {
			return num_chars >= 0 ? *this += static_cast<index_t>( num_chars )
				: *this -= static_cast<index_t>( -num_chars ); }

		// Move forward (positive) or backward (negative) a specific number of UTF-8 characters.
		inline string_iterator& operator+=( int num_chars ) noexcept {
			return num_chars >= 0 ? *this += static_cast<index_t>( num_chars )
				: *this -= static_cast<index_t>( -num_chars ); }

		// Move backward (positive) or forward (negative) a specific number of UTF-8 characters.
		inline string_iterator& operator-=( long_t num_chars ) noexcept {
			return num_chars >= 0 ? *this -= static_cast<index_t>( num_chars )
				: *this += static_cast<index_t>( -num_chars ); }

		// Move backward (positive) or forward (negative) a specific number of UTF-8 characters.
		inline string_iterator& operator-=( int num_chars ) noexcept {
			return num_chars >= 0 ? *this -= static_cast<index_t>( num_chars )
				: *this += static_cast<index_t>( -num_chars ); }


		// Get a copy of the specified iterator moved forward by the specified number of UTF-8 characters.
		inline friend string_iterator operator+( const string_iterator& itr, index_t num ) noexcept {
			return string_iterator( itr ) += num; }

		// Get a copy of the specified iterator moved forward (or backward if negative)
		// by the specified number of UTF-8 characters.
		inline friend string_iterator operator+( const string_iterator& itr, long_t num ) noexcept {
			return string_iterator( itr ) += num; }

		// Get a copy of the specified iterator moved forward (or backward if negative)
		// by the specified number of UTF-8 characters.
		inline friend string_iterator operator+( const string_iterator& itr, int num ) noexcept {
			return string_iterator( itr ) += num; }


		// Get number of UTF-8 characters between two iterators of the same source.
		// Throws [eon::WrongSource] if different sources!
		friend long_t operator-( const string_iterator& lhs, const string_iterator& rhs );

		// Get a copy of the specified iterator moved backward by the specified number of UTF-8 characters.
		inline friend string_iterator operator-( const string_iterator& itr, index_t num ) noexcept {
			return string_iterator( itr ) -= num; }

		// Get a copy of the specified iterator moved backward (or forward if negative)
		// by the specified number of UTF-8 characters.
		inline friend string_iterator operator-( const string_iterator& itr, long_t num ) noexcept {
			return string_iterator( itr ) -= num; }

		// Get a copy of the specified iterator moved backward (or forward if negative)
		// by the specified number of UTF-8 characters.
		inline friend string_iterator operator-( const string_iterator& itr, int num ) noexcept {
			return string_iterator( itr ) -= num; }




		///////////////////////////////////////////////////////////////////////
		//
		// Comparison
		//
	public:

		// Compare against another iterator by comparing source positions.
		// Returns -1 if 'this' occurs before 'other', 0 if same, and 1 if after.
		// NOTE: If different sources, then source (start) addresses are compared!
		// The following rule applies when neither iterator is 'invalid': 'rend' < 'first' < 'last' < 'end'.
		int compare( const string_iterator& other ) const noexcept;

		// Check if 'this' comes before 'other' using [compare].
		inline friend bool operator<( const string_iterator& a, const string_iterator& b ) noexcept {
			return a.compare( b ) < 0; }

		// Check if 'this' comes before or same as 'other' using [compare].
		inline friend bool operator<=( const string_iterator& a, const string_iterator& b ) noexcept {
			return a.compare( b ) <= 0; }

		// Check if 'this' comes after 'other' using [compare].
		inline friend bool operator>( const string_iterator& a, const string_iterator& b ) noexcept {
			return a.compare( b ) > 0; }

		// Check if 'this' comes after or same as 'other' using [compare].
		inline friend bool operator>=( const string_iterator& a, const string_iterator& b ) noexcept {
			return a.compare( b ) >= 0; }

		// Check if 'this' is at same position as 'other' using [compare].
		inline friend bool operator==( const string_iterator& a, const string_iterator& b ) noexcept {
			return a.compare( b ) == 0; }

		// Check if 'this' is at different position from 'other' using [compare].
		inline friend bool operator!=( const string_iterator& a, const string_iterator& b ) noexcept {
			return a.compare( b ) != 0; }




		///////////////////////////////////////////////////////////////////////
		//
		// Static utility methods
		//
	public:

		// Given a raw byte string with a 'start' and an 'end' position, set
		// 'codepoint' to the first actual codepoint at or after 'start' and
		// return the number of bytes it occupies. Zero if not invalid unicode.
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

		const char* Pos{ nullptr };			// Pointer to current character.
		index_t NumChar{ 0 };				// UTF-8 position within source.

		const char* Source{ nullptr };		// Pointer to first byte in source.
		const char* SourceEnd{ nullptr };	// Pointer to end of source.
		index_t NumSourceChars{ 0 };		// Number of UTF-8 characters in source.

		char_t Codepoint{ nochar };			// Translated code point at 'Pos'.
		charsize_t CodepointSize{ 0 };		// Number of bytes in code point.

		bool ValidUTF8{ true };				// Confirmation on whether All valid UTF-8 characters?

		// Best friends with [eon::substring]!
		friend class substring;
	};
}
