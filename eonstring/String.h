#pragma once
#include "UniChar.h"
#include "Substring.h"
#include "Locale.h"
#include <eoninlinetest/TestMacros.h>
#include <set>
#include <list>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <unordered_map>



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	// Reference to a codepoint in a UTF-8 string.
	struct Utf8CharRef
	{
		// Number of bytes from start of string.
		index_t BytePos{ 0 };

		// Number of UTF-8 characters from start of string.
		// NOTE: This will always be equal to or greater than [eon::Utf8CharRef::BytePos]!
		index_t CharPos{ 0 };
	};

	// For internal use.
	struct _TransformData;




	///////////////////////////////////////////////////////////////////////////
	//
	// Eon UTF-8 String Class - eon::string
	//
	// A fully implemented string class with a variety of useful methods for
	// string manipulation.
	//
	// NOTE: Unlike [std::string], eon::string will not let you modify characters in-place.
	// NOTE: Direct access to characters is costly, prefer using iterators!
	// WARNING: Is not thread-safe!
	//
	// Makes heavy use of [eon::string_iterator] and [eon::substring] classes.
	// The use of the latter sets eon::string apart from [std::string] in that
	// [eon::substrings] are not "real" strings but a pair of [eon::iterator]s
	// that marks the substring in the original source string.
	// While [eon::string] has many useful methods, these typically delegates
	// to [eon::substring] for the actual work.
	//
	// NOTE: [eon::substring] will often have more detailed/specialized methods \
	//       that what's available from [eon::string]!
	// NOTE: Use [eon::string::substr()] to get the entire string as a [eon::substring]!
	//
	// We can also get the entire string as a [eon::substring] by calling
	// substring's constructor with calls to [eon::string::begin] and
	// [eon::string::end] as arguments.
	//
	class string
	{
		///////////////////////////////////////////////////////////////////////
		//
		// String Definitions
		//
	public:

		// For consistency with [std::string] and std containers, we define an
		// iterator class inside the string class.
		// NOTE: eon::string::iterator is identical to [eon::string_iterator]!
		using iterator = string_iterator;




		///////////////////////////////////////////////////////////////////////
		//
		// String Construction
		//
	public:

		// Construct an empty string.
		string() = default;

		// Construct by taking ownership of the details of another string
		inline string( string&& other ) noexcept { *this = std::move( other ); }

		// Construct as a copy of another string
		inline string( const string& other ) { *this = other; }


		// Construct as a copy of a substring marked by the specified iterator pair.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline string( const iterator& input_beg, const iterator& input_end ) {
			*this = substring( input_beg, input_end ); }

		// Construct as a copy of a substring.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline string( const substring& input ) { *this = input; }


		// Construct as a copy of a [std::string].
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline string( const std::string& input ) { *this = input; }

		// Construct by taking ownership of a [std::string].
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline string( std::string&& input ) { *this = std::move( input ); }

		// Construct as a copy of a C string.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline string( const char* input ) { *this = input; }

		// Construct as a copy of a C string of specified length.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline string( const char* input, index_t input_length ) { *this = std::string( input, input_length ); }

		// Construct as a copy of a C string of specified length.
		// Will not throw exception on invalid UTF-8 parts but substitute those with the specified substitution string.
		string( const char* input, index_t input_size, string non_utf8_substitution ) noexcept;


		// Construct from a single Unicode codepoint.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline explicit string( char_t input ) { *this = input; }

		// Construct from a single signed character.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline explicit string( char input ) { *this = static_cast<char_t>( input ); }

		// Construct from a single unsigned character.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline explicit string( unsigned char input ) { *this = static_cast<char_t>( input ); }


		// Construct from literal list of Unicode codepoints.
		// WARNING: Throws eon::[eon::InvalidUTF8] if input is not valid UTF-8!
		inline string( std::initializer_list<char_t> input ) { *this = input; }

		// Construct from vector of Unicode codepoints.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline string( const std::vector<char_t>& input ) { *this = input; }

		// Construct from literal list of signed characters.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline string( std::initializer_list<char> input ) { *this = input; }

		// Construct from literal list of unsigned characters.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline string( std::initializer_list<unsigned char> input ) { *this = input; }


		// Construct as a number of copies of the specified Unicode codepoint.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline string( index_t copies, char_t input ) { assign( copies, input ); }

		// Construct as a number of copies of another string.
		inline string( index_t copies, const string& other ) { assign( copies, other ); }

		// Construct as a number of copies of the specified [std::string].
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline string( index_t copies, const std::string& input ) { assign( copies, input ); }

		// Construct as a number of copies of the specified substring.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline string( index_t copies, const substring& input ) { assign( copies, input ); }


		// Construct as "true" or "false" based on boolean input value.
		inline explicit string( bool value ) { *this = value; }


		// Construct as string representation of the input [eon::int_t].
		inline explicit string( int_t input ) { *this = toString( input ); }

		// Construct as string representation of the input [eon::short_t].
		inline explicit string( short_t value ) { *this = toString( value ); }

		// Construct as string representation of the input [eon::long_t].
		inline explicit string( long_t value ) { *this = toString( value ); }

		// Construct as string representation of the input [eon::index_t].
		inline explicit string( index_t value ) { *this = toString( value ); }

		// Construct as string representation of the input [eon::flt_t].
		inline explicit string( flt_t value ) { *this = toString( value ).trimFloat(); }

		// Construct as string representation of the input [eon::low_t].
		inline explicit string( low_t value ) { *this = toString( value ).trimFloat(); }

		// Construct as string representation of the input [eon::high_t].
		inline explicit string( high_t value ) { *this = toString( value ).trimFloat(); }

#ifdef EON_WINDOWS

		// Construct as string representation of the input [std::uint16_t].
		inline explicit string( uint16_t value ) { *this = toString( value ); }

#if INTPTR_MAX == INT64_MAX
		// Construct as string representation of the input [std::uint32_t].
		inline explicit string( uint32_t value ) { *this = toString( value ); }
#else
		inline explicit string( uint64_t value ) { *this = toString( value ); }
#endif

		// Construct as string representation of the input [long].
		inline explicit string( long value ) { *this = toString( value ); }

		// Construct as string representation of the input [unsigned long].
		inline explicit string( unsigned long value ) { *this = toString( value ); }

#else
		// Construct as string representation of the input [unsigned short].
		inline explicit string( unsigned short value ) { *this = toString( value ); }

		// Construct as string representation of the input [unsigned int].
		inline explicit string( unsigned int value ) { *this = toString( value ); }

#	ifdef EON_SUN
		// Construct as string representation of the input [long].
		inline explicit string( long value ) { *this = toString( value ); }

		// Construct as string representation of the input [unsigned long].
		inline explicit string( unsigned long value ) { *this = toString( value ); }

#	else
		// Construct as string representation of the input [std::int8_t].
		inline explicit string( int8_t value ) { *this = toString( value ); }

		// Construct as string representation of the input [long long unsigned int].
		inline explicit string( long long unsigned int value ) { *this = toString( value ); }

#		ifdef EON_APPLE
		// Construct as string representation of the input [long].
		inline explicit string( long value ) { *this = toString( value ); }

#		endif
#	endif
#endif


		// Default destructor.
		virtual ~string() = default;


	PRIVATE:

		// Constructor for internal use, where we know the input string is ASCII only.
		// (Faster than normal construction!)
		inline string( std::string&& value, bool identifier ) noexcept {
			Bytes = std::move( value ); NumChars = Bytes.size(); }




		///////////////////////////////////////////////////////////////////////
		//
		// String Assignment Metods
		//
	public:

		// Discard current details and copy the input codepoints as UTF-8.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid Unicoded codepoints!
		string& assign( const char_t* input, index_t input_length );

		// Discard current details and copy the input C string.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		string& assign( const char* input, index_t input_length );


		// Discard current details and assign the specified number of copies of the codepoint.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid Unicode codepoint!
		string& assign( index_t copies, char_t input );

		// Discard current details and assign the specified number of copies of the character.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid Unicode codepoint!
		inline string& assign( index_t copies, char input ) { return assign( copies, static_cast<char_t>( input ) ); }

		// Discard current details and assign the specified number of copies of the other string.
		string& assign( index_t copies, const string& other );

		// Discard current details and assign the specified number of copies of the [std::string].
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		string& assign( index_t copies, const std::string& input );

		// Discard current details and assign the specified number of copies of the substring.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8
		string& assign( index_t copies, const substring& input );


		// Discard current details and copy those of another string.
		inline string& operator=( const string& other ) { Bytes = other.Bytes; NumChars = other.NumChars; return *this; }

		// Discard current details and take over ownership of those of another string.
		inline string& operator=( string&& other ) noexcept {
			Bytes = std::move( other.Bytes ); NumChars = other.NumChars; other.NumChars = 0; return *this; }


		// Discard current details and copy new from a substring.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		string& operator=( const substring& input );


		// Discard current details and copy new from a [std::string].
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline string& operator=( const std::string& input ) { return assign( input.c_str(), input.size() ); }

		// Discard current details and take over ownership of a [std::string].
		// NOTE: This is possible because [std::string] is an underlying structure of eon::string.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		string& operator=( std::string&& input );

		// Discard current details and copy new from a C string.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline string& operator=( const char* input ) { return assign( input, strlen( input ) ); }

		// Descard current details and use a single codepoint as new value.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid Unicode codepoint!
		inline string& operator=( char_t input ) {
			uint32_t bytes; auto num = iterator::unicodeToBytes( input, bytes ); return assign( (const char*)&bytes, num ); }

		// Discard current details and use a single signed character as new value.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid Unicode codepoint!
		inline string& operator=( char input ) { return *this = static_cast<char_t>( input ); }

		// Discard current details and use a single unsigned character as new value.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid Unicode codepoint!
		inline string& operator=( unsigned char input ) { return *this = static_cast<char_t>( input ); }


		// Discard current details and use a literal list of codepoints as new value.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid Unicode codepoints!
		string& operator=( const std::initializer_list<char_t>& input );

		// Discard current details and use a vector of codepoints as new value.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid Unicode codepoints!
		string& operator=( const std::vector<char_t>& input );

		// Discard current details and use a literal list of signed characters as new value.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid Unicode codepoints!
		string& operator=( const std::initializer_list<char>& input );

		// Discard current details and use a literal list of unsigned characters as new value.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid Unicode codepoints!
		string& operator=( const std::initializer_list<unsigned char>& input );


		// Assign as "true" or "false" based on the boolean value.
		inline string& operator=( bool input ) { return *this = input ? "true" : "false"; }




		///////////////////////////////////////////////////////////////////////
		//
		// Basic Read-only Methods
		//
	public:

		// Get number of Unicode codepoints in string.
		inline index_t numChars() const noexcept { return NumChars; }

		// Get number of Unicode codepoints in string.
		inline index_t length() const noexcept { return NumChars; }


		// Get number of bytes in string.
		inline index_t numBytes() const noexcept { return Bytes.size(); }

		// Get buffer size (capacity) in number of bytes.
		inline index_t bufferSize() const noexcept { return Bytes.capacity(); }

		// Check if the string is empty.
		inline bool empty() const noexcept { return NumChars == 0; }

		// Access the underlying bytes as a [std::string].
		// NOTE: This is intant since eon::string uses std::string as underlying structure!
		inline const std::string& stdstr() const noexcept { return Bytes; }

		// Access the underlying bytes as a C-string.
		// NOTE: This is the same as calling [stdstr] and then [std::string::c_str] on the returned value!
		inline const char* c_str() const noexcept { return Bytes.c_str(); }

		// Access an individual immutable byte by index.
		// NOTE: This is instant, but the byte may be part of a larger Unicode character!
		inline char byte( index_t pos ) const { return Bytes[ pos ]; }


		// Check if the string is enclosed by double quotation marks.
		inline bool isDoubleQuoted() const noexcept { return substr().enclosed( DblQuoteChr ); }

		// Check if a part of the string is enclosed by double quotation marks.
		inline bool isDoubleQuoted( const substring& sub ) const noexcept { return sub.enclosed( DblQuoteChr ); }

		// Check if the string is enclosed by single quotation marks.
		inline bool isSingleQuoted() const noexcept { return substr().enclosed( SglQuoteChr ); }

		// Check if a part of the string is enclosed by single quotation marks.
		inline bool isSingleQuoted( const substring& sub ) const noexcept { return sub.enclosed( SglQuoteChr ); }


		// Check if the string is 'blank' (contains only whitespaces).
		inline bool blank() const noexcept { return substr().blank(); }

		// Get number of indentations for the first (and possibly only) line of text in the string.
		// NOTE: This is the number of times the indentation character occurs at the start of the string!
		inline index_t indentationLevel( char_t indentation_char = TabChr ) const noexcept {
			return substr().indentationLevel(); }


		// Get characters as a sequential container of codepoint elements.
		// NOTE: Container must support 'push_back( char_t )'!
		template<typename container>
		inline container characters() const { return substr().characters<container>(); }

		// Get part of string as a sequential container of codepoint elements.
		// High-to-low range will give characters in reverse!
		// NOTE: Container must support 'push_back( char_t )'!
		template<typename container>
		inline container characters( const substring& sub ) const { return sub.characters<container>(); }




		///////////////////////////////////////////////////////////////////////
		//
		// Iterator and Iterator Access
		//
	public:

		// Get iterator for first character in the string.
		// Returns [end] if string is empty.
		inline iterator begin() const noexcept { return iterator( Bytes.c_str(), Bytes.size(), NumChars, Bytes.c_str() ); }

		// Get iterator for end of string (one past the last character).
		inline iterator end() const noexcept {
			return iterator( Bytes.c_str(), Bytes.size(), NumChars, Bytes.c_str() + Bytes.size(), NumChars ); }

		// Get iterator for the last character in the string.
		// Returns [end] if string is empty.
		inline iterator last() const noexcept { return NumChars > 0 ? end() - 1 : end(); }


		// Given a byte position within the string, get the (closest next)
		// character position in the form of an iterator.
		// The counting will start from the beginning of the string unless an
		// already known position is specified as 'start', in which case the
		// byte position of that position will be subtracted.
		// Returns [end] if either 'pos' is beyond the end of the
		// string, or the byte position of 'start' is beyond 'pos'.
		iterator bytePos( index_t pos, iterator start = iterator() ) const;


		// Given an iterator for another string, get a new iterator for the same position in 'this'.
		// Returns [end] if the iterator from the other string is beyond what 'this' string contains.
		// This method is safe but involves linear counting!
		iterator rebase( const iterator& pos_from_other_string ) const noexcept;

		// Given a substring of another string, get a new substring for the same area within 'this'.
		// This method is safe but involves linear counting!
		inline substring rebase( const substring& other ) const noexcept {
			return substring( rebase( other.begin() ), rebase( other.end() ) ); }

		// Given an iterator for another string, get a new iterator for the same postion in 'this'.
		// Returns [end] if the iterator from the other string is beyond what 'this' string contains.
		// WARNING: While very fast (no counting), used unwisely it can result in undefined behavior!
		// You have to know for sure that both strings are identical (at least up to and including the
		// Unicode codepoint referenced by the iterator)!
		iterator rebaseMoved( const iterator& other ) const noexcept;

		// Given a substring of another string, get a new substring for the same area within 'this'.
		// WARNING: While very fast (no counting), used unwisely it can result in undefined behavior!
		// You have to know for sure that both strings are identical (at least up to and including
		// the area of the substring).
		inline substring rebaseMoved( const substring& other ) const noexcept {
			return substring( rebaseMoved( other.begin() ), rebaseMoved( other.end() ) ); }


		// Assuming the 'encoded_iterator' string is formatted as {"<byte pos>:<char pos>"},
		// convert it into an actual iterator for 'this' string.
		// If the encoded iterator does not refer to a valid position in the string,
		// the returned value will be equal to that returned by [end)]!
		// (See matching [eon::string::encode] method.)
		iterator decodeIterator( const string& encoded_iterator );

		// Assuming the 'encoded_substring' string is formatted as {"<byte pos>:<char pos>-<byte pos>:<char pos>"},
		// convert it into a substring for 'this' string.
		// If the encoded substring does not refer to a valid portion of 'this',
		// the returned value will be an 'empty' substring!
		// (See matching [eon::string::encode] method.)
		substring decodeSubstring( const string& encoded_substring );

		// Convert a string_iterator into a string formatted as {"<byte pos>:<char pos>"}.
		// NOTE: This is useful for streaming, serializing, and storing.
		// (See matching [eon::string::decodeIterator] method.)
		static inline string encode( const string_iterator& itr ) {
			return toString( itr.numByte() ) + ":" + toString( itr.numChar() ); }

		// Convert a substring into a string formatted as {"<byte pos>:<char pos>-<byte pos>:<char pos>"}.
		// NOTE: This is useful for streaming, serializing, and storing.
		// (See matching [eon::string::decodeIterator] method.)
		static inline string encode( const substring& sub ) { return encode( sub.begin() ) + "-" + encode( sub.end() ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Modifier Methods
		//
		// These will modify the string object!
		//
	public:

		// Clear the string contents
		inline void clear() noexcept { Bytes.clear(); NumChars = 0; }

		// Reserve memory in order to reduce the number of times the
		// underlying string buffer may have to grow.
		// Does nothing if the buffer size is already >= 'byte_size'.
		inline void reserve( index_t byte_size ) { Bytes.reserve( byte_size ); }


		// Concatenate another string to 'this' by serialization.
		// NOTE: {str << a << b << c ...} is more efficient than {str += a + b + c + ...}.
		inline string& operator<<( const string& other ) { return *this += other; }

		// Concatenate a substring to 'this' by serialization.
		// NOTE: {str << a << b << c ...} is more efficient than {str += a + b + c + ...}.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline string& operator<<( const substring& input ) { return *this += input; }

		// Concatenate a std::string to 'this' by serialization.
		// NOTE: {str << a << b << c ...} is more efficient than {str += a + b + c + ...}.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline string& operator<<( const std::string& input ) { return *this += input; }

		// Concatenate C string to 'this' by serialization.
		// NOTE: {str << a << b << c ...} is more efficient than {str += a + b + c + ...}.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		inline string& operator<<( const char* input ) { return *this += input; }

		// Concatenate any object type that can be given as argument to a string constructor to 'this' by serialization.
		// NOTE: {str << a << b << c ...} is more efficient than {str += a + b + c + ...}.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		template<typename T>
		inline string& operator<<( const T& input ) { return *this += string( input ); }


		// Concatenate another string to 'this'.
		inline string& operator+=( const string& other ) { Bytes += other.Bytes; NumChars += other.NumChars; return *this; }

		// Concatenate an substring to 'this'.
		// WARNING: Throws [eon::InvalidUTF8] if input is not valid UTF-8!
		string& operator+=( const substring& input );

		// Concatenate a std::string to 'this'.
		// WARNING: Throws [eon::InvalidUTF8] if not a valid UTF-8!
		inline string& operator+=( const std::string& input ) { return *this += substring( input ); }

		// Concatenate a C-string to 'this'.
		// WARNING: Throws [eon::InvalidUTF8] if not a valid UTF-8!
		inline string& operator+=( const char* input ) { return *this += substring( input ); }

		// Concatenate an Unicode codepoint to 'this'.
		// WARNING: Throws [eon::InvalidUTF8] if not a valid UTF-8!
		inline string& operator+=( char_t input ) { return *this += string( input ); }

		// Concatenate a signed char to 'this'.
		// WARNING: Throws [eon::InvalidUTF8] if not a valid UTF-8!
		inline string& operator+=( char input ) { return *this += string( input ); }

		// Concatenate an unsigned char to 'this'.
		// WARNING: Throws [eon::InvalidUTF8] if not a valid UTF-8!
		inline string& operator+=( unsigned char input ) { return *this += string( input ); }


		// Insert a substring into 'this' at the specified 'pos' (position),
		// moving existing data from (and including) the iterator position outwards.
		// Returns a new iterator for the insert position (as the old will be invalid after insert).
		iterator insert( const iterator& pos, const string& sub );

		// Insert a Unicode codepoint into 'this' at the specified 'pos' (position),
		// moving existing data from (and including) the iterator position outwards.
		// Returns a new iterator for the insert position (as the old will be invalid after insert).
		inline iterator insert( const iterator& pos, char_t codepoint ) { return insert( pos, string( codepoint ) ); }

		// Erase a substring section from the string.
		string& erase( const substring& sub ) noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Copy and Transform
		//
		// These methods will not modify the string object, but produce a new
		// one - or an [eon::substring]!
		//
		// For methods returning [eon::substring], you can easily convert the
		// substring into an independent string by using the substring as
		// argument to string constructor or assignment method/operator.
		//
	public:

		// Generic transformation.
		// Get a copy of the string with all letters in the specified range
		// transformed by the transformation function 'op'.
		template<typename func>
		string transform( const substring& area, func op, const eon::locale* custom_locale = nullptr ) const
		{
			if( area.empty() )
				return *this;
			auto real_area = area.lowToHigh();
			auto output = _prepOutput( real_area );
			const eon::locale& loc = custom_locale != nullptr ? *custom_locale : eon::locale::get();
			for( auto i = real_area.begin(); i != real_area.end(); ++i )
			{
				if( *i <= 0xFFFF )
					output += static_cast<char_t>( op( *i, loc ) );
				else
					output += *i;
			}
			if( real_area.end() != end() )
				output += substr( real_area.end(), end() );
			return output;
		}

		// Generic transformation.
		// Get a copy of the string with letters in the specified range that
		// are identified by the 'find' callable and then transformed by the
		// transformation function 'op'.
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		template<typename finder, typename func>
		string transform( const substring& area, finder find, func op, const eon::locale* custom_locale = nullptr ) const
		{
			if( area.empty() )
				return *this;
			auto real_area = area.lowToHigh();
			auto output = _prepOutput( real_area );

			const eon::locale& loc = custom_locale != nullptr ? *custom_locale : eon::locale::get();
			auto start = real_area.begin();
			for( auto next = find( start, real_area.end() ); next != end(); next = find( start, real_area.end() ) )
			{
				output += substr( start, next );
				output += static_cast<char_t>( op( *next, loc ) );
				start = next + 1;
			}

			if( start != real_area.end() )
				output += substr( start, end() );
			else if( real_area.end() != end() )
				output += substr( real_area.end(), end() );
			return output;
		}

		// Get a copy of the string with all letters transformed to upper-case.
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		inline string upper( const locale* custom_locale = nullptr ) const { return upper( substr(), custom_locale ); }

		// Get a copy of the string with only letters in the 'sub' substring of 'this' transformed to upper-case.
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		inline string upper( const substring& sub, const locale* custom_locale = nullptr ) const {
			return transform( sub, []( char_t c, const eon::locale& loc ) {
				return loc.toUpper( static_cast<wchar_t>( c ) ); }, custom_locale ); }


		// Get a copy of the string with all letters transformed to lower case.
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		inline string lower( const locale* custom_locale = nullptr ) const { return lower( substr(), custom_locale ); }

		// Get a copy of the string with only letters in the 'sub' substring of 'this' transformed to lower-case.
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		inline string lower( const substring& sub, const locale* custom_locale = nullptr ) const {
			return transform( sub, []( char_t c, const eon::locale& loc ) {
				return loc.toLower( static_cast<wchar_t>( c ) ); }, custom_locale ); }


		// Get a copy of the string with first letter transformed to upper-case.
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		inline string ucFirst( const locale* custom_locale = nullptr ) const { return ucFirst( substr(), custom_locale ); }

		// Get a copy of the string with first letter of the 'sub' substring of 'this' transformed to upper-case.
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		string ucFirst( const substring& sub, const locale* custom_locale = nullptr ) const;


		// Get a copy of the string with first letter of every word transformed
		// to upper-case. Other letters will not be modified!
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		inline string ucWords( const locale* custom_locale = nullptr ) const { return ucWords( substr(), custom_locale ); }

		// Get a copy of the string with first letter of every word in 'sub' substring
		// of 'this' transformed to upper-case. Other letters will not be modified!
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		inline string ucWords( const substring& sub, const locale* custom_locale = nullptr ) const {
			return   transform( sub, findWordStart, []( char_t c, const eon::locale& loc ) {
				return loc.toUpper( static_cast<wchar_t>( c ) ); }, custom_locale ); }


		// Get a copy of the string with first letter of every sentence transformed
		// to upper-case. Other letters will not be modified!
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		// NOTE: Only '.', ':', '!' and '?' are considered sentence dividers!
		inline string ucSentences( const locale* custom_locale = nullptr ) const {
			return ucSentences( substr(), custom_locale ); }

		// Get a copy of the string with first letter of every sentence in 'sub' substring
		// of 'this' transformed to upper-case. Other letters will not be modified!
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		// NOTE: Only '.', ':', '!' and '?' are considered sentence dividers!
		inline string ucSentences( const substring& sub, const locale* custom_locale = nullptr ) const {
			return transform( sub, findSentenceStart, []( char_t c, const eon::locale& loc ) {
				return loc.toUpper( static_cast<wchar_t>( c ) ); }, custom_locale ); }


		// Get entire string as a substring.
		inline substring substr() const { return substring( begin(), end() ); }

		// Get substring from specified 'start' position and running to the end of the string.
		// Throws [eon::WrongSource] if 'start' is not an iterator of 'this'!
		inline substring substr( const iterator& start ) const {
			start.assertSameSource( Bytes.c_str() ); return substring( start, end() ); }

		// Get substring from specified 'start' position and running to the 'end' position.
		// NOTE: If 'start' is after 'end', a 'high-to-low' ordered substring is returned!
		// Throws [eon::WrongSource] if 'start' or 'end' are not iterators of 'this'!
		inline substring substr( const iterator& start, const iterator& end ) const {
			start.assertSameSource( Bytes.c_str() ); return substring( start, end ); }


		// Get substring based on character position and count.
		// NOTE: This involves counting characters, but will count from the end if closer than start!
		inline substring substr( index_t start, index_t size ) const {
			auto sub = substr(); return substring( sub.iterator( start ), sub.iterator( start + size ) ); }


		// Get a substring (slice) starting at 'start' position and ending at 'end'. If either value is
		// negative, the actual position is counted from the end of the string (-1 being the last character).
		// NOTE: Both 'start' and 'end' are inclusive!
		// A slice {0, -1} will include the entire string, while {2, 2} will
		// return a substring containing a single character.
		// NOTE: If 'start' is less than 'end', a 'high-to-low' ordered substring is returned!
		substring slice( int64_t start, int64_t end ) const;


		// Get as substring everything inside 'this' string that appears before
		// the first occurrence of 'delimiter' string.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring beforeFirst(
			const string& delimiter, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().beforeFirst( delimiter.substr(), cmp ); }

		// Get as substring everything inside 'this' string that appears before
		// the first occurrence of 'delimiter' character.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring beforeFirst( char_t delimiter, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().beforeFirst( delimiter, cmp ); }

		// Get as substring everything inside 'this' string appearing before
		// the last occurrence of 'delimiter' string.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring beforeLast(
			const string& delimiter, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().beforeLast( delimiter.substr(), cmp ); }

		// Get as substring everything inside 'this' string appearing before
		// the last occurrence of 'delimiter' character.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring beforeLast( char_t delimiter, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().beforeLast( delimiter, cmp ); }

		// Get as substring everything inside 'this' string appearing after the
		// first occurrence of 'delimiter' substring.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring afterFirst(
			const string& delimiter, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().afterFirst( delimiter.substr(), cmp ); }

		// Get as substring everything inside 'this' string appearing after the
		// first occurrence of 'delimiter' character.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring afterFirst( char_t delimiter, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().afterFirst( delimiter, cmp ); }

		// Get as substring everything inside 'this' string appearingafter the
		// last occurrence of 'delimiter' substring.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring afterLast(
			const string& delimiter, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().afterLast( delimiter.substr(), cmp ); }

		// Get as substring everything inside 'this' string appearing after the
		// last occurrence of 'delimiter' character.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring afterLast( char_t delimiter, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().afterLast( delimiter, cmp ); }


		// Get a copy of the entire string with all occurrences of 'find' replaced with 'replacement'.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline string replace(
			const string& find, const string& replacement, const compare_T& cmp = substring::FastChrCompare ) const {
			return replace( find, replacement, substr(), cmp ); }

		// Get a copy of the entire string with all occurrences of 'find'
		// within 'sub' substring of 'this' replaced with 'replacement'.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		string replace(
			const string& find,
			const string& replacement,
			const substring& sub,
			const compare_T& cmp = substring::FastChrCompare ) const
		{
			if( sub.empty() )
				return *this;
			auto found = this->findFirst( find, sub );
			if( !found )
				return *this;

			string output = _prepOutput( found );
			auto pos = end();
			while( found )
			{
				output += replacement;
				pos = found.end();
				found = this->findFirst( find, substring( pos, sub.end() ), cmp );
				if( found && found.begin() > pos )
					output += substr( pos, found.begin() );
			}
			if( pos != end() )
				output += substr( pos );
			return output;
		}

		// Get a copy of the entire string with all occurrences of 'find' replaced with 'replacement'.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline string replace( char_t find, char_t replacement, const compare_T& cmp = substring::FastChrCompare ) const {
			return replace( find, replacement, substr(), cmp ); }

		// Get a copy of the entire string with all occurrences of 'find'
		// within 'sub' substring of 'this' replaced with 'replacement'.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		string replace(
			char_t find, char_t replacement, const substring& sub, const compare_T& cmp = substring::FastChrCompare ) const
		{
			if( sub.empty() )
				return *this;
			auto area = sub.lowToHigh();
			string output = _prepOutput( area );
			for( string::iterator i = area.begin(); i != area.end(); ++i )
			{
				if( cmp( *i, find ) == 0 )
					output += replacement;
				else
					output += *i;
			}
			if( area.end() != end() )
				output += substr( area.end() );
			return output;
		}

		// Get a copy of the entire string with all occurrences of 'find' replaced with 'replacement'.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline string replace( char find, char replacement, const compare_T& cmp = substring::FastChrCompare ) const {
			return replace( static_cast<char_t>( find ), static_cast<char_t>( replacement ), cmp ); }

		// Get a copy of the entire string with all occurrences of 'find'
		// within 'sub' substring of 'this' replaced with 'replacement'.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline string replace(
			char find, char replacement, const substring& sub, const compare_T& cmp = substring::FastChrCompare ) const {
			return replace( static_cast<char_t>( find ), static_cast<char_t>( replacement ), sub, cmp ); }

		// Get a copy of the entire string with 'sub' substring replaced with 'replacement'.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline string replace(
			const substring& sub, const string& replacement, const compare_T& cmp = substring::FastChrCompare ) const {
			return string( substr( begin(), sub.begin() ) ) + replacement + string( substr( sub.end() ) ); }

		// Get a copy of the entire string with 'sub' substring replaced with 'replacement'.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline string replace(
			const substring& sub, char_t replacement, const compare_T& cmp = substring::FastChrCompare ) const {
			return string( substr( begin(), sub.begin() ) ) + replacement + string( substr( sub.end() ) ); }


		// Get a copy of the entire string with each occurrence of any map
		// 'key' replaced by the corresponding map 'value'.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline string replace(
			const std::map<string, string>& find_replace, const compare_T& cmp = substring::FastChrCompare ) const {
			return replace( find_replace, substr() ); }

		// Get a copy of the entire string with each occurrence of any map 'key' within
		// 'sub' substring of 'this' replaced by the corresponding map 'value'.
		string replace( const std::map<string, string>& find_replace, const substring& sub ) const;

		// Get a copy of the entire string with each occurrence of any map
		// 'key' replaced by the corresponding map 'value'.
		inline string replace( const std::map<char_t, char_t>& find_replace ) const {
			return replace( find_replace, substr() ); }

		// Get a copy of the entire string with each occurrence of any map 'key' within
		// 'sub' substring of 'this' replaced by the corresponding map 'value'.
		string replace( const std::map<char_t, char_t>& find_replace, const substring& sub ) const;


		// Get a copy of the entire string with all occurrences of 'to_remove' removed.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline string remove( const string& to_remove, const compare_T& cmp = substring::FastChrCompare ) const {
			return replace( to_remove, string::Empty, substr(), cmp ); }

		// Get a copy of the entire string with all occurrences of 'to_remove'
		// within 'sub' substring of 'this' removed.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline string remove(
			const string& to_remove, const substring& sub, const compare_T& cmp = substring::FastChrCompare ) const {
			return replace( to_remove, string::Empty, sub, cmp ); }

		// Get a copy of the entire string with all occurrences of 'to_remove' removed.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline string remove( char_t to_remove, const compare_T& cmp = substring::FastChrCompare ) const {
			return replace( string( to_remove ), string::Empty, substr(), cmp ); }

		// Get a copy of the entire string with all occurrences of 'to_remove'
		// within 'sub' substring of 'this' removed.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline string remove(
			char_t to_remove, const substring& sub, const compare_T& cmp = substring::FastChrCompare ) const {
			return replace( string( to_remove ), string::Empty, sub, cmp ); }

		// Get a copy of the entire string with all occurrences of 'to_remove' removed.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline string remove( char to_remove, const compare_T& cmp = substring::FastChrCompare ) const {
			return replace( string( to_remove ), string::Empty, substr(), cmp ); }

		// Get a copy of the entire string with all occurrences of 'to_remove'
		// within 'sub' substring of 'this' removed.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline string remove( char to_remove, const substring& sub, const compare_T& cmp = substring::FastChrCompare ) const {
			return replace( string( to_remove ), string::Empty, sub ); }


		// Get a copy of the string with standard C single character escapes processed.
		// NOTE: Single quotes, question marks, and tabs are not escaped!
		inline string escape() const { return escape( substr() ); }

		// Get a copy of the string with standard C single character escapes
		// processed only within 'sub' substring of 'this'.
		// NOTE: Single quotes, question marks, and tabs are not escaped!
		string escape( const substring& sub ) const;

		// Get a copy of the string with standard C single character escapes processed.
		// NOTE: Single quotes, question marks, and tabs are also escaped!
		inline string escapeAll() const { return escapeAll( substr() ); }

		// Get a copy of the string with standard C single character escapes
		// processed only within 'sub' substring of 'this'.
		// NOTE: Single quotes, question marks, and tabs are also escaped!
		string escapeAll( const substring& sub ) const;

		// Get a copy of the string with standard C single character escapes reverse processed.
		inline string unescape() const { return unescape( substr() ); }

		// Get a copy of the string with standard C single character escapes
		// reverse processed only within 'sub' substring of 'this'.
		string unescape( const substring& sub ) const;


		// Get a copy of the string with all non-printable ASCII characters and
		// all non-ASCII characters replaced with HTML formatting.
		// Example: "\t" -> "&#9;"
		inline string toHtml() const { return toHtml( substr() ); }

		// Get a copy of the string with all non-printable ASCII characaters and all non-ASCII
		// characters inside 'sub' substring of 'this' replaced with HTML formatting.
		// Example: "\t" -> "&#9;"
		string toHtml( const substring& sub ) const;

		// Get a copy of the string with all HTML formatted characters replaced
		// with the actual characaters they represent.
		// Example: "&#9;" -> "\t"
		inline string fromHtml() const { return fromHtml( substr() ); }

		// Get a copy of the string with all HTML formatted characters inside 'sub'
		// substring of 'this' replaced with the actual characaters they represent.
		// Example: "&#9;" -> "\t"
		string fromHtml( const substring& sub ) const;


		// Get a copy of the string enclosed in double quotes.
		// NOTE: Will not add quotation marks if already existing at both ends!
		inline string doubleQuote() const { return doubleQuote( substr() ); }

		// Get a copy of the string with the 'sub' substring of 'this' enclosed in double quotes.
		// NOTE: Will not add quotation marks if already existing at both ends!
		string doubleQuote( const substring& sub ) const;

		// Get a copy of the string enclosed in single quotes.
		// NOTE: Will not add quotation marks if already existing at both ends!
		inline string singleQuote() const { return singleQuote( substr() ); }

		// Get a copy of the string with the 'sub' substring of 'this' enclosed in single quotes
		// NOTE: Will not add quotation marks if already existing at both ends!
		string singleQuote( const substring& sub ) const;

		// Get a copy of the string with double or single quotations marks removed.
		// NOTE: Will only remove if the same quotation character is at both ends!
		inline string unQuote() const { return unQuote( substr() ); }

		// Get a copy of the string with double or single quotations marks on
		// the 'sub' substring of 'this' removed.
		// NOTE: Will only remove if the same quotation character is at both ends!
		string unQuote( const substring& sub ) const;


		// Get a copy of the string with all characters in reverse order.
		inline string reverse() const { return reverse( substr() ); }

		// Get a copy of the string with all characters in the 'sub' substring
		// of 'this' in reverse order. Other characters are not modified!
		string reverse( const substring& sub ) const;


		// Split string on every occurrence of 'delimiter' (delimiter string not included in result)
		// into a sequential container (such as std::list or std::vector). Uses the 'push_back' method!
		// If a 'max_elements' limit is reached, the last element will reference the unsplit remainder.
		// NOTE: All elements will be substring objects so specify the template type as such!
		// Example: {splitSequential<std::list<eon::substring>>( ...}
		template<typename container_t>
		container_t splitSequential( const string& delimiter, index_t max_elements = INDEX_MAX ) const {
			return substr().splitSequential<container_t>( delimiter.substr(), max_elements ); }

		// Split string on every occurrence of 'delimiter' (delimiter char not included in result)
		// into a sequential container (such as std::list or std::vector). Uses the 'push_back' method!
		// If a 'max_elements' limit is reached, the last element will reference the unsplit remainder.
		// NOTE: All elements will be substring objects so specify the template type as such!
		// Example: {splitSequential<std::list<eon::substring>>( ...}
		template<typename container_t>
		container_t splitSequential( char_t delimiter, index_t max_elements = INDEX_MAX ) const {
			return substr().splitSequential<container_t>( delimiter, max_elements ); }

		// Split string on every occurrence of 'delimiter' (delimiter string not included in result)
		// into a non-sequential container (such as std::set or std::multiset). Uses the 'insert' method!
		// NOTE: All elements will be substring objects so specify the template type as such!
		// Example: {splitNonSequential<std::set<eon::substring>>( ...}
		template<typename container_t>
		container_t splitNonSequential( const string& delimiter ) const {
			return substr().splitNonSequential<container_t>( delimiter.substr() ); }

		// Split string on every occurrence of 'delimiter' (delimiter char not included in result)
		// into a non-sequential container (such as std::set or std::multiset). Uses the 'insert' method!
		// NOTE: All elements will be substring objects so specify the template type as such!
		// Example: {splitNonSequential<std::set<eon::substring>>( ...}
		template<typename container_t>
		container_t splitNonSequential( char_t delimiter ) const {
			return substr().splitNonSequential<container_t>( delimiter ); }


		// Get a string that joins the elements from 'start' to 'end' using 'this' string as 'glue' in between.
		template<typename iterator_t>
		string join( iterator_t start, iterator_t end ) const
		{
			string str;
			for( auto i = start; i != end; ++i )
			{
				if( i != start )
					str += *this;
				str += *i;
			}
			return str;
		}

		// Get a string that joins the elements of the 'source' container using 'this' string as 'glue' in between.
		template<typename container_t>
		string join( const container_t& source ) const {
			return join<typename container_t::const_iterator>( source.begin(), source.end() ); }


		// Get a substring with leading and trailing spaces of 'this' excluded.
		inline substring trim() const { return substr().trim(); }

		// Get an substring with leading spaces of 'this' excluded.
		inline substring trimLeading() const { return substr().trimLeading(); }

		// Get an substring with trailing spaces of 'this' excluded.
		inline substring trimTrailing() const { return substr().trimTrailing(); }


		// Get a copy of the string, if it is shorter than 'target_size', add enough copies of
		// 'fill' on the left side to make it that long.
		inline string padLeft( index_t target_size, char_t fill = SpaceChr ) const {
			return NumChars < target_size ? string( target_size - NumChars, fill ) += *this : *this; }

		// Get a copy of the string, if it is shorter than 'target_size', add enough copies of
		// 'fill' on the right side to make it that long.
		inline string padRight( index_t target_size, char_t fill = SpaceChr ) const {
			return NumChars < target_size ? *this + string( target_size - NumChars, fill ) : *this; }

		// Get a copy of the string, if it is shorter than 'target_size', add enough copies of
		// 'fill' on the left and right side to make it that long.
		// NOTE: If an uneven number of fills are needed, the left end will be the shorter one!
		// (Centers the string)
		string padLeftAndRight( index_t target_size, char_t fill = SpaceChr ) const;


		// Get a copy of the string with all lines 'indented'.
		// NOTE: If the first line is already at the specified 'indentation_level', it will not be indented further!
		// NOTE: Subsequent lines will be indented fully regardless of existing indentation!
		// NOTE: Empty lines will not be indented!
		string indentLines( index_t indentation_level, char_t indentation_char = TabChr ) const;


		// Return a new string that is the result of 'a' concatenated with 'b'.
		friend inline string operator+( const string& a, const string& b ) { return string( a ) += b; }

		// Return a new string that is the result of 'a' concatenated with 'b'.
		friend inline string operator+( const string& a, const substring& b ) { return string( a ) += b; }

		// Return a new string that is the result of 'a' concatenated with 'b'.
		friend inline string operator+( const substring& a, const string& b ) { return string( a ) += b; }

		// Return a new string that is the result of 'a' concatenated with 'b'.
		friend inline string operator+( const string& a, const std::string& b ) { return string( a ) += string( b ); }

		// Return a new string that is the result of 'a' concatenated with 'b'.
		friend inline string operator+( const std::string& a, const string& b ) { return string( a ) += b; }

		// Return a new string that is the result of 'a' concatenated with 'b'.
		friend inline string operator+( const string& a, const char* b ) { return string( a ) += string( b ); }

		// Return a new string that is the result of 'a' concatenated with 'b'.
		friend inline string operator+( const char* a, const string& b ) { return string( a ) += b; }

		// Return a new string that is the result of 'a' concatenated with 'b'.
		friend inline string operator+( const string& a, char_t b ) { return string( a ) += b; }

		// Return a new string that is the result of 'a' concatenated with 'b'.
		friend inline string operator+( char_t a, const string& b ) { return string( a ) += b; }




		///////////////////////////////////////////////////////////////////////
		//
		// Strings as Numbers
		//
		// These methods provide various number operations to strings.
		//
	public:

		// Check if all characaters are in the 'Number, Decimal Digit' category.
		inline bool numeralsOnly() const noexcept { return substr().numeralsOnly(); }

		// Check if all characaters in 'sub' substring of 'this' are in the 'Number, Decimal Digit' category.
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		inline bool numeralsOnly( const substring& sub ) const {
			sub.assertSameSource( Bytes.c_str() ); return sub.numeralsOnly(); }


		// Check if all characters are ASCII digits (only!).
		inline bool isUInt() const noexcept { return substr().isUInt(); }

		// Check if all characters in 'sub' substring of 'this' are ASCII digits (only!).
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		inline bool isUInt( const substring& sub ) const { sub.assertSameSource( Bytes.c_str() ); return sub.isUInt(); }


		// Check if all characters are ASCII digits (only!), prefixed by a single '+' or '-'.
		inline bool isInt() const noexcept { return substr().isInt(); }

		// Check if all characters in 'sub' substring of 'this' are
		// ASCII digits (only!), prefixed by a single '+' or '-'.
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		inline bool isInt( const substring& sub ) const { sub.assertSameSource( Bytes.c_str() ); return sub.isInt(); }


		// Check if the string makes up an ASCII floating point number, optionally prefixed by a single '+' or '-'.
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		// NOTE: The decimal separator may occur as first or last character!
		inline bool isFloat( const locale* custom_locale = nullptr ) const noexcept {
			return substr().isFloat( custom_locale ); }

		// Check if 'sub' (a substring of 'this') makes up an ASCII floating point number,
		// optionally prefixed by a single '+' or '-'.
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		// NOTE: The decimal separator may occur as first or last character!
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		inline bool isFloat( const substring& sub, const locale* custom_locale = nullptr ) const {
			sub.assertSameSource( Bytes.c_str() ); return sub.isFloat( custom_locale ); }

		// Check if the string [isInt] or [isUInt] or [isFloat].
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		inline bool isNumerical( const locale* custom_locale = nullptr ) const noexcept {
			auto sub = substr(); return sub.isInt() || sub.isUInt() || sub.isFloat( custom_locale ); }

		// Check if 'sub' (substring of 'this') [isInt] or [isUInt] or [isFloat].
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		inline bool isNumerical( const substring& sub, const locale* custom_locale = nullptr ) const {
			sub.assertSameSource( Bytes.c_str() );
			return sub.isInt() || sub.isUInt() || sub.isFloat( custom_locale ); }


		// Convert String to int_t.
		inline int_t toIntT() const { return substr().toIntT(); }

		// Convert String to short_t.
		inline short_t toShortT() const { return substr().toShortT(); }

		// Convert String to long_t.
		inline long_t toLongT() const { return substr().toLongT(); }

		// Convert String to flt_t.
		inline flt_t toFltT() const { return substr().toFltT(); }

		// Convert String to low_t.
		inline low_t toLowT() const { return substr().toLowT(); }

		// Convert String to hight_t.
		inline high_t toHighT() const { return substr().toHighT(); }

		// Convert String to index_t.

		// Convert String to [eon::index_t].
		inline index_t toIndex() const { return substr().toIndex(); }


		// Convert String to number.
		inline int32_t toInt32() const { return substr().toInt32(); }

		// Convert String to number.
		// Throws [eon::WrongSource] if 'area' is not a substring of 'this'!
		inline int32_t toInt32( const substring& area ) const {
			area.assertSameSource( Bytes.c_str() ); return area.toInt32(); }

		// Convert String to number.

		// Convert String to number.
		inline int64_t toInt64() const { return substr().toInt64(); }

		// Convert String to number.
		// Throws [eon::WrongSource] if 'area' is not a substring of 'this'!
		inline int64_t toInt64( const substring& area ) const {
			area.assertSameSource( Bytes.c_str() ); return area.toInt64(); }

		// Convert String to number.

		// Convert String to number.
		inline uint32_t toUInt32() const { return substr().toUInt32(); }

		// Convert String to number.
		// Throws [eon::WrongSource] if 'area' is not a substring of 'this'!
		inline uint32_t toUInt32( const substring& area ) const {
			area.assertSameSource( Bytes.c_str() ); return area.toUInt32(); }

		// Convert String to number.

		// Convert String to number.
		inline uint64_t toUInt64() const { return substr().toUInt64(); }

		// Convert String to number.
		// Throws [eon::WrongSource] if 'area' is not a substring of 'this'!
		inline uint64_t toUInt64( const substring& area ) const {
			area.assertSameSource( Bytes.c_str() ); return area.toUInt64(); }

		// Convert String to number.

		// Convert String to number.
		inline size_t toSize() const { return substr().toSize(); }

		// Convert String to number.
		// Throws [eon::WrongSource] if 'area' is not a substring of 'this'!
		inline size_t toSize( const substring& area ) const {
			area.assertSameSource( Bytes.c_str() ); return area.toSize(); }

		// Convert String to number.

		// Convert String to number.
		inline double toDouble() const { return substr().toDouble(); }

		// Convert String to number.
		// Throws [eon::WrongSource] if 'area' is not a substring of 'this'!
		inline double toDouble( const substring& area ) const {
			area.assertSameSource( Bytes.c_str() ); return area.toDouble(); }

		// Convert String to number.

		// Convert String to number.
		inline long double toLongDouble() const { return substr().toLongDouble(); }

		// Convert String to number.
		// Throws [eon::WrongSource] if 'area' is not a substring of 'this'!
		inline long double toLongDouble( const substring& area ) const {
			area.assertSameSource( Bytes.c_str() ); return area.toLongDouble(); }


		// Assuming the entire substring is a number, get a reduced substring where leading
		// integer zeros and trailing fractional zeros are removed - including the
		// decimal separator if all fractional digits are zeros.
		// (If the number ends with the separator, it will be excluded.)
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		// NOTE: If the number is signed, leading integer zeros will be included to keep the sign!
		// Example: {0001.0000 -> 1}
		// Example: {+02.10 -> +02.1}
		inline substring trimNumber( const locale* custom_locale = nullptr ) const {
			return substr().trimNumber( custom_locale ); }

		// Assuming the entire substring is a number, get a reduced substring where leading
		// integer zeros and trailing fractional zeros are removed. If all fractional digits are
		// zeros, keep the decimal separator and one zero.
		// (If the number ends with the separator, it will be included.)
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		// NOTE: If the number is signed, leading integer zeros will be included to keep the sign!
		// Example: {0001.0000 -> 1.0}
		// Example: {+02.10 -> +02.1}
		inline substring trimFloat( const locale* custom_locale = nullptr ) const {
			return substr().trimFloat( custom_locale ); }


		// Get a copy of the (number) string with the integer part separated into thousands.
		// Example: 34525525.4621 -> 34,525,525.4621
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		// NOTE: Will locate first number in string and work from there!
		// NOTE: If the decimal separator appears before the first numeral, it will not be counted as part of the number!
		string separateThousands( char_t thousands_sep = CommaChr, const locale* custom_locale = nullptr ) const;

		// Get a copy of the number string with fractional part reduced to 'max_decimals',
		// rounded up or down to nearest value. If there is no fractional part or it is
		// already at the same or fewer decimals, an unaltered copy is returned.
		// Zero 'max_decimals' will result in removal of the fractional part and
		// possibly an increase of the integer part (when fractional part requires rounding up).
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		// NOTE: The entire string must be a floating point number representation!
		string roundNumber( index_t max_decimals = 0, const locale* custom_locale = nullptr ) const;




		///////////////////////////////////////////////////////////////////////
		//
		// String hashing
		//
		// Uses the FNV-1a hash algorithm
		//
	public:

		// Get a 32-bit hash value using FNV-1a hash algorithm .
		inline uint32_t hash32() const noexcept { return substring::hash32( Bytes.c_str(), Bytes.c_str() + Bytes.size() ); }

		// Get a 64-bit hash value using FNV-1a hash algorithm .
		inline uint64_t hash64() const noexcept { return substring::hash64( Bytes.c_str(), Bytes.c_str() + Bytes.size() ); }

#if defined(_WIN64) || defined(__x86_64__)
		// Get a 'size_t' size hash value using FNV-1a hash algorithm .
		inline size_t hash() const noexcept { return substring::hash64( Bytes.c_str(), Bytes.c_str() + Bytes.size() ); }
#else
		// Get a 'size_t' size hash value using FNV-1a hash algorithm .
		inline size_t hash() const noexcept { return substring::hash32( Bytes.c_str(), Bytes.c_str() + Bytes.size() ); }
#endif




		///////////////////////////////////////////////////////////////////////
		//
		// Comparing
		//
	public:

		// Check if string starts with a specific string 'value'.
		// Comparison is done using a binary substring predicate.
		template<typename compare_T = substring::fast_compare>
		inline bool startsWith( const string& value, const compare_T& cmp = substring::FastCompare ) const noexcept {
			return substr().startsWith( value.substr(), cmp ); }

		// Check if string starts with a specific C-string 'value'.
		// Comparison is done using a binary substring predicate.
		template<typename compare_T = substring::fast_compare>
		inline bool startsWith( const char* value, const compare_T& cmp = substring::FastCompare ) const noexcept {
			return substr().startsWith( substring( value ), cmp ); }

		// Check if string starts with a specific character.
		// Comparison is done using a binary char_t predicate.
		template<typename compare_T = substring::fast_chr_compare>
		inline bool startsWith( char_t value, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return cmp( *begin(), value ) == 0; }

		// Check if string ends with a specific string 'value'.
		// Comparison is done using a binary substring predicate.
		template<typename compare_T = substring::fast_compare>
		inline bool endsWith( const string& value, const compare_T& cmp = substring::FastCompare ) const noexcept {
			return substr().endsWith( value.substr(), cmp ); }

		// Check if string ends with a specific C-string 'value'.
		// Comparison is done using a binary substring predicate.
		template<typename compare_T = substring::fast_compare>
		inline bool endsWith( const char* value, const compare_T& cmp = substring::FastCompare ) const noexcept {
			return substr().endsWith( substring( value ), cmp ); }

		// Check if string ends with a specific character.
		// Comparison is done using a binary char_t predicate.
		template<typename compare_T = substring::fast_chr_compare>
		inline bool endsWith( char_t value, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return cmp( *last(), value ) == 0; }


		// Basic comparison.
		// Comparison is done using a binary substring predicate.
		// NOTE: You can also use [substring::compare] for comparing sections of strings!
		template<typename compare_T = substring::fast_compare>
		inline int compare( const string& other, const compare_T& cmp = substring::FastCompare ) const noexcept {
			return substr().compare( other.substr(), cmp ); }

		// Check if 'this' string sorts before 'other' using [eon::substring::FastCompare].
		inline bool operator<( const string& other ) const noexcept { return compare( other, substring::FastCompare ) < 0; }

		// Check if 'this' string sorts before or same as 'other' using [eon::substring::FastCompare].
		inline bool operator<=( const string& other ) const noexcept {
			return compare( other, substring::FastCompare ) <= 0; }

		// Check if 'this' string sorts after 'other' using [eon::substring::FastCompare].
		inline bool operator>( const string& other ) const noexcept { return compare( other, substring::FastCompare ) > 0; }

		// Check if 'this' string sorts after or same as 'other' using [eon::substring::FastCompare].
		inline bool operator>=( const string& other ) const noexcept {
			return compare( other, substring::FastCompare ) >= 0; }

		// Check if 'this' string sorts same as 'other' using [eon::substring::FastCompare].
		inline bool operator==( const string& other ) const noexcept {
			return compare( other, substring::FastCompare ) == 0; }

		// Check if 'this' string sorts before or after 'other' using [eon::substring::FastCompare].
		inline bool operator!=( const string& other ) const noexcept {
			return compare( other, substring::FastCompare ) != 0; }


		// Compare against [substring].
		// Comparison is done using a binary substring predicate.
		// NOTE: You can also use [substring::compare] for comparing sections of strings!
		template<typename compare_T = substring::fast_compare>
		inline int compare( const substring& sub, const compare_T& cmp = substring::FastCompare ) const noexcept {
			return substr().compare( sub, cmp ); }

		// Check if 'this' string sorts before 'sub' using [eon::substring::FastCompare].
		inline bool operator<( const substring& sub ) const noexcept { return compare( sub, substring::FastCompare ) < 0; }

		// Check if 'this' string sorts before or same as 'sub' using [eon::substring::FastCompare].
		inline bool operator<=( const substring& sub ) const noexcept { return compare( sub, substring::FastCompare ) <= 0; }

		// Check if 'this' string sorts after 'sub' using [eon::substring::FastCompare].
		inline bool operator>( const substring& sub ) const noexcept { return compare( sub, substring::FastCompare ) > 0; }

		// Check if 'this' string sorts after or same as 'sub' using [eon::substring::FastCompare].
		inline bool operator>=( const substring& sub ) const noexcept { return compare( sub, substring::FastCompare ) >= 0; }

		// Check if 'this' string sorts same as 'sub' using [eon::substring::FastCompare].
		inline bool operator==( const substring& sub ) const noexcept { return compare( sub, substring::FastCompare ) == 0; }

		// Check if 'this' string sorts before or after 'sub' using [eon::substring::FastCompare].
		inline bool operator!=( const substring& sub ) const noexcept { return compare( sub, substring::FastCompare ) != 0; }

		inline friend bool operator<( const substring& sub, const string& str ) noexcept { return str >= sub; }
		inline friend bool operator<=( const substring& sub, const string& str ) noexcept { return str > sub; }
		inline friend bool operator>( const substring& sub, const string& str ) noexcept { return str <= sub; }
		inline friend bool operator>=( const substring& sub, const string& str ) noexcept { return str < sub; }
		inline friend bool operator==( const substring& sub, const string& str ) noexcept { return str == sub; }
		inline friend bool operator!=( const substring& sub, const string& str ) noexcept { return str != sub; }


		// Compare against 'C-string'.
		// Comparison is done using a binary substring predicate.
		// NOTE: You can also use [substring::compare] for comparing sections of strings!
		template<typename compare_T = substring::fast_compare>
		inline int compare( const char* cstr, const compare_T& cmp = substring::FastCompare ) const noexcept {
			return substr().compare( substring( cstr ), cmp ); }

		// Check if 'this' string sorts before 'cstr' using [eon::substring::FastCompare].
		inline bool operator<( const char* cstr ) const noexcept { return compare( cstr, substring::FastCompare ) < 0; }

		// Check if 'this' string sorts before or same as 'cstr' using [eon::substring::FastCompare].
		inline bool operator<=( const char* cstr ) const noexcept { return compare( cstr, substring::FastCompare ) <= 0; }

		// Check if 'this' string sorts after 'cstr' using [eon::substring::FastCompare].
		inline bool operator>( const char* cstr ) const noexcept { return compare( cstr, substring::FastCompare ) > 0; }

		// Check if 'this' string sorts after or same as 'cstr' using [eon::substring::FastCompare].
		inline bool operator>=( const char* cstr ) const noexcept { return compare( cstr, substring::FastCompare ) >= 0; }

		// Check if 'this' string sorts same as 'cstr' using [eon::substring::FastCompare].
		inline bool operator==( const char* cstr ) const noexcept { return compare( cstr, substring::FastCompare ) == 0; }

		// Check if 'this' string sorts before or after 'cstr' using [eon::substring::FastCompare].
		inline bool operator!=( const char* cstr ) const noexcept { return compare( cstr, substring::FastCompare ) != 0; }

		inline friend bool operator<( const char* cstr, const string& str ) noexcept { return str >= cstr; }
		inline friend bool operator<=( const char* cstr, const string& str ) noexcept { return str > cstr; }
		inline friend bool operator>( const char* cstr, const string& str ) noexcept { return str <= cstr; }
		inline friend bool operator>=( const char* cstr, const string& str ) noexcept { return str < cstr; }
		inline friend bool operator==( const char* cstr, const string& str ) noexcept { return str == cstr; }
		inline friend bool operator!=( const char* cstr, const string& str ) noexcept { return str != cstr; }


		// Compare against 'std::string'.
		// Comparison is done using a binary substring predicate.
		// NOTE: You can also use [substring::compare] for comparing sections of strings!
		template<typename compare_T = substring::fast_compare>
		inline int compare( const std::string& stdstr, const compare_T& cmp = substring::FastCompare ) const noexcept {
			return substr().compare( substring( stdstr ), cmp ); }

		// Check if 'this' string sorts before 'stdstr' using [eon::substring::FastCompare].
		inline bool operator<( const std::string& stdstr ) const noexcept {
			return compare( stdstr, substring::FastCompare ) < 0; }

		// Check if 'this' string sorts before or same as 'stdstr' using [eon::substring::FastCompare].
		inline bool operator<=( const std::string& stdstr ) const noexcept {
			return compare( stdstr, substring::FastCompare ) <= 0; }

		// Check if 'this' string sorts after 'stdstr' using [eon::substring::FastCompare].
		inline bool operator>( const std::string& stdstr ) const noexcept {
			return compare( stdstr, substring::FastCompare ) > 0; }

		// Check if 'this' string sorts after or same as 'stdstr' using [eon::substring::FastCompare].
		inline bool operator>=( const std::string& stdstr ) const noexcept {
			return compare( stdstr, substring::FastCompare ) >= 0; }

		// Check if 'this' string sorts same as 'stdstr' using [eon::substring::FastCompare].
		inline bool operator==( const std::string& stdstr ) const noexcept {
			return compare( stdstr, substring::FastCompare ) == 0; }

		// Check if 'this' string sorts before or after 'stdstr' using [eon::substring::FastCompare].
		inline bool operator!=( const std::string& stdstr ) const noexcept {
			return compare( stdstr, substring::FastCompare ) != 0; }

		inline friend bool operator<( const std::string& stdstr, const string& str ) noexcept { return str >= stdstr; }
		inline friend bool operator<=( const std::string& stdstr, const string& str ) noexcept { return str > stdstr; }
		inline friend bool operator>( const std::string& stdstr, const string& str ) noexcept { return str <= stdstr; }
		inline friend bool operator>=( const std::string& stdstr, const string& str ) noexcept { return str < stdstr; }
		inline friend bool operator==( const std::string& stdstr, const string& str ) noexcept { return str == stdstr; }
		inline friend bool operator!=( const std::string& stdstr, const string& str ) noexcept { return str != stdstr; }


		// Comparing against a single Unicode codepoint.
		// Comparison is done using a binary char_t predicate.
		template<typename compare_T = substring::fast_chr_compare>
		inline int compare( char_t codepoint, const compare_T& cmp = substring::FastChrCompare ) const noexcept
		{
			if( NumChars > 0 ) {
				int cmp_val = cmp( *begin(), codepoint ); return NumChars > 1 && cmp_val == 0 ? 1 : cmp_val; }
			else return -1;
		}

		// Check if 'this' string sorts before 'codepoint' using [eon::substring::FastCmpCompare].
		inline bool operator<( char_t codepoint ) const noexcept {
			return compare( codepoint, substring::FastChrCompare ) < 0; }

		// Check if 'this' string sorts before or same as 'codepoint' using [eon::substring::FastCmpCompare].
		inline bool operator<=( char_t codepoint ) const noexcept {
			return compare( codepoint, substring::FastChrCompare ) <= 0; }

		// Check if 'this' string sorts after 'codepoint' using [eon::substring::FastCmpCompare].
		inline bool operator>( char_t codepoint ) const noexcept {
			return compare( codepoint, substring::FastChrCompare ) > 0; }

		// Check if 'this' string sorts after or same as 'codepoint' using [eon::substring::FastCmpCompare].
		inline bool operator>=( char_t codepoint ) const noexcept {
			return compare( codepoint, substring::FastChrCompare ) >= 0; }

		// Check if 'this' string sorts same as 'codepoint' using [eon::substring::FastCmpCompare].
		inline bool operator==( char_t codepoint ) const noexcept {
			return compare( codepoint, substring::FastChrCompare ) == 0; }

		// Check if 'this' string sorts before or after 'codepoint' using [eon::substring::FastCmpCompare].
		inline bool operator!=( char_t codepoint ) const noexcept {
			return compare( codepoint, substring::FastChrCompare ) != 0; }

		inline friend bool operator<( char_t codepoint, const string& str ) noexcept { return str >= codepoint; }
		inline friend bool operator<=( char_t codepoint, const string& str ) noexcept { return str > codepoint; }
		inline friend bool operator>( char_t codepoint, const string& str ) noexcept { return str <= codepoint; }
		inline friend bool operator>=( char_t codepoint, const string& str ) noexcept { return str < codepoint; }
		inline friend bool operator==( char_t codepoint, const string& str ) noexcept { return str == codepoint; }
		inline friend bool operator!=( char_t codepoint, const string& str ) noexcept { return str != codepoint; }


		// Comparing against a single ASCII character.
		// Comparison is done using a binary char_t predicate.
		template<typename compare_T = substring::fast_chr_compare>
		inline int compare( char c, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return compare( static_cast<char_t>( c ), cmp ); }

		// Check if 'this' string sorts before 'c' using [eon::substring::FastCmpCompare].
		inline bool operator<( char c ) const noexcept {
			return compare( static_cast<char_t>( c ), substring::FastChrCompare ) < 0; }

		// Check if 'this' string sorts before or same as 'c' using [eon::substring::FastCmpCompare].
		inline bool operator<=( char c ) const noexcept {
			return compare( static_cast<char_t>( c ), substring::FastChrCompare ) <= 0; }

		// Check if 'this' string sorts after 'c' using [eon::substring::FastCmpCompare].
		inline bool operator>( char c ) const noexcept {
			return compare( static_cast<char_t>( c ), substring::FastChrCompare ) > 0; }

		// Check if 'this' string sorts after or same as 'c' using [eon::substring::FastCmpCompare].
		inline bool operator>=( char c ) const noexcept {
			return compare( static_cast<char_t>( c ), substring::FastChrCompare ) >= 0; }

		// Check if 'this' string sorts same as 'c' using [eon::substring::FastCmpCompare].
		inline bool operator==( char c ) const noexcept {
			return compare( static_cast<char_t>( c ), substring::FastChrCompare ) != 0; }

		// Check if 'this' string sorts before or after 'c' using [eon::substring::FastCmpCompare].
		inline bool operator!=( char c ) const noexcept {
			return compare( static_cast<char_t>( c ), substring::FastChrCompare ) != 0; }

		inline friend bool operator<( char c, const string& str ) noexcept { return str >= c; }
		inline friend bool operator<=( char c, const string& str ) noexcept { return str > c; }
		inline friend bool operator>( char c, const string& str ) noexcept { return str <= c; }
		inline friend bool operator>=( char c, const string& str ) noexcept { return str < c; }
		inline friend bool operator==( char c, const string& str ) noexcept { return str == c; }
		inline friend bool operator!=( char c, const string& str ) noexcept { return str != c; }




		///////////////////////////////////////////////////////////////////////
		//
		// Searching
		//
		// Methods searching for substrings will return [eon::substring] objects
		// as result. These will be "false" for unsuccessful searches.
		//
	public:

		// Check if string contains the specified string.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline bool contains( const string& sub, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().contains( sub.substr(), cmp ); }

		// Check if substring contains the specified codepoint.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline bool contains( char_t codepoint, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().contains( codepoint, cmp ); }

		// Check if string contains any of the characters in the specified string.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline bool containsAnyOf(
			const string& characters, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().containsAnyOf( characters.substr(), cmp ); }

		// Check if string contains none of the characters in the specified string.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline bool containsNoneOf(
			const string& characters, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().containsNoneOf( characters.substr(), cmp ); }


		// Find first occurrence of 'to_find' string in 'this'.
		// Returns the found substring within 'this' - 'false' substring if not found.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring findFirst( const string& to_find, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().findFirst( to_find.substr(), cmp ); }

		// Find first occurrence of 'to_find' string in 'sub' substring of 'this'.
		// Returns the found substring within 'this' - 'false' substring if not found.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring findFirst(
			const string& to_find, const substring& sub, const compare_T& cmp = substring::FastChrCompare ) const {
			sub.assertSameSource( Bytes.c_str() ); return sub.lowToHigh().findFirst( to_find.substr(), cmp ); }

		// Find first occurrence of 'to_find' codepoint in 'this'.
		// Returns the found codepoint as a substring - 'false' substring if not found.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring findFirst( char_t to_find, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().findFirst( to_find, cmp ); }

		// Find first occurrence of 'to_find' codepoint in 'sub' substring of 'this'.
		// Returns the found substring within 'this' - 'false' substring if not found.
		// Comparison is done using a binary substring predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastCompare] is used!
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring findFirst(
			char_t to_find, const substring& sub, const compare_T& cmp = substring::FastChrCompare ) const {
			sub.assertSameSource( Bytes.c_str() ); return sub.lowToHigh().findFirst( to_find, cmp ); }

		// Find first occurrence of 'to_find' ASCII character in 'this'.
		// Returns the found codepoint as a substring - 'false' substring if not found.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring findFirst( char to_find, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().findFirst( static_cast<char_t>( to_find ), cmp ); }

		// Find first occurrence of 'to_find' ASCII character in 'sub' substring of 'this'.
		// Returns the found substring within 'this' - 'false' substring if not found.
		// Comparison is done using a binary substring predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastCompare] is used!
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring findFirst(
			char to_find, const substring& sub, const compare_T& cmp = substring::FastChrCompare ) const {
			sub.assertSameSource( Bytes.c_str() ); return sub.lowToHigh().findFirst(
				static_cast<char_t>( to_find ), cmp ); }

		// Find the first number within the string.
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		// Returns a substring wrapping only the found number, 'false' if not found.
		// If the 'separator' iterator argument is specified, it will contain
		// the position of the decimal separator - if it exists.
		// NOTE: If the decimal separator appears before the first numeral, it will not be counted as part of the number!
		inline substring findFirstNumber(
			iterator* separator = nullptr, const locale* custom_locale = nullptr ) const noexcept {
			return substr().findFirstNumber( separator, custom_locale ); }

		// Find the first number in 'sub' substring of 'this'.
		// Will use 'custom_locale' if specified and the default Eon locale if not!
		// Returns a substring wrapping only the found number, 'false' if not found.
		// If the 'separator' iterator argument is specified, it will contain
		// the position of the decimal separator - if it exists.
		// NOTE: If the decimal separator appears before the first numeral, it will not be counted as part of the number!
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		inline substring findFirstNumber(
			const substring& sub, iterator* separator = nullptr, const locale* custom_locale = nullptr ) {
			sub.assertSameSource( Bytes.c_str() ); return sub.lowToHigh().findFirstNumber( separator, custom_locale ); }


		// Find first occurrence of any of the specified 'characters'.
		// Returns iterator for the found character - 'false' iterator if not found.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline iterator findFirstOf(
			const string& characters, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().findFirstOf( characters.substr(), cmp ); }

		// Find first occurrence of any character in the specified character class(es).
		// NOTE: Multiple classes can be or'ed together!
		// Returns iterator for the found character - 'false' iterator if not found.
		inline iterator findFirstOf( charcat category ) const noexcept { return substr().findFirstOf( category ); }

		// Find first occurrence in 'sub' substring of 'this' of any of the specified 'characters'.
		// Returns iterator for the found character - 'false' iterator if not found.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		template<typename compare_T = substring::fast_chr_compare>
		inline iterator findFirstOf(
			const string& characters, substring sub, const compare_T& cmp = substring::FastChrCompare ) const {
			sub.assertSameSource( Bytes.c_str() ); return sub.findFirstOf( characters.substr(), cmp ); }

		// Find first occurrence of any character in the specified character class(es) in 'sub' substring of 'this'.
		// NOTE: Multiple classes can be or'ed together!
		// Returns iterator for the found character - 'false' iterator if not found.
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		inline iterator findFirstOf( charcat characters, substring sub ) const {
			sub.assertSameSource( Bytes.c_str() ); return sub.findFirstOf( characters ); }


		// Find first occurrence of any character not among the specified 'characters'.
		// Returns iterator for the found character - 'false' iterator if not found.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline iterator findFirstNotOf(
			const string& characters, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().findFirstNotOf( characters.substr(), cmp ); }

		// Find first occurrence of any character in the specified character class(es).
		// NOTE: Multiple classes can be or'ed together!
		// Returns iterator for the found character - 'false' iterator if not found.
		inline iterator findFirstNotOf( charcat characters ) const noexcept {
			return substr().findFirstNotOf( characters ); }

		// Find first occurrence in 'sub' substring of 'this' of any character not among the specified 'characters'.
		// Returns iterator for the found character - 'false' iterator if not found.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		template<typename compare_T = substring::fast_chr_compare>
		inline iterator findFirstNotOf(
			const string& characters, substring sub, const compare_T& cmp = substring::FastChrCompare ) const {
			sub.assertSameSource( Bytes.c_str() ); return sub.findFirstNotOf( characters.substr(), cmp ); }

		// Find first occurrence in 'sub' substring of 'this' of any character in the specified character class(es).
		// NOTE: Multiple classes can be or'ed together!
		// Returns iterator for the found character - 'false' iterator if not found.
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		inline iterator findFirstNotOf( charcat characters, substring sub ) const {
			sub.assertSameSource( Bytes.c_str() ); return sub.findFirstNotOf( characters ); }


		// Similar to [findFirst] for string argument but sections enclosed by double quotes will be skipped.
		// NOTE: Escaped double quote characters within or outside an enclosed section will not be interpreted
		//       as neither start nor end of section!
		// Comparison is done using a binary substring predicate.
		template<typename compare_T = substring::fast_chr_compare>
		inline substring findFirstNotDoubleQuoted(
			const string& other, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().findFirstIgnoreSections( other.substr(), DblQuoteChr, DblQuoteChr, cmp ); }

		// Similar to [findFirst] for char_t argument but sections enclosed by double quotes will be skipped.
		// NOTE: Escaped double quote characters within or outside an enclosed section will not be interpreted
		//       as neither start nor end of section!
		// Comparison is done using a binary char_t predicate.
		template<typename compare_T = substring::fast_chr_compare>
		inline substring findFirstNotDoubleQuoted(
			char_t codepoint, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().findFirstIgnoreSections( codepoint, DblQuoteChr, DblQuoteChr, cmp ); }

		// Similar to [findFirst] for string argument but sections enclosed by single quotes will be skipped.
		// NOTE: Escaped single quote characters within or outside an enclosed section will not be interpreted
		//       as neither start nor end of section!
		// Comparison is done using a binary char_t predicate.
		template<typename compare_T = substring::fast_chr_compare>
		inline substring findFirstNotSingleQuoted(
			const string& other, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().findFirstIgnoreSections( other.substr(), SglQuoteChr, SglQuoteChr, cmp ); }

		// Similar to [findFirst] for char_t argument but sections enclosed by single quotes will be skipped.
		// NOTE: Escaped single quote characters within or outside an enclosed section will not be interpreted
		//       as neither start nor end of section!
		// Comparison is done using a binary char_t predicate.
		template<typename compare_T = substring::fast_chr_compare>
		inline substring findFirstNotSingleQuoted(
			char_t codepoint, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().findFirstIgnoreSections( codepoint, SglQuoteChr, SglQuoteChr, cmp ); }

		// Similar to [findFirst] for string argument but sections enclosed by
		// 'double'(' and ')', or '[' and ']', or '{' and '}' will be skipped.
		// NOTE: Nested enclosures will be accounted for.
		// Comparison is done using a binary char_t predicate.
		template<typename compare_T = substring::fast_chr_compare>
		inline substring findFirstNotBraced(
			const string& other, char_t brace = '(', const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().findFirstIgnoreSections( other.substr(), brace,
				brace == '(' ? ')' : brace == '[' ? ']' : brace == '{' ? '}' : brace, cmp ); }

		// Similar to [findFirst] for char_t argument but sections enclosed by
		// 'double'(' and ')', or '[' and ']', or '{' and '}' will be skipped.
		// NOTE: Nested enclosures will be accounted for.
		// Comparison is done using a binary char_t predicate.
		template<typename compare_T = substring::fast_chr_compare>
		inline substring findFirstNotBraced(
			char_t codepoint, char_t brace = '(', const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().findFirstIgnoreSections(
				codepoint, brace, brace == '(' ? ')' : brace == '[' ? ']' : brace == '{' ? '}' : brace, cmp ); }


		// Find the first character in 'this' string that differs from 'other',
		// 'false' iterator if none (equal substrings).
		// Comparison is done using a binary char_t predicate.
		template<typename compare_T = substring::fast_chr_compare>
		inline iterator findFirstDiff(
			const string& other, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().findFirstDiff( other.substr(), cmp ); }

		// Find the first character in 'this' string that differs from 'sub' substrings,
		// 'false' iterator if none (equal substrings).
		// Comparison is done using a binary char_t predicate.
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		template<typename compare_T = substring::fast_chr_compare>
		inline iterator findFirstDiff( const substring& sub, const compare_T& cmp = substring::FastChrCompare ) const {
			sub.assertSameSource( Bytes.c_str() ); return substr().findFirstDiff( sub, cmp ); }



		// Find last occurrence of 'to_find' string in 'this'.
		// Returns the found substring within 'this' - 'false' substring if not found.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring findLast( const string& to_find, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().highToLow().findLast( to_find.substr(), cmp ); }

		// Find last occurrence of 'to_find' string in 'sub' substring of 'this'.
		// Returns the found substring within 'this' - 'false' substring if not found.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring findLast(
			const string& to_find, const substring& sub, const compare_T& cmp = substring::FastChrCompare ) const {
			sub.assertSameSource( Bytes.c_str() ); return sub.highToLow().findLast( to_find.substr().highToLow(), cmp ); }

		// Find last occurrence of 'to_find' codepoint in 'this'.
		// Returns the found codepoint as a substring - 'false' substring if not found.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring findLast( char_t to_find, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().highToLow().findLast( to_find, cmp ); }

		// Find last occurrence of 'to_find' codepoint in 'sub' substring of 'this'.
		// Returns the found substring within 'this' - 'false' substring if not found.
		// Comparison is done using a binary substring predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastCompare] is used!
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring findLast(
			char_t to_find, const substring& sub, const compare_T& cmp = substring::FastChrCompare ) const {
			sub.assertSameSource( Bytes.c_str() ); return sub.highToLow().findLast( to_find, cmp ); }

		// Find last occurrence of 'to_find' ASCII character in 'this'.
		// Returns the found codepoint as a substring - 'false' substring if not found.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring findLast( char to_find, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().highToLow().findLast( static_cast<char_t>( to_find ), cmp ); }

		// Find last occurrence of 'to_find' ASCII character in 'sub' substring of 'this'.
		// Returns the found substring within 'this' - 'false' substring if not found.
		// Comparison is done using a binary substring predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastCompare] is used!
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		template<typename compare_T = substring::fast_chr_compare>
		inline substring findLast(
			char to_find, const substring& sub, const compare_T& cmp = substring::FastChrCompare ) const {
			sub.assertSameSource( Bytes.c_str() ); return sub.highToLow().findLast(
				static_cast<char_t>( to_find ), cmp ); }


		// Find last occurrence of any of the specified 'characters'.
		// Returns iterator for the found character - 'false' iterator if not found.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline iterator findLastOf(
			const string& characters, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().highToLow().findLastOf( characters.substr(), cmp ); }

		// Find last occurrence of any character in the specified character class(es).
		// NOTE: Multiple classes can be or'ed together!
		// Returns iterator for the found character - 'false' iterator if not found.
		inline iterator findLastOf( charcat category ) const noexcept {
			return substr().highToLow().findLastOf( category ); }

		// Find last occurrence in 'sub' substring of 'this' of any of the specified 'characters'.
		// Returns iterator for the found character - 'false' iterator if not found.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		template<typename compare_T = substring::fast_chr_compare>
		inline iterator findLastOf(
			const string& characters, substring sub, const compare_T& cmp = substring::FastChrCompare ) const {
			sub.assertSameSource( Bytes.c_str() ); return sub.highToLow().findLastOf( characters.substr(), cmp ); }

		// Find last occurrence of any character in the specified character class(es) in 'sub' substring of 'this'.
		// NOTE: Multiple classes can be or'ed together!
		// Returns iterator for the found character - 'false' iterator if not found.
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		inline iterator findLastOf( charcat characters, substring sub ) const {
			sub.assertSameSource( Bytes.c_str() ); return sub.highToLow().findLastOf( characters ); }


		// Find last occurrence of any character not among the specified 'characters'.
		// Returns iterator for the found character - 'false' iterator if not found.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		template<typename compare_T = substring::fast_chr_compare>
		inline iterator findLastNotOf(
			const string& characters, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().highToLow().findLastNotOf( characters.substr(), cmp ); }

		// Find last occurrence of any character in the specified character class(es).
		// NOTE: Multiple classes can be or'ed together!
		// Returns iterator for the found character - 'false' iterator if not found.
		inline iterator findLastNotOf( charcat characters ) const noexcept {
			return substr().highToLow().findLastNotOf( characters ); }

		// Find last occurrence in 'sub' substring of 'this' of any character not among the specified 'characters'.
		// Returns iterator for the found character - 'false' iterator if not found.
		// Comparison is done using a binary char_t predicate.
		// NOTE: ASCII-only optimization is only done if [eon::substring::FastChrCompare] is used!
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		template<typename compare_T = substring::fast_chr_compare>
		inline iterator findLastNotOf(
			const string& characters, substring sub, const compare_T& cmp = substring::FastChrCompare ) const {
			sub.assertSameSource( Bytes.c_str() ); return sub.highToLow().findLastNotOf( characters.substr(), cmp ); }

		// Find last occurrence in 'sub' substring of 'this' of any character in the specified character class(es).
		// NOTE: Multiple classes can be or'ed together!
		// Returns iterator for the found character - 'false' iterator if not found.
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		inline iterator findLastNotOf( charcat characters, substring sub ) const {
			sub.assertSameSource( Bytes.c_str() );  return sub.highToLow().findLastNotOf( characters ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Counting
		//
	public:

		// Count occurrences of a character.
		// Comparison is done using a binary char_t predicate.
		template<typename compare_T = substring::fast_chr_compare>
		index_t count( char_t to_count, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().count( to_count, cmp ); }

		// Count occurrences of a character in 'sub' substring of 'this'.
		// Comparison is done using a binary char_t predicate.
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		template<typename compare_T = substring::fast_chr_compare>
		inline index_t count(
			char_t to_count, const substring& sub, const compare_T& cmp = substring::FastChrCompare ) const {
			sub.assertSameSource( Bytes.c_str() ); return sub.count( to_count, cmp ); }

		// Count occurrences of a (sub)string.
		// NOTE: All occurrences are counted, including overlaps!
		// Comparison is done using a binary substring predicate.
		template<typename compare_T = substring::fast_chr_compare>
		inline index_t count( const string& to_count, const compare_T& cmp = substring::FastChrCompare ) const noexcept {
			return substr().count( to_count.substr(), cmp ); }

		// Count occurrences of a (sub)string in 'sub' substring of 'this'.
		// NOTE: All occurrences are counted, including overlaps!
		// Comparison is done using a binary substring predicate.
		// Throws [eon::WrongSource] if 'sub' is not a substring of 'this'!
		template<typename compare_T = substring::fast_chr_compare>
		inline index_t count(
			const string& to_count, const substring& sub, const compare_T& cmp = substring::FastChrCompare ) const {
			sub.assertSameSource( Bytes.c_str() ); return sub.count( to_count.substr(), cmp ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Wide-string Support
		//
		// Note that this support is limited and conversion between UTF-8 and
		// wide-string is rather costly.
		//
	public:

		// Construct string from 'std::wstring'.
		inline string( const std::wstring& stdwstr ) { *this = stdwstr; }

		// Construct string from C-style wide-string.
		inline string( const wchar_t* cstr ) { *this = cstr; }


		// Discard current details and assign from 'std::wstring'.
		string& operator=( const std::wstring& stdwstr );

		// Discard current details and assign from C-style wide-string.
		string& operator=( const wchar_t* cstr );


		// Allow use of 'this' in place of a 'std::wstring' object.
		inline operator std::wstring() const { return stdwstr(); }

		// Get a 'std::wstring' conversion copy of 'this'.
		std::wstring stdwstr() const;

		// Concatenate a 'std::wstring' to 'this'.
		inline string& operator+=( const std::wstring& to_add ) { return *this += string( to_add ); }




		///////////////////////////////////////////////////////////////////////
		//
		// String as List
		// The string is considered a list of elements where each element is
		// separated by a common character or sub-string (';' by default). The
		// separator is only between elements, not before the first or after
		// the last.
		//
		// WARNING: No separator checking is done when adding!
		// Adding "alpha" first and then "beta" is the same as adding
		// "alpha;beta" in one go (using default separator).
		//
		// To split the elements into a container, use [eon::string::splitSequential]
		// or [eon::string::splitNonSequential].
		//
	public:

		// Add an element to the end of the string-list.
		string& push_back( const string& element, const string& separator = string( SemiColonChr ) );

		// Add an element to the front of the string-list.
		// NOTE: This is somewhat costly.
		string& push_front( const string& element, const string& separator = string( SemiColonChr ) );

		// Remove duplicates and sort the list.
		// WARNING: This is potentially very costly, depending on the number of elements!
		string& orderList( const string& separator = string( SemiColonChr ) );




		///////////////////////////////////////////////////////////////////////
		//
		// Static utility methods
		//
	public:

		// Convert other types into string (using 'std::to_string')
		// This method is limited to whatever is supported by 'std::to_string'!
		template<typename T>
		static inline string toString( T value ) { return string( std::to_string( value ), true ); }

		// Convert 'double' into string (specifically).
		// This provides greater precision than the template version, and removal of trailing decimal zeros.
		static string toString( double value );

		// Convert 'double' into string (specifically).
		// This provides greater precision than the template version,
		// and removal of trailing decimal zeros.
		static string toString( long double value );


		// Get Byte Order Marker (BOM) for UTF-8.
		static const std::string& bom();


		// Find start of next word within the specified range
		static iterator findWordStart( iterator start, iterator end );

		// Find start of next sentence within the specified range.
		static iterator findSentenceStart( iterator start, iterator end );


		// Check if the specified Unicode codepoint is in the 'Letter' general category.
		static inline bool isLetter( char_t codepoint ) noexcept { return Characters::get().isLetter( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Letter, Upper Case' category.
		static inline bool isLetterUpperCase( char_t codepoint ) noexcept { return Characters::get().isLetterUpperCase( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Letter, Lower Case' category.
		static inline bool isLetterLowerCase( char_t codepoint ) noexcept { return Characters::get().isLetterLowerCase( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Letter, Title Case' category.
		static inline bool isLetterTitleCase( char_t codepoint ) noexcept { return Characters::get().isLetterTitleCase( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Letter, Modifier' category.
		static inline bool isLetterModifier( char_t codepoint ) noexcept { return Characters::get().isLetterModifier( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Letter, Other' category.
		static inline bool isLetterOther( char_t codepoint ) noexcept { return Characters::get().isLetterOther( codepoint ); }


		// Check if the specified Unicode codepoint is in the 'Mark, Spacing Combining' category.
		static inline bool isMarkSpacingCombining( char_t codepoint ) noexcept {
			return Characters::get().isMarkSpacingCombining( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Mark, Nonspacing' category.
		static inline bool isMarknonSpacing( char_t codepoint ) noexcept { return Characters::get().isMarkNonspacing( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Mark, Enclosing' category.
		static inline bool isMarkEnclosing( char_t codepoint ) noexcept { return Characters::get().isMarkEnclosing( codepoint ); }


		// Check if the specified Unicode codepoint is in the 'Number' general category.
		static inline bool isNumber( char_t codepoint ) noexcept {
			return isNumberDecimalDigit( codepoint ) || isNumberLetter( codepoint ) || isNumberOther( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Number, Ascii Digit' custom category.
		static inline bool isNumberAsciiDigit( char_t codepoint ) noexcept { return Characters::get().isNumberAsciiDigit( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Number, Decimal Digit' category.
		static inline bool isNumberDecimalDigit( char_t codepoint ) noexcept {
			return Characters::get().isNumberDecimalDigit( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Number, Letter' category.
		static inline bool isNumberLetter( char_t codepoint ) noexcept { return Characters::get().isNumberLetter( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Number, Other' category.
		static inline bool isNumberOther( char_t codepoint ) noexcept { return Characters::get().isNumberOther( codepoint ); }


		// Check if the specified Unicode codepoint is in the 'Punctuation' general category.
		static inline bool isPunctuation( char_t codepoint ) noexcept { return Characters::get().isPunctuation( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Punctuation, Connector' category.
		static inline bool isPunctuationConnector( char_t codepoint ) noexcept {
			return Characters::get().isPunctuationConnector( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Punctuation, Dash' category.
		static inline bool isPunctuationDash( char_t codepoint ) noexcept { return Characters::get().isPunctuationDash( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Punctuation, Open' category.
		static inline bool isPunctuationOpen( char_t codepoint ) noexcept { return Characters::get().isPunctuationOpen( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Punctuation, Close' category.
		static inline bool isPunctuationClose( char_t codepoint ) noexcept { return Characters::get().isPunctuationClose( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Punctuation, Initial Quote' category.
		static inline bool isPunctuationInitialQuote( char_t codepoint ) noexcept {
			return Characters::get().isPunctuationInitialQuote( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Punctuation, Final Quote' category.
		static inline bool isPunctuationFinalQuote( char_t codepoint ) noexcept {
			return Characters::get().isPunctuationFinalQuote( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Punctuation, Other' category.
		static inline bool isPunctuationOther( char_t codepoint ) noexcept { return Characters::get().isPunctuationOther( codepoint ); }


		// Check if the specified Unicode codepoint is in the 'Symbol' general category.
		static inline bool isSymbol( char_t codepoint ) noexcept { return Characters::get().isSymbol( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Symbol, Currency' category.
		static inline bool isSymbolCurrency( char_t codepoint ) noexcept { return Characters::get().isSymbolCurrency( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Symbol, Modifier' category.
		static inline bool isSymbolModifier( char_t codepoint ) noexcept { return Characters::get().isSymbolModifier( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Symbol, Math' category.
		static inline bool isSymbolMath( char_t codepoint ) noexcept { return Characters::get().isSymbolMath( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Symbol, Other' category.
		static inline bool isSymbolOther( char_t codepoint ) noexcept { return Characters::get().isSymbolOther( codepoint ); }


		// Check if the specified Unicode codepoint is in the 'Separator' general category.
		static inline bool isSeparator( char_t codepoint ) noexcept { return Characters::get().isSeparator( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Separator, Line' category.
		static inline bool isSeparatorLine( char_t codepoint ) noexcept { return Characters::get().isSeparatorLine( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Separator, Paragraph' category.
		static inline bool isSeparatorParagraph( char_t codepoint ) noexcept {
			return Characters::get().isSeparatorParagraph( codepoint ); }

		// Check if the specified Unicode codepoint is in the 'Separator, Space' category.
		static inline bool isSeparatorSpace( char_t codepoint ) noexcept { return Characters::get().isSeparatorSpace( codepoint ); }


		// Check if a Unicode codepoint is a word character.
		// NOTE: Includes underscore plus all characters from the categories 'Letters', 'Number, and Decimal Digits'!
		static inline bool isWordChar( char_t codepoint ) noexcept {
			return isLetter( codepoint ) || isNumberDecimalDigit( codepoint ) || codepoint == '_'; }

		// Check if a Unicode codepoint is a space character.
		// NOTE: Includes tab, space, and all characters in the 'Separator' category!
		static inline bool isSpaceChar( char_t codepoint ) noexcept { return ( codepoint >= 0x09 && codepoint <= 0x0D ) || isSeparator( codepoint ); }





		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	PRIVATE:

		template<typename T>
		inline void _assertValidUtf8( const T& value ) const { if( !value.validUTF8() ) throw InvalidUTF8(); }

		string& _assignLowToHigh( const substring& input );
		string& _assignHighToLow( const substring& input );

		string::iterator _optimizedAsciiBytePos( index_t pos, const iterator& start ) const;
		string::iterator _ensureValidStart( iterator& start ) const;
		string::iterator _count( index_t pos, iterator& start ) const;

		inline string _prepOutput( const substring& area ) const {
			string output; if( area.begin() > begin() ) output = substr( begin(), area.begin() ); return output; }

		string _escape( const substring& sub, const std::unordered_map<char_t, char_t>& singletons ) const;
		string _escape( const substring& area, string output, const std::unordered_map<char_t, char_t>& singletons ) const;

		string _unescape( const substring& sub, const std::unordered_map<char_t, char_t>& singletons ) const;
		string _unescape(
			const substring& area, string output, const std::unordered_map<char_t, char_t>& singletons ) const;

		template<typename container>
		static index_t _findDecimalSeparator( container& char_digits, char_t decimal_separator ) noexcept
		{
			for( index_t i = 0; i < char_digits.size(); ++i )
			{
				if( char_digits[ i ] == decimal_separator )
					return i;
			}
			return char_digits.size();
		}

		template<typename container>
		static inline void _addPrefixDigit( container& char_digits )
		{
			*char_digits.begin() = '9';
			char_digits.insert( char_digits.begin(), static_cast<byte_t>( '1' ) );
		}

		template<typename container>
		static void _roundUp( container& char_digits, typename container::iterator i, char_t decimal_separator ) noexcept
		{
			// NOTE: This method is only called if the digit at i+1 is >= 5!

			// Bypass decimal separator if that's at position i.
			if( *i == decimal_separator )
				--i;

			// If we run out of digits, we must add one at the start.
			if( i == char_digits.begin() )
			{
				_addPrefixDigit( char_digits );
				return;
			}

			// If rounding up causes the digit to "overflow", we must also round up i-1.
			auto zero = zeroChar( *i );
			auto value = *i - zero;
			if( value == 9 )
			{
				*i = '0';
				_roundUp( char_digits, i - 1, decimal_separator );
			}

			// No overflow, just increment.
			else
				*i = static_cast<char_t>( value + 1 + zero );
		}

		inline bool _ascii() const noexcept { return numBytes() == NumChars; }




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//

	PRIVATE:
		std::string Bytes;
		index_t NumChars{ 0 };	// Number of code points

	public:
		static const string Empty;
	};


	struct _TransformData
	{
		inline _TransformData( const string& source, const substring& sub ) {
			if( sub.empty() ) { Str = source; Complete = true; } }

		substring Area;
		string Str;
		bool Complete{ false };
	};




	// Get a string that is the result of concatenating one substring with another.
	inline string operator+( const substring& a, const substring& b ) { return string( a ) += b; }

	// Get a string that is the result of concatenating a substring with a 'std::string'.
	inline string operator+( const substring& a, const std::string& b ) { return string( a ) += b; }

	// Get a string that is the result of concatenating a 'std::string' with a substring.
	inline string operator+( const std::string& a, const substring& b ) { return string( a ) += b; }

	// Get a string that is the result of concatenating a substring with a C string.
	inline string operator+( const substring& a, const char* b ) { return string( a ) += b; }

	// Get a string that is the result of concatenating a C string with a substring.
	inline string operator+( const char* a, const substring& b ) { return string( a ) += b; }


	// Smart-pointer to string.
	using string_ptr = std::shared_ptr<string>;

	// Binary predicate for comparing two [eon::string_ptr] smart-pointers.
	struct string_lt
	{
		inline bool operator()( const string* lhs, const string* rhs ) const {
			return lhs->compare( *rhs, substring::FastCompare ) < 0; }
	};
};


namespace std
{
	// Allow implicit use of [eon::string] as key when used in containers such
	// as 'std::unordered_map' and 'std::unordered_set'.
	template<>
	struct hash<::eon::string> {
		inline size_t operator()( const ::eon::string& rhs ) const {
			return static_cast<size_t>( rhs.hash() ); } };

	// Allow implicit use of [eon::string] as value when used in containers such
	// as 'std::unordered_map' and 'std::unordered_set'.
	template<>
	struct equal_to<::eon::string> {
		inline bool operator()( const ::eon::string& lhs, const ::eon::string& rhs ) const {
			return lhs == rhs; } };


	// Allow implicit use of [eon::string_ptr] as key when used in containers
	// such as 'std::unordered_map' and 'std::unordered_set'.
	template<>
	struct hash<::eon::string_ptr> {
		inline size_t operator()( const ::eon::string_ptr& rhs ) const {
			return static_cast<size_t>( rhs->hash() ); } };

	// Allow implicit use of [eon::string_ptr] as value when used in containers
	// such as 'std::unordered_map' and 'std::unordered_set'.
	template<>
	struct equal_to<::eon::string_ptr> {
		inline bool operator()( const ::eon::string_ptr& lhs, const ::eon::string_ptr& rhs ) const {
			return *lhs == *rhs; } };


	// Allow implicit use of [eon::string_ptr] as key when used in containers
	// such as 'std::map' and 'std::set'.
	struct string_ptr_lt {
		inline bool operator()( const ::eon::string_ptr& lhs, const ::eon::string_ptr& rhs ) const {
			return *lhs < *rhs; } };


	// Allow [eon::string] to be serialized to 'std::ostream' objects.
	inline ostream& operator<<( ostream& o, const ::eon::string& str ) { o << str.stdstr(); return o; }

	// Allow [eon::string] to be de-serialized from 'std::istream' objects.
	inline istream& operator>>( istream& i, ::eon::string& str ) { std::string tmp; i >> tmp; str = tmp; return i; }
};
