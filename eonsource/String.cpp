#include "String.h"


namespace eon
{
	namespace source
	{
		Pos String::push( Pos start_point, size_t num_characters )
		{
			if( start_point.bytePos() >= Data.numBytes() )
				throw EndOfSource();
			string::iterator i( Data.c_str(), Data.numBytes(), Data.numChars(),
				Data.c_str() + start_point.bytePos(), start_point.charPos() );
			Pos pos = start_point;
			for( ; i && num_characters > 0; ++i, --num_characters )
			{
				if( *i == NewlineChr )
				{
					if( pos.Line == LineLengths.size() )
						LineLengths.push_back( pos.PosOnLine );
					++pos.Line;
					pos.PosOnLine = 0;
				}
				else
					++pos.PosOnLine;
				++pos.CharPos;
				pos.BytePos += i.codepointSize();
			}
			return pos;
		}

		Pos String::pull( Pos start_point, Pos end_point, size_t num_characters )
		{
			if( end_point >= start_point )
				throw EndOfSource();
			string::iterator end( Data.c_str(), Data.numBytes(), Data.numChars(),
				Data.c_str() + end_point.bytePos(), end_point.charPos() );
			string::iterator i = start_point.bytePos() >= Data.numBytes() ? Data.end()
				: string::iterator( Data.c_str(), Data.numBytes(), Data.numChars(), Data.c_str() + start_point.bytePos(),
					start_point.charPos() );
			Pos pos = start_point;
			for( --i; num_characters > 0; --i, --num_characters )
			{
				if( *i == NewlineChr )
				{
					--pos.Line;
					pos.PosOnLine = LineLengths[ pos.Line ];
				}
				else
					--pos.PosOnLine;
				--pos.CharPos;
				pos.BytePos = i.numByte();
				if( i == end )
					break;
			}
			return pos;
		}

		string String::str( Pos start, Pos end ) noexcept
		{
			if( end.bytePos() == 0 )
			{
				auto last = Data.last();
				end.BytePos = last.numByte() + 1;
				end.CharPos = last.numChar() + 1;
			}
			return Data.substr(
				string::iterator( Data.c_str(), Data.numBytes(), Data.numChars(),
					Data.c_str() + start.bytePos(), start.charPos() ),
				end.bytePos() > Data.numBytes() ? Data.end() :
				string::iterator( Data.c_str(), Data.numBytes(), Data.numChars(),
					Data.c_str() + end.bytePos(), end.charPos() ) );
		}
	}
}
