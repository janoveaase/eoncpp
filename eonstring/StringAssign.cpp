#include "String.h"
#include <eoninlinetest/InlineTest.h>
#include <cctype>
#include <regex>
#include <unordered_map>


namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// String Assignment Metods
	//

	string& string::assign( const char_t* input, index_t input_length )
	{
		clear();
		reserve( input_length );
		for( index_t i = 0; i < input_length; ++i )
			*this += input[ i ];
		return *this;
	}
	EON_TEST_3STEP( string, assign, char_t_string_length_empty,
		auto source = eonitest::PrimitiveArray<char_t>(),
		string obj,
		EON_EQ( 0, obj.assign( source.value(), 0 ).length() ) );
	EON_TEST_3STEP( string, assign, char_t_string_length_nonempty_length,
		auto source = eonitest::PrimitiveArray<char_t>( EON_CURLY( 0x41, 0x42, 0x63, 0 ) ),
		string obj,
		EON_EQ( 3, obj.assign( source.value(), 3 ).length() ) );
	EON_TEST_3STEP( string, assign, char_t_string_length_nonempty_value,
		auto source = eonitest::PrimitiveArray<char_t>( EON_CURLY( 0x41, 0x42, 0x63, 0 ) ),
		string obj,
		EON_EQ( "ABc", obj.assign( source.value(), 3 ) ) );

	string& string::assign( const char* input, index_t input_length )
	{
		iterator i( input, input_length );	// Using iterator to scan the raw string for us!
		_assertValidUtf8( i );
		NumChars = i.numSourceChars();
		Bytes.assign( input, input_length );
		return *this;
	}
	EON_TEST_3STEP( string, assign, cstr_length_empty_length,
		const char* source = "",
		string obj,
		EON_EQ( 0, obj.assign( source, 0 ).length() ) );
	EON_TEST_3STEP( string, assign, cstr_length_nonempty_length,
		const char* source = "abcdef",
		string obj,
		EON_EQ( 4, obj.assign( source, 4 ).length() ) );
	EON_TEST_3STEP( string, assign, cstr_length_nonempty_value,
		const char* source = "abcdef",
		string obj,
		EON_EQ( "abcd", obj.assign( "abcdef", 4 ) ) );
	EON_TEST_2STEP( string, assign, cstr_string_invalid_utf8,
		auto source = eonitest::PrimitiveArray<char>( EON_CURLY( 0x41, -28, 0x63, 0 ) ),
		EON_RAISE( string().assign( source.value(), 3 ), InvalidUTF8 ) );

	string& string::assign( index_t copies, char_t input )
	{
		uint32_t bytes;
		auto size = iterator::unicodeToBytes( input, bytes );
		Bytes.reserve( size * copies );
		for( index_t i = 0; i < copies; ++i )
			Bytes.append( (const char*)&bytes, size );
		NumChars = copies;
		return *this;
	}
	EON_TEST( string, assign, char_t_copies_none,
		EON_EQ( "", string().assign( static_cast<index_t>( 0 ), char_t( 'a' ) ) ) );
	EON_TEST( string, assign, char_t_copies_multiple,
		EON_EQ( "aaaaa", string().assign( static_cast<index_t>( 5 ), char_t( 'a' ) ) ) );

	EON_TEST( string, assign, char_copies_none,
		EON_EQ( "", string().assign( static_cast<index_t>( 0 ), 'a' ) ) );
	EON_TEST( string, assign, char_copies_multiple,
		EON_EQ( "aaaaa", string().assign( static_cast<index_t>( 5 ), 'a' ) ) );

	string& string::assign( index_t copies, const string& other )
	{
		// Make sure 'other' and 'this' are not the same!
		if( &other.Bytes != &Bytes )
		{
			Bytes.reserve( other.numBytes() * copies );
			for( index_t i = 0; i < copies; ++i )
				Bytes.append( other.Bytes );
			NumChars = other.NumChars * copies;
			return *this;
		}
		else
			return assign( copies, string( other ) );
	}
	EON_TEST( string, assign, string_copies_none,
		EON_EQ( "", string().assign( static_cast<index_t>( 0 ), string( "ab" ) ) ) );
	EON_TEST( string, assign, string_copies_multiple,
		EON_EQ( "ababababab", string().assign( static_cast<index_t>( 5 ), string( "ab" ) ) ) );

	string& string::assign( index_t copies, const std::string& input )
	{
		// Make sure 'other' is not our own buffer!
		if( &input != &Bytes )
		{
			iterator i( input );
			_assertValidUtf8( i );
			Bytes.reserve( input.size() * copies );
			for( index_t i = 0; i < copies; ++i )
				Bytes.append( input );
			NumChars = i.numChar() * copies;
			return *this;
		}
		else
			return assign( copies, std::string( input ) );
	}
	EON_TEST( string, assign, stdstring_copies_none,
		EON_EQ( "", string().assign( static_cast<index_t>( 0 ), std::string( "ab" ) ) ) );
	EON_TEST( string, assign, stdstring_copies_multiple,
		EON_EQ( "ababababab", string().assign( static_cast<index_t>( 5 ), std::string( "ab" ) ) ) );

	string& string::assign( index_t copies, const substring& input )
	{
		// Make sure 'input' is not from 'this'!
		if( !input.sameSource( Bytes.c_str() ) )
		{
			_assertValidUtf8( input );
			clear();
			Bytes.reserve( input.numBytes() * copies );
			for( index_t i = 0; i < copies; ++i )
				*this += input;
			return *this;
		}
		else
			return assign( copies, string( input ) );
	}
	EON_TEST( string, assign, substringcopies_none,
		EON_EQ( string(), string().assign( static_cast<index_t>( 0 ), substring( "ab" ) ) ) );
	EON_TEST( string, assign, substringcopies_one,
		EON_EQ( "ab", string().assign( static_cast<index_t>( 1 ), substring( "ab" ) ) ) );
	EON_TEST( string, assign, substringcopies_many,
		EON_EQ( "ababababab", string().assign( static_cast<index_t>( 5 ), substring( "ab" ) ) ) );

	EON_TEST_3STEP( string, operator_asgn, copy_1,
		string source( "abcdef" ),
		string obj = source,
		EON_EQ( "abcdef", source ) );
	EON_TEST_3STEP( string, operator_asgn, copy_2,
		string source( "abcdef" ),
		string obj = source,
		EON_EQ( "abcdef", obj ) );

	EON_TEST_3STEP( string, operator_asgn, move_1,
		string source( "abcdef" ),
		string obj = std::move( source ),
		EON_EQ( "", source ) );
	EON_TEST_3STEP( string, operator_asgn, move_2,
		string source( "abcdef" ),
		string obj = std::move( source ),
		EON_EQ( "abcdef", obj ) );

	string& string::operator=( const substring& input )
	{
		_assertValidUtf8( input );
		return input.isLowToHigh() ? _assignLowToHigh( input ) : _assignHighToLow( input );
	}
	EON_TEST( string, operator_asgn, substring_empty,
		EON_EQ( "", string() = substring( "" ) ) );
	EON_TEST( string, operator_asgn, substring_lowTohigh,
		EON_EQ( "abc", string() = substring( "abc" ) ) );
	EON_TEST_2STEP( string, operator_asgn, substring_highTolow,
		auto inp = substring( "cba" ).highToLow(),
		EON_EQ( "abc", string() = inp ) );

	string& string::_assignHighToLow( const substring& input )
	{
		clear();
		for( auto i = input.begin(); i != input.end(); --i )
			*this += *i;
		return *this;
	}
	EON_NO_TEST( string, _assignHighToLow );

	string& string::_assignLowToHigh( const substring& input )
	{
		NumChars = input.numChars();
		Bytes.assign( input.begin().byteData(), input.numBytes() );
		return *this;
	}
	EON_NO_TEST( string, _assignLowToHigh );

	EON_TEST( string, operator_asgn, stdstring_empty,
		EON_EQ( "", string() = std::string( "" ) ) );
	EON_TEST( string, operator_asgn, stdstring_something,
		EON_EQ( "abcdef", string() = std::string( "abcdef" ) ) );

	string& string::operator=( std::string&& input )
	{
		substring sub( input );
		_assertValidUtf8( sub );
		NumChars = sub.numChars();
		Bytes = std::move( input );
		return *this;
	}
#ifdef EON_TEST_MODE
	struct strpair
	{
		std::string stdstr;
		eon::string eonstr;

		strpair( std::string value ) : stdstr( std::move( value ) ) {}
	};
#endif
	EON_TEST_3STEP( string, operator_asgn, stdstring_move_1,
		strpair data( "abcdef" ),
		data.eonstr = std::move( data.stdstr ),
		EON_EQ( "", data.stdstr ) );
	EON_TEST_3STEP( string, operator_asgn, stdstring_move_2,
		strpair data( "abcdef" ),
		data.eonstr = std::move( data.stdstr ),
		EON_EQ( "abcdef", data.eonstr ) );

	EON_TEST( string, operator_asgn, cstring_empty,
		EON_EQ( "", string() = "" ) );
	EON_TEST( string, operator_asgn, cstring_something,
		EON_EQ( "abcdef", string() = "abcdef" ) );

	EON_TEST( string, operator_asgn, char_t_ASCII,
		EON_EQ( "a", string() = char_t( 'a' ) ) );
	EON_TEST( string, operator_asgn, char_t_UTF8,
		EON_EQ( u8"ã", string() = char_t( 0xE3 ) ) );

	EON_TEST( string, operator_asgn, char,
		EON_EQ( "a", string() = 'a' ) );
	EON_TEST( string, operator_asgn, char_invalid_UTF8,
		EON_RAISE( string() = static_cast<char>( -22 ), InvalidUTF8 ) );

	EON_TEST( string, operator_asgn, unsigned_char,
		EON_EQ( "a", string() = 'a' ) );
	EON_TEST( string, operator_asgn, unsigned_char_invalid_UTF8,
		EON_RAISE( string() = static_cast<char>( 0xEA ), InvalidUTF8 ) );

	string& string::operator=( const std::initializer_list<char_t>& input )
	{
		clear();
		reserve( input.size() );
		for( auto& c : input )
			*this += c;
		return *this;
	}
	EON_TEST( string, operator_asgn, char_t_initializer,
		EON_EQ( "abc", string() = EON_CURLY( char_t( 'a' ), char_t( 'b' ), char_t( 'c' ) ) ) );

	string& string::operator=( const std::vector<char_t>& input )
	{
		clear();
		reserve( input.size() );
		for( auto& c : input )
			*this += c;
		return *this;
	}
	EON_TEST( string, operator_asgn, char_t_vector,
		EON_EQ( "abc", string() = std::vector<char_t>( { char_t( 'a' ), char_t( 'b' ), char_t( 'c' ) } ) ) );

	string& string::operator=( const std::initializer_list<char>& input )
	{
		clear();
		reserve( input.size() );
		for( auto& chr : input )
			*this += static_cast<byte_t>( chr );
		return *this;
	}
	EON_TEST( string, operator_asgn, char_initializer,
		EON_EQ( "abc", string() = EON_CURLY( 'a', 'b', 'c' ) ) );

	string& string::operator=( const std::initializer_list<unsigned char>& input )
	{
		clear();
		reserve( input.size() );
		for( auto& chr : input )
			*this += static_cast<byte_t>( chr );
		return *this;
	}
	EON_TEST( string, operator_asgn, unsigned_char_initializer,
		EON_EQ( "abc", string() = EON_CURLY(
			eonitest::uchar_t( 'a' ), eonitest::uchar_t( 'b' ), eonitest::uchar_t( 'c' ) ) ) );

	EON_TEST( string, operator_asgn, bool_true,
		EON_EQ( "true", string() = true ) );
	EON_TEST( string, operator_asgn, bool_false,
		EON_EQ( "false", string() = false ) );
}
