#include "Compare.h"
#include <eoninlinetest/InlineTest.h>
#include <cctype>


namespace eon::strcmp
{
	const byte byte::Cmp;
	EON_TEST( byte, operator_call, null_null,
		EON_EQ( 0, byte::Cmp( '\0', '\0' ) ) );
	EON_TEST( byte, operator_call, null_something,
		EON_EQ( -1, byte::Cmp( 0, 'A' ) ) );
	EON_TEST( byte, operator_call, something_null,
		EON_EQ( 1, byte::Cmp( 'A', 0 ) ) );
	EON_TEST( byte, operator_call, lt,
		EON_EQ( -1, byte::Cmp( 'A', 'B' ) ) );
	EON_TEST( byte, operator_call, eq,
		EON_EQ( 0, byte::Cmp( 'A', 'A' ) ) );
	EON_TEST( byte, operator_call, gt,
		EON_EQ( 1, byte::Cmp( 'B', 'A' ) ) );


	const chr chr::Cmp;
	EON_TEST( chr, operator_call, null_null,
		EON_EQ( 0, chr::Cmp( 0, 0 ) ) );
	EON_TEST( chr, operator_call, null_something,
		EON_EQ( -1, chr::Cmp( 0, 'A' ) ) );
	EON_TEST( chr, operator_call, something_null,
		EON_EQ( 1, chr::Cmp( 'A', 0 ) ) );
	EON_TEST( chr, operator_call, lt,
		EON_EQ( -1, chr::Cmp( 'A', 'B' ) ) );
	EON_TEST( chr, operator_call, eq,
		EON_EQ( 0, chr::Cmp( 'A', 'A' ) ) );
	EON_TEST( chr, operator_call, gt,
		EON_EQ( 1, chr::Cmp( 'B', 'A' ) ) );


	const icase_chr icase_chr::Cmp;
	EON_TEST( icase_chr, operator_call, null_null,
		EON_EQ( 0, icase_chr::Cmp( 0, 0 ) ) );
	EON_TEST( icase_chr, operator_call, null_something,
		EON_EQ( -1, icase_chr::Cmp( 0, 'A' ) ) );
	EON_TEST( icase_chr, operator_call, something_null,
		EON_EQ( 1, icase_chr::Cmp( 'A', 0 ) ) );
	EON_TEST( icase_chr, operator_call, lt,
		EON_EQ( -1, icase_chr::Cmp( 'A', 'b' ) ) );
	EON_TEST( icase_chr, operator_call, eq,
		EON_EQ( 0, icase_chr::Cmp( 'A', 'A' ) ) );
	EON_TEST( icase_chr, operator_call, gt,
		EON_EQ( 1, icase_chr::Cmp( 'B', 'a' ) ) );


	const bytes bytes::Cmp;
	int bytes::operator()( const char* a_start, size_t a_numbytes, const char* b_start, size_t b_numbytes ) const noexcept
	{
		if( a_numbytes == 0 || b_numbytes == 0 )
		{
			if( a_numbytes == b_numbytes )
				return 0;
			else
				return a_numbytes == 0 ? -1 : 1;
		}
		auto cmp = memcmp( a_start, b_start, a_numbytes < b_numbytes ? a_numbytes : b_numbytes );
		if( cmp != 0 || a_numbytes == b_numbytes )
			return cmp;
		else
			return a_numbytes < b_numbytes ? -1 : 1;
	}
	EON_TEST( bytes, operator_call, void_void,
		EON_EQ( 0, bytes::Cmp( 0, 0, 0, 0 ) ) );
	EON_TEST_2STEP( bytes, operator_call, void_empty,
		const char* b{ "abc" },
		EON_EQ( 0, bytes::Cmp( 0, 0, b, 0 ) ) );
	EON_TEST_2STEP( bytes, operator_call, empty_void,
		const char* a{ "abc" },
		EON_EQ( 0, bytes::Cmp( a, 0, 0, 0 ) ) );
	EON_TEST_3STEP( bytes, operator_call, empty_empty,
		const char* a{ "abc" },
		const char* b{ "abc" },
		EON_EQ( 0, bytes::Cmp( a, 0, b, 0 ) ) );
	EON_TEST_3STEP( bytes, operator_call, empty_nonempty,
		const char* a{ "abc" },
		const char* b{ "abc" },
		EON_EQ( -1, bytes::Cmp( a, 0, b, 3 ) ) );
	EON_TEST_3STEP( bytes, operator_call, nonempty_empty,
		const char* a{ "abc" },
		const char* b{ "abc" },
		EON_EQ( 1, bytes::Cmp( a, 3, b, 0 ) ) );
	EON_TEST_3STEP( bytes, operator_call, lt,
		const char* a{ "abc" },
		const char* b{ "abd" },
		EON_EQ( -1, bytes::Cmp( a, 3, b, 3 ) ) );
	EON_TEST_3STEP( bytes, operator_call, eq,
		const char* a{ "abc" },
		const char* b{ "abc" },
		EON_EQ( 0, bytes::Cmp( a, 3, b, 3 ) ) );
	EON_TEST_3STEP( bytes, operator_call, gt,
		const char* a{ "abd" },
		const char* b{ "abc" },
		EON_EQ( 1, bytes::Cmp( a, 3, b, 3 ) ) );



	const utf8 utf8::Cmp;
	int utf8::operator()(
		const string_iterator& a_start,
		const string_iterator& a_end,
		const string_iterator& b_start,
		const string_iterator& b_end ) const noexcept
	{
		string_iterator a_i = a_start;
		string_iterator b_i = b_start;
		int equal = 0;
		for( ; a_i != a_end && b_i != b_end; ++a_i, ++b_i )
		{
			++equal;
			if( *a_i < *b_i )
				return -equal;
			else if( *b_i < *a_i )
				return equal;
		}
		if( a_i != a_end )
			return equal + 1;
		else if( b_i != b_end )
			return -( equal + 1 );
		else
			return 0;
	}
	EON_TEST( utf8, operator_call, void_void,
		EON_EQ( 0, utf8::Cmp( string_iterator(), string_iterator(), string_iterator(), string_iterator() ) ) );
	EON_TEST_2STEP( utf8, operator_call, void_empty,
		string_iterator b( "" ),
		EON_EQ( 0, utf8::Cmp( string_iterator(), string_iterator(), b, b ) ) );
	EON_TEST_2STEP( utf8, operator_call, empty_void,
		string_iterator a( "" ),
		EON_EQ( 0, utf8::Cmp( a, a, string_iterator(), string_iterator() ) ) );
	EON_TEST_3STEP( utf8, operator_call, empty_empty,
		string_iterator a( "abc" ),
		string_iterator b( "abc" ),
		EON_EQ( 0, utf8::Cmp( a, a, b, b ) ) );
	EON_TEST_3STEP( utf8, operator_call, empty_nonempty,
		string_iterator a( "abc" ),
		string_iterator b( "abc" ),
		EON_EQ( -1, utf8::Cmp( a, a, b, b + 3 ) ) );
	EON_TEST_3STEP( utf8, operator_call, nonempty_empty,
		string_iterator a( "abc" ),
		string_iterator b( "abc" ),
		EON_EQ( 1, utf8::Cmp( a, a + 3, b, b ) ) );
	EON_TEST_3STEP( utf8, operator_call, lt,
		string_iterator a( "abc" ),
		string_iterator b( "abd" ),
		EON_EQ( -3, utf8::Cmp( a, a + 3, b, b + 3 ) ) );
	EON_TEST_3STEP( utf8, operator_call, eq,
		string_iterator a( "abc" ),
		string_iterator b( "abc" ),
		EON_EQ( 0, utf8::Cmp( a, a + 3, b, b + 3 ) ) );
	EON_TEST_3STEP( utf8, operator_call, gt,
		string_iterator a( "abd" ),
		string_iterator b( "abc" ),
		EON_EQ( 3, utf8::Cmp( a, a + 3, b, b + 3 ) ) );



	const icase_utf8 icase_utf8::Cmp;
	int icase_utf8::operator()(
		const string_iterator& a_start,
		const string_iterator& a_end,
		const string_iterator& b_start,
		const string_iterator& b_end ) const noexcept
	{
		string_iterator a_i = a_start;
		string_iterator b_i = b_start;
		int equal = 0;
		for( ; a_i != a_end && b_i != b_end; ++a_i, ++b_i )
		{
			++equal;
			if( *a_i != *b_i )
			{
				auto lw_a = Loc->toLower( static_cast<wchar_t>( *a_i ) );
				auto lw_b = Loc->toLower( static_cast<wchar_t>( *b_i ) );
				if( lw_a != lw_b )
					return lw_a < lw_b ? -equal : equal;
			}
		}
		if( a_i != a_end )
			return equal + 1;
		else if( b_i != b_end )
			return -( equal + 1 );
		else
			return 0;
	}
	EON_TEST( icase_utf8, operator_call, void_void,
		EON_EQ( 0, icase_utf8::Cmp( string_iterator(), string_iterator(), string_iterator(), string_iterator() ) ) );
	EON_TEST_2STEP( icase_utf8, operator_call, void_empty,
		string_iterator b( "abc" ),
		EON_EQ( 0, icase_utf8::Cmp( string_iterator(), string_iterator(), b, b ) ) );
	EON_TEST_2STEP( icase_utf8, operator_call, empty_void,
		string_iterator a( "abc" ),
		EON_EQ( 0, icase_utf8::Cmp( a, a, string_iterator(), string_iterator() ) ) );
	EON_TEST_3STEP( icase_utf8, operator_call, empty_empty,
		string_iterator a( "abc" ),
		string_iterator b( "abc" ),
		EON_EQ( 0, icase_utf8::Cmp( a, a, b, b ) ) );
	EON_TEST_3STEP( icase_utf8, operator_call, empty_nonempty,
		string_iterator a( "abc" ),
		string_iterator b( "abc" ),
		EON_EQ( -1, icase_utf8::Cmp( a, a, b, b + 3 ) ) );
	EON_TEST_3STEP( icase_utf8, operator_call, nonempty_empty,
		string_iterator a( "abc" ),
		string_iterator b( "abc" ),
		EON_EQ( 1, icase_utf8::Cmp( a, a + 3, b, b ) ) );
	EON_TEST_3STEP( icase_utf8, operator_call, lt,
		string_iterator a( "abc" ),
		string_iterator b( "Abd" ),
		EON_EQ( -3, icase_utf8::Cmp( a, a + 3, b, b + 3 ) ) );
	EON_TEST_3STEP( icase_utf8, operator_call, eq,
		string_iterator a( "abc" ),
		string_iterator b( "Abc" ),
		EON_EQ( 0, icase_utf8::Cmp( a, a + 3, b, b + 3 ) ) );
	EON_TEST_3STEP( icase_utf8, operator_call, gt,
		string_iterator a( "abd" ),
		string_iterator b( "Abc" ),
		EON_EQ( 3, icase_utf8::Cmp( a, a + 3, b, b + 3 ) ) );
}
