#include "String.h"
#include <eoninlinetest/InlineTest.h>
#include <cctype>
#include <regex>
#include <unordered_map>
#include <limits.h>


namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// String Construction
	//

	EON_TEST( string, string, default,
		EON_EQ( "", string() ) );

	EON_TEST_3STEP( string, string, move_1,
		string a( "value" ),
		string b( std::move( a ) ),
		EON_EQ( "", a ) );
	EON_TEST_3STEP( string, string, move_2,
		string a( "value" ),
		string b( std::move( a ) ),
		EON_EQ( "value", b ) );

	EON_TEST_3STEP( string, string, copy_1,
		string a( "value" ),
		string b( a ),
		EON_EQ( "value", a ) );
	EON_TEST_3STEP( string, string, copy_2,
		string a( "value" ),
		string b( a ),
		EON_EQ( "value", b ) );

	EON_TEST_3STEP( string, string, iterator_copy,
		string a( "one two three" ),
		string b( a.begin() + 3, a.begin() + 8 ),
		EON_EQ( " two ", b ) );

	EON_TEST_3STEP( string, string, substring_copy,
		substring a( "one two three" ),
		string b( a ),
		EON_EQ( "one two three", b ) );
	EON_TEST_3STEP( string, string, substring_highToLow,
		substring a( "one two three" ),
		string b( a.highToLow() ),
		EON_EQ( "eerht owt eno", b ) );
	EON_TEST_3STEP( string, string, substring_invalid,
		auto source = eonitest::PrimitiveArray<char>( EON_CURLY( 'o', 'O', char( 243 ), '0' ) ),
		substring a( source.value() ),
		EON_RAISE( string b( a ), InvalidUTF8 ) );

	EON_TEST_3STEP( string, string, stdstring_copy_1,
		std::string a( "one two three" ),
		string b( a ),
		EON_EQ( "one two three", a ) );
	EON_TEST_3STEP( string, string, stdstring_copy_2,
		std::string a( "one two three" ),
		string b( a ),
		EON_EQ( "one two three", b ) );
	EON_TEST_2STEP( string, string, stdstring_invalid,
		std::string a( EON_CURLY( 'o', 'O', char( 243 ), '0' ) ),
		EON_RAISE( string b( a ), InvalidUTF8 ) );

	EON_TEST_3STEP( string, string, stdstring_move_1,
		std::string a( "one two three" ),
		string b( std::move( a ) ),
		EON_EQ( "", a ) );
	EON_TEST_3STEP( string, string, stdstring_move_2,
		std::string a( "one two three" ),
		string b( std::move( a ) ),
		EON_EQ( "one two three", b ) );

	EON_TEST_3STEP( string, string, cstring_copy,
		const char* a = "one two three",
		string b( a ),
		EON_EQ( "one two three", b ) );
	EON_TEST_2STEP( string, string, cstring_invalid,
		auto source = eonitest::PrimitiveArray<char>( EON_CURLY( 'o', 'O', char( 243 ), '0' ) ),
		EON_RAISE( string b( source.value() ), InvalidUTF8 ) );

	EON_TEST_3STEP( string, string, cstring_copy_length,
		const char* a = "one two three",
		string b( a, 7 ),
		EON_EQ( "one two", b ) );

	class _ByteParser
	{
	public:
		inline _ByteParser( const char* input, index_t input_length ) noexcept {
			Beg = input; End = input + input_length;
		}

		inline operator bool() const noexcept { return Beg < End; }
		inline bool fillBytes() noexcept
		{
			State = 0;
			CodePoint = 0;
			const char* beg = Beg;
			for( int i = 0; i < 4 && *this; ++i, ++beg )
			{
				if( !string_iterator::utf8Decode( State, CodePoint, static_cast<unsigned char>( *beg ) ) )
				{
					Beg = beg + 1;
					return true;
				}
				Bytes[ i ] = static_cast<unsigned char>( *beg );
			}
			++Beg;
			return false;
		}
		inline char_t utf8_char() const noexcept { return CodePoint; }
		inline void operator++() noexcept { ++Beg; }
	private:
		const char* Beg{ nullptr };
		const char* End{ nullptr };
		char_t CodePoint{ 0 }, State{ 0 };
		unsigned char Bytes[ 4 ]{ 0, 0, 0, 0 };
	};

	string::string( const char* input, index_t input_length, string non_utf8_substitution ) noexcept
	{
		_ByteParser parser( input, input_length );
		while( parser )
		{
			if( parser.fillBytes() )
				*this += parser.utf8_char();
			else
				*this += non_utf8_substitution;
		}
	}
	EON_TEST_3STEP( string, string, non_utf8_substitution,
		auto source = eonitest::PrimitiveArray<char>(
			EON_CURLY( 'o', 'n', char( 233 ), ' ', 't', 'w', char( 243 ), ' ', 't', 'h', 'r', char( 232 ), char( 233 ) ) ),
		string b( source.value(), 12, "X" ),
		EON_EQ( "onX twX thrX", b ) );

	EON_TEST( string, string, char_t_ASCII,
		EON_EQ( "x", string( char_t( 'x' ) ) ) );
	EON_TEST( string, string, char_t_UTF8,
		EON_EQ( u8"é", string( char_t( 0xE9 ) ) ) );

	EON_TEST( string, string, char,
		EON_EQ( "x", string( 'x' ) ) );

	EON_TEST( string, string, unsigned_char,
		EON_EQ( "y", string( (unsigned char)'y' ) ) );

	EON_TEST_2STEP( string, string, init_list_char_t,
		string obj( EON_CURLY( char_t( 'a' ), char_t( 'b' ), char_t( 'C' ) ) ),
		EON_EQ( "abC", obj ) );

	EON_TEST_2STEP( string, string, vector_char_t,
		string obj( std::vector<char_t>( EON_CURLY( char_t( 'A' ), char_t( 'B' ), char_t( 'c' ) ) ) ),
		EON_EQ( "ABc", obj ) );

	EON_TEST_2STEP( string, string, init_list_char,
		string obj( EON_CURLY( 'a', 'b', 'C' ) ),
		EON_EQ( "abC", obj ) );

	EON_TEST_2STEP( string, string, vector_char,
		string obj( std::vector<char_t>( EON_CURLY( 'A', 'B', 'c' ) ) ),
		EON_EQ( "ABc", obj ) );

	EON_TEST( string, string, char_t_copies,
		EON_EQ( "aaaaa", string( static_cast<index_t>( 5 ), char_t( 'a' ) ) ) );

	EON_TEST( string, string, string_copies,
		EON_EQ( "ababababab", string( static_cast<index_t>( 5 ), string( "ab" ) ) ) );

	EON_TEST( string, string, stdstring_copies,
		EON_EQ( "ababababab", string( static_cast<index_t>( 5 ), std::string( "ab" ) ) ) );

	EON_TEST( string, string, substring_copies,
		EON_EQ( "ababababab", string( static_cast<index_t>( 5 ), substring( "ab" ) ) ) );

	EON_TEST( string, string, bool_true,
		EON_EQ( "true", string( true ) ) );
	EON_TEST( string, string, false,
		EON_EQ( "false", string( false ) ) );

	EON_TEST( string, string, int_t_zero,
		EON_EQ( "0", string( int_t( 0 ) ) ) );
	EON_TEST( string, string, int_t_positive,
		EON_EQ( "99", string( int_t( 99 ) ) ) );
	EON_TEST( string, string, int_t_negative,
		EON_EQ( "-12345", string( int_t( -12345 ) ) ) );
	EON_TEST( string, string, int_t_max,
		EON_EQ( "2147483647", string( int_t( INT_MAX ) ) ) );
	EON_TEST( string, string, int_t_min,
		EON_EQ( "-2147483648", string( int_t( INT_MIN ) ) ) );

	EON_TEST( string, string, short_t_zero,
		EON_EQ( "0", string( short_t( 0 ) ) ) );
	EON_TEST( string, string, short_t_positive,
		EON_EQ( "99", string( short_t( 99 ) ) ) );
	EON_TEST( string, string, short_t_negative,
		EON_EQ( "-12345", string( short_t( -12345 ) ) ) );
	EON_TEST( string, string, short_t_max,
		EON_EQ( "32767", string( short_t( INT16_MAX ) ) ) );
	EON_TEST( string, string, short_t_min,
		EON_EQ( "-32768", string( short_t( INT16_MIN ) ) ) );

	EON_TEST( string, string, long_t_zero,
		EON_EQ( "0", string( long_t( 0 ) ) ) );
	EON_TEST( string, string, long_t_positive,
		EON_EQ( "99", string( long_t( 99 ) ) ) );
	EON_TEST( string, string, long_t_negative,
		EON_EQ( "-12345", string( long_t( -12345 ) ) ) );
	EON_TEST( string, string, long_t_max,
		EON_EQ( "9223372036854775807", string( long_t( INT64_MAX ) ) ) );
	EON_TEST( string, string, long_t_min,
		EON_EQ( "-9223372036854775808", string( long_t( INT64_MIN ) ) ) );

	EON_TEST( string, string, index_t_zero,
		EON_EQ( "0", string( index_t( 0 ) ) ) );
	EON_TEST( string, string, index_t_positive,
		EON_EQ( "99", string( index_t( 99 ) ) ) );
#ifdef EON_64BIT
	EON_TEST( string, string, index_t_max,
		EON_EQ( "18446744073709551615", string( index_t( SIZE_MAX ) ) ) );
#else
	EON_TEST( string, string, index_t_max,
		EON_EQ( "4294967295", string( index_t( SIZE_MAX ) ) ) );
#endif

	EON_TEST( string, string, flt_t_zero,
		EON_EQ( "0.0", string( flt_t( 0.0 ) ) ) );
	EON_TEST( string, string, flt_t_positive,
		EON_RANGE( "99.98", string( flt_t( 99.99 ) ), "99.999" ) );
	EON_TEST( string, string, flt_t_negative,
		EON_RANGE( "-123.44", string( flt_t( -123.45 ) ), "-123.46" ) );

	EON_TEST( string, string, low_t_zero,
		EON_EQ( "0.0", string( low_t( 0.0 ) ) ) );
	EON_TEST( string, string, low_t_positive,
		EON_RANGE( "99.98", string( low_t( 99.99 ) ), "99.999" ) );
	EON_TEST( string, string, low_t_negative,
		EON_RANGE( "-123.44", string( low_t( -123.45 ) ), "-123.455" ) );

	EON_TEST( string, string, high_t_zero,
		EON_EQ( "0.0", string( high_t( 0.0 ) ) ) );
	EON_TEST( string, string, high_t_positive,
		EON_RANGE( "99.98", string( high_t( 99.99 ) ), "99.999" ) );
	EON_TEST( string, string, high_t_negative,
		EON_RANGE( "-123.44", string( high_t( -123.45 ) ), "-123.455" ) );

	EON_TEST( string, string, ascii_only,
		EON_EQ( "abcdef", string( std::string( "abcdef" ), true ) ) );
}
