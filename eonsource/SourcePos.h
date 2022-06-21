#pragma once
#include <eonexcept/Exception.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'source' namespace encloses all source functionality
	**************************************************************************/
	namespace source
	{
		/**********************************************************************
		  Eon Source Position Class - eon::source::Pos

		  A source position is a reference to a specific location within a
		  source (that is not known by the source position object).
		  It contains a byte position from the start of the source, a character
		  position from the start, a line number, and a position on that line.
		**********************************************************************/
		class Pos
		{
		public:

			//* Default construction, a non-reference
			Pos() = default;

			//* Construct as a copy of the 'other' source position
			inline Pos( const Pos& other ) noexcept { *this = other; }

			//* Construct with specific details
			inline Pos( index_t byte_pos, index_t char_pos, index_t line, index_t pos_on_line ) noexcept {
				BytePos = byte_pos; CharPos = char_pos; Line = line; PosOnLine = pos_on_line; }

			//* Default destruction
			~Pos() = default;


			//* Assign as a copy of the 'other' source position
			inline Pos& operator=( const Pos& other ) noexcept {
				BytePos = other.BytePos; CharPos = other.CharPos; Line = other.Line; PosOnLine = other.PosOnLine;
				return *this; }


			//* Check if the position is zero
			inline operator bool() const noexcept { return BytePos > 0; }

			//* Get the raw byte position (counted from start of source)
			inline index_t bytePos() const noexcept { return BytePos; }

			//* Get the character position (counted from the start of source)
			inline index_t charPos() const noexcept { return CharPos; }

			//* Get the line number
			inline index_t line() const noexcept { return Line; }

			//* Get the position on the lien
			inline index_t posOnLine() const noexcept { return PosOnLine; }


			//* Get line and position on line as string format
			//* Format: <line>:<pos>
			inline string str() const { return string( Line + 1 ) + ":" + string( PosOnLine + 1 ); }


			//* Compare with another position - presumably from the same source
			inline bool operator<( const Pos& other ) const noexcept { return BytePos < other.BytePos; }
			inline bool operator<=( const Pos& other ) const noexcept { return BytePos <= other.BytePos; }
			inline bool operator>( const Pos& other ) const noexcept { return BytePos > other.BytePos; }
			inline bool operator>=( const Pos& other ) const noexcept { return BytePos >= other.BytePos; }
			inline bool operator==( const Pos& other ) const noexcept { return BytePos == other.BytePos; }
			inline bool operator!=( const Pos& other ) const noexcept { return BytePos != other.BytePos; }




		public:
			index_t BytePos{ 0 };
			index_t CharPos{ 0 };
			index_t Line{ 0 };
			index_t PosOnLine{ 0 };
		};
	}
}
