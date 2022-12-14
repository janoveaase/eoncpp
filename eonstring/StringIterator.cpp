#include "StringIterator.h"
#include <cctype>


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
	inline char32_t _utf8Decode( char32_t& state, char32_t& codep, char32_t byte )
	{
		char32_t type = utf8d[ byte ];
		codep = ( state != UTF8_ACCEPT ) ? ( byte & 0x3fu ) | ( codep << 6 ) : ( 0xff >> type ) & ( byte );
		state = utf8d[ 256 + state * 16 + type ];
		return state;
	}



	string_iterator::string_iterator( const string_iterator& source, const char* pos, index_t num_char ) noexcept
	{
		Source = source.Source;
		SourceEnd = source.SourceEnd;
		NumChars = source.NumChars;
		ValidUTF8 = source.ValidUTF8;
		Pos = pos;
		NumChar = num_char;
		if( NumChar == 0 && Pos != Source )
			_utf8CharacterCount();
		_translateCodepoint();
	}

	string_iterator& string_iterator::reset() noexcept
	{
		Source = nullptr;
		SourceEnd = nullptr;
		Pos = nullptr;
		Codepoint = nochar;
		CodepointSize = 0;
		NumChar = 0;
		NumChars = 0;
		return *this;
	}
	string_iterator& string_iterator::resetToBegin() noexcept
	{
		if( Pos != Source )
		{
			Pos = Source;
			CodepointSize = 0;
			NumChar = 0;
			_translateCodepoint();
		}
		return *this;
	}
	string_iterator& string_iterator::resetToEnd() noexcept
	{
		if( Pos != SourceEnd )
		{
			Pos = SourceEnd;
			Codepoint = nochar;
			CodepointSize = 0;
			NumChar = NumChars;
		}
		return *this;
	}



	string_iterator& string_iterator::operator++() noexcept
	{
		// Cannot increment if at the end
		if( CodepointSize == 0 && Pos > Source )
			return *this;

		// Next byte position
		Pos += CodepointSize;
		Codepoint = nochar;		// Unknown at this time

		// End of string?
		if( Pos == SourceEnd )
		{
			CodepointSize = 0;
			NumChar = NumChars;
			return *this;
		}

		// Get the code point
		char32_t state = 0;
		CodepointSize = 1;			// Always at least 1 byte long
		index_t max_char_len = ( SourceEnd - Pos ) < 4 ? ( SourceEnd - Pos ) : 4;
		for( index_t i = 0; i < max_char_len; ++i, ++CodepointSize )
		{
			if( !_utf8Decode( state, Codepoint, static_cast<unsigned char>( *( Pos + i ) ) ) )
				break;
		}
		++NumChar;
		return *this;
	}

	string_iterator& string_iterator::operator-=( index_t chars ) noexcept
	{
		// Cannot subtract if at the beginning
		if( CodepointSize == 0 && Pos == Source )
			return *this;

		if( chars == 0 )
			return *this;

		// If at start, we stay at this position, but invalidate the iterator
		if( Pos == Source )
		{
			CodepointSize = 0;
			Codepoint = nochar;
			return *this;
		}

		// We move backwards until we find a byte where the most
		// significant bit isn't set, or the two most significant are.
		do
		{
			unsigned char c = *--Pos;
			if( c < 0x80 || c >> 6 == 0x03 )
			{
				--chars;
				--NumChar;
			}
		} while( chars > 0 );

		_translateCodepoint();
		return *this;
	}

	int64_t operator-( const string_iterator& lhs, const string_iterator& rhs )
	{
		lhs.assertCompatibleWith( rhs );
		if( lhs.atREnd() )
		{
			if( rhs.atREnd() )
				return 0;
			else
				return ( lhs.NumChar - 1 ) - rhs.NumChar;
		}
		else if( rhs.atREnd() )
			return lhs.NumChar - ( rhs.NumChar - 1 );
		else
			return lhs.NumChar - rhs.NumChar;
	}




	int string_iterator::compare( const string_iterator& other ) const noexcept
	{
		if( Source != other.Source )
			return Source < other.Source ? -1 : Source > other.Source ? 1 : 0;
		else if( atREnd() )
			return other.atREnd() ? 0 : -1;
		else if( other.atREnd() )
			return 1;
		else
			return Pos < other.Pos ? -1 : other.Pos < Pos ? 1 : 0;
	}




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

	char32_t string_iterator::utf8Decode( char32_t& state, char32_t& codep, char32_t byte ) noexcept
	{
		return _utf8Decode( state, codep, byte );
	}

	index_t string_iterator::countUtf8Chars( const char* str, index_t size )
	{
		char32_t cp{ 0 };
		char32_t state{ 0 };
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



	void string_iterator::_prep( const char* begin, const char* end, const char* pos ) noexcept
	{
		Source = begin; SourceEnd = end; Pos = pos;
		if( NumChars == 0 || ( pos > begin && NumChar == 0 ) )
			_utf8CharacterCount();
		if( Pos < SourceEnd )
			_translateCodepoint();
	}

	void string_iterator::_utf8CharacterCount() noexcept
	{
		char32_t state = 0, cp = 0;
		const char* cs{ nullptr };
		NumChars = 0;
		for( auto c = Source; c != SourceEnd; ++c )
		{
			if( cs == nullptr )
				cs = c;
			if( !_utf8Decode( state, cp, static_cast<unsigned char>( *c ) ) )
			{
				if( cs == Pos )
					NumChar = NumChars;
				++NumChars;
				cs = nullptr;
			}
		}
		ValidUTF8 = state == UTF8_ACCEPT;
		if( !ValidUTF8 )
			NumChars = SourceEnd - Source;
	}

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
			NumChar = NumChars;
		}
	}
	void string_iterator::_retreatBytes( index_t bytes ) noexcept
	{
		if( bytes < static_cast<index_t>( Pos - Source ) )
		{
			Pos -= bytes;
			CodepointSize = 1;
			Codepoint = *Pos;
			NumChar += bytes;
		}
		else
		{
			Pos = SourceEnd;
			CodepointSize = 0;
			Codepoint = nochar;
			NumChar = NumChars;
		}
	}
}
