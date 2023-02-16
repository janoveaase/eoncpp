#pragma once
#include <eonexcept/Exception.h>
#include <eoninlinetest/InlineTest.h>


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// The 'source' namespace encloses all source functionality
	//
	namespace source
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Eon Source Position Class - eon::source::Pos
		//
		// A source::Pos is a reference to a specific location within a source.
		// It contains a byte position from the start of the source, a character
		// position from the start, a line number, and a position on that line.
		//
		class Pos
		{
		public:

			// Default construction, a non-reference.
			Pos() = default;

			// Construct as a copy of the 'other' source position.
			inline Pos( const Pos& other ) noexcept { *this = other; }

			// Construct with specific details
			inline Pos( index_t byte_pos, index_t char_pos, index_t line, index_t pos_on_line ) noexcept {
				BytePos = byte_pos; CharPos = char_pos; Line = line; PosOnLine = pos_on_line; Valid = true; }

			// Default destruction
			~Pos() = default;



			// Discard existing details and assign as a copy of the 'other'.
			inline Pos& operator=( const Pos& other ) noexcept {
				BytePos = other.BytePos; CharPos = other.CharPos; Line = other.Line; PosOnLine = other.PosOnLine;
				Valid = other.Valid; return *this; }


			// Check if the position is valid (has been set).
			inline bool isValid() const noexcept { return BytePos > 0; }


			// Get line number and position on line as string format: "<line>:<pos>".
			// Returns empty string if not valid!
			inline string str() const { return Valid ? string( Line + 1 ) + ":" + string( PosOnLine + 1 ) : ""; }

			// Get position as a whole as a raw string, format: "<bytepos>,<charpos>,<line>,<pos-on-line>,<valid>".
			inline string rawStr() const {
				return string( BytePos ) + "," + string( CharPos ) + "," + string( Line ) + "," + string( PosOnLine )
					+ "," + string( Valid ? "valid" : "invalid" ); }



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
			bool Valid{ false };
		};
	}
}
