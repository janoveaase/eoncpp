#pragma once
#include "UniChar.h"
#include "Substring.h"
#include <set>
#include <list>
#include <cstdlib>


// Detect platform
#if defined( WIN32 ) || defined( _WIN32 ) || defined( __WIN32__ ) || defined( __NT__ )
#	define EON_WINDOWS
#elif defined( __APPLE__ ) || defined( __MACH__ )
#	define EON_APPLE
#elif defined( __unix ) || defined( __unix__ )
#	define EON_UNIX
#endif


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	// A reference to a codepoint in a string
	struct Utf8CharRef
	{
		size_t BytePos{ 0 };	// Position in bytes
		size_t CharPos{ 0 };	// Position in utf8chars
	};


	/**************************************************************************
	  Eon UTF-8 String Class - eon::string

	  A fully implemented string class with lots of useful methods for string
	  manipulation.

	  NOTE: Unlike std::string, eon::string will not let you modify characters
	        in-place.
	  NOTE: Direct access to characters is not possible, use iterators for such
		    access.
	  WARNING: Is not thread-safe!

	  Makes heavy use of [eon::string_iterator] and [eon::substring] classes.
	  The use of the latter sets eon::string apart from std::string in that
	  substrings are not real strings, but iterators into the parent string.
	  It is often useful to access the entire string as a substring in order to
	  utilize more specialized methods of eon::substring. The
	  [eon::string::substr()] method will do this, or you can call
	  [eon::substring(string::begin(), string::end())]
	**************************************************************************/
	class string
	{
		/**********************************************************************
		  String Definitions
		**********************************************************************/
	public:

		//* string::iterator is identical to [eon::string_iterator]
		using iterator = string_iterator;




		/**********************************************************************
		  String Construction
		**********************************************************************/
	public:

		//* Construct an empty string
		string() = default;
		
		//* Construct by taking ownership of the underlying string data from
		//* another string
		inline string( string&& other ) noexcept { *this = std::move( other ); }

		//* Construct as a copy of another string
		inline string( const string& other ) { *this = other; }


		//* Construct as a copy of the iterator range from another string
		//* Throws [eon::InvalidUTF8] if not a valid UTF-8!
		inline string( const iterator& beg, const iterator& end ) { *this = substring( beg, end ); }

		//* Construct as a copy of a substring of another string
		//* Throws [eon::InvalidUTF8] if not a valid UTF-8!
		inline string( const substring& other ) { *this = other; }


		//* Construct as a copy of an 'std::string'
		//* Throws [eon::InvalidUTF8] if not a valid UTF-8!
		inline string( const std::string& stdstr ) { *this = stdstr; }

		//* Construct by taking ownership of an 'std::string'
		//* Throws [eon::InvalidUTF8] if not a valid UTF-8!
		inline string( std::string&& stdstr ) { *this = std::move( stdstr ); }

		//* Construct as a copy of a C string
		//* Throws [eon::InvalidUTF8] if not a valid UTF-8!
		inline string( const char* cstr ) { *this = cstr; }

		//* Construct as a copy of a character buffer of a specified size
		//* Throws [eon::InvalidUTF8] if not a valid UTF-8!
		inline string( const char* buffer, size_t size ) { *this = std::string( buffer, size ); }


		//* Construct from a single [[eon::char_t][Unicode codepoint]]
		//* Throws [eon::InvalidUTF8] if not a valid codepoint!
		inline explicit string( char_t cp ) { *this = cp; }

		//* Construct from a single signed character
		//* Throws [eon::InvalidUTF8] if not a valid UTF-8 character!
		inline explicit string( char c ) { *this = static_cast<char_t>( c ); }

		//* Construct from a single unsigned character
		//* Throws [eon::InvalidUTF8] if not a valid UTF-8 character!
		inline explicit string( unsigned char c ) { *this = static_cast<char_t>( c ); }


		//* Construct from literal list of
		//* [[eon::char_t][Unicode codepoints]]
		//* Throws eon::[eon::InvalidUTF8] if not a valid code points!
		inline string( std::initializer_list<char_t> codepoints ) { *this = codepoints; }

		//* Construct from vector of Unicode codepoints
		//* Throws eon::[eon::InvalidUTF8] if not a valid code points!
		inline string( const std::vector<char_t>& codepoints ) { *this = codepoints; }

		//* Construct from literal list of signed characters
		//* Throws [eon::InvalidUTF8] if not a valid UTF-8 characters!
		inline string( std::initializer_list<char> chars ) { *this = chars; }

		//* Construct from literal list of unsigned characters
		//* Throws [eon::InvalidUTF8] if not a valid UTF-8 characters
		inline string( std::initializer_list<unsigned char> chars ) { *this = chars; }


		//* Construct as a number of copies of the specified codepoint
		//* Throws [eon::InvalidUTF8] if not a valid codepoint
		inline string( size_t copies, char_t cp ) { assign( copies, cp ); }

		//* Construct as a number of copies of another string
		inline string( size_t copies, const string& other ) { assign( copies, other ); }

		//* Construct as a number of copies of the specified std::string
		//* Throws [eon::InvalidUTF8] if not valid UTF-8
		inline string( size_t copies, const std::string& stdstr ) { assign( copies, stdstr ); }

		//* Construct as a number of copies of the specified [eon::substring]
		//* Throws [eon::InvalidUTF8] if not valid UTF-8
		inline string( size_t copies, const substring& other ) { assign( copies, other ); }


		//* Construct as "true" or "false" based on boolean input value
		inline explicit string( bool value ) { *this = value; }


		/** Construct as conversion from number **/
		inline explicit string( long value ) { *this = toString( value ); }
		inline explicit string( unsigned long value ) { *this = toString( value ); }
#ifdef EON_WINDOWS
		inline explicit string( int16_t value ) { *this = toString( value ); }
		inline explicit string( uint16_t value ) { *this = toString( value ); }
		inline explicit string( int32_t value ) { *this = toString( value ); }
		inline explicit string( uint32_t value ) { *this = toString( value ); }
		inline explicit string( int64_t value ) { *this = toString( value ); }
		inline explicit string( uint64_t value ) { *this = toString( value ); }
#else
		inline explicit string( short value ) { *this = toString( value ); }
		inline explicit string( unsigned short value ) { *this = toString( value ); }
		inline explicit string( int value ) { *this = toString( value ); }
		inline explicit string( unsigned int value ) { *this = toString( value ); }
		inline explicit string( int8_t value ) { *this = toString( value ); }
		inline explicit string( long long unsigned int value ) { *this = toString( value ); }
#endif
#ifdef EON_APPLE
		inline explicit string( int64_t value ) { *this = toString( value ); }
#endif
		inline explicit string( float value ) { *this = toString( static_cast<double>( value ) ); }
		inline explicit string( double value ) { *this = toString( value ); }
		inline explicit string( long double value ) { *this = toString( value ); }


		//* Default destructor
		virtual ~string() = default;




		/**********************************************************************
		  String Assignment Metods
		**********************************************************************/
	public:

		/** The 'assign' Method **/

		//* Assign from array of codepoints
		//* Throws [eon::InvalidUTF8] if not valid codepoints
		string& assign( const char_t* cp_array, size_t cp_array_size );

		//* Assign from array of characters
		//* Throws [eon::InvalidUTF8] if not valid UTF-8
		string& assign( const char* chr_array, size_t chr_array_size );


		//* Assign the specified number of copies of the codepoint
		//* Throws [eon::InvalidUTF8] if not valid codepoint
		string& assign( size_t copies, char_t cp );

		//* Assign the specified number of copies of the character
		//* Throws [eon::InvalidUTF8] if not valid UTF-8
		inline string& assign( size_t copies, char chr ) {
			return assign( copies, static_cast<char_t>( chr ) ); }

		//* Assign the specified number of copies of the other string
		string& assign( size_t copies, const string& other );

		//* Assign the specified number of copies of the std::string
		//* Throws [eon::InvalidUTF8] if not valid UTF-8
		string& assign( size_t copies, const std::string& stdstr );

		//* Assign the specified number of copies of the substring
		//* Throws [eon::InvalidUTF8] if not valid UTF-8
		string& assign( size_t copies, const substring& sub );


		/** Assignment Operator **/

		//* Assign a copy of another string
		inline string& operator=( const string& other ) { Bytes = other.Bytes; NumChars = other.NumChars; return *this; }

		//* Take ownership of the underlying string data from another string
		inline string& operator=( string&& other ) noexcept {
			Bytes = std::move( other.Bytes ); NumChars = other.NumChars; other.NumChars = 0; return *this; }


		//* Assign from a substring
		//* Throws [eon::InvalidUTF8] if not valid UTF-8
		string& operator=( const substring& sub );


		//* Assign from an 'std::string'
		//* Throws [eon::InvalidUTF8] if not valid UTF-8
		inline string& operator=( const std::string& stdstr ) { return assign( stdstr.c_str(), stdstr.size() ); }

		//* Assign by taking ownership of an 'std::string'
		//* Throws [eon::InvalidUTF8] if not valid UTF-8
		string& operator=( std::string&& stdstr );

		//* Assign from a C string
		//* Throws [eon::InvalidUTF8] if not valid UTF-8
		inline string& operator=( const char* cstr ) { return assign( cstr, strlen( cstr ) ); }

		//* Assign from a single codepoint
		//* Throws [eon::InvalidUTF8] if not valid codepoint
		inline string& operator=( char_t cp ) {
			uint32_t bytes; auto num = iterator::unicodeToBytes( cp, bytes ); return assign( (const char*)&bytes, num ); }

		//* Assign from a single signed character
		//* Throws [eon::InvalidUTF8] if not valid UTF-8
		inline string& operator=( char c ) { return *this = static_cast<char_t>( c ); }

		//* Assign from a single unsigned character
		//* Throws [eon::InvalidUTF8] if not valid UTF-8
		inline string& operator=( unsigned char c ) { return *this = static_cast<char_t>( c ); }


		//* Assign from a literal list of codepoints
		//* Throws [eon::InvalidUTF8] if not valid codepoints
		string& operator=( const std::initializer_list<char_t>& codepoints );

		//* Assign from a vector of codepoints
		//* Throws [eon::InvalidUTF8] if not valid codepoints
		string& operator=( const std::vector<char_t>& codepoints );

		//* Assign from a liter list of signed characters
		//* Throws [eon::InvalidUTF8] if not valid UTF-8
		string& operator=( const std::initializer_list<char>& chars );

		//* Assign from a liter list of unsigned characters
		//* Throws [eon::InvalidUTF8] if not valid UTF-8
		string& operator=( const std::initializer_list<unsigned char>& chars );


		//* Assign as "true" or "false" based on the boolean value
		inline string& operator=( bool value ) { return *this = value ? "true" : "false"; }
		



		/**********************************************************************
		  Basic Read-only Methods
		**********************************************************************/
	public:

		//* Get number of unicode codepoints in string
		inline size_t numChars() const noexcept { return NumChars; }
		inline size_t length() const noexcept { return NumChars; }

		//* Get number of bytes in string
		inline size_t numBytes() const noexcept { return Bytes.size(); }

		//* Get buffer size (capacity) in number of bytes
		inline size_t bufferSize() const noexcept { return Bytes.capacity(); }

		//* Check if the string is empty
		inline bool empty() const noexcept { return NumChars == 0; }

		//* Access the underlying bytes as an std::string
		inline const std::string& stdstr() const noexcept { return Bytes; }

		//* Access the underlying bytes as a C-string
		inline const char* c_str() const noexcept { return Bytes.c_str(); }

		//* Access an individual immutable byte
		inline char byte( size_t pos ) const { return Bytes[ pos ]; }


		//* Check if the string is surrounded by double quotation marks
		inline bool isDoubleQuoted() const noexcept { return substr().enclosed( DblQuoteChr ); }

		//* Check if a substring of 'this' is surrounded by double quotation
		//* marks
		inline bool isDoubleQuoted( const substring& sub ) const noexcept { return sub.enclosed( DblQuoteChr ); }

		//* Check if the string is surrounded by single quotation marks
		inline bool isSingleQuoted() const noexcept { return substr().enclosed( SglQuoteChr ); }

		//* Check if a substring of 'this' is surrounded by single quotation
		//* marks
		inline bool isSingleQuoted( const substring& sub ) const noexcept { return sub.enclosed( SglQuoteChr ); }


		//* Check if the string is 'blank' (only whitespaces)
		inline bool blank() const noexcept { return substr().blank(); }

		//* Get number of indentations for the first (and possibly only) line
		//* of text in the string
		inline size_t indentationLevel( char_t indentation_char = TabChr ) const noexcept {
			return substr().indentationLevel(); }


		//* Get string as codepoints in a vector
		inline std::vector<char_t> chars() const { return substr().chars(); }

		//* Get substring as codepoints in a vector
		inline std::vector<char_t> chars( const substring& sub ) const { return sub.lowToHigh().chars(); }


		//* Assuming the 'encoded_iterator' string is formatted as
		//* "<byte pos>:<char pos>", convert it into an iterator for 'this'
		//* string. Returns [end()] if invalid position!
		//* (See matching [encode(iterator)] method.)
		iterator decodeIterator( const string& encoded_iterator );

		//* Assuming the 'encoded_substring' string is formatted as
		//* "<byte pos>:<char pos>-<byte pos>:<char pos>", convert it into a
		//* substring for 'this' string. Returns invalid substring if it does
		//* not fit 'this' string.
		//* (See matching [encode(substring)] method.)
		substring decodeSubstring( const string& encoded_substring );



		/**********************************************************************
		  Iterator and Iterator Access
		**********************************************************************/
	public:

		//* Get iterator for first character in the string
		//* Returns [end()] if string is empty.
		inline iterator begin() const noexcept { return iterator( Bytes.c_str(), Bytes.size(), NumChars, Bytes.c_str() ); }

		//* Get iterator for end of string (one past the last character)
		inline iterator end() const noexcept {
			return iterator( Bytes.c_str(), Bytes.size(), NumChars, Bytes.c_str() + Bytes.size(), NumChars ); }

		//* Get iterator for the last character in the string
		//* Returns [end()] if string is empty.
		inline iterator last() const noexcept { return NumChars > 0 ? end() - 1 : end(); }


		//* Given a byte position within the string, get the (closest next)
		//* character position in the form of an iterator.
		//* The counting can start from a known position if it is assumed to be
		//* closer than [begin()].
		// Returns [end()] if out of scope!
		iterator bytePos( size_t pos, iterator start = iterator() ) const;


		//* Given an iterator for another string (typically the original, where
		//* 'this' is a moved or modified version), get a matching iterator
		//* into 'this' string.
		//* If the other iterator is totally incompatible, the return value
		//* will be 'end()'.
		//* This method is safe, but involves linear counting!
		iterator rebase( const iterator& other ) const noexcept;

		//* Given a [eon::substring] for another string (typically the
		//* original, where 'this' is a moved or modified version), get a
		//* matching substring with iterators rebased to 'this'.
		//* This method is safe, but involves linear counting!
		inline substring rebase( const substring& other ) const noexcept {
			return substring( rebase( other.begin() ), rebase( other.end() ) ); }

		//* Given an iterator for another string (typically the original, where
		//* 'this' is a moved or modified version), get a matching iterator
		//* into 'this' string.
		//* If the other iterator is totally incompatible, the return value
		//* will be 'end()'.
		//* WARNING:
		//* This method is fast and does not count, but it requires that you
		//* know for sure! that the string of 'other' and that of 'this' are
		//* identical - at least up to and including the codepoint referenced
		//* by the iterator.
		//* Getting this wrong may put the new iterator in an invalid state,
		//* which can lead to undefined behavior when used!
		iterator rebaseMoved( const iterator& other ) const noexcept;

		//* Given a [eon::substring] for another string (typically the
		//* original, where 'this' is a moved or modified version), get a
		//* matching substring with iterators rebased to 'this'.
		//* WARNING:
		//* This method is fast and does not count, but it requires that you
		//* know for sure! that the string of 'other' and that of 'this' are
		//* identical - at least up to and including the codepoint referenced
		//* by the iterator.
		//* Getting this wrong may put the new iterator in an invalid state,
		//* which can lead to undefined behavior when used!
		inline substring rebaseMoved( const substring& other ) const noexcept {
			return substring( rebaseMoved( other.begin() ), rebaseMoved( other.end() ) ); }




		/**********************************************************************
		  Searching

		  Methods searching for substrings will return [eon::substring] objects
		  as result. These will be 'false' for unsuccessful searches.
		**********************************************************************/
	public:

		//* Check if string contains the specified codepoint
		inline bool contains( char_t cp ) const noexcept { return substr().contains( cp ); }

		//* Check if string contains the specified substring
		inline bool contains( const string& sub ) const noexcept { return substr().contains( sub.substr() ); }

		//* Check if string contains any of the charcters in the 'characters'
		//* string
		inline bool containsAnyOf( const string& characters ) const noexcept {
			return substr().containsAnyOf( characters.substr() ); }

		//* Check if string contains any charcters other than the ones in the
		//* 'characters' string
		inline bool containsOtherThan( const string& characters ) const noexcept {
			return substr().containsOtherThan( characters.substr() ); }


		/** Find First Occurrence **/

		//* Find first occurrence of another (equal or shorter) string
		inline substring findFirst( const string& to_find ) const noexcept { return substr().findFirst( to_find.substr() ); }

		//* Find first occurrence of another string, but limit search to the
		//* specified 'sub' [eon::substring].
		inline substring findFirst( const string& to_find, const substring& sub ) const noexcept {
			sub.assertSameBuffer( Bytes.c_str() ); return sub.lowToHigh().findFirst( to_find.substr() ); }

		//* Find first occurrence of a codepoint
		inline substring findFirst( char_t to_find ) const noexcept { return substr().findFirst( to_find ); }

		//* Find first occurrence of a codepoint, but limit search to the
		//* specified 'sub' [eon::substring].
		inline substring findFirst( char_t to_find, const substring& sub ) const noexcept {
			sub.assertSameBuffer( Bytes.c_str() ); return sub.lowToHigh().findFirst( to_find ); }

		//* Find first occurrence of an ASCII character
		inline substring findFirst( char to_find ) const noexcept {
			return substr().findFirst( static_cast<char_t>( to_find ) ); }

		//* Find first occurrence of an ASCII character, but limit search to
		//* the specified 'sub' [eon::substring].
		inline substring findFirst( char to_find, const substring& sub ) const noexcept {
			sub.assertSameBuffer( Bytes.c_str() ); return sub.lowToHigh().findFirst( static_cast<char_t>( to_find ) ); }


		//* Find first occurrence of any character in 'characters'
		inline iterator findFirstOf( const string& characters ) const noexcept {
			return substr().findFirstOf( characters.substr() ); }
		
		//* Find first occurrence of any character in 'characters', but limit
		//* search to the specified 'sub' [eon::substring].
		inline iterator findFirstOf( const string& characters, substring sub ) const noexcept {
			sub.assertSameBuffer( Bytes.c_str() ); return sub.findFirstOf( characters.substr() ); }


		//* Find first occurrence of any character not in 'characters'
		inline iterator findFirstNotOf( const string& characters ) const noexcept {
			return substr().findFirstNotOf( characters.substr() ); }

		//* Find first occurrence of any character not in 'characters', but
		//* limit search to the specified 'sub' [eon::substring].
		inline iterator findFirstNotOf( const string& characters, substring sub ) const noexcept {
			sub.assertSameBuffer( Bytes.c_str() );  return sub.findFirstNotOf( characters.substr() ); }


		//* Find first occurrence of another string, but skip past sections
		//* of 'this' enclosed by double quotes. Any back-slashed quote mark in
		//* 'this' will be ignored (not counted as quotation mark).
		inline substring findFirstNotDoubleQuoted( const string& other ) const noexcept {
			return substr().findFirstIgnoreSections( other.substr(), DblQuoteChr ); }

		//* Find first occurrence of codepoint, but skip past sections
		//* of 'this' enclosed by double quotes. Any back-slashed quote mark in
		//* 'this' will be ignored (not counted as quotation mark).
		inline substring findFirstNotDoubleQuoted( char_t cp ) const noexcept {
			return substr().findFirstIgnoreSections( cp, DblQuoteChr ); }

		//* Find first occurrence of another string, but skip past sections
		//* of 'this' enclosed by single quotes. Any back-slashed quote mark in
		//* 'this' will be ignored (not counted as quotation mark).
		inline substring findFirstNotSingleQuoted( const string& other ) const noexcept {
			return substr().findFirstIgnoreSections( other.substr(), SglQuoteChr ); }

		//* Find first occurrence of codepoint, but skip past sections
		//* of 'this' enclosed by single quotes. Any back-slashed quote mark in
		//* 'this' will be ignored (not counted as quotation mark).
		inline substring findFirstNotSingleQuoted( char_t cp ) const noexcept {
			return substr().findFirstIgnoreSections( cp, SglQuoteChr ); }

		//* Find first occurrence of another string, but skip past sections
		//* of 'this' enclosed by '(' and ')', '[' and ']' and '{' and '}'.
		//* Nested braces will be accounted for.
		//* See [eon::substring::findFirstIgnoreSections] for how to customize
		//* searches.
		inline substring findFirstNotBraced( const string& other, char_t brace = '(' ) const noexcept {
			return substr().findFirstIgnoreSections( other.substr(), brace,
				brace == '(' ? ')' : brace == '[' ? ']' : brace == '{' ? '}' : brace ); }

		//* Find first occurrence of codepoint, but skip past sections
		//* of 'this' enclosed by '(' and ')', '[' and ']' and '{' and '}'.
		//* Nested braces will be accounted for.
		//* See [eon::substring::findFirstIgnoreSections] for how to customize
		//* searches.
		inline substring findFirstNotBraced( char_t cp, char_t brace = '(' ) const noexcept {
			return substr().findFirstIgnoreSections(
				cp, brace, brace == '(' ? ')' : brace == '[' ? ']' : brace == '{' ? '}' : brace ); }


		//* Find the first character in 'this' that differs from the 'other'
		//* string. If both strings are equal (no diff), then [string::end()]
		//* is returned.
		inline iterator findFirstDiff( const string& other ) const noexcept {
			return substr().findFirstDiff( other.substr() ); }

		//* Find the first character in 'this' that differs from the 'other'
		//* [eon::substring]. If both strings are equal (no diff), then
		//* [string::end()] is returned.
		inline iterator findFirstDiff( const substring& sub ) const noexcept {
			sub.assertSameBuffer( Bytes.c_str() ); return substr().findFirstDiff( sub ); }

		
		//* Find first character in 'this' that is from the specified [charcat]
		//* 'category'
		inline iterator findFirst( charcat category ) const noexcept { return substr().findFirst( category ); }

		//* Find first character in 'this' that is from the specified [charcat]
		//* 'category', limit the search to 'sub' [eon::substring].
		inline iterator findFirst( charcat category, const substring& sub ) const noexcept {
			sub.assertSameBuffer( Bytes.c_str() ); return sub.findFirst( category ); }


		/** Find Last Occurrence **/

		//* Find last occurrence of another (equal or shorter) string
		inline substring findLast( const string& to_find ) const noexcept {
			return substr().highToLow().findLast( to_find.substr() ); }

		//* Find last occurrence of another string, but limit search to the
		//* specified 'sub' [eon::substring].
		inline substring findLast( const string& to_find, substring sub ) const noexcept {
			sub.assertSameBuffer( Bytes.c_str() ); return sub.highToLow().findLast( to_find.substr().highToLow() ); }

		//* Find last occurrence of a codepoint
		inline substring findLast( char_t to_find ) const noexcept { return substr().highToLow().findLast( to_find ); }

		//* Find last occurrence of a codepoint, but limit search to the
		//* specified 'sub' [eon::substring].
		inline substring findLast( char_t to_find, substring sub ) const noexcept {
			sub.assertSameBuffer( Bytes.c_str() ); return sub.highToLow().findLast( to_find ); }

		//* Find last occurrence of an ASCII character
		inline substring findLast( char to_find ) const noexcept {
			return substr().highToLow().findLast( static_cast<char_t>( to_find ) ); }

		//* Find last occurrence of an ASCII character, but limit search to
		//* the specified 'sub' [eon::substring].
		inline substring findLast( char to_find, substring sub ) const noexcept {
			sub.assertSameBuffer( Bytes.c_str() ); return sub.highToLow().findLast( static_cast<char_t>( to_find ) ); }


		//* Find last occurrence of any character in 'characters'
		inline iterator findLastOf( const string& characters ) const noexcept {
			return substr().highToLow().findLastOf( characters.substr() ); }
		
		//* Find last occurrence of any character in 'characters', but limit
		//* search to the specified 'sub' [eon::substring].
		inline iterator findLastOf( const string& characters, substring sub ) const noexcept {
			sub.assertSameBuffer( Bytes.c_str() ); return sub.highToLow().findLastOf( characters.substr() ); }
		

		//* Find last occurrence of any character not in 'characters'
		inline iterator findLastNotOf( const string& characters ) const noexcept {
			return substr().highToLow().findLastNotOf( characters.substr() ); }

		//* Find last occurrence of any character not in 'characters', but
		//* limit search to the specified 'sub' [eon::substring].
		inline iterator findLastNotOf( const string& characters, substring sub ) const noexcept {
			sub.assertSameBuffer( Bytes.c_str() ); return sub.highToLow().findLastNotOf( characters.substr() ); }


		//* Find last character in 'this' that is from the specified [charcat]
		//* 'category'
		inline iterator findLast( charcat category ) const noexcept {
			return substr().highToLow().findLast( category ); }

		//* Find last character in 'this' that is from the specified [charcat]
		//* 'category', limit the search to 'sub' [eon::substring].
		inline iterator findLast( charcat category, const substring& sub ) const noexcept {
			sub.assertSameBuffer( Bytes.c_str() ); return sub.highToLow().findLast( category ); }




		/**********************************************************************
		  Counting
		**********************************************************************/
	public:

		//* Count number of occurrences of the specified codepoint
		inline size_t count( char_t to_count ) const noexcept { return substr().count( to_count ); }

		//* Count number of occurrences of the specified codepoint, limit the
		//* counting to the specified 'sub' [eon::substring].
		inline size_t count( char_t to_count, const substring& sub ) const noexcept {
			sub.assertSameBuffer( Bytes.c_str() ); return sub.count( to_count ); }

		//* Count number of occurrences of the specified string
		//* NOTE: All occurrences are counted, even overlaps!
		inline size_t count( const string& to_count ) const noexcept { return substr().count( to_count.substr() ); }

		//* Count number of occurrences of the specified string, limit the
		//* counting to the specified 'sub' [eon::substring].
		//* NOTE: All occurrences are counted, even overlaps!
		inline size_t count( const string& to_count, const substring& sub ) const noexcept {
			sub.assertSameBuffer( Bytes.c_str() ); return sub.count( to_count.substr() ); }




		/**********************************************************************
		  Modifier Methods

		  These will modify the string object!
		**********************************************************************/
	public:

		//* Clear the string contents
		inline void clear() noexcept { Bytes.clear(); NumChars = 0; }

		//* Reserve memory in order to reduce the number of times the
		//* underlying string buffer may have to grow.
		//* Does nothing if the buffer size is already >= 'byte_size'.
		inline void reserve( size_t byte_size ) { Bytes.reserve( byte_size ); }


		/** Concatenation Assignments **/

		//* Concatenate another string to 'this'
		inline string& operator+=( const string& other ) { Bytes += other.Bytes; NumChars += other.NumChars; return *this; }

		//* Concatenate 'sub' [eon::substring] to 'this'
		inline string& operator+=( const substring& sub ) {
			if( sub.validUTF8() ) {
				Bytes.append( sub.begin().byteData(), sub.numBytes() ); NumChars += sub.numChars(); return *this; }
			else return *this += string( sub ); }

		//* Concatename a std::string to 'this'
		//* Throws [eon::InvalidUTF8] if not a valid UTF-8!
		inline string& operator+=( const std::string& stdstr ) { return *this += substring( stdstr ); }

		//* Concatename a C-string to 'this'
		//* Throws [eon::InvalidUTF8] if not a valid UTF-8!
		inline string& operator+=( const char* cstr ) { return *this += substring( cstr ); }

		//* Concatename a codpoint to 'this'
		//* Throws [eon::InvalidUTF8] if not a valid codepoint!
		inline string& operator+=( char_t cp ) { return *this += string( cp ); }

		//* Concatename a signed char to 'this'
		//* Throws [eon::InvalidUTF8] if not a valid UTF-8!
		inline string& operator+=( char chr ) { return *this += string( chr ); }

		//* Concatename an unsigned char to 'this'
		//* Throws [eon::InvalidUTF8] if not a valid UTF-8!
		inline string& operator+=( unsigned char chr ) { return *this += string( chr ); }


		/** Insert and Erase **/

		//* Insert a [eon::substring] into 'this' at the specified iterator
		//* 'pos' position, moving existing data from (and including) the
		//* iterator position outwards.
		//* Returns a new iterator for the same position that where the insert
		//* as the old will be invalid after insert.
		iterator insert( const iterator& pos, const string& sub );

		//* Insert a codepoint into 'this' at the specified iterator 'pos'
		//* position, moving existing data from (and including) the iterator
		//* position outwards.
		//* Returns a new iterator for the same position that where the insert
		//* as the old will be invalid after insert.
		inline iterator insert( const iterator& pos, char_t cp ) { return insert( pos, string( cp ) ); }

		//* Erase an [eon::substring] section from the string
		string& erase( const substring& sub ) noexcept;




		/**********************************************************************
		  Copy and Transform

		  These methods will not modify the string object, but produce a new or
		  an [eon::substring]!

		  For methods that return substrings, you can easily convert the
		  substring into an independent string by using the substring as
		  argument to string constructor or assignment method/operator.
		**********************************************************************/
	public:

		/** Case Transform **/

		//* Get a copy of the string with all letters upper case
		inline string upper() const { return upper( substr() ); }

		//* Get a copy of the string with all letters in the 'sub'
		//* [eon::substring] upper case (the others left unchanged)
		string upper( const substring& sub ) const;


		//* Get a copy of the string with all letters lower case
		inline string lower() const { return lower( substr() ); }

		//* Get a copy of the string with all letters in the 'sub'
		//* [eon::substring] lower case (the others left unchanged)
		string lower( const substring& sub ) const;


		//* Get a copy of the string with first letter upper-case (if not
		//* already)
		inline string ucFirst() const { return ucFirst( substr() ); }
		
		//* Get a copy of the string with first letter of the 'sub'
		//* [eon::substring] upper-case (if not already)
		string ucFirst( const substring& sub ) const;


		//* Get a copy of the string with first letter of every word upper-
		//* cased, no other characters will be modified.
		inline string ucWords() const { return ucWords( substr() ); }
		
		//* Get a copy of the string with first letter of every word in 'sub'
		//* [eon::substring] upper-cased, no other characters will be modified.
		string ucWords( const substring& sub ) const;


		//* Get a copy of the string with first letter of every sentence upper-
		//* cased, no other characters will be modified.
		//* NOTE: Only '.', ':', '!' and '?' are considered sentence dividers!
		inline string ucSentences() const { return ucSentences( substr() ); }

		//* Get a copy of the string with first letter of every sentence in
		//* 'sub' [eon::substring] upper-cased, no other characters will be
		//* modified.
		//* NOTE: Only '.', ':', '!' and '?' are considered sentence dividers!
		string ucSentences( const substring& sub ) const;


		/** Substrings **/

		//* Get entire string as an [eon::substring]
		inline substring substr() const { return substring( begin(), end() ); }

		//* Get [eon::substring] starting at the 'start' iterator and running
		//* to the end of the string
		inline substring substr( const iterator& start ) const {
			start.assertSameBuffer( Bytes.c_str() ); return substring( start, end() ); }

		//* Get [eon::substring] starting at the 'start' iterator and running
		//* to the 'end' iterator
		inline substring substr( const iterator& start, const iterator& end ) const {
			start.assertSameBuffer( Bytes.c_str() ); end.assertSameBuffer( Bytes.c_str() ); return substring( start, end ); }


		//* Get [eon::substring] based on character position and count
		//* NOTE: This involves counting characters, but is optimized to count
		//*       from the end if that is closer.
		inline substring substr( size_t start, size_t size ) const {
			auto sub = substr(); return substring( sub.iterator( start ), sub.iterator( start + size ) ); }


		//* Get a [eon::substring] (slice) based on character position and
		//* count from start (positive value) and/or from end (negative value).
		//* NOTE: Both 'start' and 'end' are inclusive!
		//*       A slice from 0 to -1  will include the entire string, while 2
		//*       to 2 will return a substring containing a single character.
		//* NOTE: If 'start' is less than 'end', a 'high-to-low' ordered
		//*       substring is returned.
		substring slice( int64_t start, int64_t end ) const;


		//* Get [eon::substring] before first occurrence of 'delimiter'
		inline substring beforeFirst( const string& delimiter ) const noexcept {
			return substr().beforeFirst( delimiter.substr() ); }

		//* Get [eon::substring] before first occurrence of 'delimiter'
		inline substring beforeFirst( char_t delimiter ) const noexcept { return substr().beforeFirst( delimiter ); }

		//* Get [eon::substring] before last occurrence of 'delimiter'
		inline substring beforeLast( const string& delimiter ) const noexcept {
			return substr().beforeLast( delimiter.substr() ); }

		//* Get [eon::substring] before last occurrence of 'delimiter'
		inline substring beforeLast( char_t delimiter ) const noexcept { return substr().beforeLast( delimiter ); }

		//* Get [eon::substring] after first occurrence of 'delimiter'
		inline substring afterFirst( const string& delimiter ) const noexcept {
			return substr().afterFirst( delimiter.substr() ); }

		//* Get [eon::substring] after first occurrence of 'delimiter'
		inline substring afterFirst( char_t delimiter ) const noexcept { return substr().afterFirst( delimiter ); }

		//* Get [eon::substring] after last occurrence of 'delimiter'
		inline substring afterLast( const string& delimiter ) const noexcept {
			return substr().afterLast( delimiter.substr() ); }

		//* Get [eon::substring] after last occurrence of 'delimiter'
		inline substring afterLast( char_t delimiter ) const noexcept { return substr().afterLast( delimiter ); }


		/** Replace **/

		//* Get a copy of the entire string with all occurrences of 'find'
		//* replaced with 'replacement'
		inline string replace( const string& find, const string& replacement ) const {
			return replace( find, replacement, substr() ); }

		//* Get a copy of the entire string with all occurrences of 'find'
		//* within 'sub' [eon::substring] replaced with 'replacement'
		string replace( const string& find, const string& replacement, const substring& sub ) const;

		//* Get a copy of the entire string with all occurrences of 'find'
		//* replaced with 'replacement'
		inline string replace( char_t find, char_t replacement ) const { return replace( find, replacement, substr() ); }
		
		//* Get a copy of the entire string with all occurrences of 'find'
		//* within 'sub' [eon::substring] replaced with 'replacement'
		string replace( char_t find, char_t replacement, const substring& sub ) const;

		//* Get a copy of the entire string with all occurrences of 'find'
		//* replaced with 'replacement'
		inline string replace( char find, char replacement ) const {
			return replace( static_cast<char_t>( find ), static_cast<char_t>( replacement ) ); }

		//* Get a copy of the entire string with all occurrences of 'find'
		//* within 'sub' [eon::substring] replaced with 'replacement'
		inline string replace( char find, char replacement, const substring& sub ) const {
			return replace( static_cast<char_t>( find ), static_cast<char_t>( replacement ), sub ); }
		
		//* Get a copy of the entire string with 'sub' [eon::substring]
		// replaced with 'replacement'
		inline string replace( const substring& sub, const string& replacement ) const {
			return string( substr( begin(), sub.begin() ) ) + replacement + string( substr( sub.end() ) ); }

		//* Get a copy of the entire string with 'sub' [eon::substring]
		// replaced with 'replacement'
		inline string replace( const substring& sub, char_t replacement ) const {
			return string( substr( begin(), sub.begin() ) ) + replacement + string( substr( sub.end() ) ); }


		/** Remove **/

		//* Get a copy of the entire string with all occurrences of 'to_remove'
		//* removed
		inline string remove( const string& to_remove ) const { return replace( to_remove, string::Empty, substr() ); }

		//* Get a copy of the entire string with all occurrences of 'to_remove'
		//* within 'sub' [eon::substring] removed
		inline string remove( const string& to_remove, const substring& sub ) const {
			return replace( to_remove, string::Empty, sub ); }

		//* Get a copy of the entire string with all occurrences of 'to_remove'
		//* removed
		inline string remove( char_t to_remove ) const { return replace( string( to_remove ), string::Empty, substr() ); }

		//* Get a copy of the entire string with all occurrences of 'to_remove'
		//* within 'sub' [eon::substring] removed
		inline string remove( char_t to_remove, const substring& sub ) const {
			return replace( string( to_remove ), string::Empty, sub ); }

		//* Get a copy of the entire string with all occurrences of 'to_remove'
		//* removed
		inline string remove( char to_remove ) const { return replace( string( to_remove ), string::Empty, substr() ); }

		//* Get a copy of the entire string with all occurrences of 'to_remove'
		//* within 'sub' [eon::substring] removed
		inline string remove( char to_remove, const substring& sub ) const {
			return replace( string( to_remove ), string::Empty, sub ); }


		/** Escaping **/

		//* Get a copy of the string with standard C single character escapes
		//* processed
		//* NOTE: Single quotes, question marks and tabs are not escaped!
		inline string escape() const { return escape( substr() ); }

		//* Get a copy of the string with standard C single character escapes
		//* processed only within 'sub' [eon::substring]
		//* NOTE: Single quotes, question marks and tabs are not escaped!
		string escape( const substring& sub ) const;

		//* Get a copy of the string with standard C single character escapes
		//* processed
		//* NOTE: Single quotes, question marks and tabs are also escaped!
		inline string escapeAll() const { return escapeAll( substr() ); }

		//* Get a copy of the string with standard C single character escapes
		//* processed only within 'sub' [eon::substring]
		//* NOTE: Single quotes, question marks and tabs are also escaped!
		string escapeAll( const substring& sub ) const;

		//* Get a copy of the string with standard C single character escapes
		//* reverse processed
		inline string unescape() const { return unescape( substr() ); }

		//* Get a copy of the string with standard C single character escapes
		//* reverse processed only within 'sub' [eon::substring]
		string unescape( const substring& sub ) const;


		//* Get a copy of the string with all non-printable ASCII characaters
		//* and all non-ASCII characters replaced with HTML formatting.
		//* Example: "\t" -> "&#9;"
		inline string escapeNonPrintable() const { return escapeNonPrintable( substr() ); }

		//* Get a copy of the string with all non-printable ASCII characaters
		//* and all non-ASCII characters inside 'sub' [eon::substring] replaced
		//* with HTML formatting.
		//* Example: "\t" -> "&#9;"
		string escapeNonPrintable( const substring& sub ) const;

		//* Get a copy of the string with all HTML formatted characters
		//* replaced with the actual characaters they represent.
		//* Example: "&#9;" -> "\t"
		inline string unescapeNonPrintable() const { return unescapeNonPrintable( substr() ); }

		//* Get a copy of the string with all HTML formatted characters inside
		//* 'sub' [eon::substring] replaced with the actual characaters they
		//* represent.
		//* Example: "&#9;" -> "\t"
		string unescapeNonPrintable( const substring& sub ) const;


		/** Quotation Marks **/

		//* Get a copy of the string enclosed in double quotes
		//* NOTE: Will not add quotation marks if already existing at both ends
		inline string doubleQuote() const { return doubleQuote( substr() ); }

		//* Get a copy of the string with the 'sub' [eon::substring] enclosed
		//* in double quotes
		//* NOTE: Will not add quotation marks if already existing at both ends
		string doubleQuote( const substring& sub ) const;

		//* Get a copy of the string enclosed in single quotes
		//* NOTE: Will not add quotation marks if already existing at both ends
		inline string singleQuote() const { return singleQuote( substr() ); }

		//* Get a copy of the string with the 'sub' [eon::substring] enclosed
		//* in single quotes
		//* NOTE: Will not add quotation marks if already existing at both ends
		string singleQuote( const substring& sub ) const;

		//* Get a copy of the string with double or single quotations marks
		//* removed
		// NOTE: Will only remove if the same quotation character is at both
		//*      ends
		inline string unQuote() const { return unQuote( substr() ); }

		//* Get a copy of the string with double or single quotations marks on
		//* the 'sub' [eon::substring] removed
		// NOTE: Will only remove if the same quotation character is at both
		//*      ends
		string unQuote( const substring& sub ) const;


		/** Reversing **/

		//* Get a copy of the string with all characters in reverse order
		inline string reverse() const { return reverse( substr() ); }
		
		//* Get a copy of the string with all characters in the 'sub'
		//* [eon::substring] in reverse order, the others left unchanged
		string reverse( const substring& sub ) const;


		/** Simple Split and Join **/

		//* Split string on every occurrence of 'delimiter' (delimiter string
		//* not included) into a sequential container (such as std::list or
		//* std::vector). Uses the 'push_back' method!
		//* If a 'max_elements' limit is reached, the last element will
		//* reference the unsplit remainder.
		//* NOTE: All elements will be substring objects, so specify the
		//*       template type as such!
		//*       Example: splitSequential<std::list<eon::substring>>( ...
		template<typename container_t>
		container_t splitSequential( const string& delimiter, size_t max_elements = SIZE_MAX ) const {
			return substr().splitSequential<container_t>( delimiter.substr(), max_elements ); }

		//* Split string on every occurrence of 'delimiter' (delimiter char
		//* not included) into a sequential container (such as std::list or
		//* std::vector). Uses the 'push_back' method!
		//* If a 'max_elements' limit is reached, the last element will
		//* reference the unsplit remainder.
		//* NOTE: All elements will be substring objects, so specify the
		//*       template type as such!
		//*       Example: splitSequential<std::list<eon::substring>>( ...
		template<typename container_t>
		container_t splitSequential( char_t delimiter, size_t max_elements = SIZE_MAX ) const {
			return substr().splitSequential<container_t>( delimiter, max_elements ); }

		//* Split string on every occurrence of 'delimiter' (delimiter string
		//* not included) into a non-sequential container (such as std::set or
		//* std::multiset). Uses the 'insert' method!
		//* NOTE: All elements will be substring objects, so specify the
		//*       template type as such!
		//*       Example: splitNonsequential<std::set<eon::substring>>( ...
		template<typename container_t>
		container_t splitNonsequential( const string& delimiter ) const {
			return substr().splitNonSequential<container_t>( delimiter.substr() ); }

		//* Split string on every occurrence of 'delimiter' (delimiter char
		//* not included) into a non-sequential container (such as std::set or
		//* std::multiset). Uses the 'insert' method!
		//* NOTE: All elements will be substring objects, so specify the
		//*       template type as such!
		//*       Example: splitNonsequential<std::set<eon::substring>>( ...
		template<typename container_t>
		container_t splitNonSequential( char_t delimiter ) const {
			return substr().splitNonSequential<container_t>( delimiter ); }


		//* Get a string that joins the elements from 'start' to 'end', using
		//* 'this' string as 'glue'
		template<typename iterator_t>
		string join( iterator_t start, iterator_t end ) const
		{
			string str;
			for( auto i = start; i != end; ++i )
			{
				if( !str.empty() )
					str += *this;
				str += *i;
			}
			return str;
		}

		//* Get a string that joins the elements of the 'source' container,
		//* using 'this' string as 'glue'
		template<typename container_t>
		string join( const container_t& source ) const {
			return join<typename container_t::const_iterator>( source.begin(), source.end() ); }


		/** Trimming and Padding **/

		//* Get an [eon::substring] with leading and trailing spaces excluded
		inline substring trim() const { return substr().trim(); }

		//* Get an [eon::substring] with leading spaces excluded
		inline substring trimLeading() const { return substr().trimLeading(); }

		//* Get an [eon::substring] with trailing spaces excluded
		inline substring trimTrailing() const { return substr().trimTrailing(); }


		//* Get a copy of the string, if it is shorter than 'target_size', add
		//* enough copies of the the 'fill' codepoint on the left side to make
		//* it that long
		inline string padLeft( size_t target_size, char_t fill = SpaceChr ) const {
			return NumChars < target_size ? string( target_size - NumChars, fill ) += *this : *this; }

		//* Get a copy of the string, if it is shorter than 'target_size', add
		//* enough copies of the the 'fill' codepoint on the right side to make
		//* it that long
		inline string padRight( size_t target_size, char_t fill = SpaceChr ) const {
			return NumChars < target_size ? *this + string( target_size - NumChars, fill ) : *this; }

		//* Get a copy of the string, if it is shorter than 'target_size', add
		//* enough copies of the the 'fill' codepoint on the left and right
		//* side to make it that long
		//* (Centers the string)
		string padLeftAndRight( size_t target_size, char_t fill = SpaceChr ) const;


		//* Get a copy of the string with all lines 'indented'
		// NOTE: If the first line is already at the specified
		//*      'indentation_level', it will not be indented further!
		// NOTE: Subsequent lines will be indented fully regardless of existing
		//       indentation!
		// NOTE: Empty lines will not be indented!
		string indentLines( size_t indentation_level, char_t indentation_char = TabChr ) const;


		/** Concatenation **/
		friend inline string operator+( const string& a, const string& b ) { return string( a ) += b; }
		friend inline string operator+( const string& a, const substring& b ) { return string( a ) += b; }
		friend inline string operator+( const substring& a, const string& b ) { return string( a ) += b; }
		friend inline string operator+( const string& a, const std::string& b ) { return string( a ) += string( b ); }
		friend inline string operator+( const std::string& a, const string& b ) { return string( a ) += b; }
		friend inline string operator+( const string& a, const char* b ) { return string( a ) += string( b ); }
		friend inline string operator+( const char* a, const string& b ) { return string( a ) += b; }
		friend inline string operator+( const string& a, char_t b ) { return string( a ) += b; }
		friend inline string operator+( char_t a, const string& b ) { return string( a ) += b; }




		/**********************************************************************
		  Strings as Numbers

		  These methods provide various number operations to strings.
		**********************************************************************/
	public:

		//* Check if all characaters are numerals (including non-ASCII
		//* numerals)
		inline bool numeralsOnly() const noexcept { return substr().numeralsOnly(); }

		//* Check if all characaters in 'sub' [eon::substring] are numerals
		//* (including non-ASCII numerals)
		inline bool numeralsOnly( const substring& sub ) const {
			sub.assertSameBuffer( Bytes.c_str() ); return sub.numeralsOnly(); }


		//* Check if all characters are ASCII digits (only!)
		inline bool isUInt() const noexcept { return substr().isUInt(); }

		//* Check if all characters in 'sub' [eon::substring] are ASCII digits
		//* (only!)
		inline bool isUInt( const substring& sub ) const { sub.assertSameBuffer( Bytes.c_str() ); return sub.isUInt(); }


		//* Check if all characters are ASCII digits (only!), prefixed by a
		//* single '+' or '-'
		inline bool isInt() const noexcept { return substr().isInt(); }

		//* Check if all characters in 'sub' [eon::substring] are ASCII digits
		//* (only!), prefixed by a single '+' or '-'
		inline bool isInt( const substring& sub ) const { sub.assertSameBuffer( Bytes.c_str() ); return sub.isInt(); }


		//* Check if the string makes up an ASCII floating point number,
		//* optionally prefixed by a single '+' or '-'. The 'decimal_separator'
		//* may occur first or last!
		inline bool isFloat( char_t decimal_separator = PointChr ) const noexcept {
			return substr().isFloat( decimal_separator ); }

		//* Check if the [eon::substring] makes up an ASCII floating point
		//* number, optionally prefixed by a single '+' or '-'. The
		//* 'decimal_separator' may occur first or last!
		inline bool isFloat( const substring& sub, char_t decimal_separator = PointChr ) const {
			sub.assertSameBuffer( Bytes.c_str() ); return sub.isFloat( decimal_separator ); }


		/* Convert String to Number */
		inline int32_t toInt32() const { return substr().toInt32(); }
		inline int32_t toInt32( const substring& area ) const {
			area.assertSameBuffer( Bytes.c_str() ); return area.toInt32(); }
		inline int64_t toInt64() const { return substr().toInt64(); }
		inline int64_t toInt64( const substring& area ) const {
			area.assertSameBuffer( Bytes.c_str() ); return area.toInt64(); }
		inline uint32_t toUInt32() const { return substr().toUInt32(); }
		inline uint32_t toUInt32( const substring& area ) const {
			area.assertSameBuffer( Bytes.c_str() ); return area.toUInt32(); }
		inline uint64_t toUInt64() const { return substr().toUInt64(); }
		inline uint64_t toUInt64( const substring& area ) const {
			area.assertSameBuffer( Bytes.c_str() ); return area.toUInt64(); }
		inline size_t toSize() const { return substr().toSize(); }
		inline size_t toSize( const substring& area ) const {
			area.assertSameBuffer( Bytes.c_str() ); return area.toSize(); }
		inline double toDouble() const { return substr().toDouble(); }
		inline double toDouble( const substring& area ) const {
			area.assertSameBuffer( Bytes.c_str() ); return area.toDouble(); }
		inline long double toLongDouble() const { return substr().toLongDouble(); }
		inline long double toLongDouble( const substring& area ) const {
			area.assertSameBuffer( Bytes.c_str() ); return area.toLongDouble(); }
		/* (Non-numbers and non-ASCII numbers are defined as zero.) */


		//* Assuming the entire substring is a number, get a reduced
		//* [eon::substring] where leading integer zeros and trailing
		//* fractional zeros are removed - including the 'decimal_separator' if
		//* all fractional digits are zeros.
		//* (If the number ends with the separator, it will be excluded.)
		//* NOTE: If the number is signed, leading integer zeros will be
		//*       included to keep the sign.
		//* Example: 0001.0000 -> 1
		inline substring trimNumber( char_t decimal_separator = PointChr ) const {
			return substr().trimNumber( decimal_separator ); }

		//* Assuming the entire substring is a number, get a reduced
		//* [eon::substring] where leading integer zeros and trailing
		//* fractional zeros are removed. If all fractional digits are zeros,
		//* keep the 'decimal_separator' and one zero.
		//* (If the number ends with the separator, it will be included.)
		//* NOTE: If the number is signed, leading integer zeros will be
		//*       included to keep the sign.
		//* Example: 0001.0000 -> 1.0
		inline substring trimFloat( char_t decimal_separator = PointChr ) const {
			return substr().trimFloat( decimal_separator ); }


		//* Get a copy of the number string with the integer part separated
		//* into thousands.
		//* Example: 34525525.4621 -> 34,525,525.4621
		string separateThousands( char_t thousands_sep = CommaChr, char_t decimal_separator = PointChr ) const;

		//* Get a copy of the number string with fractional part reduced to
		//* 'max_decimals', rounded up or down to nearest value. If there is
		//* no fractional part or it is already at the same or fewer number of
		//* decimals, an unaltered copy is returned.
		//* Zero decimals will result in removal of the fractional part and
		//* possibly an increase of the integer part (if rounding up).
		string roundNumber( size_t max_decimals = 0, char_t decimal_separator = PointChr ) const;




		/**********************************************************************
		  String hashing

		  Uses the FNV-1a hash algorithm
		**********************************************************************/
	public:

		//* Get a 32-bit hash value
		inline uint32_t hash32() const noexcept { return substring::hash32( Bytes.c_str(), Bytes.c_str() + Bytes.size() ); }

		//* Get a 64-bit hash value
		inline uint64_t hash64() const noexcept { return substring::hash64( Bytes.c_str(), Bytes.c_str() + Bytes.size() ); }

		//* Get a 'size_t'size hash value
#if defined(_WIN64) || defined(__x86_64__)
		inline size_t hash() const noexcept { return substring::hash64( Bytes.c_str(), Bytes.c_str() + Bytes.size() ); }
#else
		inline size_t hash() const noexcept { return substring::hash32( Bytes.c_str(), Bytes.c_str() + Bytes.size() ); }
#endif




		/**********************************************************************
		  Comparing
		**********************************************************************/
	public:

		//* Check if the string starts with a specific 'value'
		inline bool startsWith( const string& value ) const noexcept { return substr().startsWith( value.substr() ); }

		//* Check if the string starts with a specific 'value'
		inline bool startsWith( char_t value ) const noexcept { return *begin() == value; }

		//* Check if string ends with a specific 'value'
		inline bool endsWith( const string& value ) const noexcept { return substr().endsWith( value.substr() ); }

		//* Check if string ends with a specific 'value'
		inline bool endsWith( char_t value ) const noexcept { return *last() == value; }


		//* Basic comparison
		//* There are two ways to compare strings, which may yield different
		//* (though not significant for the comparison as such) results
		//* (depending on lower implementation). If 'CompareType::faster' is
		//* specified, run the fastest available low-level compare function.
		//* Return values not guaranteed to be anything other then '-1', '0'
		//* and '1'. (This is the case with Visual C++.) If
		//* 'CompareType::diff_pos' is specified, return -pos, 0 or pos
		//* (guaranteed) - where pos is the first character that is different
		//* between the two.
		//* NOTE: You can also use [eon::substring::compare] for comparing
		//*       sections of strings.
		inline int compare( const string& other, CompareType type = CompareType::faster ) const noexcept {
			return substr().compare( other.substr(), type ); }

		/** Comparison operators - always using 'CompareType::faster'! **/
		inline bool operator<( const string& other ) const noexcept { return compare( other ) < 0; }
		inline bool operator<=( const string& other ) const noexcept { return compare( other ) <= 0; }
		inline bool operator>( const string& other ) const noexcept { return compare( other ) > 0; }
		inline bool operator>=( const string& other ) const noexcept { return compare( other ) >= 0; }
		inline bool operator==( const string& other ) const noexcept { return compare( other ) == 0; }
		inline bool operator!=( const string& other ) const noexcept { return compare( other ) != 0; }

		/** Compare while ignoring case **/
		inline int iCompare( const string& other ) const noexcept { return substr().iCompare( other.substr() ); }
		inline int iCompare( const char* cstr ) const noexcept { return substr().iCompare( substring( cstr ) ); }
		inline int iCompare( const std::string& stdstr ) const noexcept { return substr().iCompare( substring( stdstr ) ); }

		/** Compare against 'C-string' **/
		inline int compare( const char* cstr, CompareType type = CompareType::faster ) const noexcept {
			return substr().compare( substring( cstr ), type ); }
		inline bool operator<( const char* cstr ) const noexcept { return compare( cstr ) < 0; }
		inline bool operator<=( const char* cstr ) const noexcept { return compare( cstr ) <= 0; }
		inline bool operator>( const char* cstr ) const noexcept { return compare( cstr ) > 0; }
		inline bool operator>=( const char* cstr ) const noexcept { return compare( cstr ) >= 0; }
		inline bool operator==( const char* cstr ) const noexcept { return compare( cstr ) == 0; }
		inline bool operator!=( const char* cstr ) const noexcept { return compare( cstr ) != 0; }

		/** Compare against 'std::string' **/
		inline int compare( const std::string& stdstr, CompareType type = CompareType::faster ) const noexcept {
			return substr().compare( substring( stdstr ) ); }
		bool operator<( const std::string& stdstr ) const noexcept { return compare( stdstr ) < 0; }
		bool operator<=( const std::string& stdstr ) const noexcept { return compare( stdstr ) <= 0; }
		bool operator>( const std::string& stdstr ) const noexcept { return compare( stdstr ) > 0; }
		bool operator>=( const std::string& stdstr ) const noexcept { return compare( stdstr ) >= 0; }
		bool operator==( const std::string& stdstr ) const noexcept { return compare( stdstr ) == 0; }
		bool operator!=( const std::string& stdstr ) const noexcept { return compare( stdstr ) != 0; }

		/** Comparing against a single codepoint **/
		inline int compare( char_t cp ) const noexcept {
			return NumChars == 0 || *begin() < cp ? -1 : *begin() > cp ? 1 : NumChars == 1 ? 0 : 1; }
		inline bool operator<( char_t cp ) const noexcept { return compare( cp ) < 0; }
		inline bool operator<=( char_t cp ) const noexcept { return compare( cp ) <= 0; }
		inline bool operator>( char_t cp ) const noexcept { return compare( cp ) > 0; }
		inline bool operator>=( char_t cp ) const noexcept { return compare( cp ) >= 0; }
		inline bool operator==( char_t cp ) const noexcept { return compare( cp ) == 0; }
		inline bool operator!=( char_t cp ) const noexcept { return compare( cp ) != 0; }

		/** Compare against a single char **/
		inline int compare( char c ) const noexcept { return compare( static_cast<char_t>( c ) ); }
		inline bool operator<( char c ) const noexcept { return compare( static_cast<char_t>( c ) ) < 0; }
		inline bool operator<=( char c ) const noexcept { return compare( static_cast<char_t>( c ) ) <= 0; }
		inline bool operator>( char c ) const noexcept { return compare( static_cast<char_t>( c ) ) > 0; }
		inline bool operator>=( char c ) const noexcept { return compare( static_cast<char_t>( c ) ) >= 0; }
		inline bool operator==( char c ) const noexcept { return compare( static_cast<char_t>( c ) ) != 0; }
		inline bool operator!=( char c ) const noexcept { return compare( static_cast<char_t>( c ) ) != 0; }





		/**********************************************************************
		  Wide-string Support
		
		  Note that this support is limited and somewhat costly.
		  Only use when 'wstring_t' or 'wchar_t' is required by some third
		  party code.
		**********************************************************************/
	public:

		//* Construct string from 'std::wstring'
		inline string( const std::wstring& stdwstr ) { *this = stdwstr; }

		//* Construct string from C-style wide-string
		inline string( const wchar_t* cstr ) { *this = cstr; }


		//* Assign from 'std::wstring'
		string& operator=( const std::wstring& stdwstr );

		//* Assign from C-style wide-string
		string& operator=( const wchar_t* cstr );

		
		//* Convert 'this' into a 'std::wstring'
		inline operator std::wstring() const { return wstr(); }
		std::wstring wstr() const;

		//* Concatenate-assign an 'std::wstring'
		inline string& operator+=( const std::wstring& to_add ) { return *this += string( to_add ); }




		/**********************************************************************
		  String as List
		  The string is considered a list of elements where each element is
		  separated by a common character or sub-string (';' by default). The
		  separator is only between elements, not before the first or after
		  the last.
		
		  WARNING: No checking is done to see if an added string contains the
		           separator. Adding "alpha" first and then "beta" is the same
				   as adding "alpha;beta" in one go (if default separator).
		
		  To split the elements into a container, use a [splitSequential] or
		  [splitNonSequential] method.
		**********************************************************************/
	public:

		//* Add an element to the end of the string-list
		string& push_back( const string& element, const string& separator = string( SemiColonChr ) );

		//* Add an element to the front of the string-list
		//* Warning: This is somewhat costly
		string& push_front( const string& element, const string& separator = string( SemiColonChr ) );

		//* Remove duplicates and sort the list
		//* This is very costly!
		string& orderList( const string& separator = string( SemiColonChr ) );




		/**********************************************************************
		  Static utility methods
		**********************************************************************/
	public:

		//* Set locale (used by various compare methods).
		//* The default is set automatically at startup!
		static void setLocale( const std::string& name = "en_US.utf8" );

		//* Convert other types into string (using 'std::to_string')
		//* This method is limited to whatever is supported by
		//* 'std::to_string'!
		template<typename T>
		static inline string toString( T value ) { return string( std::to_string( value ) ); }

		//* Convert 'double' into string (specifically).
		//* (We want greater precision and to remove trailing decimal zeros!)
		static inline string toString( double value )
		{
			static char digits[ 320 ];
#ifdef EON_WINDOWS
			sprintf_s( digits, 320, "%.8f", value );
#else
			sprintf( digits, "%.8f", value );
#endif
			auto size = strlen( digits );
			for( ; digits[ size - 1 ] == '0' && digits[ size - 2 ] != '.';
				--size )
				;
			return string( digits, size );
		}
		
		//* Convert an [eon::string_iterator] into a string format (for
		//* streaming/saving etc.): "<byte pos>:<char pos>"
		//* (See matching [decodeIterator] method.)
		static inline string encode( const string_iterator& itr ) {
			return toString( itr.numByte() ) + ":" + toString( itr.numChar() ); }

		//* Convert an [eon::substring] object into a string format (for
		//* streaming/saving etc.):
		//   "<byte pos>:<char pos>-<byte pos>:<char pos>"
		//* (See matching [decodeIterator] method.)
		static inline string encode( const substring& sub ) { return encode( sub.begin() ) + "-" + encode( sub.end() ); }


		//* Get Byte Order Marker for UTF-8
		static const std::string& bom();


		//* Check if a codepoint belongs to a specific character class, as
		//* defined in
		//*   https://www.fileformat.info/info/unicode/category/index.htm
		static inline bool isLetter( char_t cp ) noexcept {
			return isLetterUpperCase( cp ) || isLetterLowerCase( cp ) || isLetterTitleCase( cp )
				|| isLetterModifier( cp ) || isLetterOther( cp ); }
		static inline bool isLetterUpperCase( char_t cp ) noexcept { return Characters::get().isLetterUpperCase( cp ); }
		static inline bool isLetterLowerCase( char_t cp ) noexcept { return Characters::get().isLetterLowerCase( cp ); }
		static inline bool isLetterTitleCase( char_t cp ) noexcept { return Characters::get().isLetterTitleCase( cp ); }
		static inline bool isLetterModifier( char_t cp ) noexcept { return Characters::get().isLetterModifier( cp ); }
		static inline bool isLetterOther( char_t cp ) noexcept { return Characters::get().isLetterOther( cp ); }
		//*
		static inline bool isMarkSpacingCombining( char_t cp ) noexcept {
			return Characters::get().isMarkSpacingCombining( cp ); }
		static inline bool isMarkNonSpacing( char_t cp ) noexcept { return Characters::get().isMarkNonSpacing( cp ); }
		static inline bool isMarkEnclosing( char_t cp ) noexcept { return Characters::get().isMarkEnclosing( cp ); }
		//*
		static inline bool isNumber( char_t cp ) noexcept {
			return isNumberDecimalDigit( cp ) || isNumberLetter( cp ) || isNumberOther( cp ); }
		static inline bool isNumberAsciiDigit( char_t cp ) noexcept { return Characters::get().isNumberAsciiDigit( cp ); }
		static inline bool isNumberDecimalDigit( char_t cp ) noexcept {
			return Characters::get().isNumberDecimalDigit( cp ); }
		static inline bool isNumberLetter( char_t cp ) noexcept { return Characters::get().isNumberLetter( cp ); }
		static inline bool isNumberOther( char_t cp ) noexcept { return Characters::get().isNumberOther( cp ); }
		//*
		static inline bool isPunctuation( char_t cp ) noexcept { return Characters::get().isPunctuation( cp ); }
		static inline bool isPunctuationConnector( char_t cp ) noexcept {
			return Characters::get().isPunctuationConnector( cp ); }
		static inline bool isPunctuationDash( char_t cp ) noexcept { return Characters::get().isPunctuationDash( cp ); }
		static inline bool isPunctuationOpen( char_t cp ) noexcept { return Characters::get().isPunctuationOpen( cp ); }
		static inline bool isPunctuationClose( char_t cp ) noexcept { return Characters::get().isPunctuationClose( cp ); }
		static inline bool isPunctuationInitialQuote( char_t cp ) noexcept {
			return Characters::get().isPunctuationInitialQuote( cp ); }
		static inline bool isPunctuationFinalQuote( char_t cp ) noexcept {
			return Characters::get().isPunctuationFinalQuote( cp ); }
		static inline bool isPunctuationOther( char_t cp ) noexcept { return Characters::get().isPunctuationOther( cp ); }
		//*
		static inline bool isSymbol( char_t cp ) noexcept { return Characters::get().isSymbol( cp ); }
		static inline bool isSymbolCurrency( char_t cp ) noexcept { return Characters::get().isSymbolCurrency( cp ); }
		static inline bool isSymbolModifier( char_t cp ) noexcept { return Characters::get().isSymbolModifier( cp ); }
		static inline bool isSymbolMath( char_t cp ) noexcept { return Characters::get().isSymbolMath( cp ); }
		static inline bool isSymbolOther( char_t cp ) noexcept { return Characters::get().isSymbolOther( cp ); }
		//*
		static inline bool isSeparator( char_t cp ) noexcept { return Characters::get().isSeparator( cp ); }
		static inline bool isSeparatorLine( char_t cp ) noexcept { return Characters::get().isSeparatorLine( cp ); }
		static inline bool isSeparatorParagraph( char_t cp ) noexcept {
			return Characters::get().isSeparatorParagraph( cp ); }
		static inline bool isSeparatorSpace( char_t cp ) noexcept { return Characters::get().isSeparatorSpace( cp ); }


		//* Check if a codepoint is a word character
		//* Word characters are: 'letters', 'number decimal digits' (of any
		//* kind) and 'underscore'.
		static inline bool isWordChar( char_t cp ) noexcept {
			return isLetter( cp ) || isNumberDecimalDigit( cp ) || cp == '_'; }

		//* Check if a codepoint is a space character
		//* Space characters are: 'character tabulation', 'space' and
		//* 'separators'
		static inline bool isSpaceChar( char_t cp ) noexcept { return ( cp >= 0x09 && cp <= 0x0D ) || isSeparator( cp ); }





		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		static size_t _findDecimalSeparator( std::vector<char_t>& digits, char_t decimal_separator = PointChr ) noexcept;

		// Given an iterator, round up all the digits in the string all the way
		// up to adding a new '1' in the front if all digits are rounded up.
		// Rounding will stop when rounding up a digit results in a new digit
		// less than 5.
		// NOTE: This method assumes (without checking) that all characters
		// from and including 'i' and to and including 'begin()' are digites or
		// a point!
		static void _roundUp( std::vector<char_t>& digits, size_t i ) noexcept;


		// Byte operations:

		// Check if pure ASCII or if we have multi-type characters
		inline bool _ascii() const noexcept { return numBytes() == NumChars; }




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//

	private:
		std::string Bytes;
		size_t NumChars{ 0 };	// Number of code points

	public:
		static const string Empty;
	};




	/**************************************************************************
	  Unattatched Concatenators
	**************************************************************************/
	inline string operator+( const substring& a, const substring& b ) { return string( a ) += b; }
	inline string operator+( const substring& a, const std::string& b ) { return string( a ) += b; }
	inline string operator+( const std::string& a, const substring& b ) { return string( a ) += b; }
	inline string operator+( const substring& a, const char* b ) { return string( a ) += b; }
	inline string operator+( const char* a, const substring& b ) { return string( a ) += b; }

	/** Definition of 'eon::string_ptr' */
	using string_ptr = std::shared_ptr<string>;

	/** Binary predicate for comparing two [eon::string] pointers **/
	struct string_lt
	{
		inline bool operator()( const string* lhs, const string* rhs ) const { return lhs->compare( *rhs ) < 0; }
	};
};


namespace std
{
	//* Allow implicit use of [eon::string] as key/value when used in
	//* containers such as 'std::unordered_map' and 'std::unordered_set'.
	template<>
	struct hash<::eon::string> {
		inline size_t operator()( const ::eon::string& rhs ) const {
			return static_cast<size_t>( rhs.hash() ); } };
	template<>
	struct equal_to<::eon::string> {
		inline bool operator()( const ::eon::string& lhs, const ::eon::string& rhs ) const {
			return lhs == rhs; } };

	//* Allow implicit use of [eon::string_ptr] as key/value when used in
	//* containers such as 'std::unordered_map' and 'std::unordered_set'
	template<>
	struct hash<::eon::string_ptr> {
		inline size_t operator()( const ::eon::string_ptr& rhs ) const {
			return static_cast<size_t>( rhs->hash() ); } };
	template<>
	struct equal_to<::eon::string_ptr> {
		inline bool operator()( const ::eon::string_ptr& lhs, const ::eon::string_ptr& rhs ) const {
			return *lhs == *rhs; } };

	//* Allow implicit use of [eon::string_ptr] as key/value when used in
	//* containers such as 'std::map' and 'std::set'
	struct string_ptr_lt {
		inline bool operator()( const ::eon::string_ptr& lhs, const ::eon::string_ptr& rhs ) const {
			return *lhs < *rhs; } };

	//* Allow [eon::string] to be streamed
	inline ostream& operator<<( ostream& o, const ::eon::string& str ) { o << str.stdstr(); return o; }
	inline istream& operator>>( istream& i, ::eon::string& str ) { std::string tmp; i >> tmp; str = tmp; return i; }
};
