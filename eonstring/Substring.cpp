#include "Substring.h"
#include <eoninlinetest/InlineTest.h>
#include <cctype>


namespace eon
{
	EON_TEST_3STEP( substring, substring, copy,
		const char* source = "abcdef",
		substring other( source ),
		EON_TRUE( other == substring( other ) ) );

	EON_TEST_3STEP( substring, substring, iterators,
		const char* source = "abcdef",
		substring obj( string_iterator( source ), ( string_iterator( source ) + 6 ) ),
		EON_TRUE( obj == substring( source ) ) );

	EON_TEST_3STEP( substring, substring, iterator,
		const char* source = "abcdef",
		substring obj( string_iterator( source ) + 3 ),
		EON_TRUE( obj.empty() && *obj.begin() == 'd' ) );

	EON_TEST( substring, substring, stdstring,
		EON_EQ( 6, substring( std::string( "abcdef" ) ).numChars() ) );

	EON_TEST( substring, substring, cstring,
		EON_EQ( 6, substring( "abcdef" ).numChars() ) );




	EON_TEST_3STEP( substring, clear, basic,
		substring obj( "abcdef" ),
		obj.clear(),
		EON_TRUE( obj.empty() ) );




	EON_NO_TEST( substring, hash32 );
	EON_NO_TEST( substring, hash64 );
	EON_NO_TEST( substring, hash );




	EON_NO_TEST( substring, begin );
	EON_NO_TEST( substring, cbegin );
	EON_NO_TEST( substring, end );
	EON_NO_TEST( substring, cend );

	EON_TEST_2STEP( substring, last, empty,
		substring obj( "" ),
		EON_EQ( obj.end(), obj.last() ) );
	EON_TEST( substring, last, nonempty,
		EON_EQ( 'f', *substring( "abcdef" ).last() ) );




	EON_TEST( substring, splitSequential, substr_empty,
		EON_EQ( 0, substring( "" ).splitSequential<std::list<string>>( substring( "::" ) ).size() ) );
	EON_TEST( substring, splitSequential, substr_singleton,
		EON_EQ( 1, substring( "one" ).splitSequential<std::list<string>>( substring( "::" ) ).size() ) );
	EON_TEST( substring, splitSequential, substr_multiple,
		EON_EQ( 3, substring( "one::two::three" ).splitSequential<std::list<string>>( substring( "::" ) ).size() ) );
	EON_TEST( substring, splitSequential, substr_empty_first,
		EON_EQ( 3, substring( "::two::three" ).splitSequential<std::list<string>>( substring( "::" ) ).size() ) );
	EON_TEST( substring, splitSequential, substr_empty_middle,
		EON_EQ( 3, substring( "one::::three" ).splitSequential<std::list<string>>( substring( "::" ) ).size() ) );
	EON_TEST( substring, splitSequential, substr_empty_last,
		EON_EQ( 3, substring( "one::two::" ).splitSequential<std::list<string>>( substring( "::" ) ).size() ) );

	EON_TEST( substring, splitSequential, char_t_empty,
		EON_EQ( 0, substring( "" ).splitSequential<std::list<string>>( ',' ).size() ) );
	EON_TEST( substring, splitSequential, char_t_singleton,
		EON_EQ( 1, substring( "one" ).splitSequential<std::list<string>>( ',' ).size() ) );
	EON_TEST( substring, splitSequential, char_t_multiple,
		EON_EQ( 3, substring( "one,two,three" ).splitSequential<std::list<string>>( ',' ).size() ) );
	EON_TEST( substring, splitSequential, char_t_empty_first,
		EON_EQ( 3, substring( ",two,three" ).splitSequential<std::list<string>>( ',' ).size() ) );
	EON_TEST( substring, splitSequential, char_t_empty_middle,
		EON_EQ( 3, substring( "one,,three" ).splitSequential<std::list<string>>( ',' ).size() ) );
	EON_TEST( substring, splitSequential, char_t_empty_last,
		EON_EQ( 3, substring( "one,two," ).splitSequential<std::list<string>>( ',' ).size() ) );

	EON_TEST( substring, splitNonSequential, substr_empty,
		EON_EQ( 0, substring( "" ).splitNonSequential<std::set<string>>( substring( "::" ) ).size() ) );
	EON_TEST( substring, splitNonSequential, substr_singleton,
		EON_EQ( 1, substring( "one" ).splitNonSequential<std::set<string>>( substring( "::" ) ).size() ) );
	EON_TEST( substring, splitNonSequential, substr_multiple,
		EON_EQ( 3, substring( "one::two::three" ).splitNonSequential<std::set<string>>( substring( "::" ) ).size() ) );
	EON_TEST( substring, splitNonSequential, substr_empty_first,
		EON_EQ( 3, substring( "::two::three" ).splitNonSequential<std::set<string>>( substring( "::" ) ).size() ) );
	EON_TEST( substring, splitNonSequential, substr_empty_middle,
		EON_EQ( 3, substring( "one::::three" ).splitNonSequential<std::set<string>>( substring( "::" ) ).size() ) );
	EON_TEST( substring, splitNonSequential, substr_empty_last,
		EON_EQ( 3, substring( "one::two::" ).splitNonSequential<std::set<string>>( substring( "::" ) ).size() ) );

	EON_TEST( substring, splitNonSequential, char_t_empty,
		EON_EQ( 0, substring( "" ).splitNonSequential<std::set<string>>( ',' ).size() ) );
	EON_TEST( substring, splitNonSequential, char_t_singleton,
		EON_EQ( 1, substring( "one" ).splitNonSequential<std::set<string>>( ',' ).size() ) );
	EON_TEST( substring, splitNonSequential, char_t_multiple,
		EON_EQ( 3, substring( "one,two,three" ).splitNonSequential<std::set<string>>( ',' ).size() ) );
	EON_TEST( substring, splitNonSequential, char_t_empty_first,
		EON_EQ( 3, substring( ",two,three" ).splitNonSequential<std::set<string>>( ',' ).size() ) );
	EON_TEST( substring, splitNonSequential, char_t_empty_middle,
		EON_EQ( 3, substring( "one,,three" ).splitNonSequential<std::set<string>>( ',' ).size() ) );
	EON_TEST( substring, splitNonSequential, char_t_empty_last,
		EON_EQ( 3, substring( "one,two," ).splitNonSequential<std::set<string>>( ',' ).size() ) );




	EON_TEST( substring, count, subst_void_void,
		EON_EQ( 0, substring().count( substring() ) ) );
	EON_TEST( substring, count, subst_void_empty,
		EON_EQ( 0, substring().count( substring( "" ) ) ) );
	EON_TEST( substring, count, subst_empty_void,
		EON_EQ( 0, substring( "" ).count( substring() ) ) );
	EON_TEST( substring, count, subst_empty_empty,
		EON_EQ( 0, substring( "" ).count( substring( "" ) ) ) );
	EON_TEST( substring, count, subst_ASCII_one_in_one,
		EON_EQ( 1, substring( "ab" ).count( substring( "ab" ) ) ) );
	EON_TEST( substring, count, subst_ASCII_all_in_all,
		EON_EQ( 5, substring( "ababababab" ).count( substring( "ab" ) ) ) );
	EON_TEST( substring, count, subst_ASCII_one_in_many,
		EON_EQ( 1, substring( "abcdefgh" ).count( substring( "ab" ) ) ) );
	EON_TEST( substring, count, subst_ASCII_many_in_many,
		EON_EQ( 4, substring( "abcdabefabghab" ).count( substring( "ab" ) ) ) );
	EON_TEST( substring, count, subst_UTF8_one_in_one,
		EON_EQ( 1, substring( u8"Ø€" ).count( substring( u8"Ø€" ) ) ) );
	EON_TEST( substring, count, subst_UTF8_all_in_all,
		EON_EQ( 5, substring( u8"Ø€Ø€Ø€Ø€Ø€" ).count( substring( u8"Ø€" ) ) ) );
	EON_TEST( substring, count, subst_UTF8_one_in_many,
		EON_EQ( 1, substring( u8"Ø€Œ™©µ" ).count( substring( u8"Ø€" ) ) ) );
	EON_TEST( substring, count, subst_UTF8_many_in_many,
		EON_EQ( 4, substring( u8"Ø€Œ™Ø€©µØ€Ø€" ).count( substring( u8"Ø€" ) ) ) );

	EON_TEST( substring, count, char_t_ASCII_one_in_one,
		EON_EQ( 1, substring( "a" ).count( char_t( 'a' ) ) ) );
	EON_TEST( substring, count, char_t_ASCII_all_in_all,
		EON_EQ( 5, substring( "aaaaa" ).count( char_t( 'a' ) ) ) );
	EON_TEST( substring, count, char_t_ASCII_one_in_many,
		EON_EQ( 1, substring( "abcdefgh" ).count( char_t( 'a' ) ) ) );
	EON_TEST( substring, count, char_t_ASCII_many_in_many,
		EON_EQ( 6, substring( "abadabaeafa" ).count( char_t( 'a' ) ) ) );
	EON_TEST( substring, count, char_t_UTF8_one_in_one,
		EON_EQ( 1, substring( u8"Ø" ).count( char_t( 216 ) ) ) );
	EON_TEST( substring, count, char_t_UTF8_all_in_all,
		EON_EQ( 5, substring( u8"ØØØØØ" ).count( char_t( 216 ) ) ) );
	EON_TEST( substring, count, char_t_UTF8_one_in_many,
		EON_EQ( 1, substring( u8"Ø€Œ™©µ" ).count( char_t( 216 ) ) ) );
	EON_TEST( substring, count, char_t_UTF8_many_in_many,
		EON_EQ( 6, substring( u8"Ø€ØŒØ™Ø©ØµØ" ).count( char_t( 216 ) ) ) );




	substring substring::_optimizedFindFirst( const substring& to_find ) const noexcept
	{
		// If the sub-string isn't bytes-only, we know it cannot exist within the source!
		if( !to_find.begin().bytesOnly() )
			return substring( End.getEnd() );

		auto found = _findFirst( Beg.Pos, numBytes(), to_find.begin().Pos, to_find.numBytes() );
		if( found != nullptr )
		{
			return substring( string_iterator( Beg, found, found - Beg.Source ),
				string_iterator( Beg, found + to_find.numBytes(), ( found + to_find.numBytes() ) - Beg.Source ) );
		}
		else
			return substring( End.getEnd() );
	}

	substring substring::_optimizedFindFirst( char_t to_find ) const noexcept
	{
		// If the codepoint isn't ASCII, then only accept search if 'this' also isn't UTF-8!
		if( to_find > 127 && Beg.ValidUTF8 )
			return substring( End.getEnd() );

		auto found = _findFirst( Beg.Pos, numBytes(), static_cast<char>( to_find ) );
		if( found != nullptr )
			return substring( string_iterator( Beg, found, found - Beg.Source ),
				string_iterator( Beg, found + 1, ( found + 1 ) - Beg.Source ) );
		else
			return substring( End.getEnd() );
	}

	substring substring::_optimizedFindLast( const substring& to_find ) const noexcept
	{
		// If the sub-string isn't bytes-only, we know it cannot exist within the source!
		if( !to_find.begin().bytesOnly() )
			return substring( End.getEnd() );

		auto found = _findLast( Beg.Pos, numBytes(), to_find.end().Pos, to_find.numBytes() );
		if( found != nullptr )
		{
			return substring(
				string_iterator( Beg, found, found - Beg.Source ),
				string_iterator( Beg, found + to_find.numBytes(), ( found + to_find.numBytes() ) - Beg.Source ) );
		}
		else
			return substring( End.getEnd() );
	}

	substring substring::_optimizedFindLast( char_t to_find ) const noexcept
	{
		// If the codepoint isn't ASCII, then only accept search if 'this' also isn't UTF-8!
		if( to_find > 127 && Beg.ValidUTF8 )
			return substring( End.getEnd() );

		auto found = _findLast( Beg.Pos, numBytes(), static_cast<char>( to_find ) );
		if( found != nullptr )
		{
			if( found < Beg.SourceEnd )
				return substring( string_iterator( Beg, found, found - Beg.Source ),
					string_iterator( Beg, found + 1, ( found + 1 ) - Beg.Source ) );
			else
				return substring( string_iterator( Beg, found,
					found - Beg.Source ), End );
		}
		else
			return substring( End.getEnd() );
	}

	void substring::_bypassSection( char_t start_sect, char_t end_sect, string_iterator& i ) const
	{
		int sections = 1;
		bool escaped = false;
		for( ++i; i != end(); ++i )
		{
			if( escaped )
				escaped = false;
			else
			{
				if( start_sect != end_sect )
				{
					if( *i == start_sect )
						++sections;
				}
				else
				{
					if( *i == BackSlashChr )
						escaped = true;
				}
				if( *i == end_sect )
				{
					if( --sections == 0 )
						break;
				}
			}
		}
	}

	const char* substring::_findFirst( const char* source, index_t source_size,
		const char* substr, index_t substr_size ) const noexcept
	{
		if( substr_size > source_size )
			return nullptr;
		const char* end = source + source_size - ( substr_size - 1 );
		for( auto c = _findFirst( source, end - source, *substr );
			c != nullptr && c != end;
			c = _findFirst( c + 1, end - ( c + 1 ), *substr ) )
		{
			if( memcmp( c + 1, substr + 1, substr_size - 1 ) == 0 )
				return c;
		}
		return nullptr;
	}
	const char* substring::_findLast( const char* str, index_t str_size, char chr ) noexcept
	{
		for( auto c = str, end = str - str_size; c != end; --c )
		{
			if( *c == chr )
				return c;
		}
		return nullptr;
	}
	const char* substring::_findLast(
		const char* source, index_t source_size, const char* substr, index_t substr_size ) const noexcept
	{
		const char* rbeg = source - substr_size + 1;
		const char* rend = source - source_size;
		for( auto c = rbeg; c != rend; --c )
		{
			if( *c != *substr )
				continue;
			if( source_size == 1 )
				return c;
			if( memcmp( c + 1, substr + 1, substr_size - 1 ) == 0 )
				return c;
		}
		return nullptr;
	}

	std::string substring::_getReverse() const
	{
		std::string revs; revs.reserve( numBytes() );
		uint32_t bytes{ 0 };
		for( auto i = begin(); i != end(); --i )
		{
			index_t size = string_iterator::unicodeToBytes( *i, bytes );
			revs += std::string( (const char*)&bytes, size );
		}
		return revs;
	}
}
