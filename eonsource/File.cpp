#include "File.h"


namespace eon
{
	namespace source
	{
		File::File( const string& name )
		{
			if( name.empty() )
				throw BadName();
			Name = name;
#ifdef EON_WINDOWS
			Data = std::ifstream( Name.stdwstr(), std::ios_base::in | std::ios_base::binary );
#else
			Data.open( Name.stdstr(), std::ios_base::in | std::ios_base::binary );
#endif
			if( !Data )
				throw BadName();

			Data.seekg( 0, std::ifstream::end );
			NumBytes = static_cast<size_t>( Data.tellg() );
			Data.seekg( 0, std::ifstream::beg );
		}


		Pos File::push( Pos start_point, size_t num_characters )
		{
			if( start_point.bytePos() >= NumBytes )
				throw EndOfSource();
			if( start_point.bytePos() != Data.tellg() )
			{
				if( Data.fail() )
					Data.clear();
				Data.seekg( start_point.bytePos(), std::ifstream::beg );
				if( Data.fail() )
					throw EndOfSource();
			}

			Pos pos = start_point;
			for( ; static_cast<size_t>( Data.tellg() ) < NumBytes && num_characters > 0; --num_characters )
			{
				char32_t cp{ 0 };
				char32_t state{ 0 };
				size_t num{ 0 };
				for( auto c = Data.get(); Data; c = Data.get() )
				{
					++num;
					if( !string_iterator::utf8Decode( state, cp, c ) )
						break;
				}
				if( state != UTF8_ACCEPT )
					throw InvalidUTF8( "Not a valid UTF-8 file" );
				if( cp == NewlineChr )
				{
					if( pos.Line == LineLengths.size() )
						LineLengths.push_back( pos.PosOnLine );
					++pos.Line;
					pos.PosOnLine = 0;
				}
				else if( cp == CReturnChr )
					++num_characters;
				else
					++pos.PosOnLine;
				++pos.CharPos;
				pos.BytePos += num;
			}
			return pos;
		}

		Pos File::pull( Pos start_point, Pos end_point, size_t num_characters )
		{
			if( start_point.bytePos() > NumBytes || end_point.bytePos() >= NumBytes )
				throw EndOfSource();
			if( start_point.bytePos() != Data.tellg() )
			{
				if( Data.fail() )
					Data.clear();
				Data.seekg( start_point.bytePos(), std::ifstream::beg );
				if( Data.fail() )
					throw EndOfSource();
			}
			Pos pos = start_point;
			for( ; num_characters > 0; --num_characters )
			{
				char32_t cp{ 0 };
				char* end = (char*)&cp + sizeof( char32_t );
				char* beg = end;
				for( int i = 0; i < 4; ++i )
				{
					Data.seekg( -1, std::ifstream::cur );
					*--beg = static_cast<char>( Data.peek() );
					if( *beg >> 7 == 0 )
						break;
				}
				char32_t chr{ 0 };
				string_iterator::bytesToUnicode( beg, end, chr );
				if( chr == NewlineChr )
				{
					--pos.Line;
					pos.PosOnLine = LineLengths[ pos.Line ];
				}
				else
					--pos.PosOnLine;
				--pos.CharPos;
				pos.BytePos -= ( end - beg );
			}
			return pos;
		}

		char_t File::chr( const Pos& pos ) noexcept
		{
			if( pos.bytePos() >= NumBytes )
				return nochar;
			if( pos.bytePos() != Data.tellg() )
			{
				if( Data.fail() )
					Data.clear();
				Data.seekg( pos.bytePos(), std::ifstream::beg );
				if( Data.fail() )
					return nochar;
			}
			char32_t cp{ 0 };
			char32_t state{ 0 };
			for( auto c = Data.get(); Data; c = Data.get() )
			{
				if( !string_iterator::utf8Decode( state, cp, c ) )
					break;
			}
			return cp;
		}
		int File::byte( index_t pos ) noexcept
		{
			if( pos >= NumBytes )
				return -1;
			if( pos != Data.tellg() )
			{
				if( Data.fail() )
					Data.clear();
				Data.seekg( pos, std::ifstream::beg );
				if( Data.fail() )
					return -1;
			}
			return Data.get();
		}

		string File::str( Pos start, Pos end ) noexcept
		{
			if( end.bytePos() == 0 )
				end.BytePos = NumBytes;
			if( start.bytePos() >= NumBytes || end <= start )
				return string();
			string str;
			for( auto p = start; p != end; p = push( p, 1 ) )
				str += chr( p );
			return str;
		}

		std::string File::bytes( Pos start, Pos end ) noexcept
		{
			if( end.bytePos() == 0 )
				end.BytePos = NumBytes;
			if( start.bytePos() >= NumBytes || end <= start )
				return std::string();
			std::string str;
			for( auto p = start; p != end; p = push( p, 1 ) )
				str += byte( p );
			return str;
		}
	}
}
