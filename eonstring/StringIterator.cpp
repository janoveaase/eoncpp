#include "StringIterator.h"
#include "String.h"
#include <eoninlinetest/InlineTest.h>
#include <cctype>
#include <regex>


namespace eon
{
	// Copyright (c) 2008-2009 Bjoern Hoehrmann <bjoern@hoehrmann.de>
	// See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.
	static const uint8_t utf8d[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 00..1f
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 20..3f
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 40..5f
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 60..7f
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, // 80..9f
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, // a0..bf
		8, 8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // c0..df
		0xa, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x3, // e0..ef
		0xb, 0x6, 0x6, 0x6, 0x5, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, // f0..ff
		0x0, 0x1, 0x2, 0x3, 0x5, 0x8, 0x7, 0x1, 0x1, 0x1, 0x4, 0x6, 0x1, 0x1, 0x1, 0x1, // s0..s0
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, // s1..s2
		1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, // s3..s4
		1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 3, 1, 1, 1, 1, 1, 1, // s5..s6
		1, 3, 1, 1, 1, 1, 1, 3, 1, 3, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // s7..s8
	};
	inline char32_t _utf8Decode( char32_t& state, char32_t& codep, char32_t byte ) noexcept
	{
		char32_t type = utf8d[ byte ];
		codep = ( state != UTF8_ACCEPT ) ? ( byte & 0x3fu ) | ( codep << 6 ) : ( 0xff >> type ) & ( byte );
		state = utf8d[ 256 + state * 16 + type ];
		return state;
	}
	EON_NO_TEST( string_iterator, _utf8Decode );




	EON_TEST( string_iterator, string_iterator, stdstr_empty,
		EON_EQ( "0:0", string_iterator( std::string() ).encode() ) );
	EON_TEST( string_iterator, string_iterator, stdstr_ASCII_start,
		EON_EQ( "0:0", string_iterator( std::string( "abcdef" ) ).encode() ) );
	EON_TEST( string_iterator, string_iterator, stdstr_ASCII_mid,
		EON_EQ( "3:3", ( string_iterator( std::string( "abcdef" ) ) + 3 ).encode() ) );
	EON_TEST( string_iterator, string_iterator, stdstr_ASCII_end,
		EON_EQ( "6:6", ( string_iterator( std::string( "abcdef" ) ) + 6 ).encode() ) );
	EON_TEST( string_iterator, string_iterator, stdstr_UTF8_start,
		EON_EQ( "0:0", string_iterator( std::string( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ) ).encode() ) );
	EON_TEST( string_iterator, string_iterator, stdstr_UTF8_mid,
		EON_EQ( "7:3", ( string_iterator( std::string( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ) ) + 3 ).encode() ) );
	EON_TEST( string_iterator, string_iterator, stdstr_UTF8_end,
		EON_EQ( "14:6", ( string_iterator( std::string( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ) ) + 6 ).encode() ) );

	EON_TEST( string_iterator, string_iterator, stdstr_set_ASCII_start,
		EON_EQ( "0:0", string_iterator( std::string( "abcdef" ), 0 ).encode() ) );
	EON_TEST( string_iterator, string_iterator, stdstr_set_ASCII_mid,
		EON_EQ( "3:3", string_iterator( std::string( "abcdef" ), 3 ).encode() ) );
	EON_TEST( string_iterator, string_iterator, stdstr_set_ASCII_end,
		EON_EQ( "6:6", string_iterator( std::string( "abcdef" ), 6 ).encode() ) );
	EON_TEST( string_iterator, string_iterator, stdstr_set_UTF8_start,
		EON_EQ( "0:0", string_iterator( std::string( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ), 0 ).encode() ) );
	EON_TEST( string_iterator, string_iterator, stdstr_set_UTF8_mid,
		EON_EQ( "7:3", ( string_iterator( std::string( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ), 3 ) ).encode() ) );
	EON_TEST( string_iterator, string_iterator, stdstr_set_UTF8_end,
		EON_EQ( "14:6", ( string_iterator( std::string( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ), 6 ) ).encode() ) );

	EON_TEST( string_iterator, string_iterator, cstr_empty,
		EON_EQ( "0:0", string_iterator( "" ).encode() ) );
	EON_TEST( string_iterator, string_iterator, cstr_ASCII_start,
		EON_EQ( "0:0", string_iterator( "abcdef" ).encode() ) );
	EON_TEST( string_iterator, string_iterator, cstr_ASCII_mid,
		EON_EQ( "3:3", ( string_iterator( "abcdef" ) + 3 ).encode() ) );
	EON_TEST( string_iterator, string_iterator, cstr_ASCII_end,
		EON_EQ( "6:6", ( string_iterator( "abcdef" ) + 6 ).encode() ) );
	EON_TEST( string_iterator, string_iterator, cstr_UTF8_start,
		EON_EQ( "0:0", string_iterator( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ).encode() ) );
	EON_TEST( string_iterator, string_iterator, cstr_UTF8_mid,
		EON_EQ( "7:3", ( string_iterator( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ) + 3 ).encode() ) );
	EON_TEST( string_iterator, string_iterator, cstr_UTF8_end,
		EON_EQ( "14:6", ( string_iterator( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ) + 6 ).encode() ) );

	EON_TEST( string_iterator, string_iterator, ucstr_ASCII_mid,
		EON_EQ( "3:3", ( string_iterator( (const unsigned char*)"abcdef" ) + 3 ).encode() ) );

	EON_TEST( string_iterator, string_iterator, cstrlen_ASCII_mid,
		EON_EQ( "3:3", ( string_iterator( "abcdef", 6 ) + 3 ).encode() ) );

	EON_TEST( string_iterator, string_iterator, cstr_ASCII_details1,
		EON_EQ( "6:6", ( string_iterator( "abcdef", 6, 6 ) + 6 ).encode() ) );
	EON_TEST( string_iterator, string_iterator, cstr_UTF8_details1,
		EON_EQ( "12:5", ( string_iterator( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5", 14, 6 ) + 5 ).encode() ) );

	EON_TEST_2STEP( string_iterator, string_iterator, cstr_UTF8_details2,
		const char* source = u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5",
		EON_EQ(  "12:5", string_iterator( source, 14, source + 12 ).encode() ) );

	EON_TEST_2STEP( string_iterator, string_iterator, cstr_UTF8_details3,
		const char* source = u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5",
		EON_EQ( "12:5", string_iterator( source, 14, 6, source + 12 ).encode() ) );

	EON_TEST_2STEP( string_iterator, string_iterator, cstr_UTF8_details4,
		const char* source = u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5",
		EON_EQ( "12:5", string_iterator( source, 14, 6, source + 12, 5 ).encode() ) );


	string_iterator::string_iterator( const string_iterator& source, const char* pos, index_t num_char ) noexcept
	{
		Source = source.Source;
		SourceEnd = source.SourceEnd;
		NumSourceChars = source.NumSourceChars;
		ValidUTF8 = source.ValidUTF8;
		Pos = pos;
		NumChar = num_char;
		if( NumChar == 0 && Pos != Source )
			_utf8CharacterCount();
		_translateCodepoint();
	}
	EON_TEST_3STEP( string_iterator, string_iterator, cstr_UTF8_copy_set,
		const char* source = u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5",
		string_iterator obj( source ),
		EON_EQ( "12:5", string_iterator( obj, source + 12, 5 ).encode() ) );




	EON_TEST( string_iterator, encode, no_source,
		EON_EQ( "", string_iterator().encode() ) );
	EON_TEST( string_iterator, encode, empty,
		EON_EQ( "0:0", string_iterator( "" ).encode() ) );

	string_iterator& string_iterator::decode(
		const std::string& iterator_format, const char* source_start, const char* source_end, index_t num_source_chars )
	{
		std::regex pattern{ R"(^(\d+):(\d+)$)" };
		std::smatch match;
		if( !std::regex_match( iterator_format, match, pattern ) )
			throw WrongSource( "Invalid iterator string format to decode: \"" + iterator_format + "\"!" );

		if( source_start == nullptr )
			throw WrongSource( "Invalid source specified for iterator format decode: nullptr!" );
		Source = source_start;

		SourceEnd = source_end != nullptr ? source_end : Source + strlen( Source );

		Pos = Source + std::stoi( match[ 1 ].str() );
		if( Pos > SourceEnd )
			throw WrongSource( "Invalid byte position specified in iterator format to decode!\n"
				"  Format (\"<byte-pos>:<char-pos>\"): \"" + iterator_format + "\"\n"
				"  Byte position                     : " + std::to_string( (Pos - SourceEnd ) ) + "\n"
				"  Max byte position possible        : " + std::to_string( SourceEnd - Source ) );

		NumSourceChars = num_source_chars;
		if( num_source_chars == no_index )
			_utf8CharacterCount();

		NumChar = std::stoull( match[ 2 ].str() );
		if( NumChar > NumSourceChars )
			throw WrongSource( "Invalid character position specified in iterator format to decode!\n"
				"  Format (\"<byte-pos>:<char-pos>\"): \"" + iterator_format + "\"\n"
				"  Character position                : " + std::to_string( NumChar ) + "\n"
				"  Max character position possible   : " + std::to_string( NumSourceChars ) );

		_translateCodepoint();
		return *this;
	}
	EON_TEST_2STEP( string_iterator, decode, no_source,
		string_iterator obj,
		EON_RAISE( obj.decode( "", nullptr ), WrongSource ) );
	EON_TEST_3STEP( string_iterator, decode, ASCII_overstep,
		const char* source = "abcdef",
		string_iterator obj,
		EON_RAISE( obj.decode( "7:7", source ), WrongSource ) );
	EON_TEST_3STEP( string_iterator, decode, UTF8_char_overstep,
		const char* source = u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5",
		string_iterator obj,
		EON_RAISE( obj.decode( "15:6", source ), WrongSource ) );

	EON_TEST_3STEP( string_iterator, decode, ASCII_start,
		const char* source = "abcdef",
		string_iterator obj,
		EON_EQ( "0:0", obj.decode( "0:0", source ).encode() ) );
	EON_TEST_3STEP( string_iterator, decode, ASCII_mid,
		const char* source = "abcdef",
		string_iterator obj,
		EON_EQ( "3:3", obj.decode( "3:3", source ).encode() ) );
	EON_TEST_3STEP( string_iterator, decode, UTF8_mid,
		const char* source = u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5",
		string_iterator obj,
		EON_EQ( "7:3", obj.decode( "7:3", source ).encode() ) );
	EON_TEST_3STEP( string_iterator, decode, UTF8_end,
		const char* source = u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5",
		string_iterator obj,
		EON_EQ( "14:6", obj.decode( "14:6", source ).encode() ) );




	EON_TEST_3STEP( string_iterator, operator_eq, stdstr,
		string_iterator obj,
		obj = std::string( "abcdef" ),
		EON_EQ( "3:3", ( obj + 3 ).encode() ) );

	string_iterator& string_iterator::reset() noexcept
	{
		Source = nullptr;
		SourceEnd = nullptr;
		Pos = nullptr;
		Codepoint = nochar;
		CodepointSize = 0;
		NumChar = 0;
		NumSourceChars = 0;
		return *this;
	}
	EON_TEST_3STEP( string_iterator, reset, basic,
		string_iterator obj( "abcdef" ),
		obj += 3,
		EON_TRUE( string_iterator() == obj.reset() ) );

	string_iterator& string_iterator::resetToBegin() noexcept
	{
		if( Pos == Source && CodepointSize != 0 )
			return *this;
		bool must_translate = Pos != Source || Codepoint == 0;
		Pos = Source;
		CodepointSize = 0;
		NumChar = 0;
		if( must_translate )
			_translateCodepoint();
		return *this;
	}
	EON_TEST_3STEP( string_iterator, resetToBegin, basic,
		string_iterator obj( "abcdef" ),
		obj += 3,
		EON_EQ( "0:0", obj.resetToBegin().encode() ) );

	string_iterator& string_iterator::resetToEnd() noexcept
	{
		Pos = SourceEnd;
		Codepoint = nochar;
		CodepointSize = 0;
		NumChar = NumSourceChars;
		return *this;
	}
	EON_TEST_3STEP( string_iterator, resetToEnd, basic,
		string_iterator obj( "abcdef" ),
		obj += 3,
		EON_EQ( "6:6", obj.resetToEnd().encode() ) );

	string_iterator& string_iterator::resetToREnd() noexcept
	{
		Pos = Source;
		Codepoint = nochar;
		CodepointSize = 0;
		NumChar = 0;
		return *this;
	}
	EON_TEST_3STEP( string_iterator, resetToREnd, basic,
		string_iterator obj( "abcdef" ),
		obj += 3,
		EON_TRUE( obj.resetToREnd().atREnd() ) );




	EON_TEST( string_iterator, operator_bool, true_start,
		EON_TRUE( string_iterator( "abcdef" ) ) );
	EON_TEST( string_iterator, operator_bool, false_end,
		EON_FALSE( string_iterator( "abcdef" ) + 6 ) );
	EON_TEST( string_iterator, operator_bool, false_no_source,
		EON_FALSE( string_iterator() ) );

	EON_TEST( string_iterator, hasSource, true_start,
		EON_TRUE( string_iterator( "abcdef" ).hasSource() ) );
	EON_TEST( string_iterator, hasSource, true_end,
		EON_TRUE( ( string_iterator( "abcdef" ) + 6 ).hasSource() ) );
	EON_TEST( string_iterator, hasSource, false,
		EON_FALSE( string_iterator().hasSource() ) );

	EON_TEST( string_iterator, atREnd, false_start,
		EON_FALSE( string_iterator( "abcdef" ).atREnd() ) );
	EON_TEST( string_iterator, atREnd, false_end,
		EON_FALSE( ( string_iterator( "abcdef" ) + 6 ).atREnd() ) );
	EON_TEST( string_iterator, atREnd, true_rend,
		EON_TRUE( ( --string_iterator( "abcdef" ) ).atREnd() ) );

	EON_TEST( string_iterator, atEnd, false_start,
		EON_FALSE( string_iterator( "abcdef" ).atEnd() ) );
	EON_TEST( string_iterator, atEnd, true_end,
		EON_TRUE( ( string_iterator( "abcdef" ) + 6 ).atEnd() ) );
	EON_TEST( string_iterator, atEnd, false_rend,
		EON_FALSE( ( --string_iterator( "abcdef" ) ).atEnd() ) );

	EON_TEST_3STEP( string_iterator, sameSource, true,
		const char* source = "abcdef",
		string_iterator obj( source ),
		EON_TRUE( obj.sameSource( source ) ) );
	EON_TEST_3STEP( string_iterator, sameSource, false,
		const char* source = "abcdef",
		string_iterator obj( "bcdefa" ),
		EON_FALSE( obj.sameSource( source ) ) );

	EON_TEST_3STEP( string_iterator, assertSameSource, iterator_same,
		const char* source = "abcdef",
		string_iterator obj( source ),
		EON_NO_X( string_iterator( source ).assertSameSource( obj ) ) );
	EON_TEST_3STEP( string_iterator, assertSameSource, iterator_different,
		const char* source = "abcdef",
		string_iterator obj( "bcdefa" ),
		EON_RAISE( string_iterator( source ).assertSameSource( obj ), WrongSource ) );

	EON_TEST_3STEP( string_iterator, assertSameSource, raw_same,
		const char* source = "abcdef",
		string_iterator obj( source ),
		EON_NO_X( obj.assertSameSource( source ) ) );
	EON_TEST_3STEP( string_iterator, assertSameSource, raw_different,
		const char* source = "abcdef",
		string_iterator obj( "bcdefa" ),
		EON_RAISE( obj.assertSameSource( source ), WrongSource ) );

	EON_TEST( string_iterator, operator_call, ASCII_start,
		EON_EQ( char_t( 'a' ), *string_iterator( "abcdef" ) ) );
	EON_TEST( string_iterator, operator_call, ASCII_mid,
		EON_EQ( char_t( 'd' ), *( string_iterator( "abcdef" ) + 3 ) ) );
	EON_TEST( string_iterator, operator_call, UTF8_start,
		EON_EQ( char_t( 216 ), *string_iterator( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ) ) );
	EON_TEST( string_iterator, operator_call, UTF8_mid,
		EON_EQ( char_t( 8482 ), *( string_iterator( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ) + 3 ) ) );

	EON_TEST( string_iterator, numChar, empty,
		EON_EQ( 0, string_iterator( "" ).numChar() ) );
	EON_TEST( string_iterator, numChar, ASCII_start,
		EON_EQ( 0, string_iterator( "abcdef" ).numChar() ) );
	EON_TEST( string_iterator, numChar, ASCII_mid,
		EON_EQ( 3, ( string_iterator( "abcdef" ) + 3 ).numChar() ) );
	EON_TEST( string_iterator, numChar, ASCII_end,
		EON_EQ( 6, ( string_iterator( "abcdef" ) + 6 ).numChar() ) );
	EON_TEST( string_iterator, numChar, UTF8_start,
		EON_EQ( 0, string_iterator( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ).numChar() ) );
	EON_TEST( string_iterator, numChar, UTF8_mid,
		EON_EQ( 3, ( string_iterator( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ) + 3 ).numChar() ) );
	EON_TEST( string_iterator, numChar, UTF8_end,
		EON_EQ( 6, ( string_iterator( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ) + 6 ).numChar() ) );

	EON_TEST( string_iterator, numByte, empty,
		EON_EQ( 0, string_iterator( "" ).numByte() ) );
	EON_TEST( string_iterator, numByte, ASCII_start,
		EON_EQ( 0, string_iterator( "abcdef" ).numByte() ) );
	EON_TEST( string_iterator, numByte, ASCII_mid,
		EON_EQ( 3, ( string_iterator( "abcdef" ) + 3 ).numByte() ) );
	EON_TEST( string_iterator, numByte, ASCII_end,
		EON_EQ( 6, ( string_iterator( "abcdef" ) + 6 ).numByte() ) );
	EON_TEST( string_iterator, numByte, UTF8_start,
		EON_EQ( 0, string_iterator( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ).numByte() ) );
	EON_TEST( string_iterator, numByte, UTF8_mid,
		EON_EQ( 7, ( string_iterator( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ) + 3 ).numByte() ) );
	EON_TEST( string_iterator, numByte, UTF8_end,
		EON_EQ( 14, ( string_iterator( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ) + 6 ).numByte() ) );

	EON_TEST( string_iterator, codepointSize, empty,
		EON_EQ( 0, string_iterator().codepointSize() ) );
	EON_TEST( string_iterator, codepointSize, byte,
		EON_EQ( 1, string_iterator( "a" ).codepointSize() ) );
	EON_TEST( string_iterator, codepointSize, UTF8_double,
		EON_EQ( 2, string_iterator( u8"\u00D8" ).codepointSize() ) );
	EON_TEST( string_iterator, codepointSize, UTF8_triple,
		EON_EQ( 3, string_iterator( u8"\u20A0" ).codepointSize() ) );
	EON_TEST_2STEP( string_iterator, codepointSize, UTF8_quadruple,
		eon::string input( char_t( 129989 ) ),
		EON_EQ( 4, string_iterator( input.c_str() ).codepointSize() ) );

	EON_TEST( string_iterator, bytesOnly, true,
		EON_TRUE( string_iterator( "abcdef" ).bytesOnly() ) );
	EON_TEST( string_iterator, bytesOnly, false,
		EON_FALSE( string_iterator( u8"abc\u20A0def" ).bytesOnly() ) );

	EON_TEST( string_iterator, validUTF8, true,
		EON_TRUE( string_iterator( "abcdef" ).validUTF8() ) );
	EON_TEST( string_iterator, validUTF8, false,
		EON_FALSE( string_iterator( "abcdef\xF8" ).validUTF8() ) );

	EON_TEST_2STEP( string_iterator, byteData, start,
		const char* source = "abcdef",
		EON_EQ( "abcdef", string_iterator( source ).byteData() ) );
	EON_TEST_2STEP( string_iterator, byteData, mid,
		const char* source = "abcdef",
		EON_EQ( "def", std::string( ( string_iterator( source ) + 3 ).byteData() ) ) );
	EON_TEST_2STEP( string_iterator, byteData, end,
		const char* source = "abcdef",
		EON_EQ( "", std::string( ( string_iterator( source ) + 6 ).byteData() ) ) );

	EON_TEST( string_iterator, numSourceBytes, empty,
		EON_EQ( 0, string_iterator( "" ).numSourceBytes() ) );
	EON_TEST( string_iterator, numSourceBytes, ASCII,
		EON_EQ( 6, string_iterator( "abcdef" ).numSourceBytes() ) );
	EON_TEST( string_iterator, numSourceBytes, UTF8,
		EON_EQ( 14, string_iterator( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ).numSourceBytes() ) );

	EON_TEST( string_iterator, numSourceChars, empty,
		EON_EQ( 0, string_iterator( "" ).numSourceChars() ) );
	EON_TEST( string_iterator, numSourceChars, ASCII,
		EON_EQ( 6, string_iterator( "abcdef" ).numSourceChars() ) );
	EON_TEST( string_iterator, numSourceChars, UTF8,
		EON_EQ( 6, string_iterator( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ).numSourceChars() ) );

	EON_TEST_2STEP( string_iterator, getEnd, empty,
		const char* source = "",
		EON_TRUE( string_iterator( source ) == string_iterator( source ).getEnd() ) );
	EON_TEST_2STEP( string_iterator, getEnd, ASCII,
		const char* source = "abcdef",
		EON_TRUE( string_iterator( source ) + 6 == string_iterator( source ).getEnd() ) );
	EON_TEST_2STEP( string_iterator, getEnd, UTF8,
		const char* source = u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5",
		EON_TRUE( string_iterator( source ) + 6 == string_iterator( source ).getEnd() ) );

	EON_TEST_2STEP( string_iterator, getBegin, empty,
		const char* source = "",
		EON_TRUE( string_iterator( source ) == string_iterator( source ).getBegin() ) );
	EON_TEST_2STEP( string_iterator, getBegin, ASCII,
		const char* source = "abcdef",
		EON_TRUE( string_iterator( source ) == ( string_iterator( source ) + 3 ).getBegin() ) );
	EON_TEST_2STEP( string_iterator, getBegin, UTF8,
		const char* source = u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5",
		EON_TRUE( string_iterator( source ) == ( string_iterator( source ) + 3 ).getBegin() ) );




	string_iterator& string_iterator::operator++() noexcept
	{
		if( atEnd() )
			return *this;
		if( Pos + CodepointSize == SourceEnd )
			return resetToEnd();

		Pos += CodepointSize;	// Next byte position.
		Codepoint = nochar;		// Unknown until we look ahead.
		char32_t state = 0;
		CodepointSize = 1;		// Always at least 1 byte long.
		index_t max_char_len = ( SourceEnd - Pos ) < 4 ? ( SourceEnd - Pos ) : 4;
		for( index_t i = 0; i < max_char_len; ++i, ++CodepointSize )
		{
			if( !_utf8Decode( state, Codepoint, static_cast<unsigned char>( *( Pos + i ) ) ) )
				break;
		}
		++NumChar;
		return *this;
	}
	EON_TEST_3STEP( string_iterator, operator_inc, from_rend,
		string_iterator obj( "abcdef" ),
		obj.resetToREnd(),
		EON_EQ( char_t( 'a' ), *++obj ) );
	EON_TEST_2STEP( string_iterator, operator_inc, from_start,
		string_iterator obj( "abcdef" ),
		EON_EQ( char_t( 'b' ), *++obj ) );
	EON_TEST_3STEP( string_iterator, operator_inc, to_end,
		string_iterator obj( "abcdef" ),
		obj += 5,
		EON_TRUE( ++obj == obj.getEnd() ) );

	EON_TEST_2STEP( string_iterator, operator_dec, to_rend,
		string_iterator obj( "abcdef" ),
		EON_TRUE( (--obj).atREnd() ) );
	EON_TEST_3STEP( string_iterator, operator_dec, to_start,
		string_iterator obj( "abcdef" ),
		++obj,
		EON_EQ( char_t( 'a' ), *--obj ) );
	EON_TEST_3STEP( string_iterator, operator_dec, from_end,
		string_iterator obj( "abcdef" ),
		obj.resetToEnd(),
		EON_EQ( --obj, obj.getBegin() + 5 ) );

	EON_TEST_3STEP( string_iterator, operator_pfinc, from_rend,
		string_iterator obj( "abcdef" ),
		bool first = char_t( 'a' ) == *obj--,
		EON_TRUE( obj.atREnd() && first ) );
	EON_TEST_3STEP( string_iterator, operator_pfinc, from_start,
		string_iterator obj( "abcdef" ),
		bool first = char_t( 'a' ) == *obj++,
		EON_TRUE( char_t( 'b' ) == *obj && first ) );
	EON_TEST_3STEP( string_iterator, operator_pfinc, to_end,
		string_iterator obj( "abcdef" ),
		bool last = char_t( 'f' ) == *( obj += 5 )++,
		EON_TRUE( obj == obj.getEnd() && last ) );

	EON_TEST_3STEP( string_iterator, operator_pfdec, to_rend,
		string_iterator obj( "abcdef" ),
		bool first = char_t( 'a' ) == *obj--,
		EON_TRUE( ( --obj ).atREnd() && first ) );
	EON_TEST_3STEP( string_iterator, operator_pfdec, to_start,
		string_iterator obj( "abcdef" ),
		bool second = char_t( 'b' ) == *( obj += 1 )--,
		EON_TRUE( char_t( 'a' ) == *obj && second ) );
	EON_TEST_3STEP( string_iterator, operator_pfdec, from_end,
		string_iterator obj( "abcdef" ),
		bool at_end = ( obj.resetToEnd()-- ).atEnd(),
		EON_TRUE( char_t( 'f' ) == *obj && at_end ) );

	EON_TEST_2STEP( string_iterator, operator_plusasgn, one,
		string_iterator obj( "abcdef" ),
		EON_EQ( char_t( 'b' ), *( obj += static_cast<index_t>( 1 ) ) ) );
	EON_TEST_2STEP( string_iterator, operator_plusasgn, two,
		string_iterator obj( "abcdef" ),
		EON_EQ( char_t( 'c' ), *( obj += static_cast<index_t>( 2 ) ) ) );
	EON_TEST_2STEP( string_iterator, operator_plusasgn, UTF8_three,
		string_iterator obj( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ),
		EON_EQ( char_t( 8482 ), *( obj += static_cast<index_t>( 3 ) ) ) );

	string_iterator& string_iterator::operator-=( index_t num_chars ) noexcept
	{
		if( atREnd() || num_chars == 0 )
			return *this;
		if( Pos == Source )
			return resetToREnd();
		_backtrack( num_chars );
		_translateCodepoint();
		return *this;
	}
	EON_TEST_3STEP( string_iterator, operator_decasgn, one,
		string_iterator obj( "abcdef" ),
		obj.resetToEnd(),
		EON_EQ( char_t( 'f' ), *( obj -= static_cast<index_t>( 1 ) ) ) );
	EON_TEST_3STEP( string_iterator, operator_decasgn, two,
		string_iterator obj( "abcdef" ),
		obj.resetToEnd(),
		EON_EQ( char_t( 'e' ), *( obj -= static_cast<index_t>( 2 ) ) ) );
	EON_TEST_3STEP( string_iterator, operator_decasgn, UTF8_three,
		string_iterator obj( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5" ),
		obj.resetToEnd(),
		EON_EQ( char_t( 8482 ), *( obj -= static_cast<index_t>( 3 ) ) ) );

	EON_TEST_2STEP( string_iterator, operator_incasgn, long_positive,
		string_iterator obj( "abcdef" ),
		EON_EQ( char_t( 'd' ), *( obj += static_cast<long_t>( 3 ) ) ) );
	EON_TEST_3STEP( string_iterator, operator_incasgn, long_negative,
		string_iterator obj( "abcdef" ),
		obj.resetToEnd(),
		EON_EQ( char_t( 'd' ), *( obj += static_cast<long_t>( -3 ) ) ) );

	EON_TEST_2STEP( string_iterator, operator_incasgn, int_positive,
		string_iterator obj( "abcdef" ),
		EON_EQ( char_t( 'd' ), *( obj += 3 ) ) );
	EON_TEST_3STEP( string_iterator, operator_incasgn, int_negative,
		string_iterator obj( "abcdef" ),
		obj.resetToEnd(),
		EON_EQ( char_t( 'd' ), *( obj += -3 ) ) );

	EON_TEST_3STEP( string_iterator, operator_decasgn, long_positive,
		string_iterator obj( "abcdef" ),
		obj.resetToEnd(),
		EON_EQ( char_t( 'd' ), *( obj += static_cast<long_t>( -3 ) ) ) );
	EON_TEST_2STEP( string_iterator, operator_decasgn, long_negative,
		string_iterator obj( "abcdef" ),
		EON_EQ( char_t( 'd' ), *( obj += static_cast<long_t>( 3 ) ) ) );

	EON_TEST_3STEP( string_iterator, operator_decasgn, int_positive,
		string_iterator obj( "abcdef" ),
		obj.resetToEnd(),
		EON_EQ( char_t( 'd' ), *( obj -= 3 ) ) );
	EON_TEST_2STEP( string_iterator, operator_decasgn, int_negative,
		string_iterator obj( "abcdef" ),
		EON_EQ( char_t( 'd' ), *( obj -= -3 ) ) );

	EON_TEST_2STEP( string_iterator, operator_plus, index,
		const char* source = "abcdef",
		EON_EQ( char_t( 'd' ), *( string_iterator( source ) + static_cast<index_t>( 3 ) ) ) );

	EON_TEST_2STEP( string_iterator, operator_plus, long_positive,
		const char* source = "abcdef",
		EON_EQ( char_t( 'd' ), *( string_iterator( source ) + static_cast<long_t>( 3 ) ) ) );
	EON_TEST_2STEP( string_iterator, operator_plus, long_negative,
		const char* source = "abcdef",
		EON_EQ( char_t( 'd' ), *( string_iterator( source ).resetToEnd() + static_cast<long_t>( -3 ) ) ) );

	EON_TEST_2STEP( string_iterator, operator_plus, int_positive,
		const char* source = "abcdef",
		EON_EQ( char_t( 'd' ), *( string_iterator( source ) + 3 ) ) );
	EON_TEST_2STEP( string_iterator, operator_plus, int_negative,
		const char* source = "abcdef",
		EON_EQ( char_t( 'd' ), *( string_iterator( source ).resetToEnd() + -3 ) ) );

	int64_t operator-( const string_iterator& lhs, const string_iterator& rhs )
	{
		lhs.assertSameSource( rhs );
		auto lhs_chr = static_cast<int64_t>( lhs.atREnd() ? 0 : lhs.atEnd() ? lhs.NumSourceChars : lhs.NumChar );
		auto rhs_chr = static_cast<int64_t>( rhs.atREnd() ? 0 : rhs.atEnd() ? rhs.NumSourceChars : rhs.NumChar );
		return lhs_chr - rhs_chr;
	}
	EON_TEST_2STEP( string_iterator, operator_minus, iterator_ASCII,
		const char* source = "abcdef",
		EON_EQ( 2, ( string_iterator( source ) += 5 ) - ( string_iterator( source ) += 3 ) ) );
	EON_TEST_2STEP( string_iterator, operator_minus, iterator_UTF8,
		const char* source = u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5",
		EON_EQ( 2, ( string_iterator( source ) += 5 ) - ( string_iterator( source ) += 3 ) ) );

	EON_TEST_2STEP( string_iterator, operator_minus, index,
		const char* source = "abcdef",
		EON_EQ( char_t( 'e' ), *( string_iterator( source ).resetToEnd() - static_cast<index_t>( 2 ) ) ) );

	EON_TEST_2STEP( string_iterator, operator_minus, long_positive,
		const char* source = "abcdef",
		EON_EQ( char_t( 'e' ), *( string_iterator( source ).resetToEnd() - static_cast<long_t>( 2 ) ) ) );
	EON_TEST_2STEP( string_iterator, operator_minus, long_negative,
		const char* source = "abcdef",
		EON_EQ( char_t( 'c' ), *( string_iterator( source ) - static_cast<long_t>( -2 ) ) ) );

	EON_TEST_2STEP( string_iterator, operator_minus, int_positive,
		const char* source = "abcdef",
		EON_EQ( char_t( 'e' ), *( string_iterator( source ).resetToEnd() - 2 ) ) );
	EON_TEST_2STEP( string_iterator, operator_minus, int_negative,
		const char* source = "abcdef",
		EON_EQ( char_t( 'c' ), *( string_iterator( source ) - -2 ) ) );




	int string_iterator::compare( const string_iterator& other ) const noexcept
	{
		if( Source != other.Source )
			return Source < other.Source ? -1 : Source > other.Source ? 1 : 0;
		else if( atEnd() )
			return other.atEnd() ? 0 : 1;
		else if( other.atEnd() )
			return -1;
		else if( atREnd() )
			return other.atREnd() ? 0 : -1;
		else if( other.atREnd() )
			return 1;
		else
			return Pos < other.Pos ? -1 : other.Pos < Pos ? 1 : 0;
	}
	EON_TEST_2STEP( string_iterator, compare, diff_source_lt,
		const char* source = "abcdef",
		EON_EQ( -1, string_iterator( source ).compare( string_iterator( source + 1 ) ) ) );
	EON_TEST_2STEP( string_iterator, compare, diff_source_gt,
		const char* source = "abcdef",
		EON_EQ( 1, string_iterator( source + 1 ).compare( string_iterator( source ) ) ) );
	EON_TEST_2STEP( string_iterator, compare, start_eq,
		const char* source = "abcdef",
		EON_EQ( 0, string_iterator( source ).compare( string_iterator( source ) ) ) );
	EON_TEST_2STEP( string_iterator, compare, start_lt,
		const char* source = "abcdef",
		EON_EQ( -1, string_iterator( source ).compare( string_iterator( source ) + 1 ) ) );
	EON_TEST_2STEP( string_iterator, compare, start_gt,
		const char* source = "abcdef",
		EON_EQ( 1, ( string_iterator( source ) + 1 ).compare( string_iterator( source ) ) ) );
	EON_TEST_2STEP( string_iterator, compare, rend_vs_first,
		const char* source = "abcdef",
		EON_EQ( -1, string_iterator( source ).resetToREnd().compare( string_iterator( source ) ) ) );
	EON_TEST_2STEP( string_iterator, compare, first_vs_rend,
		const char* source = "abcdef",
		EON_EQ( 1, string_iterator( source ).compare( string_iterator( source ).resetToREnd() ) ) );
	EON_TEST_2STEP( string_iterator, compare, end_vs_first,
		const char* source = "abcdef",
		EON_EQ( 1, string_iterator( source ).resetToEnd().compare( string_iterator( source ) ) ) );
	EON_TEST_2STEP( string_iterator, compare, first_vs_end,
		const char* source = "abcdef",
		EON_EQ( -1, string_iterator( source ).compare( string_iterator( source ).resetToEnd() ) ) );

	EON_TEST_2STEP( string_iterator, operator_lt, true,
		const char* source = "abcdef",
		EON_TRUE( string_iterator( source ) < string_iterator( source ) + 1 ) );
	EON_TEST_2STEP( string_iterator, operator_lt, false,
		const char* source = "abcdef",
		EON_FALSE( string_iterator( source ) < string_iterator( source ) ) );

	EON_TEST_2STEP( string_iterator, operator_le, true,
		const char* source = "abcdef",
		EON_TRUE( string_iterator( source ) <= string_iterator( source ) ) );
	EON_TEST_2STEP( string_iterator, operator_le, false,
		const char* source = "abcdef",
		EON_FALSE( string_iterator( source ) + 1 <= string_iterator( source ) ) );

	EON_TEST_2STEP( string_iterator, operator_gt, true,
		const char* source = "abcdef",
		EON_TRUE( string_iterator( source ) + 1 > string_iterator( source ) ) );
	EON_TEST_2STEP( string_iterator, operator_gt, false,
		const char* source = "abcdef",
		EON_FALSE( string_iterator( source ) > string_iterator( source ) ) );

	EON_TEST_2STEP( string_iterator, operator_ge, true,
		const char* source = "abcdef",
		EON_TRUE( string_iterator( source ) >= string_iterator( source ) ) );
	EON_TEST_2STEP( string_iterator, operator_ge, false,
		const char* source = "abcdef",
		EON_FALSE( string_iterator( source ) >= string_iterator( source ) + 1 ) );

	EON_TEST_2STEP( string_iterator, operator_eq, true,
		const char* source = "abcdef",
		EON_TRUE( string_iterator( source ) == string_iterator( source ) ) );
	EON_TEST_2STEP( string_iterator, operator_eq, false,
		const char* source = "abcdef",
		EON_FALSE( string_iterator( source ) == string_iterator( source ) + 1 ) );

	EON_TEST_2STEP( string_iterator, operator_ne, true,
		const char* source = "abcdef",
		EON_TRUE( string_iterator( source ) != string_iterator( source ) + 1 ) );
	EON_TEST_2STEP( string_iterator, operator_ne, false,
		const char* source = "abcdef",
		EON_FALSE( string_iterator( source ) != string_iterator( source ) ) );




	index_t string_iterator::bytesToUnicode( const char* start, const char* end, char_t& cp )
	{
		auto size = end - start;
		if( size > 4 )
			size = 4;
		cp = 0;
		char32_t state = 0;
		index_t num = 0;
		for( auto c = start; size > 0; ++c, --size )
		{
			if( !_utf8Decode( state, cp, static_cast<unsigned char>( *c ) ) )
				return c + 1 - start;
		}
		return 0;
	}
	EON_TEST_3STEP( string_iterator, bytesToUnicode, ASCII,
		const char* source = "abcdef",
		char_t codepoint = 0,
		EON_TRUE( string_iterator::bytesToUnicode( source + 3, source + 6, codepoint ) == 1 && codepoint == 'd' ) );
	EON_TEST_3STEP( string_iterator, bytesToUnicode, UTF8,
		const char* source = u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5",
		char_t codepoint = 0,
		EON_TRUE( string_iterator::bytesToUnicode( source + 7, source + 14, codepoint ) == 3 && codepoint == 8482 ) );
	EON_TEST_3STEP( string_iterator, bytesToUnicode, invalid,
		auto source = eonitest::PrimitiveArray<char>( EON_CURLY( 'a', 'b', 'c', char( 197 ) , 'e', 'f' ) ),
		char_t codepoint = 0,
		EON_EQ( 0, string_iterator::bytesToUnicode( source.value() + 3, source.value() + 6, codepoint ) ) );

	index_t string_iterator::unicodeToBytes( char_t cp, uint32_t& bytes )
	{
		if( !isValidCodepoint( cp ) )
			throw InvalidUTF8();
		bytes = 0;
		char* b = (char*)&bytes;

		if( cp < 0x80 )
		{
			*b = static_cast<char>( cp );
			return 1;
		}
		else if( cp < 0x800 )
		{
			*b = cp >> 6 & 0x1F | 0xC0;
			*++b = cp >> 0 & 0x3F | 0x80;
			return 2;
		}
		else if( cp < 0x10000 )
		{
			*b = cp >> 12 & 0x0F | 0xE0;
			*++b = cp >> 6 & 0x3F | 0x80;
			*++b = cp >> 0 & 0x3F | 0x80;
			return 3;
		}
		else if( cp < 0x110000 )
		{
			*b = cp >> 18 & 0x07 | 0xF0;
			*++b = cp >> 12 & 0x3F | 0x80;
			*++b = cp >> 6 & 0x3F | 0x80;
			*++b = cp >> 0 & 0x3F | 0x80;
			return 4;
		}
		else
			throw InvalidUTF8();
	}
	EON_TEST_3STEP( string_iterator, unicodeToBytes, invalid,
		char_t codepoint = 0x140000,
		uint32_t bytes = 0,
		EON_RAISE( string_iterator::unicodeToBytes( codepoint, bytes ), InvalidUTF8 ) );
	EON_TEST_3STEP( string_iterator, unicodeToBytes, ASCII,
		char_t codepoint = 'a',
		uint32_t bytes = 0,
		EON_TRUE( string_iterator::unicodeToBytes( codepoint, bytes ) == 1 && bytes == 'a' ) );
	EON_TEST_3STEP( string_iterator, unicodeToBytes, UTF8,
		char_t codepoint = 8482,
		uint32_t bytes = 0,
		EON_TRUE( string_iterator::unicodeToBytes( codepoint, bytes ) == 3 && bytes == 10650850 ) );

	char32_t string_iterator::utf8Decode( char32_t& state, char32_t& codep, char32_t byte ) noexcept {
		return _utf8Decode( state, codep, byte ); }
	EON_NO_TEST( string_iterator, utf8Decode );

	index_t string_iterator::countUtf8Chars( const char* str, index_t size )
	{
		char32_t cp{ 0 }, state{ 0 };
		index_t num = 0;
		for( auto c = str, end_c = str + size; c != end_c; ++c )
		{
			if( !_utf8Decode( state, cp, static_cast<unsigned char>( *c ) ) )
				++num;
		}
		if( state != UTF8_ACCEPT )
			throw InvalidUTF8( "Not a valid UTF-8 string value" );
		return num;
	}
	EON_TEST( string_iterator, countUtf8Chars, invalid,
		EON_RAISE( string_iterator::countUtf8Chars( "ab\u00C6", 3 ), InvalidUTF8 ) );
	EON_TEST( string_iterator, countUtf8Chars, ASCII,
		EON_EQ( 6, string_iterator::countUtf8Chars( "abcdef", 6 ) ) );
	EON_TEST( string_iterator, countUtf8Chars, UTF8,
		EON_EQ( 6, string_iterator::countUtf8Chars( u8"\u00D8\u20A0\u0153\u2122\u00A9\u00B5", 14 ) ) );



	void string_iterator::_prep( const char* begin, const char* end, const char* pos ) noexcept
	{
		Source = begin; SourceEnd = end; Pos = pos;
		if( NumSourceChars == 0 || ( pos > begin && NumChar == 0 ) )
			_utf8CharacterCount();
		if( Pos < SourceEnd )
			_translateCodepoint();
	}
	EON_NO_TEST( string_iterator, _prep );

	void string_iterator::_utf8CharacterCount() noexcept
	{
		char32_t state = 0, cp = 0;
		const char* cs{ nullptr };
		NumSourceChars = 0;
		for( auto c = Source; c != SourceEnd; ++c )
		{
			if( cs == nullptr )
				cs = c;
			if( !_utf8Decode( state, cp, static_cast<unsigned char>( *c ) ) )
			{
				if( cs == Pos )
					NumChar = NumSourceChars;
				++NumSourceChars;
				cs = nullptr;
			}
		}
		ValidUTF8 = state == UTF8_ACCEPT;
		if( !ValidUTF8 )
			NumSourceChars = SourceEnd - Source;
	}
	EON_NO_TEST( string_iterator, _utf8CharacterCount );

	void string_iterator::_translateCodepoint() noexcept
	{
		Codepoint = nochar;
		if( Pos == SourceEnd )
		{
			CodepointSize = 0;
			return;
		}

		if( bytesOnly() )
		{
			// Optimize for single byte characters
			Codepoint = *Pos;
			CodepointSize = 1;
			return;
		}

		char32_t state = 0;
		index_t max_char_len = ( SourceEnd - Pos ) < 4 ? ( SourceEnd - Pos ) : 4;
		CodepointSize = 1;
		for( index_t i = 0; i < max_char_len; ++i, ++CodepointSize )
		{
			if( !_utf8Decode( state, Codepoint, static_cast<unsigned char>( *( Pos + i ) ) ) )
				return;
		}
	}
	EON_NO_TEST( string_iterator, _translateCodepoint );

	void string_iterator::_backtrack( index_t num_chars ) noexcept
	{
		// Move backward until we find a byte where the most significant bit isn't set or the two most significant are.
		do
		{
			unsigned char c = *--Pos;
			if( c < 0x80 || c >> 6 == 0x03 )
			{
				--num_chars;
				--NumChar;
			}
		} while( num_chars > 0 );
	}
	EON_NO_TEST( string_iterator, _backtrack );

	void string_iterator::_advanceBytes( index_t bytes ) noexcept
	{
		if( bytes < static_cast<index_t>( SourceEnd - Pos ) )
		{
			if( !atREnd() )
				Pos += bytes;
			CodepointSize = 1;
			Codepoint = *Pos;
			NumChar += bytes;
		}
		else
		{
			Pos = SourceEnd;
			CodepointSize = 0;
			Codepoint = nochar;
			NumChar = NumSourceChars;
		}
	}
	EON_NO_TEST( string_iterator, _advanceBytes );
}
