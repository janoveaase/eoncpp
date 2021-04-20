#pragma once
#include "UniChar.h"
#include "Substring.h"
#include <set>
#include <list>

/*
	UTF-8 String Class

	Full UTF-8 support, with lots of useful methods - a lot more than found in
	std::string (which is not good for UTF-8).

	For most string needs, you should use this class and not std::string!

	Will throw InvalidUTF8 if not valid UTF-8 character/string!
	WARNING: Not thread-safe!

	NOTE that the string class uses string_iterator for iteration. It is always
	a const-iterator (you cannot modify UTF-8 characters in-place), and it
	allows both forward and backward iteration.

	NOTE that the string class uses substring objects to represent substrings
	of itself. (A substring can easily be turned into a string of its own by
	using it as argument to a string constructor. A substring is just two
	iterators referring to locations within the same actual string. (Which can
	also be a std::string or const char* (C string).)
	
	NOTE that calling the string::substr method without any arguments produces
	a substring object that encompasses the entire string. Calling
	string::substr with one or two iterator arguments, creates an actual
	substring for the specified area of the string.

	The substring class implements many of the methods that are used by string,
	and so you can use these methods directly on substring, or create
	substrings of strings to make it fit into one of the methods. (The string
	class expects you to construct substrings as arguments for many of its
	methods, rather than allowing iterators directly.)
*/


// Detect platform
#if defined( WIN32 ) || defined( _WIN32 ) || defined( WIN64 ) || defined( _WIN64 )
#	define EON_WINDOWS
#elif defined( __APPLE__ ) || defined( __MACH__ )
#	define EON_APPLE
#elif defined( __unix ) || defined( __unix__ )
#	define EON_UNIX
#endif

namespace eon
{
	// A reference to a eon_char in a string
	struct Utf8CharRef
	{
		eon_pos BytePos{ 0 };	// Position in bytes
		eon_pos CharPos{ 0 };	// Position in utf8chars
	};


	// Main string class
	class string
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Definitions
		//
	public:
		using iterator = string_iterator;
		using const_iterator = string_iterator;




		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:
		string() = default;
		
		// Standard copy and move construction
		inline string( string&& other ) noexcept {
			*this = std::move( other ); }
		inline string( const string& other ) { *this = other; }

		// Construct from substring
		inline string( const iterator& beg, const iterator& end ) {
			*this = substring( beg, end ); }
		inline string( const substring& other ) { *this = other; }

		// Construct from other string sources
		inline string( const std::string& stdstr ) { *this = stdstr; }
		inline string( const char* cstr ) { *this = cstr; }
		inline string( const char* cstr, size_t size ) {
			*this = std::string( cstr, size ); }

		// Construct from single codepoint/character
		inline explicit string( eon_char codepoint ) { *this = codepoint; }
		inline explicit string( char c ) {
			*this = static_cast<eon_char>( c ); }
		inline explicit string( unsigned char c ) {
			*this = static_cast<eon_char>( c ); }

		// Construct from list of codepoints/characters
		inline string( std::initializer_list<eon_char> codepoints ) {
			*this = codepoints; }
		inline string( const std::vector<eon_char>& codepoints ) {
			*this = codepoints; }
		inline string( std::initializer_list<char> chars ) { *this = chars; }
		inline string( std::initializer_list<unsigned char> chars ) {
			*this = chars; }

		// Construct as a number of copies of a codepoint or other string
		inline string( size_t copies, eon_char codepoint ) {
			assign( copies, codepoint ); }
		inline string( size_t copies, const string& other ) {
			assign( copies, other ); }
		inline string( size_t copies, const std::string& stdstr ) {
			assign( copies, stdstr ); }
		inline string( size_t copies, const substring& other ) {
			assign( copies, other ); }

		// Construct as "true" or "false" based on a boolean input value
		inline explicit string( bool value ) { *this = value; }

		// Construct as conversion from number
		inline explicit string( long value ) { *this = toString( value ); }
		inline explicit string( unsigned long value ) {
			*this = toString( value ); }
#ifndef EON_WINDOWS
		inline explicit string( short value ) { *this = toString( value ); }
		inline explicit string( unsigned short value ) {
			*this = toString( value ); }
		inline explicit string( int value ) { *this = toString( value ); }
		inline explicit string( unsigned int value ) {
			*this = toString( value ); }
		inline explicit string( int8_t value ) { *this = toString( value ); }
		inline explicit string( long long unsigned int value ) {
			*this = toString( value ); }
#endif
#ifndef EON_UNIX
		inline explicit string( int16_t value ) { *this = toString( value ); }
		inline explicit string( uint16_t value ) { *this = toString( value ); }
		inline explicit string( int32_t value ) { *this = toString( value ); }
		inline explicit string( uint32_t value ) { *this = toString( value ); }
		inline explicit string( int64_t value ) { *this = toString( value ); }
		inline explicit string( uint64_t value ) { *this = toString( value ); }
#endif
#ifdef EON_APPLE
		inline explicit string( size_t value ) {
			*this = static_cast<uint64_t>( value ); }
#endif
		inline explicit string( float value ) {
			*this = toString( static_cast<double>( value ) ); }
		inline explicit string( double value ) { *this = toString( value ); }

		virtual ~string() = default;




		///////////////////////////////////////////////////////////////////////
		//
		// Assignment methods
		//
	public:

		// Assign from array of codepoints or characters
		string& assign( const eon_char* codepoints, size_t size );
		string& assign( const char* chars, size_t size );

		// Assign a number of copies
		string& assign( size_t copies, eon_char codepoint );
		inline string& assign( size_t copies, char chr ) {
			return assign( copies, static_cast<eon_char>( chr ) ); }
		string& assign( size_t copies, const string& other );
		string& assign( size_t copies, const std::string& stdstr );
		string& assign( size_t copies, const substring& sub );

		// Basic copy and move assign
		inline string& operator=( const string& other ) {
			Bytes = other.Bytes; NumChars = other.NumChars; return *this; }
		inline string& operator=( string&& other ) noexcept {
			Bytes = std::move( other.Bytes );
			NumChars = other.NumChars; other.NumChars = 0; return *this; }

		// Copy assign from substring
		string& operator=( const substring& sub );

		// Copy assign from other string sources
		inline string& operator=( const std::string& stdstr ) {
			return assign( stdstr.c_str(), stdstr.size() ); }
		inline string& operator=( const char* cstr ) {
			return assign( cstr, strlen( cstr ) ); }

		// Copy assign from single codepoint/character
		inline string& operator=( eon_char codepoint ) { uint32_t bytes;
			auto num = iterator::unicodeToBytes( codepoint, bytes );
			return assign( (const char*)&bytes, num ); }
		inline string& operator=( char c ) {
			return *this = static_cast<eon_char>( c ); }
		inline string& operator=( unsigned char c ) {
			return *this = static_cast<eon_char>( c ); }

		// Copy assign from list of codepoints/characters
		string& operator=(
			const std::initializer_list<eon_char>& codepoints );
		string& operator=( const std::vector<eon_char>& codepoints );
		string& operator=( const std::initializer_list<char>& chars );
		string& operator=( const std::initializer_list<unsigned char>& chars );

		// Copy assign as "true" or "false" based on a boolean input value
		inline string& operator=( bool value ) {
			return *this = value ? "true" : "false"; }
		



		///////////////////////////////////////////////////////////////////////
		//
		// Basic read-only access
		//
	public:

		// Get number of unicode characters in string
		inline eon_pos numChars() const noexcept { return NumChars; }
		inline eon_pos length() const noexcept { return NumChars; }

		// Get number of bytes in string
		inline eon_pos numBytes() const noexcept { return Bytes.size(); }

		// Get buffer size in number of bytes
		inline eon_pos bufferSize() const noexcept { return Bytes.capacity(); }

		// Check if the string is empty
		inline bool empty() const noexcept { return NumChars == 0; }

		// Access the underlying bytes as a const std::string
		inline const std::string& stdstr() const noexcept { return Bytes; }

		// Access the underlying bytes as a const C string
		inline const char* c_str() const noexcept { return Bytes.c_str(); }

		// Access individual immutable bytes
		inline char byte( eon_pos pos ) const { return Bytes[ pos ]; }

		// Check if the string is surrounded by quotation marks
		inline bool isDoubleQuoted() const noexcept {
			return substr().enclosed( DblQuoteChr ); }
		inline bool isDoubleQuoted( const substring& sub ) const noexcept {
			return sub.enclosed( DblQuoteChr ); }

		inline bool isSingleQuoted() const noexcept {
			return substr().enclosed( SglQuoteChr ); }
		inline bool isSingleQuoted( const substring& sub ) const noexcept {
			return sub.enclosed( SglQuoteChr ); }

		// Check if the string is 'blank' (only whitespaces)
		inline bool blank() const noexcept { return substr().blank(); }

		// Get number of indentations for the first (possibly only) line of
		// text in the string.
		inline eon_pos indentationLevel( eon_char indentation_char = TabChr )
			const noexcept { return substr().indentationLevel(); }

		// Get characters as a vector of eon_char elements
		inline std::vector<eon_char> chars() const { return substr().chars(); }
		inline std::vector<eon_char> chars( const iterator& start ) const {
			return substr( start, end() ).chars(); }
		inline std::vector<eon_char> chars( const iterator& start,
			const iterator& end ) const {
			return substr( start, end ).chars(); }
		inline std::vector<eon_char> chars( const substring& sub ) const {
			return sub.lowToHigh().chars(); }

		// Convert string formatted as "<byte pos>:<char pos>" into an iterator
		// for this string. Returns end() if invalid position!
		// (See encode(iterator) method.)
		iterator decodeIterator( const string& encoded_iterator );

		// Convert string formatted as
		// "<byte pos>:<char pos>-<byte pos>:<char pos>" into a substring for
		// this string. Returns invalid substring if it does not fit the string.
		// (See encode(substring) method.)
		substring decodeSubstring( const string& encode_substring );




		///////////////////////////////////////////////////////////////////////
		//
		// Iteration and iterator access
		//
	public:
		inline iterator begin() const noexcept { return iterator(
			Bytes.c_str(), Bytes.size(), NumChars, Bytes.c_str() ); }
		inline iterator end() const noexcept { return iterator( Bytes.c_str(),
			Bytes.size(), NumChars, Bytes.c_str() + Bytes.size(), NumChars ); }
		inline iterator last() const noexcept {
			return NumChars > 0 ? end() - 1 : end(); }

		// Given a byte position within the string, get the (closest) character
		// position (in the form of an iterator).
		// The counting can start from a known position
		// Returns 'end()' if out of scope!
		iterator bytePos( eon_pos pos, iterator start = iterator() ) const;

		// Given an iterator for another string (typically the original, where
		// 'this' is a moved or modified version), get a matching iterator into
		// 'this' string.
		// If the other iterator is totally incompatible, the return value will
		// be 'end()'.
		// This method is safe, but involves linear counting!
		iterator rebase( const iterator& other ) const noexcept;
		inline substring rebase( const substring& other ) const noexcept {
			return substring( rebase( other.begin() ), rebase(
				other.end() ) ); }

		// Same as rebase, but faster (since there is no counting).
		// WARNING: Using this method requires that you know for sure that the
		//          strings of 'other' and 'this' are identical - at least up
		//          to and including the codepoint of the iterator.
		//          Getting this wrong may put the new iterator in an
		//          undefined state or result in undefined behavior when used!
		iterator rebaseMoved( const iterator& other ) const noexcept;
		inline substring rebaseMoved( const substring& other ) const noexcept {
			return substring( rebaseMoved( other.begin() ), rebaseMoved(
				other.end() ) ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Searching
		//
		// Methods searching for sub-strings will return a substr object (that
		// is 'false' (invalid) if not found).
		//
	public:

		// Check if the string contains the specific value (character or
		// substring) anywhere inside
		inline bool contains( eon_char codepoint ) const noexcept {
			return substr().contains( codepoint ); }
		inline bool contains( const string& sub ) const noexcept {
			return substr().contains( sub.substr() ); }

		// Check if the string contains any of the charcters in the given
		// string
		inline bool containsAnyOf( const string& characters ) const noexcept {
			return substr().containsAnyOf( characters.substr() ); }


		// Find first occurrence
		inline substring findFirst( const string& to_find ) const noexcept {
			return substr().findFirst( to_find.substr() ); }
		inline substring findFirst( const string& to_find,
			const substring& sub ) const noexcept {
			sub.assertSameBuffer( Bytes.c_str() );
			return sub.lowToHigh().findFirst( to_find.substr() ); }

		inline substring findFirst( eon_char to_find ) const noexcept {
			return substr().findFirst( to_find ); }
		inline substring findFirst( eon_char to_find, const substring& sub )
			const noexcept { sub.assertSameBuffer( Bytes.c_str() );
				return sub.lowToHigh().findFirst( to_find ); }

		inline substring findFirst( char to_find ) const noexcept { return
			substr().findFirst( static_cast<eon_char>( to_find ) ); }
		inline substring findFirst( char to_find, const substring& sub )
			const noexcept { sub.assertSameBuffer( Bytes.c_str() );
				return sub.lowToHigh().findFirst( static_cast<eon_char>(
					to_find ) ); }

		// Find last occurrence
		inline substring findLast( const string& to_find ) const noexcept {
			return substr().highToLow().findLast( to_find.substr() ); }
		inline substring findLast( const string& to_find, substring sub )
			const noexcept { sub.assertSameBuffer( Bytes.c_str() );
				return sub.highToLow().findLast(
					to_find.substr().highToLow() ); }

		inline substring findLast( eon_char to_find ) const noexcept {
			return substr().highToLow().findLast( to_find ); }
		inline substring findLast( eon_char to_find, substring sub ) const
			noexcept { sub.assertSameBuffer( Bytes.c_str() );
				return sub.highToLow().findLast( to_find ); }

		inline substring findLast( char to_find ) const noexcept {
			return substr().highToLow().findLast( static_cast<eon_char>(
				to_find ) ); }
		inline substring findLast( char to_find, substring sub )
			const noexcept { sub.assertSameBuffer( Bytes.c_str() );
				return sub.highToLow().findLast( static_cast<eon_char>(
					to_find ) ); }


		// Find first occurrence of a character in the specified set
		inline iterator findFirstOf( const string& characters ) const
			noexcept { return substr().findFirstOf( characters.substr() ); }
		inline iterator findFirstOf( const string& characters, substring sub )
			const noexcept { sub.assertSameBuffer( Bytes.c_str() );
				return sub.findFirstOf( characters.substr() ); }
		
		// Find last occurrence of a character in the specified set
		inline iterator findLastOf( const string& characters ) const
			noexcept { return substr().findLastOf( characters.substr() ); }
		inline iterator findLastOf( const string& characters, substring sub )
			const noexcept { sub.assertSameBuffer( Bytes.c_str() );
				return sub.findLastOf( characters.substr() ); }

		// Find first occurrence of a character not in the specified set
		inline iterator findFirstNotOf( const string& characters ) const
			noexcept { return substr().findFirstNotOf( characters.substr() ); }
		inline iterator findFirstNotOf( const string& characters,
			substring sub ) const noexcept {
			sub.assertSameBuffer( Bytes.c_str() ); 
			return sub.findFirstNotOf( characters.substr() ); }
		
		// Find last occurrence of a character not in the specified set
		inline iterator findLastNotOf( const string& characters ) const
			noexcept { return substr().findLastNotOf( characters.substr() ); }
		inline iterator findLastNotOf( const string& characters,
			substring sub ) const noexcept {
			sub.assertSameBuffer( Bytes.c_str() );
			return sub.findLastNotOf( characters.substr() ); }


		// Find the first character in 'this' that differs from the other
		// string, end() if none (equal strings).
		inline iterator findFirstDiff( const string& other ) const noexcept {
			return substr().findFirstDiff( other.substr() ); }
		inline iterator findFirstDiff( const substring& sub ) const noexcept {
			sub.assertSameBuffer( Bytes.c_str() );
			return substr().findFirstDiff( sub ); }


		// Find first occurrence, but skip sections enclosed by quotes or
		// parenthesis (braces). For braced sections, you should only use '(',
		// '[' or '{'. For any other type of bracing, use
		//             substring::findFirstIgnoreSections method!
		// For quoted text, backslashed quote marks will be ignored!
		inline substring findFirstNotDoubleQuoted( const string& other )
			const noexcept { return substr().findFirstIgnoreSections(
				other.substr(), DblQuoteChr ); }
		inline substring findFirstNotDoubleQuoted( eon_char codepoint )
			const noexcept { return substr().findFirstIgnoreSections(
				codepoint, DblQuoteChr ); }
		inline substring findFirstNotSingleQuoted( const string& other )
			const noexcept { return substr().findFirstIgnoreSections(
				other.substr(), SglQuoteChr ); }
		inline substring findFirstNotSingleQuoted( eon_char codepoint )
			const noexcept { return substr().findFirstIgnoreSections(
				codepoint, SglQuoteChr ); }
		inline substring findFirstNotBraced( const string& other,
			eon_char brace = '(' ) const noexcept {
			return substr().findFirstIgnoreSections( other.substr(), brace,
				brace == '(' ? ')' : brace == '[' ? ']' : brace == '{' ? '}'
				: brace ); }
		inline substring findFirstNotBraced( eon_char codepoint,
			eon_char brace = '(' ) const noexcept {
			return substr().findFirstIgnoreSections( codepoint, brace,
				brace == '(' ? ')' : brace == '[' ? ']' : brace == '{' ? '}'
				: brace ); }


		// Find character classes
		inline iterator findFirst( charcat category ) const noexcept {
			return substr().findFirst( category ); }
		inline iterator findFirst( charcat category, const substring& sub )
			const noexcept { sub.assertSameBuffer( Bytes.c_str() );
				return sub.findFirst( category ); }
		inline iterator findLast( charcat category ) const noexcept {
			return substr().highToLow().findLast( category ); }
		inline iterator findLast( charcat category, const substring& sub )
			const noexcept { sub.assertSameBuffer( Bytes.c_str() );
				return sub.highToLow().findLast( category ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Counting
		//
	public:

		// Count occurrences of a character
		inline size_t count( eon_char to_count ) const noexcept {
			return substr().count( to_count ); }
		inline size_t count( eon_char to_count, const substring& sub )
			const noexcept { sub.assertSameBuffer( Bytes.c_str() );
				return sub.count( to_count ); }

		// Count occurrences of a sub-string
		// NOTE: All occurrences are counted, even overlaps!
		inline size_t count( const string& to_count ) const noexcept {
			return substr().count( to_count.substr() ); }
		inline size_t count( const string& to_count, const substring& sub )
			const noexcept { sub.assertSameBuffer( Bytes.c_str() );
				return sub.count( to_count.substr() ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Modifiers
		//

	public:

		// Clear the string contents
		inline void clear() noexcept { Bytes.clear(); NumChars = 0; }

		// Reserve a number of bytes in cache
		// Does nothing if byte size is already >= specified size
		inline void reserve( size_t byte_size ) { Bytes.reserve( byte_size ); }

		// Concatenation
		inline string& operator+=( const string& other ) {
			Bytes += other.Bytes; NumChars += other.NumChars; return *this; }
		inline string& operator+=( const substring& area ) {
			if( area.validUTF8() ) { Bytes.append( area.begin().byteData(),
				area.numBytes() ); NumChars += area.numChars(); return *this; }
			else return *this += string( area ); }
		inline string& operator+=( const std::string& stdstr ) {
			return *this += substring( stdstr ); }
		inline string& operator+=( const char* cstr ) {
			return *this += substring( cstr ); }
		inline string& operator+=( eon_char codepoint ) {
			return *this += string( codepoint ); }
		inline string& operator+=( char chr ) {
			return *this += string( chr ); }
		inline string& operator+=( unsigned char chr ) {
			return *this += string( chr ); }

		// Insert into the string, moving existing data from (and including)
		// the iterator position outwards. Returns a new iterator for the same
		// position that where the insert was done. (The old is invalid after
		// insert!)
		iterator insert( const iterator& pos, const string& substr );
		inline iterator insert( const iterator& pos, eon_char codepoint ) {
			return insert( pos, string( codepoint ) ); }

		// Erase a substring from the string
		string& erase( const substring& sub ) noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Copy and transform
		//
	public:

		// Return a copy of the entire string with all letters (in the
		// substring) upper or lower case
		inline string upper() const { return upper( substr() ); }
		inline string lower() const { return lower( substr() ); }
		string upper( const substring& sub ) const;
		string lower( const substring& sub ) const;

		// Return a copy of the entire string with first letter (within the
		// substring) upper case (if not already), leave the others as-is.
		inline string ucFirst() const { return ucFirst( substr() ); }
		string ucFirst( const substring& sub ) const;

		// Return a copy of the entire string with first letter of every word
		// (within the substring) transformed to upper case (if not already).
		// Other characters will not be changed.
		inline string ucWords() const { return ucWords( substr() ); }
		string ucWords( const substring& sub ) const;

		// Return a copy of the entire string with first letter of every first
		// word of every sentence (within the substring)) transformed to upper
		// case (if not already). Other words will not be changed.
		// NOTE: Only '.', ':', '!' and '?' are considered sentence dividers!
		inline string ucSentences() const { return ucSentences( substr() ); }
		string ucSentences( const substring& sub ) const;

		// Get a substring
		// NOTE: These methods return a substring object that only references a
		//       section of 'this' string. To get an independent string, create
		//       a string with the substring object as argument to the
		//       constructor.
		inline substring substr() const { return substring( begin(), end() ); }
		inline substring substr( const iterator& start ) const {
			start.assertSameBuffer( Bytes.c_str() );
			return substring( start, end() ); }
		inline substring substr( const iterator& start, const iterator& end )
			const { start.assertSameBuffer( Bytes.c_str() );
				end.assertSameBuffer( Bytes.c_str() );
				return substring( start, end ); }

		// Get a substring based on character position and count
		// This involves counting characters, but is optimized to count from
		// closest known position.
		inline substring substr( size_t start, size_t size ) const {
			auto sub = substr(); return substring( sub.iterator( start ),
				sub.iterator( start + size ) ); }

		// Get a substring (slice) based on character position and count from
		// start (positive value) and/or from end (negative value).
		// Note that both 'start' and 'end' are inclusive. A slice from 0 to -1 
		// will include the entire string, 2 to 2 will return a substring
		// containing a single character.
		// If 'start' is less than 'end', then a high-to-low substring is
		// returned.
		substring slice( int64_t start, int64_t end ) const;

		// Get substring before/after first/last occurrence of the delimiter
		substring beforeFirst( const string& delimiter ) const;
		substring beforeFirst( eon_char delimiter ) const;
		substring beforeLast( const string& delimiter ) const;
		substring beforeLast( eon_char delimiter ) const;
		substring afterFirst( const string& delimiter ) const;
		substring afterFirst( eon_char delimiter ) const;
		substring afterLast( const string& delimiter ) const;
		substring afterLast( eon_char delimiter ) const;


		// Get a copy of the entire string with all occurrences of 'find'
		// (within the substring) replaced with 'replacement'
		inline string replace( const string& find, const string& replacement )
			const { return replace( find, replacement, substr() ); }
		string replace( const string& find, const string& replacement,
			const substring& sub ) const;

		inline string replace( eon_char find, eon_char replacement ) const {
			return replace( find, replacement, substr() ); }
		string replace( eon_char find, eon_char replacement,
			const substring& sub ) const;

		inline string replace( char find, char replacement ) const {
			return replace( static_cast<eon_char>( find ),
				static_cast<eon_char>( replacement ) ); }
		inline string replace( char find, char replacement,
			const substring& sub ) const { return replace(
				static_cast<eon_char>( find ), static_cast<eon_char>(
					replacement ), sub ); }
		
		// Get a copy of the entire string with the specified substring
		// replaced with 'replacement'
		inline string replace( const substring& sub,
			const string& replacement ) const {
			return string( substr( begin(), sub.begin() ) ) + replacement
				+ string( substr( sub.end() ) ); }
		inline string replace( const substring& sub, eon_char replacement )
			const { return string( substr( begin(), sub.begin() ) )
				+ replacement + string( substr( sub.end() ) ); }


		// Get a copy of the entire string with all occurrences of 'to_remove'
		// removed (within the substring)
		inline string remove( const string& to_remove ) const {
			return replace( to_remove, string::Empty, substr() ); }
		inline string remove( const string& to_remove, const substring& sub )
			const { return replace( to_remove, string::Empty, sub ); }

		inline string remove( eon_char to_remove ) const { return
			replace( string( to_remove ), string::Empty, substr() ); }
		inline string remove( eon_char to_remove, const substring& sub )
			const { return replace( string( to_remove ),
				string::Empty, sub ); }

		inline string remove( char to_remove ) const { return
			replace( string( to_remove ), string::Empty, substr() ); }
		inline string remove( char to_remove, const substring& sub ) const {
			return replace( string( to_remove ), string::Empty, sub ); }


		// Return a copy of the entire string with standard C escapes processed
		// or reverted (within the substring).
		inline string escape() const { return escape( substr() ); }
		string escape( const substring& sub ) const;
		inline string unescape() const { return unescape( substr() ); }
		string unescape( const substring& sub ) const;

		// Return a copy of the entire string with non-printable ASCII
		// characters and non-ASCII characters (within the substring) replaced
		// with HTML character codes
		// E.g. "\t" as "&#9;".
		inline string escapeNonPrintable() const {
			return escapeNonPrintable( substr() ); }
		string escapeNonPrintable( const substring& sub ) const;
		inline string unescapeNonPrintable() const {
			return unescapeNonPrintable( substr() ); }
		string unescapeNonPrintable( const substring& sub ) const;


		// Return a copy of the entire string, (substring) double or single
		// quoted
		// NOTE: Will only add quotes if not already quoted!
		inline string doubleQuote() const { return doubleQuote( substr() ); }
		string doubleQuote( const substring& sub ) const;

		inline string singleQuote() const { return singleQuote( substr() ); }
		string singleQuote( const substring& sub ) const;


		// Return a copy of the entire string, (substring) surrounding double
		// or single quotation marks removed
		// NOTE: Will only remove if the same quotation character is at the
		//       start and the end!
		inline string unQuote() const { return unQuote( substr() ); }
		string unQuote( const substring& sub ) const;


		// Return a copy of the entire string, (substring) characters in
		// reversed order
		inline string reverse() const { return reverse( substr() ); }
		string reverse( const substring& sub ) const;


		// Split string on every occurrence of the delimiter into a sequential
		// container (list/vector) - requires the push_back method!
		// If a max number of elements is specified, the last element will
		// reference the unsplit remainder.
		// All elements will be substring objects!
		template<typename container_t>
		container_t splitSequential( const string& delimiter,
			size_t max_elements = SIZE_MAX ) const {
			return substr().splitSequential<container_t>( delimiter.substr(),
				max_elements ); }
		template<typename container_t>
		container_t splitSequential( eon_char delimiter,
			size_t max_elements = SIZE_MAX ) const {
			return substr().splitSequential<container_t>( delimiter,
				max_elements ); }

		// Split string on every occurrence of the delimiter into a non-
		// sequential container (set, multiset, unordered_set,
		// unordered_multiset) - requires the insert method!
		template<typename container_t>
		container_t splitNonsequential( const string& delimiter ) const {
			return substr().splitNonSequential<container_t>(
				delimiter.substr() ); }
		template<typename container_t>
		container_t splitNonSequential( eon_char delimiter ) const {
			return substr().splitNonSequential<container_t>( delimiter ); }


		// Join string elements to make a single string, using 'this' as the
		// delimiter
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
		template<typename container_t>
		string join( const container_t& source ) const {
			return join<container_t::const_iterator>(
				source.begin(), source.end() ); }


		// Get a substring with leading and/or trailing spaces excluded
		inline substring trim() const { return substr().trim(); }
		inline substring trimLeading() const { return substr().trimLeading(); }
		inline substring trimTrailing() const { return substr().trimTrailing(); }


		// Get a copy of the string padded up to the specified size
		inline string padLeft( size_t target_size,
			eon_char fill = SpaceChr ) const {
			return NumChars < target_size
				? string( target_size - NumChars, fill ) += *this : *this; }
		inline string padRight( size_t target_size,
			eon_char fill = SpaceChr ) const {
			return NumChars < target_size
				? *this + string( target_size - NumChars, fill ) : *this; }
		string padLeftAndRight( size_t target_size,
			eon_char fill = SpaceChr ) const;


		// Get a copy of the string with all lines indented
		// NOTE: If the first line is already at the specified level of
		//       indentation, it will not be indented further!
		// NOTE: Subsequent lines will be indented fully regardless of existing
		//       indentation!
		// NOTE: Empty lines will not be indented!
		string indentLines( size_t indentation_level,
			eon_char indentation_char = TabChr ) const;


		// Concatenation
		friend inline string operator+( const string& a, const string& b ) {
			return string( a ) += b; }
		friend inline string operator+( const string& a, const substring& b ) {
			return string( a ) += b; }
		friend inline string operator+( const substring& a, const string& b ) {
			return string( a ) += b; }
		friend inline string operator+( const string& a, const std::string& b )
			{ return string( a ) += string( b ); }
		friend inline string operator+( const std::string& a, const string& b )
			{ return string( a ) += b; }
		friend inline string operator+( const string& a, const char* b ) {
			return string( a ) += string( b ); }
		friend inline string operator+( const char* a, const string& b ) {
			return string( a ) += b; }
		friend inline string operator+( const string& a, eon_char b ) {
			return string( a ) += b; }
		friend inline string operator+( eon_char a, const string& b ) {
			return string( a ) += b; }




		///////////////////////////////////////////////////////////////////////
		//
		// Numbers as strings
		//
		// Strings can hold numbers and these methods provides some additional
		// functionality for handling these.
		//
	public:

		// Check if numeral characters only (including non-ASCII)
		inline bool numeralsOnly() const noexcept {
			return substr().numeralsOnly(); }
		inline bool numeralsOnly( const substring& sub ) const {
			sub.assertSameBuffer( Bytes.c_str() );
			return sub.numeralsOnly(); }

		// Check if ASCII digits only
		inline bool isUInt() const noexcept { return substr().isUInt(); }
		inline bool isUInt( const substring& sub ) const {
			sub.assertSameBuffer( Bytes.c_str() ); return sub.isUInt(); }

		// Check if '+' or '-' followed by ASCII digits only
		inline bool isInt() const noexcept { return substr().isInt(); }
		inline bool isInt( const substring& sub ) const {
			sub.assertSameBuffer( Bytes.c_str() ); return sub.isInt(); }

		// Check if optional '+' or '-', followed by ASCII digits and a single
		// decimal separator only (which may occur first or last)
		inline bool isFloat( eon_char decimal_separator = PointChr )
			const noexcept { return substr().isFloat( decimal_separator ); }
		inline bool isFloat( const substring& sub,
			eon_char decimal_separator = PointChr ) const {
			sub.assertSameBuffer( Bytes.c_str() ); return sub.isFloat(
				decimal_separator ); }


		// Convert String to number
		// Non-numbers (including non-ASCII numerals) will be returned as zero!
		inline int32_t toInt32() const { return substr().toInt32(); }
		inline int32_t toInt32( const substring& pos ) const {
			pos.assertSameBuffer( Bytes.c_str() );
			return substr().toInt32(); }
		inline int64_t toInt64() const { return substr().toInt64(); }
		inline int64_t toInt64( const substring& pos ) const {
			pos.assertSameBuffer( Bytes.c_str() );
			return substr().toInt64(); }
		inline uint32_t toUInt32() const { return substr().toUInt32(); }
		inline uint32_t toUInt32( const substring& pos ) const {
			pos.assertSameBuffer( Bytes.c_str() );
			return substr().toUInt32(); }
		inline uint64_t toUInt64() const { return substr().toUInt64(); }
		inline uint64_t toUInt64( const substring& pos ) const {
			pos.assertSameBuffer( Bytes.c_str() );
			return substr().toUInt64(); }
		inline double toDouble() const { return substr().toDouble(); }
		inline double toDouble( const substring& pos ) const {
			pos.assertSameBuffer( Bytes.c_str() );
			return substr().toDouble(); }


		// Assuming the entire substring is a number, get a reduced substring
		// where leading integer zeros and trailing fractional zeros are
		// removed - including the decimal separator if all fractional zeros.
		// (If the number ends with the separator, it will be excluded.)
		// NOTE: If the number is signed, leading integer zeros will be
		//       included to keep the sign.
		// Example: 0001.0000 -> 1
		inline substring trimNumber() const { return substr().trimNumber(); }

		// Same as trimNumber, but if all fractional zeros, leave the decimal
		// separator and one zero. (If the number ends with the separator, it
		// will be included.)
		// Example: 0001.0000 -> 1.0
		inline substring trimFloat() const { return substr().trimFloat(); }


		// Get a copy of the number string with the integer part separated into
		// thousands.
		// Example: 34525525.4621 -> 34,525,525.4621
		string separateThousands( eon_char thousands_sep = CommaChr,
			eon_char decimal_separator = PointChr ) const;

		// Get a copy of the number string with fractional part reduced to a
		// specific number of decimals, rounded off to nearest value. If there
		// is no fractional part or it is already at the same or fewer
		// decimals, then an unaltered copy is returned.
		// Zero decimals will result in removal of the fractional part and
		// possibly an increase of the integer part (if rounding up).
		string roundNumber( size_t max_decimals = 0,
			eon_char decimal_separator = PointChr ) const;




		///////////////////////////////////////////////////////////////////////
		//
		// String hashing
		//
	public:

		// FNV-1a hash algorithm
		inline uint32_t hash32() const noexcept { return substr().hash32(); }
		inline uint64_t hash64() const noexcept { return substr().hash64(); }
		inline size_t hash() const noexcept {
			return sizeof( size_t ) == 4 ? hash32() : hash64(); }




		///////////////////////////////////////////////////////////////////////
		//
		// Comparing
		//
	public:

		// Check if the string starts with a specific value
		inline bool startsWith( const string& value ) const noexcept {
			return substr().startsWith( value.substr() ); }
		inline bool startsWith( eon_char value ) const noexcept {
			return *begin() == value; }

		// Check if string ends with a specific value
		inline bool endsWith( const string& value ) const noexcept {
			return substr().endsWith( value.substr() ); }
		inline bool endsWith( eon_char value ) const noexcept {
			return *last() == value; }


		// Basic comparison
		// If CompareType::faster is specified, then compare as fast as
		// possible and only return -1, 0 or 1. If CompareType::diff_pos is
		// specified, return -pos, 0 or pos - where pos is the first character
		// that is different between the two.
		// NOTE: You can also use substring::compare for comparing sections of
		//       strings.
		inline int compare( const string& other,
			CompareType type = CompareType::faster ) const noexcept {
			return substr().compare( other.substr(), type ); }

		// Comparison operators - always using CompareType::faster!
		inline bool operator<( const string& other ) const noexcept {
			return compare( other ) < 0; }
		inline bool operator<=( const string& other ) const noexcept {
			return compare( other ) <= 0; }
		inline bool operator>( const string& other ) const noexcept {
			return compare( other ) > 0; }
		inline bool operator>=( const string& other ) const noexcept {
			return compare( other ) >= 0; }
		inline bool operator==( const string& other ) const noexcept {
			return compare( other ) == 0; }
		inline bool operator!=( const string& other ) const noexcept {
			return compare( other ) != 0; }

		// Compare while ignoring case
		inline int iCompare( const string& other ) const noexcept {
			return substr().iCompare( other.substr() ); }
		inline int iCompare( const char* cstr ) const noexcept {
			return substr().iCompare( substring( cstr ) ); }
		inline int iCompare( const std::string& stdstr ) const noexcept {
			return substr().iCompare( substring( stdstr ) ); }

		// Compare against C-string
		inline int compare( const char* cstr,
			CompareType type = CompareType::faster ) const noexcept {
			return substr().compare( substring( cstr ), type ); }
		inline bool operator<( const char* cstr ) const noexcept {
			return compare( cstr ) < 0; }
		inline bool operator<=( const char* cstr ) const noexcept {
			return compare( cstr ) <= 0; }
		inline bool operator>( const char* cstr ) const noexcept {
			return compare( cstr ) > 0; }
		inline bool operator>=( const char* cstr ) const noexcept {
			return compare( cstr ) >= 0; }
		inline bool operator==( const char* cstr ) const noexcept {
			return compare( cstr ) == 0; }
		inline bool operator!=( const char* cstr ) const noexcept {
			return compare( cstr ) != 0; }

		// Comparing with std::string
		inline int compare( const std::string& stdstr,
			CompareType type = CompareType::faster ) const noexcept {
			return substr().compare( substring( stdstr ) ); }
		bool operator<( const std::string& stdstr ) const noexcept {
			return compare( stdstr ) < 0; }
		bool operator<=( const std::string& stdstr ) const noexcept {
			return compare( stdstr ) <= 0; }
		bool operator>( const std::string& stdstr ) const noexcept {
			return compare( stdstr ) > 0; }
		bool operator>=( const std::string& stdstr ) const noexcept {
			return compare( stdstr ) >= 0; }
		bool operator==( const std::string& stdstr ) const noexcept {
			return compare( stdstr ) == 0; }
		bool operator!=( const std::string& stdstr ) const noexcept {
			return compare( stdstr ) != 0; }

		// Comparing with codepoint
		inline int compare( eon_char codepoint ) const noexcept {
			return NumChars == 0 || *begin() < codepoint ? -1
				: *begin() > codepoint ? 1 : NumChars == 1 ? 0 : 1; }
		inline bool operator<( eon_char codepoint ) const noexcept {
			return compare( codepoint ) < 0; }
		inline bool operator<=( eon_char codepoint ) const noexcept {
			return compare( codepoint ) <= 0; }
		inline bool operator>( eon_char codepoint ) const noexcept {
			return compare( codepoint ) > 0; }
		inline bool operator>=( eon_char codepoint ) const noexcept {
			return compare( codepoint ) >= 0; }
		inline bool operator==( eon_char codepoint ) const noexcept {
			return compare( codepoint ) == 0; }
		inline bool operator!=( eon_char codepoint ) const noexcept {
			return compare( codepoint ) != 0; }

		// Comparing with character
		inline int compare( char c ) const noexcept {
			return compare( static_cast<eon_char>( c ) ); }
		inline bool operator<( char c ) const noexcept {
			return compare( static_cast<eon_char>( c ) ) < 0; }
		inline bool operator<=( char c ) const noexcept {
			return compare( static_cast<eon_char>( c ) ) <= 0; }
		inline bool operator>( char c ) const noexcept {
			return compare( static_cast<eon_char>( c ) ) > 0; }
		inline bool operator>=( char c ) const noexcept {
			return compare( static_cast<eon_char>( c ) ) >= 0; }
		inline bool operator==( char c ) const noexcept {
			return compare( static_cast<eon_char>( c ) ) != 0; }
		inline bool operator!=( char c ) const noexcept {
			return compare( static_cast<eon_char>( c ) ) != 0; }





		///////////////////////////////////////////////////////////////////////
		//
		// Wide string support
		//
		// Note that this support is limited and somewhat costly.
		// Only use when wstring_t or wchar_t is required by some third party
		// code.
		//
	public:

		inline string( const std::wstring& stdwstr ) { *this = stdwstr; }
		inline string( const wchar_t* cstr ) { *this = cstr; }
		string& operator=( const std::wstring& stdwstr );
		string& operator=( const wchar_t* cstr );

		inline operator std::wstring() const { return wstr(); }
		std::wstring wstr() const;

		inline string& operator+=( const std::wstring& to_add ) {
			return *this += string( to_add ); }




		///////////////////////////////////////////////////////////////////////
		//
		// String as list
		// The string is considered a list of elements where each element is
		// separated by a common character or sub-string (';' by default). The
		// separator is only between elements, not before the first or after
		// the last.
		//
		// WARNING: No checking is done to see if an added string contains the
		// separator. Adding "alpha" first and then "beta" is the same as
		// adding "alpha;beta" in one go (if default separator).
		//
		// To split the elements into a container, use a split method.
		//
	public:

		// Add an element to the end of the string-list
		string& push_back( const string& element,
			const string& separator = string( SemiColonChr ) );

		// Add an element to the front of the string-list
		string& push_front( const string& element,
			const string& separator = string( SemiColonChr ) );

		// Remove duplicates and sort the list
		string& orderList( const string& separator = string( SemiColonChr ) );




		///////////////////////////////////////////////////////////////////////
		//
		// Static utility methods
		//
	public:

		// Set locale (used by various compare methods).
		// The default is set automatically at startup!
		static void setLocale( const std::string& name = ".utf8" );

		// Convert other types into string (using std::to_string)
		// This method is limited to whatever is supported by std::to_string.
		template<typename T>
		static inline string toString( T value ) {
			return string( std::to_string( value ) ); }
		
		// Convert iterator into a string format: "<byte pos>:<char pos>"
		// (See decodeIterator method.)
		static inline string encode( const string_iterator& itr ) {
			return toString( itr.numByte() ) + ":" + toString(
				itr.numChar() ); }

		// Convert substring into a string format:
		// "<byte pos>:<char pos>-<byte pos>:<char pos>"
		// (See decodeSubstring method)
		static inline string encode( const substring& sub ) {
			return encode( sub.begin() ) + "-" + encode( sub.end() ); }

		// Get Byte Order Marker for UTF-8
		static const std::string& bom();

		// Check if character belongs to a specific character class, as defined
		// by https://www.fileformat.info/info/unicode/category/index.htm
		static inline bool isLetter( eon_char c ) noexcept { return
			isLetterUpperCase( c ) || isLetterLowerCase( c )
			|| isLetterTitleCase( c ) || isLetterModifier( c )
			|| isLetterOther( c ); }
		static inline bool isLetterUpperCase( eon_char c ) noexcept {
			return Characters::get().isLetterUpperCase( c ); }
		static inline bool isLetterLowerCase( eon_char c ) noexcept {
			return Characters::get().isLetterLowerCase( c ); }
		static inline bool isLetterTitleCase( eon_char c ) noexcept {
			return Characters::get().isLetterTitleCase( c ); }
		static inline bool isLetterModifier( eon_char c ) noexcept {
			return Characters::get().isLetterModifier( c ); }
		static inline bool isLetterOther( eon_char c ) noexcept {
			return Characters::get().isLetterOther( c ); }

		static inline bool isMarkSpacingCombining( eon_char c )
			noexcept { return Characters::get().isMarkSpacingCombining( c ); }
		static inline bool isMarkNonSpacing( eon_char c ) noexcept {
			return Characters::get().isMarkNonSpacing( c ); }
		static inline bool isMarkEnclosing( eon_char c ) noexcept {
			return Characters::get().isMarkEnclosing( c ); }

		static inline bool isNumberAsciiDigit( eon_char c ) noexcept {
			return Characters::get().isNumberAsciiDigit( c ); }
		static inline bool isNumberDecimalDigit( eon_char c )
			noexcept { return Characters::get().isNumberDecimalDigit( c ); }
		static inline bool isNumberLetter( eon_char c ) noexcept {
			return Characters::get().isNumberLetter( c ); }
		static inline bool isNumberOther( eon_char c ) noexcept {
			return Characters::get().isNumberOther( c ); }

		static inline bool isPunctuation( eon_char c ) noexcept {
			return Characters::get().isPunctuation( c ); }
		static inline bool isPunctuationConnector( eon_char c )
			noexcept { return Characters::get().isPunctuationConnector( c ); }
		static inline bool isPunctuationDash( eon_char c ) noexcept {
			return Characters::get().isPunctuationDash( c ); }
		static inline bool isPunctuationOpen( eon_char c ) noexcept {
			return Characters::get().isPunctuationOpen( c ); }
		static inline bool isPunctuationClose( eon_char c ) noexcept {
			return Characters::get().isPunctuationClose( c ); }
		static inline bool isPunctuationInitialQuote( eon_char c )
			noexcept {
			return Characters::get().isPunctuationInitialQuote( c ); }
		static inline bool isPunctuationFinalQuote( eon_char c )
			noexcept { return Characters::get().isPunctuationFinalQuote( c ); }
		static inline bool isPunctuationOther( eon_char c ) noexcept {
			return Characters::get().isPunctuationOther( c ); }

		static inline bool isSymbol( eon_char c ) noexcept {
			return Characters::get().isSymbol( c ); }
		static inline bool isSymbolCurrency( eon_char c ) noexcept {
			return Characters::get().isSymbolCurrency( c ); }
		static inline bool isSymbolModifier( eon_char c ) noexcept {
			return Characters::get().isSymbolModifier( c ); }
		static inline bool isSymbolMath( eon_char c ) noexcept {
			return Characters::get().isSymbolMath( c ); }
		static inline bool isSymbolOther( eon_char c ) noexcept {
			return Characters::get().isSymbolOther( c ); }

		static inline bool isSeparator( eon_char c ) noexcept {
			return Characters::get().isSeparator( c ); }
		static inline bool isSeparatorLine( eon_char c ) noexcept {
			return Characters::get().isSeparatorLine( c ); }
		static inline bool isSeparatorParagraph( eon_char c )
			noexcept { return Characters::get().isSeparatorParagraph( c ); }
		static inline bool isSeparatorSpace( eon_char c ) noexcept {
			return Characters::get().isSeparatorSpace( c ); }


		// Word characters are: letters, number decimal digits and underscore
		static inline bool isWordChar( eon_char c ) noexcept { return
			isLetter( c ) || isNumberDecimalDigit( c ) || c == '_'; }

		// Space characters are: character tabulation and separators
		static inline bool isSpaceChar( eon_char c ) noexcept {
			return ( c >= 0x09 && c <= 0x0D ) || isSeparator( c ); }





		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		static size_t _findDecimalSeparator( std::vector<eon_char>& digits,
			eon_char decimal_separator = PointChr ) noexcept;

		// Given an iterator, round up all the digits in the string all the way
		// up to adding a new '1' in the front if all digits are rounded up.
		// Rounding will stop when rounding up a digit results in a new digit
		// less than 5.
		// NOTE: This method assumes (without checking) that all characters
		// from and including 'i' and to and including 'begin()' are digites or
		// a point!
		static void _roundUp( std::vector<eon_char>& digits, size_t i )
			noexcept;


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

	// More concatenation
	inline string operator+( const substring& a, const substring& b ) {
		return string( a ) += b; }
	inline string operator+( const substring& a, const std::string& b ) {
		return string( a ) += b; }
	inline string operator+( const std::string& a, const substring& b ) {
		return string( a ) += b; }
	inline string operator+( const substring& a, const char* b ) {
		return string( a ) += b; }
	inline string operator+( const char* a, const substring& b ) {
		return string( a ) += b; }

	using string_ptr = std::shared_ptr<string>;


	struct string_lt
	{
		inline bool operator()( const string* lhs, const string* rhs ) const {
			return lhs->compare( *rhs ) < 0; }
	};
};

namespace std
{
	template<>
	struct hash<::eon::string> {
		inline size_t operator()( const ::eon::string& rhs ) const {
			return static_cast<size_t>( rhs.hash64() ); } };
	template<>
	struct equal_to<::eon::string> {
		inline bool operator()( const ::eon::string& lhs,
			const ::eon::string& rhs ) const { return lhs == rhs; } };

/*	template<>
	struct hash<::eon::string*> {
		inline size_t operator()( const ::eon::string* rhs ) const {
			return static_cast<size_t>( rhs->hash64() ); } };
	template<>
	struct equal_to<::eon::string*> {
		inline bool operator()( const ::eon::string* lhs,
			const ::eon::string* rhs ) const {
			return *lhs == *rhs; } };
	struct stringp_lt {
		inline bool operator()( const ::eon::string* lhs,
			const ::eon::string* rhs ) const { return *lhs < *rhs; } };*/

	template<>
	struct hash<::eon::string_ptr> {
		inline size_t operator()( const ::eon::string_ptr& rhs ) const {
			return static_cast<size_t>( rhs->hash64() ); } };
	template<>
	struct equal_to<::eon::string_ptr> {
		inline bool operator()( const ::eon::string_ptr& lhs,
			const ::eon::string_ptr& rhs ) const {
			return *lhs == *rhs; } };
	struct string_ptr_lt {
		inline bool operator()( const ::eon::string_ptr& lhs,
			const ::eon::string_ptr& rhs ) const { return *lhs < *rhs; } };

//	inline ostream& operator<<( ostream& o, const ::eon::string& str ) {
//		o << str.str(); return o; }
//	inline istream& operator>>( istream& i, ::eon::string& str ) {
//		std::string tmp; i >> tmp; str = tmp; return i; }
};
