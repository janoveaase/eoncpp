#include "File.h"


namespace eon
{
	namespace source
	{
		File::File( const string& name ) : Raw( name )
		{
#ifdef EON_WINDOWS
			Data = std::ifstream( sourceName().stdwstr(), std::ios_base::in | std::ios_base::binary );
#else
			Data.open( sourceName().stdstr(), std::ios_base::in | std::ios_base::binary );
#endif
			if( !Data )
				throw BadName();

			Data.seekg( 0, std::ifstream::end );
			NumBytes = static_cast<size_t>( Data.tellg() );
			Data.seekg( 0, std::ifstream::beg );
		}
		EON_TEST_3STEP_SANDBOX( File, File, name,
			saveFile( "source.txt", "one two three" ),
			File obj( ( sandboxDir() / "source.txt" ).string() ),
			EON_EQ( ( sandboxDir() / "source.txt" ).string(), obj.sourceName() ) );


		File::~File()
		{
			if( Data )
			{
				try
				{
					Data.close();
				}
				catch( ... )
				{
					// Nothing we can do - except not throw anything from this destructor!
				}
			}
		}




		EON_TEST_2STEP_SANDBOX( File, numBytesInSource, empty,
			saveFile( "source.txt", "" ),
			EON_EQ( 0, File( ( sandboxDir() / "source.txt" ).string() ).numBytesInSource() ) );
		EON_TEST_2STEP_SANDBOX( File, numBytesInSource, non_empty,
			saveFile( "source.txt", "one two three" ),
			EON_EQ( 13, File( ( sandboxDir() / "source.txt" ).string() ).numBytesInSource() ) );




		Pos File::getPosAtOffset( Pos base_position, int offset_chars )
		{
			if( offset_chars < 0 )
				_backward( base_position, -offset_chars );
			else if( offset_chars > 0 )
				_forward( base_position, offset_chars );
			return base_position;
		}
		EON_TEST_3STEP_SANDBOX( File, getPosAtOffset, zero,
			saveFile( "source.txt", "one two three" ),
			Pos base( 5, 5, 0, 5 ),
			EON_EQ( base, File( ( sandboxDir() / "source.txt" ).string() ).getPosAtOffset( base, 0 ) ) );
		EON_TEST_3STEP_SANDBOX( File, getPosAtOffset, positive_one,
			saveFile( "source.txt", "one two three" ),
			Pos base( 5, 5, 0, 5 ),
			EON_EQ( Pos( 6, 6, 0, 6 ), File( ( sandboxDir() / "source.txt" ).string() ).getPosAtOffset( base, 1 ) ) );
		EON_TEST_3STEP_SANDBOX( File, getPosAtOffset, negative_one,
			saveFile( "source.txt", "one two three" ),
			Pos base( 5, 5, 0, 5 ),
			EON_EQ( Pos( 4, 4, 0, 4 ), File( ( sandboxDir() / "source.txt" ).string() ).getPosAtOffset( base, -1 ) ) );
		EON_TEST_3STEP_SANDBOX( File, getPosAtOffset, positive_cross_line,
			saveFile( "source.txt", "one\ntwo\nthree" ),
			Pos base( 5, 5, 1, 1 ),
			EON_EQ( Pos( 9, 9, 2, 1 ).rawStr(), File( ( sandboxDir() / "source.txt" ).string() ).getPosAtOffset( base, 4 ).rawStr() ) );
		EON_TEST_3STEP_SANDBOX( File, getPosAtOffset, negative_cross_line,
			saveFile( "source.txt", "one\ntwo\nthree" ),
			Pos base( 5, 5, 1, 1 ),
			EON_EQ( Pos( 1, 1, 0, 1 ), File( ( sandboxDir() / "source.txt" ).string() ).getPosAtOffset( base, -4 ) ) );

		EON_TEST_3STEP_SANDBOX( File, match, str_positive,
			saveFile( "source.txt", "one two three" ),
			string value = "two",
			EON_TRUE( File( ( sandboxDir() / "source.txt" ).string() ).match( Pos( 4, 4, 0, 4 ), Pos( 7, 7, 0, 7 ), value ) ) );
		EON_TEST_3STEP_SANDBOX( File, match, str_negative,
			saveFile( "source.txt", "one two three" ),
			string value = "two",
			EON_FALSE( File( ( sandboxDir() / "source.txt" ).string() ).match( Pos( 5, 5, 0, 5 ), Pos( 8, 8, 0, 8 ), value ) ) );

		EON_TEST_2STEP_SANDBOX( File, match, cstr_positive,
			saveFile( "source.txt", "one two three" ),
			EON_TRUE( File( ( sandboxDir() / "source.txt" ).string() ).match( Pos( 4, 4, 0, 4 ), Pos( 7, 7, 0, 7 ), "two" ) ) );
		EON_TEST_2STEP_SANDBOX( File, match, cstr_negative,
			saveFile( "source.txt", "one two three" ),
			EON_FALSE( File( ( sandboxDir() / "source.txt" ).string() ).match( Pos( 5, 5, 0, 5 ), Pos( 8, 8, 0, 8 ), "two" ) ) );

		char_t File::chr( const Pos& pos ) noexcept
		{
			if( pos.BytePos >= NumBytes )
				return nochar;
			if( pos.BytePos != Data.tellg() )
			{
				if( Data.fail() )
					Data.clear();
				Data.seekg( pos.BytePos, std::ifstream::beg );
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
		EON_TEST_2STEP_SANDBOX( File, chr, first,
			saveFile( "source.txt", "123456789" ),
			EON_EQ( '1', File( ( sandboxDir() / "source.txt" ).string() ).chr( Pos( 0, 0, 0, 0 ) ) ) );
		EON_TEST_2STEP_SANDBOX( File, chr, last,
			saveFile( "source.txt", "123456789" ),
			EON_EQ( '9', File( ( sandboxDir() / "source.txt" ).string() ).chr( Pos( 8, 8, 0, 8 ) ) ) );
		EON_TEST_2STEP_SANDBOX( File, chr, beyond_last,
			saveFile( "source.txt", "123456789" ),
			EON_EQ( nochar, File( ( sandboxDir() / "source.txt" ).string() ).chr( Pos( 9, 9, 0, 9 ) ) ) );

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
		EON_TEST_2STEP_SANDBOX( File, byte, first,
			saveFile( "source.txt", "123456789" ),
			EON_EQ( '1', File( ( sandboxDir() / "source.txt" ).string() ).byte( 0 ) ) );
		EON_TEST_2STEP_SANDBOX( File, byte, last,
			saveFile( "source.txt", "123456789" ),
			EON_EQ( '9', File( ( sandboxDir() / "source.txt" ).string() ).byte( 8 ) ) );
		EON_TEST_2STEP_SANDBOX( File, byte, beyond_last,
			saveFile( "source.txt", "123456789" ),
			EON_EQ( -1, File( ( sandboxDir() / "source.txt" ).string() ).byte( 9 ) ) );

		string File::str( const Pos& start, const Pos& end ) noexcept
		{
			auto real_end = _realEnd( end );
			if( start.BytePos >= NumBytes || real_end <= start )
				return string();
			string str;
			for( auto p = start; p != real_end; p = getPosAtOffset( p, 1 ) )
				str += chr( p );
			return str;
		}
		EON_TEST_2STEP_SANDBOX( File, str, empty,
			saveFile( "source.txt", "" ),
			EON_EQ( "", File( ( sandboxDir() / "source.txt" ).string() ).str( Pos( 0, 0, 0, 0 ), Pos( 9, 9, 0, 9 ) ) ) );
		EON_TEST_2STEP_SANDBOX( File, str, non_empty,
			saveFile( "source.txt", "one two three" ),
			EON_EQ( "two", File( ( sandboxDir() / "source.txt" ).string() ).str( Pos( 4, 4, 0, 4 ), Pos( 7, 7, 0, 7 ) ) ) );

		std::string File::bytes( const Pos& start, const Pos& end ) noexcept
		{
			auto real_end = _realEnd( end );
			if( start.BytePos >= NumBytes || real_end <= start )
				return std::string();
			std::string str;
			for( auto p = start; p != real_end; p = getPosAtOffset( p, 1 ) )
				str += static_cast<char>( byte( p.BytePos ) );
			return str;
		}
		EON_TEST_2STEP_SANDBOX( File, bytes, empty,
			saveFile( "source.txt", "" ),
			EON_EQ( "", File( ( sandboxDir() / "source.txt" ).string() ).bytes( Pos( 0, 0, 0, 0 ), Pos( 9, 9, 0, 9 ) ) ) );
		EON_TEST_2STEP_SANDBOX( File, bytes, non_empty,
			saveFile( "source.txt", "one two three" ),
			EON_EQ( "two", File( ( sandboxDir() / "source.txt" ).string() ).bytes( Pos( 4, 4, 0, 4 ), Pos( 7, 7, 0, 7 ) ) ) );




		void File::_forward( Pos& pos, index_t num_chars )
		{
			if( num_chars == 0 )
				return;
			if( pos.BytePos >= NumBytes )
				throw EndOfSource();
			if( pos.BytePos != Data.tellg() )
			{
				if( Data.fail() )
					Data.clear();
				Data.seekg( pos.BytePos, std::ifstream::beg );
				if( Data.fail() )
					throw EndOfSource();
			}

			for( ; static_cast<size_t>( Data.tellg() ) < NumBytes && num_chars > 0; --num_chars )
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
					if( pos.Line == _numCharsOnLines().size() )
						_numCharsOnLines().push_back( pos.PosOnLine );
					++pos.Line;
					pos.PosOnLine = 0;
				}
				else if( cp == CReturnChr )
					++num_chars;
				else
					++pos.PosOnLine;
				++pos.CharPos;
				pos.BytePos += num;
			}
		}
		EON_TEST_4STEP_SANDBOX( File, _forward, legal,
			saveFile( "source.txt", "one" ),
			Pos pos( 0, 0, 0, 0 ),
			File( ( sandboxDir() / "source.txt" ).string() )._forward( pos, 1 ),
			EON_EQ( 1, pos.BytePos ) );
		EON_TEST_4STEP_SANDBOX( File, _forward, legal_past_newline,
			saveFile( "source.txt", "one\ntwo" ),
			Pos pos( 0, 0, 0, 0 ),
			File( ( sandboxDir() / "source.txt" ).string() )._forward( pos, 4 ),
			EON_EQ( Pos( 4, 4, 1, 0 ), pos ) );
		EON_TEST_4STEP_SANDBOX( File, _forward, beyond_last,
			saveFile( "source.txt", "one" ),
			Pos pos( 0, 0, 0, 0 ),
			File( ( sandboxDir() / "source.txt" ).string() )._forward( pos, 10 ),
			EON_EQ( 3, pos.BytePos ) );
		EON_TEST_3STEP_SANDBOX( File, _forward, end_of_source,
			saveFile( "source.txt", "one" ),
			Pos pos( 4, 4, 0, 4 ),
			EON_RAISE( File( ( sandboxDir() / "source.txt" ).string() )._forward( pos, 1 ), EndOfSource ) );

		void File::_backward( Pos& pos, index_t num_chars )
		{
			if( pos.BytePos > NumBytes )
				throw EndOfSource();
			if( pos.BytePos != Data.tellg() )
			{
				if( Data.fail() )
					Data.clear();
				Data.seekg( pos.BytePos, std::ifstream::beg );
				if( Data.fail() )
					throw EndOfSource();
			}
			for( ; pos.BytePos > 0 && num_chars > 0; --num_chars )
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
					if( _numCharsOnLines().empty() )
						_scanFile();
					pos.PosOnLine = _numCharsOnLines()[ pos.Line ];
				}
				else
					--pos.PosOnLine;
				--pos.CharPos;
				pos.BytePos -= ( end - beg );
			}
		}
		EON_TEST_4STEP_SANDBOX( File, _backward, legal,
			saveFile( "source.txt", "one" ),
			Pos pos( 3, 3, 0, 3 ),
			File( ( sandboxDir() / "source.txt" ).string() )._backward( pos, 1 ),
			EON_EQ( 2, pos.BytePos ) );
		EON_TEST_4STEP_SANDBOX( File, _backward, legal_past_newline,
			saveFile( "source.txt", "one\ntwo" ),
			Pos pos( 7, 7, 1, 7 ),
			File( ( sandboxDir() / "source.txt" ).string() )._backward( pos, 4 ),
			EON_EQ( Pos( 3, 3, 0, 3 ), pos ) );
		EON_TEST_4STEP_SANDBOX( File, _backward, beyond_first,
			saveFile( "source.txt", "one" ),
			Pos pos( 3, 3, 0, 3 ),
			File( ( sandboxDir() / "source.txt" ).string() )._backward( pos, 10 ),
			EON_EQ( 0, pos.BytePos ) );
		EON_TEST_3STEP_SANDBOX( File, _backward, end_of_source,
			saveFile( "source.txt", "one" ),
			Pos pos( 10, 10, 0, 10 ),
			EON_RAISE( File( ( sandboxDir() / "source.txt" ).string() )._backward( pos, 2 ), EndOfSource ) );

		void File::_scanFile()
		{
			_numCharsOnLines().clear();
			Pos pos( 0, 0, 0, 0 );
			_forward( pos, NumBytes );
			_numCharsOnLines().push_back( pos.PosOnLine );
		}
		EON_TEST_4STEP_SANDBOX( File, _scanFile, empty,
			saveFile( "source.txt", "" ),
			File obj( ( sandboxDir() / "source.txt" ).string() ),
			obj._scanFile(),
			EON_EQ( 1, obj.NumCharsOnLines.size() ) );
		EON_TEST_4STEP_SANDBOX( File, _scanFile, singleton,
			saveFile( "source.txt", "one" ),
			File obj( ( sandboxDir() / "source.txt" ).string() ),
			obj._scanFile(),
			EON_EQ( 1, obj.NumCharsOnLines.size() ) );
		EON_TEST_4STEP_SANDBOX( File, _scanFile, multiple,
			saveFile( "source.txt", "one\ntwo\nthree" ),
			File obj( ( sandboxDir() / "source.txt" ).string() ),
			obj._scanFile(),
			EON_EQ( 3, obj.NumCharsOnLines.size() ) );
		EON_TEST_4STEP_SANDBOX( File, _scanFile, multiple_line_length,
			saveFile( "source.txt", "one\ntwo\nthree" ),
			File obj( ( sandboxDir() / "source.txt" ).string() ),
			obj._scanFile(),
			EON_EQ( 5, obj.NumCharsOnLines[ 2 ] ) );
	}
}
