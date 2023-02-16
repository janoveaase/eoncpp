#include "String.h"


namespace eon
{
	namespace source
	{
		EON_TEST( String, String, name,
			EON_EQ( "test", String( "test", string() ).sourceName() ) );
		EON_TEST_2STEP( String, String, empty,
			String obj( "test", string() ),
			EON_EQ( "", obj.Data ) );
		EON_TEST_2STEP( String, String, non_empty,
			String obj( "test", "one two three" ),
			EON_EQ( "one two three", obj.Data ) );




		EON_TEST_3STEP( String, reclaim, source,
			String obj( "test", "one two three" ),
			string str = obj.reclaim(),
			EON_EQ( "", obj.Data ) );
		EON_TEST_3STEP( String, reclaim, target,
			String obj( "test", "one two three" ),
			string str = obj.reclaim(),
			EON_EQ( "one two three", str ) );




		EON_TEST( String, numBytesInSource, empty,
			EON_EQ( 0, String( "test", "" ).numBytesInSource() ) );
		EON_TEST( String, numBytesInSource, non_empty,
			EON_EQ( 13, String( "test", "one two three" ).numBytesInSource() ) );

		Pos String::getPosAtOffset( Pos base_position, int offset_chars )
		{
			if( offset_chars < 0 )
				_backward( base_position, -offset_chars );
			else if( offset_chars > 0 )
				_forward( base_position, offset_chars );
			return base_position;
		}
		EON_TEST_2STEP( String, getPosAtOffset, zero,
			Pos base( 5, 5, 0, 5 ),
			EON_EQ( base, String( "test", "one two three" ).getPosAtOffset( base, 0 ) ) );
		EON_TEST_2STEP( String, getPosAtOffset, positive_one,
			Pos base( 5, 5, 0, 5 ),
			EON_EQ( Pos( 6, 6, 0, 6 ), String( "test", "one two three" ).getPosAtOffset( base, 1 ) ) );
		EON_TEST_2STEP( String, getPosAtOffset, negative_one,
			Pos base( 5, 5, 0, 5 ),
			EON_EQ( Pos( 4, 4, 0, 4 ), String( "test", "one two three" ).getPosAtOffset( base, -1 ) ) );
		EON_TEST_2STEP( String, getPosAtOffset, positive_cross_line,
			Pos base( 5, 5, 1, 1 ),
			EON_EQ( Pos( 9, 9, 2, 1 ), String( "test", "one\ntwo\nthree" ).getPosAtOffset( base, 4 ) ) );
		EON_TEST_2STEP( String, getPosAtOffset, negative_cross_line,
			Pos base( 5, 5, 1, 1 ),
			EON_EQ( Pos( 1, 1, 0, 1 ), String( "test", "one\ntwo\nthree" ).getPosAtOffset( base, -4 ) ) );

		EON_TEST_2STEP( String, match, str_positive,
			string value = "two",
			EON_TRUE( String( "test", "one two three" ).match( Pos( 4, 4, 0, 4 ), Pos( 7, 7, 0, 7 ), value ) ) );
		EON_TEST_2STEP( String, match, str_negative,
			string value = "two",
			EON_FALSE( String( "test", "one two three" ).match( Pos( 5, 5, 0, 5 ), Pos( 8, 8, 0, 8 ), value ) ) );

		EON_TEST( String, match, cstr_positive,
			EON_TRUE( String( "test", "one two three" ).match( Pos( 4, 4, 0, 4 ), Pos( 7, 7, 0, 7 ), "two" ) ) );
		EON_TEST( String, match, cstr_negative,
			EON_FALSE( String( "test", "one two three" ).match( Pos( 5, 5, 0, 5 ), Pos( 8, 8, 0, 8 ), "two" ) ) );

		EON_TEST( String, chr, first,
			EON_EQ( '1', String( "test", "123456789" ).chr( Pos( 0, 0, 0, 0 ) ) ) );
		EON_TEST( String, chr, last,
			EON_EQ( '9', String( "test", "123456789" ).chr( Pos( 8, 8, 0, 8 ) ) ) );
		EON_TEST( String, chr, beyond_last,
			EON_EQ( nochar, String( "test", "123456789" ).chr( Pos( 9, 9, 0, 9 ) ) ) );

		EON_TEST( String, byte, first,
			EON_EQ( '1', String( "test", "123456789" ).byte( 0 ) ) );
		EON_TEST( String, byte, last,
			EON_EQ( '9', String( "test", "123456789" ).byte( 8 ) ) );
		EON_TEST( String, byte, beyond_last,
			EON_EQ( -1, String( "test", "123456789" ).byte( 9 ) ) );

		string String::str( Pos start, Pos end ) noexcept
		{
			if( end.BytePos == 0 )
			{
				auto last = Data.last();
				end.BytePos = last.numByte() + 1;
				end.CharPos = last.numChar() + 1;
			}
			return Data.substr(
				string::iterator( Data.c_str(), Data.numBytes(), Data.numChars(),
					Data.c_str() + start.BytePos, start.CharPos ),
				end.BytePos > Data.numBytes() ? Data.end() :
				string::iterator( Data.c_str(), Data.numBytes(), Data.numChars(),
					Data.c_str() + end.BytePos, end.CharPos ) );
		}
		EON_TEST( String, str, empty,
			EON_EQ( "", String( "test", "" ).str( Pos( 0, 0, 0, 0 ), Pos( 9, 9, 0, 9 ) ) ) );
		EON_TEST( String, str, non_empty,
			EON_EQ( "two", String( "test", "one two three" ).str( Pos( 4, 4, 0, 4 ), Pos( 7, 7, 0, 7 ) ) ) );

		std::string String::bytes( Pos start, Pos end ) noexcept
		{
			if( end.BytePos == 0 )
			{
				auto last = Data.last();
				end.BytePos = last.numByte() + 1;
				end.CharPos = last.numChar() + 1;
			}
			return Data.stdstr().substr( start.BytePos, end.BytePos - start.BytePos );
		}
		EON_TEST( String, bytes, empty,
			EON_EQ( "", String( "test", "" ).bytes( Pos( 0, 0, 0, 0 ), Pos( 9, 9, 0, 9 ) ) ) );
		EON_TEST( String, bytes, non_empty,
			EON_EQ( "two", String( "test", "one two three" ).bytes( Pos( 4, 4, 0, 4 ), Pos( 7, 7, 0, 7 ) ) ) );




		void String::_forward( Pos& pos, index_t num_chars )
		{
			if( num_chars == 0 )
				return;
			if( pos.BytePos >= Data.numBytes() )
				throw EndOfSource();
			string::iterator i( Data.c_str(), Data.numBytes(), Data.numChars(), Data.c_str() + pos.BytePos, pos.CharPos );
			for( ; i && num_chars > 0; ++i, --num_chars )
			{
				if( *i == NewlineChr )
				{
					if( pos.Line == NumCharsOnLines.size() )
						NumCharsOnLines.push_back( pos.PosOnLine );
					++pos.Line;
					pos.PosOnLine = 0;
				}
				else
					++pos.PosOnLine;
				++pos.CharPos;
				pos.BytePos += i.codepointSize();
			}
		}
		EON_TEST_3STEP( String, _forward, legal,
			Pos pos( 0, 0, 0, 0 ),
			String( "test", "one" )._forward( pos, 1 ),
			EON_EQ( 1, pos.BytePos ) );
		EON_TEST_3STEP( String, _forward, legal_past_newline,
			Pos pos( 0, 0, 0, 0 ),
			String( "test", "one\ntwo" )._forward( pos, 4 ),
			EON_EQ( Pos( 4, 4, 1, 0 ), pos ) );
		EON_TEST_3STEP( String, _forward, beyond_last,
			Pos pos( 0, 0, 0, 0 ),
			String( "test", "one" )._forward( pos, 10 ),
			EON_EQ( 3, pos.BytePos ) );
		EON_TEST( String, _forward, end_of_source,
			EON_RAISE( String( "test", "one" )._forward( Pos( 4, 4, 0, 4 ), 1 ), EndOfSource ) );

		void String::_backward( Pos& pos, index_t num_chars )
		{
			if( pos.BytePos >= Data.numBytes() )
			{
				pos.BytePos = Data.numBytes();
				pos.CharPos = Data.numChars();
			}
			string::iterator i( Data.c_str(), Data.numBytes(), Data.numChars(), Data.c_str() + pos.BytePos, pos.CharPos );
			for( --i; i && num_chars > 0; --i, --num_chars )
			{
				if( *i == NewlineChr )
				{
					--pos.Line;
					if( NumCharsOnLines.empty() )
						_scanString();
					pos.PosOnLine = NumCharsOnLines[ pos.Line ];
				}
				else
					--pos.PosOnLine;
				--pos.CharPos;
				pos.BytePos = i.numByte();
			}
		}
		EON_TEST_3STEP( String, _backward, legal,
			Pos pos( 3, 3, 0, 3 ),
			String( "test", "one" )._backward( pos, 1 ),
			EON_EQ( 2, pos.BytePos ) );
		EON_TEST_3STEP( String, _backward, legal_past_newline,
			Pos pos( 7, 7, 1, 7 ),
			String( "test", "one\ntwo" )._backward( pos, 4 ),
			EON_EQ( Pos( 3, 3, 0, 3 ), pos ) );
		EON_TEST_3STEP( String, _backward, beyond_first,
			Pos pos( 3, 3, 0, 3 ),
			String( "test", "one" )._backward( pos, 10 ),
			EON_EQ( 0, pos.BytePos ) );
		EON_TEST_3STEP( String, _backward, end_of_source,
			Pos pos( 10, 10, 0, 10 ),
			String( "test", "one" )._backward( pos, 2 ),
			EON_EQ( 1, pos.BytePos ) );

		void String::_scanString()
		{
			NumCharsOnLines.clear();
			Pos pos( 0, 0, 0, 0 );
			_forward( pos, Data.numBytes() );
			NumCharsOnLines.push_back( pos.PosOnLine );
		}
		EON_TEST_3STEP( String, _scanString, empty,
			String obj( "test", "" ),
			obj._scanString(),
			EON_EQ( 1, obj.NumCharsOnLines.size() ) );
		EON_TEST_3STEP( String, _scanString, singleton,
			String obj( "test", "one" ),
			obj._scanString(),
			EON_EQ( 1, obj.NumCharsOnLines.size() ) );
		EON_TEST_3STEP( String, _scanString, multiple,
			String obj( "test", "one\ntwo\nthree" ),
			obj._scanString(),
			EON_EQ( 3, obj.NumCharsOnLines.size() ) );
		EON_TEST_3STEP( String, _scanString, multiple_line_length,
			String obj( "test", "one\ntwo\nthree" ),
			obj._scanString(),
			EON_EQ( 5, obj.NumCharsOnLines[ 2 ] ) );
	}
}
