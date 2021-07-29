#pragma once
#include "UniChar.h"
#include <memory>
#include <codecvt>
#include <iostream>
#include <functional>
#include <exception>
#include <string.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
#ifndef _DEBUG
	using charsize_t = uint8_t;
#else
	using charsize_t = uint16_t;
#endif

	/**************************************************************************
	  Exception used when two iterators are supposed to be for the same source
	  string are not, or an interator is supposed to be for a specific source
	  string, is not.
	**************************************************************************/
	class WrongSource : public std::exception
	{
	private:
		std::string Message;
	public:
		WrongSource() : std::exception() {}
		WrongSource( const char* message ) : std::exception() {
			Message = message; }
		WrongSource( const std::string& message ) : std::exception() {
			Message = message; }
		WrongSource( std::string&& message ) noexcept : std::exception() {
			Message = std::move( message ); }
		const char* what() const noexcept override { return Message.c_str(); }
	};

	/**************************************************************************
	  Exception used when UTF-8 is required, but the source string contains one
	  or more invalid characters.
	**************************************************************************/
	class InvalidUTF8 : public std::exception
	{
	private:
		std::string Message;
	public:
		InvalidUTF8() : std::exception() {}
		InvalidUTF8( const char* message ) : std::exception() {
			Message = message; }
		InvalidUTF8( const std::string& message ) : std::exception() {
			Message = message; }
		InvalidUTF8( std::string&& message ) noexcept : std::exception() {
			Message = std::move( message ); }
		const char* what() const noexcept override { return Message.c_str(); }
	};



	/**************************************************************************
	  Eon UTF-8 Capable String Iterator Class - eon::string_iterator

	  While the iterator is primarily purposed for [eon::string] use, it can be
	  applied to both 'std::string' and old 'C-strings'. On its own, it does
	  not require the source string to be UTF-8 (while [eon::string] does!).
	  (Though, if you intend to iterate a non-UTF-8 string, you are probably
	  better off using 'std::string::iterator' etc.)

	  WARNING: A 'string_iterator' is only valid for as long as the source
	           string exists in memory and remains unaltered!
			   Deleting the source string will almost certainly cause nasty
			   things to happen and altering it will almost certainly cause it
			   to malfunction!
	**************************************************************************/
	class string_iterator
	{
	public:
		using iterator_category = std::input_iterator_tag;
		using value_type = char_t;
		using difference_type = char_t;
		using pointer = const char_t*;
		using reference = char_t;

		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		//* Construct an unattached, invalid iterator (typically as a place-
		//* holder)
		string_iterator() = default;

		//* Construct as a copy of another iterator
		inline string_iterator( const string_iterator& other ) {
			*this = other; }


		//* Construct for an 'std::string'
		//* NOTE: If the string contains invalid UTF-8 characters, it will be
		//*       treated as a pure C string with byte characters only!
		inline explicit string_iterator( const std::string& stdstr ) noexcept {
			_prep( stdstr.c_str(), stdstr.c_str() + stdstr.size(),
				stdstr.c_str() ); }

		//* Construct for an 'std::string', but set to a specific character
		//* position
		//* NOTE: If the string contains invalid UTF-8 characters, it will be
		//*       treated as a pure C string with byte characters only!
		inline string_iterator( const std::string& stdstr, size_t pos )
			noexcept { _prep( stdstr.c_str(), stdstr.c_str() + stdstr.size(),
				stdstr.c_str() + pos ); }


		//* Construct for a 'C-string'
		//* NOTE: If the string contains invalid UTF-8 characters, it will be
		//*       treated as a pure C string with byte characters only!
		inline explicit string_iterator( const char* cstr ) noexcept {
			_prep( cstr, cstr + strlen( cstr ), cstr ); }

		//* Construct for a 'C-string'
		//* NOTE: If the string contains invalid UTF-8 characters, it will be
		//*       treated as a pure C string with byte characters only!
		inline explicit string_iterator( const unsigned char* cstr ) noexcept {
			_prep( (const char*)cstr, (const char*)cstr + strlen(
				(const char*)cstr ), (const char*)cstr ); }

		//* Construct for a 'C-string', but set to a specific byte position
		//* NOTE: If the string contains invalid UTF-8 characters, it will be
		//*       treated as a pure C string with byte characters only!
		inline string_iterator( const char* cstr, size_t size ) noexcept {
			_prep( cstr, cstr + size, cstr ); }


		//* Construct from already known details, position at start of source
		//* Fast, does not have to count any UTF-8 characters!
		//* WARNING: Setting invalid details may likely result in undefined
		//*          behavior!
		//* Parameters:
		//*   - begin     : Start of source string
		//*   - num_bytes : Total number of bytes in source string
		//*   - num_chars : Total number of UTF-8 characters in source string
		inline string_iterator( const char* begin, size_t num_bytes,
			size_t num_chars ) noexcept { NumChars = num_chars; _prep(
				begin, begin + num_bytes, begin ); }
		
		//* Construct from already known details, position specified
		//* Slow, has to count UTF-8 characters!
		//* WARNING: Setting invalid details may likely result in undefined
		//*          behavior!
		//* Parameters:
		//*   - begin     : Start of source string
		//*   - num_bytes : Total number of bytes in source string
		//*   - pos       : Iterator position in source string
		inline string_iterator( const char* begin, size_t num_bytes,
			const char* pos ) noexcept {
			_prep( begin, begin + num_bytes, pos ); }

		//* Construct from already known details, position specified
		//* Slowish, does not have to count all UTF-8 characters, only up to
		//* 'pos'!
		//* WARNING: Setting invalid details may likely result in undefined
		//*          behavior!
		//* Parameters:
		//*   - begin     : Start of source string
		//*   - num_bytes : Total number of bytes in source string
		//*   - num_chars : Total number of UTF-8 characters in source string
		//*   - pos       : Iterator position in source string
		inline string_iterator( const char* begin, size_t num_bytes,
			size_t num_chars, const char* pos ) noexcept {
			NumChars = num_chars; _prep( begin, begin + num_bytes, pos ); }

		//* Construct from already known details, position specified
		//* Fast, does not have to count any UTF-8 characters!
		//* WARNING: Setting invalid details may likely result in undefined
		//*          behavior!
		//* Parameters:
		//*   - begin     : Start of source string
		//*   - num_bytes : Total number of bytes in source string
		//*   - num_chars : Total number of UTF-8 characters in source string
		//*   - pos       : Iterator position in source string
		//*   - num_char  : UTF-8 character number of iterator position
		inline string_iterator( const char* begin, size_t num_bytes,
			size_t num_chars, const char* pos, size_t num_char ) noexcept {
			NumChars = num_chars; NumChar = num_char; _prep(
				begin, begin + num_bytes, pos ); }

		//* Construct using another 'string_iterator' as 'source', but set a
		//* specific position
		//* Fast, does not have to count any UTF-8 characters!
		//* WARNING: Setting invalid details may likely result in undefined
		//*          behavior!
		//* Parameters:
		//*   - source   : 'string_iterator' to copy source details from
		//*   - pos      : New iterator position
		//*   - num_char : UTF-8 character number of new iterator position
		string_iterator( const string_iterator& source, const char* pos,
			size_t num_char ) noexcept;


		//* Default destructor
		virtual ~string_iterator() = default;



		/**********************************************************************
		  Modifier Methods
		**********************************************************************/
	public:

		//* Copy another 'string_iterator'
		string_iterator& operator=( const string_iterator& other ) noexcept;

		//* Set from 'std::string'
		//* Will use UTF-8 iteration if no invalid UTF-8 characters, if not,
		//* byte sized characters are assumed.
		inline string_iterator& operator=( const std::string& stdstr )
			noexcept { _prep( stdstr.c_str(), stdstr.c_str() + stdstr.size(),
				stdstr.c_str() ); return *this; }

		//* Reset string_iterator to default constructor state
		string_iterator& reset() noexcept;

		//* Reset string_iterator to 'begin' position
		string_iterator& resetToBegin() noexcept;

		//* Reset string_iterator to 'end' position
		string_iterator& resetToEnd() noexcept;



		/**********************************************************************
		  Read methods
		**********************************************************************/
	public:

		//* Check if the 'string_iterator' is valid
		//* NOTE: End-of-string iterators are NOT VALID, but can be decremented
		//        once to become valid - if the source string isn't empty!
		inline operator bool() const noexcept { return CodepointSize > 0; }

		//* Check if the iterator is at reverse end of string (before first)
		inline bool atREnd() const noexcept {
			return CodepointSize == 0 && Pos == Begin; }

		//* Check if the iterator is at the end of the string (after last)
		inline bool atEnd() const noexcept {
			return CodepointSize == 0 && Pos > Begin; }


		//* Check if this iterator is for a string starting at the specified
		//* raw buffer address
		inline bool sameBuffer( const char* buffer_start ) const noexcept {
			return Begin == buffer_start; }

		//* Check if compatible with other iterator (same source string)
		inline bool compatibleWith( const string_iterator& other )
			const noexcept { return sameBuffer( other.Begin ); }

		//* Assert compatible iterators
		//* Throws [eon::WrongSource] if not.
		inline void assertCompatibleWith( const string_iterator& other )
			const { if( Begin != other.Begin ) throw WrongSource(); }

		//* Assert that iterator is for a source string starting at the
		//* specified raw buffer address
		// Throws [eon::WrongSource] if not.
		inline void assertSameBuffer( const char* begin ) const {
			if( Begin != begin ) throw WrongSource(); }


		//* Get the codepoint at 'this'  position
		inline char_t operator*() const noexcept { return Codepoint; }

		//* Get character position in source string
		//* For UTF-8 strings, this will be the number of UTF-8 characters.
		//* For plain strings, the number of bytes.
		inline size_t numChar() const noexcept { return NumChar; }

		//* Get byte position in source string
		inline size_t numByte() const noexcept { return Pos - Begin; }


		//* Check if the entire string is byte characters only
		//* (Allows for optimizations.)
		inline bool bytesOnly() const noexcept {
			return NumChars == End - Begin; }

		//* Check if the string contains only valid UTF-8 characters
		//* Will return 'true' if ASCII only, but not if there are any invalid
		//* UTF-8 characters.
		inline bool validUTF8() const noexcept { return ValidUTF8; }


		//* Get raw byte data starting at the iterator position
		inline const char* byteData() const noexcept { return Pos; }

		//* Get total number of (UTF-8 or byte-sized) characters in string
		inline size_t numChars() const noexcept { return NumChars; }

		//* Get a copy of the iterator, but reset to end
		inline string_iterator getEnd() const noexcept { if( !*this )
			return *this; auto end = *this; return end.resetToEnd(); }

		//* Get a copy of the iterator, but reset to the beginning
		inline string_iterator getBegin() const noexcept { if( !*this )
			return *this; auto beg = *this; return beg.resetToBegin(); }



		/**********************************************************************
		  Iteration
		**********************************************************************/
	public:

		//* Prefix iteration by increment
		string_iterator& operator++() noexcept;

		//* Prefix iteration by decrement
		inline string_iterator& operator--() noexcept { return *this -= 1; }

		//* Postfix iteration by increment
		inline string_iterator operator++( int ) noexcept {
			auto _this = *this; ++*this; return _this; }

		//* Postfix iteration by decrement
		inline string_iterator operator--( int ) noexcept {
			auto _this = *this; *this -= 1; return _this; }


		//* Move forward a number of characters
		inline string_iterator& operator+=( size_t chars ) noexcept {
			if( bytesOnly() ) _advanceBytes( chars ); else {
				for( size_t i = 0; i < chars && *this; ++i, ++ * this ); }
			return *this; }

		//* Move backward a number of characaters
		string_iterator& operator-=( size_t chars ) noexcept;

		
		//* Multiple characters in one go, possibly negative (counting from end
		//* of the source string)
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

		//* Get number of characters in difference between two iterators
		//* Throws [eon::WrongSource] if incompatible iterators.
		friend int64_t operator-( const string_iterator& lhs,
			const string_iterator& rhs );

		//* Get a copy of 'itr', moved forwards or backwards by 'num'
		//* characters
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



		/**********************************************************************
		  Comparison
		**********************************************************************/
	public:

		//* Compare against another 'string_iterator'
		//* Returns -1 if 'this' is 'before' 'other', 0 if at the same position
		//* and 1 if 'this' is after 'other'
		//* NOTE: No compatibility checking is done!
		int compare( const string_iterator& other ) const noexcept;

		//* Comparison operators are using the [compare] method
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



		/**********************************************************************
		  Static utility methods
		**********************************************************************/
	public:

		//* Given a raw byte string with a 'start' and an 'end' position, set
		//* 'cp' to the next codepoint and return the number of bytes it
		//* occupies (zero if not a valid unicode code point)
		static size_t bytesToUnicode( const char* start, const char* end,
			char_t& cp );

		//* Given a codepoint, convert it into bytes
		//* The bytes are stored in the 'bytes' uint32_t, up to 4 of them
		//* Returns the number of bytes used
		//* Throws [eon::InvalidUTF8] if not a valid code point
		static size_t unicodeToBytes( char_t cp, uint32_t& bytes );

		//* Run as part of a UTF-8 decoding algorithm
		//* WARNING: Only use this if you really know what you are doing!
		//* See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details about
		//* the UTF-8 decoder algorithm used here.
		static char32_t utf8Decode( char32_t& state, char32_t& codep,
			char32_t byte ) noexcept;

		//* Count the number of UTF-8 characters in the given string
		//* Throws [eon::InvalidUTF8] if not valid!
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
		char_t Codepoint{ nochar };			// Translated code point
		charsize_t CodepointSize{ 0 };		// Number of bytes in code point
		size_t NumChar{ 0 };				// Character position from start
		size_t NumChars{ 0 };				// Total number of characters
		bool ValidUTF8{ true };				// All valid UTF-8 characters?


		friend class substring;
	};
}
