#pragma once
#include "UniChar.h"
#include <memory>
#include <codecvt>
#include <iostream>
#include <functional>
#include <exception>

/*
	UTF-8 String string_iterator Class. (Can also iterate bytes if the string
	contains invalid UTF-8 characters.)

	This iterator is used by eon::string, but can also be used to iterate
	std::string and const char* (C string).

	WARNING: A string_iterator is only valid for as long as the underlying
	         string remains unchanged since the time of iterator construction.
			 If the string is deleted or modified in any way, the iterator
			 behavior becomes undefined!
*/



namespace eon
{
#ifndef _DEBUG
	using charsize_t = uint8_t;
#else
	using charsize_t = uint16_t;
#endif

	// Exception thrown if thow iterators are supposed to be for the same
	// source string, but aren't.
	class WrongSource : public std::exception
	{
	public:
		WrongSource() : std::exception() {}
		WrongSource( const char* message ) : std::exception( message ) {}
	};

	// Exception thrown when UTF-8 is required, but the source string contains
	// one or more invalid UTF-8 characters.
	class InvalidUTF8 : public std::exception
	{
	public:
		InvalidUTF8() : std::exception() {}
		InvalidUTF8( const char* message ) : std::exception( message ) {}
	};


	class string_iterator
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:
		string_iterator() = default;
		inline string_iterator( const string_iterator& other ) {
			*this = other; }

		// Construct from std::string
		inline explicit string_iterator( const std::string& stdstr ) noexcept {
			_prep( stdstr.c_str(), stdstr.c_str() + stdstr.size(),
				stdstr.c_str() ); }
		inline string_iterator( const std::string& stdstr, size_t pos )
			noexcept { _prep( stdstr.c_str(), stdstr.c_str() + stdstr.size(),
				stdstr.c_str() + pos ); }

		// Construct from c-string
		// NOTE: If the string contains invalid UTF-8 characters, it will be
		// treated as a pure C string with byte characters only!
		inline explicit string_iterator( const char* cstr ) noexcept {
			_prep( cstr, cstr + strlen( cstr ), cstr ); }
		inline explicit string_iterator( const unsigned char* cstr ) noexcept {
			_prep( (const char*)cstr, (const char*)cstr + strlen(
				(const char*)cstr ), (const char*)cstr ); }
		inline string_iterator( const char* cstr, size_t size ) noexcept {
			_prep( cstr, cstr + size, cstr ); }

		// Construct from already known details
		// Undefined behaviour from string_iterator if details are not valid!
		inline string_iterator( const char* begin, size_t num_bytes,
			size_t num_chars ) noexcept { NumChars = num_chars; _prep(
				begin, begin + num_bytes, begin ); }
		inline string_iterator( const char* begin, size_t num_bytes,
			const char* pos ) noexcept {
			_prep( begin, begin + num_bytes, pos ); }
		inline string_iterator( const char* begin, size_t num_bytes,
			size_t num_chars, const char* pos ) noexcept {
			NumChars = num_chars; _prep( begin, begin + num_bytes, pos ); }
		inline string_iterator( const char* begin, size_t num_bytes,
			size_t num_chars, const char* pos, size_t num_char ) noexcept {
			NumChars = num_chars; NumChar = num_char; _prep(
				begin, begin + num_bytes, pos ); }
		string_iterator( const string_iterator& source, const char* pos,
			size_t num_char ) noexcept;

		virtual ~string_iterator() = default;



		///////////////////////////////////////////////////////////////////////
		//
		// Modifiers
		//
	public:

		// Copy another string_iterator
		string_iterator& operator=( const string_iterator& other ) noexcept;

		// Set from std::string - will be UTF-8 if no invalid UTF-8 characters
		inline string_iterator& operator=( const std::string& stdstr )
			noexcept { _prep( stdstr.c_str(), stdstr.c_str() + stdstr.size(),
				stdstr.c_str() ); return *this; }

		// Reset string_iterator to default construction state
		string_iterator& reset() noexcept;

		// Reset string_iterator to 'begin' position
		string_iterator& resetToBegin() noexcept;

		// Reset string_iterator to 'end' position
		string_iterator& resetToEnd() noexcept;



		///////////////////////////////////////////////////////////////////////
		//
		// Read methods
		//
	public:

		// Is this a valid string_iterator
		// NOTE: End-of-string iterators are NOT VALID, but can be decremented
		//       once to become valid - if the string isn't empty!
		inline operator bool() const noexcept { return CodepointSize > 0; }

		// Check if the iterator is at reverse end of string (before first)
		inline bool atREnd() const noexcept {
			return CodepointSize == 0 && Pos == Begin; }

		// Check if the iterator is at the end of the string (after last)
		inline bool atEnd() const noexcept {
			return CodepointSize == 0 && Pos > Begin; }


		// Check if this iterator is for a string starting at the specified raw
		// buffer address.
		inline bool sameBuffer( const char* buffer_start ) const noexcept {
			return Begin == buffer_start; }

		// Check if compatible with other iterator (same string)
		inline bool compatibleWith( const string_iterator& other )
			const noexcept { return sameBuffer( other.Begin ); }

		// Assert compatible iterators
		// Throws WrongSource if not
		inline void assertCompatibleWith( const string_iterator& other )
			const { if( Begin != other.Begin ) throw WrongSource(); }

		// Assert that iterator is for a string starting at the specified raw
		// buffer address.
		// Throws WrongSource if not
		inline void assertSameBuffer( const char* begin ) const {
			if( Begin != begin ) throw WrongSource(); }

		// Get the codepoint at this string_iterator position
		inline eon_char operator*() const noexcept { return Codepoint; }

		// Character position in string
		inline size_t numChar() const noexcept { return NumChar; }

		// Byte position in string
		inline size_t numByte() const noexcept { return Pos - Begin; }


		// Check if the entire string is byte characters only
		// (Allows for optimizations.)
		inline bool bytesOnly() const noexcept {
			return NumChars == End - Begin; }

		// Check if the string contains only valid UTF-8 characters
		inline bool validUTF8() const noexcept { return ValidUTF8; }


		// Get raw byte data starting at the iterator position
		inline const char* byteData() const noexcept { return Pos; }

		// Get total number of characters in string
		inline size_t numChars() const noexcept { return NumChars; }

		// Get a copy of the iterator, but reset to end
		inline string_iterator getEnd() const noexcept { if( !*this )
			return *this; auto end = *this; return end.resetToEnd(); }



		///////////////////////////////////////////////////////////////////////
		//
		// Iteration
		//
	public:

		// Prefix iteration
		string_iterator& operator++() noexcept;
		inline string_iterator& operator--() noexcept { return *this -= 1; }

		// Postfix iteration
		inline string_iterator operator++( int ) noexcept {
			auto _this = *this; ++*this; return _this; }
		inline string_iterator operator--( int ) noexcept {
			auto _this = *this; *this -= 1; return _this; }

		// Multiple characters in one go
		inline string_iterator& operator+=( size_t chars ) noexcept {
			if( bytesOnly() ) _advanceBytes( chars ); else {
				for( size_t i = 0; i < chars && *this; ++i, ++ * this ); }
			return *this; }
		string_iterator& operator-=( size_t chars ) noexcept;
		
		// Multiple characters in one go, possibly negative
		inline string_iterator& operator+=( int64_t chars ) noexcept {
			return chars >= 0 ? *this += static_cast<size_t>( chars )
				: *this -= static_cast<size_t>( -chars ); }
		inline string_iterator& operator+=( int chars ) noexcept {
			return chars >= 0 ? *this += static_cast<size_t>( chars )
				: *this -= static_cast<size_t>( -chars ); }
		inline string_iterator& operator-=( int64_t chars ) noexcept {
			return chars >= 0 ? *this -= static_cast<size_t>( chars )
				: *this += static_cast<size_t>( -chars ); }
		inline string_iterator& operator-=( int chars ) noexcept {
			return chars >= 0 ? *this -= static_cast<size_t>( chars )
				: *this += static_cast<size_t>( -chars ); }

		// Get number of characters in difference between two iterators
		friend int64_t operator-( const string_iterator& lhs,
			const string_iterator& rhs );

		// Get a copy of the string_iterator, advanced or retreated
		inline friend string_iterator operator+( const string_iterator& itr,
			size_t num ) noexcept { return string_iterator( itr ) += num; }
		inline friend string_iterator operator+( const string_iterator& itr,
			int64_t num ) noexcept { return string_iterator( itr ) += num; }
		inline friend string_iterator operator+( const string_iterator& itr,
			int num ) noexcept { return string_iterator( itr ) += num; }
		inline friend string_iterator operator-( const string_iterator& itr,
			size_t num ) noexcept { return string_iterator( itr ) -= num; }
		inline friend string_iterator operator-( const string_iterator& itr,
			int64_t num ) noexcept { return string_iterator( itr ) -= num; }
		inline friend string_iterator operator-( const string_iterator& itr,
			int num ) noexcept { return string_iterator( itr ) -= num; }



		///////////////////////////////////////////////////////////////////////
		//
		// Comparison
		//
	public:

		int compare( const string_iterator& other ) const noexcept;

		inline bool operator<( const string_iterator& other ) const noexcept {
			return compare( other ) < 0; }
		inline bool operator<=( const string_iterator& other ) const noexcept {
			return compare( other ) <= 0; }
		inline bool operator>( const string_iterator& other ) const noexcept {
			return compare( other ) > 0; }
		inline bool operator>=( const string_iterator& other ) const noexcept {
			return compare( other ) >= 0; }
		inline bool operator==( const string_iterator& other ) const noexcept {
			return compare( other ) == 0; }
		inline bool operator!=( const string_iterator& other ) const noexcept {
			return compare( other ) != 0; }



		///////////////////////////////////////////////////////////////////////
		//
		// Static tools
		//
	public:

		// Given a raw byte string with a start and an end position, set chr to
		// the next code point and return the number of bytes it occupies (zero
		// if not a valid unicode code point)
		static size_t bytesToUnicode( const char* start, const char* end,
			eon_char& codepoint );

		// Given a unicode code point, convert it into bytes
		// The bytes are stored in the 'bytes' uint32_t, up to 4 of them
		// Returns the number of bytes used
		// Throws InvalidUTF8 if not a valid code point
		static size_t unicodeToBytes( eon_char codepoint, uint32_t& bytes );

		// Only use this if you really know what you are doing!
		static char32_t utf8Decode( char32_t& state, char32_t& codep,
			char32_t byte ) noexcept;

		// Count the number of UTF-8 characters in the given string
		// Throws InvalidUTF8 if not valid!
		static size_t countUtf8Chars( const char* str, size_t size );



		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		void _prep( const char* begin, const char* end, const char* pos )
			noexcept;

		void _utf8CharacterCount() noexcept;

		void _translateCodepoint() noexcept;

		void _advanceBytes( size_t bytes ) noexcept;
		void _retreatBytes( size_t bytes ) noexcept;



		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:
		const char* Pos{ nullptr };			// Address of character in string
		const char* Begin{ nullptr };		// Start of string
		const char* End{ nullptr };			// End of string
		eon_char Codepoint{ NullChr };		// Translated code point
		charsize_t CodepointSize{ 0 };		// Number of bytes in code point
		size_t NumChar{ 0 };				// Character position from start
		size_t NumChars{ 0 };				// Total number of characters
		bool ValidUTF8{ true };				// All valid UTF-8 characters?


		friend class substring;
	};
}
