#pragma once
#include "Raw.h"


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
		// Exception used when attempting to access a referenced source when no
		// such source exists.
		//
		EONEXCEPT( NoSource );




		///////////////////////////////////////////////////////////////////////
		//
		// Eon Source Reference Class - eon::source::Ref
		//
		// A source::Ref includes one source::Pos marking the start of the
		// referenced area and another that marks the end.
		// The end position is non-inclusive!
		//
		// Because we in the case of file input do not read the entire file from
		// start to end beforehand, we do not know the exact number of UTF-8
		// characters in the file. (Not until we have worked our way through it
		// to the end.) Because of this, initial (default) end position for a
		// source reference will only have the byte count of the source, not the
		// character count. This is called an 'implicit' end position, as
		// opposed to an 'explicit' where the character count is also known.
		// The effect is that some methods will not work when end position is
		// implicit. Each affected method will have information about a this in
		// their comments!
		//
		class Ref
		{
		public:

			///////////////////////////////////////////////////////////////////////
			//
			// Construction
			//

			// Default constructor, no source reference.
			// WARNING: This is just a place holder for class attributes!
			//          Using a source reference without a source will result in undefined behavior!
			Ref() noexcept = default;

			// Copy another source reference.
			Ref( const Ref& ) noexcept = default;

			// Construct for a source, reference the entire source.
			inline Ref( Raw& src ) noexcept { *this = src; }

			// Construct for a source, reference from specified start to the end of the source.
			inline Ref( Raw& src, const Pos& start ) noexcept : Source( &src ), Start( start ) {
				End.BytePos = Source->numBytesInSource(); }

			// Construct for a source, reference the specified area.
			inline Ref( Raw& src, const Pos& start, const Pos& end ) noexcept
				: Source( &src ), Start( start ), End( end ) {}




			///////////////////////////////////////////////////////////////////////
			//
			// Modifier Methods
			//
		public:

			// Discard existing details and assign a copy the 'other' source reference.
			Ref& operator=( const Ref& ) noexcept = default;

			// Discard existing details and assing a new source.
			inline Ref& operator=( Raw& src ) noexcept {
				Source = &src; Start = Pos( 0, 0, 0, 0 ); End.BytePos = Source->numBytesInSource(); return *this; }


			// Move start position a number of characters forward (positive argument) or backward (negative argument).
			// If moving to after current end position, the end position will be moved to match start!
			// Returns true if moved one or more characters
			bool moveStart( int num_chars = 1 ) noexcept;

			// Move start position to start of current line.
			// Will not move if already at start of line!
			// Returns true if moved one or more characters
			bool moveStartToStartOfLine() noexcept;

			// Move start position to end position
			// END POSITION MUST BE EXPLICIT!
			inline void moveStartToEnd() noexcept { if( End.CharPos > 0 ) { Start = End; } }

			// Set start position to specified value
			// If new start is beyond the current end, the end position will be moved to match start!
			// Returns true if set!
			inline bool start( const Pos& pos ) noexcept
			{
				if( !Source->afterLast( pos ) )
				{
					Start = pos;
					if( End < Start )
						End = Start;
					return true;
				}
				return false;
			}

			// Move end position a number of characters forward (positive argument) or backward (negative argument).
			// END POSITION MUST BE EXPLICIT!
			// If moving to before current start position, the start position will be moved to match end!
			// Returns true if moved one or more characters
			bool moveEnd( int num_chars = 1 ) noexcept;

			// Move end position to end of the current line.
			// Will not move if already at end of line!
			// Returns true if moved one or more characters
			bool moveEndToEndOfLine() noexcept;

			// Send end position to specified value
			// Will not set to after end of source nor before current start
			// position!
			// Returns true if set
			inline bool end( const Pos& pos ) noexcept {
				if( pos >= Start && Source->isValid( pos ) ) { End = pos; return true; } return false; }





			///////////////////////////////////////////////////////////////////////
			//
			// Read-only Methods
			//

			// Check if source reference is empty (end == start)
			inline bool empty() const noexcept { return numBytes() == 0; }

			// Check if source reference refers to something
			inline explicit operator bool() const { return numBytes() > 0; }

			// Check if end position is explicit
			inline bool explicitEnd() const noexcept { return End.CharPos > 0 && End.BytePos > 0; }

			// Get source name
			inline const string& name() const noexcept { return Source->sourceName(); }

			// Get start position
			inline const Pos& start() const noexcept { return Start; }

			// Get end position
			inline const Pos& end() const noexcept { return End; }

			// Get last position (just before end).
			// END POSITION MUST BE EXPLICIT!
			inline Pos last() {
				return End == Start || End.BytePos == Start.BytePos + 1 ? Start : Source->getPosAtOffset( End, -1 ); }

			// Get number of bytes in reference area (including newlines and carriage return, if any).
			inline size_t numBytes() const noexcept { return End.BytePos - Start.BytePos; }

			// Get number of characters in reference area (including newlines
			// and carriage return, if any)
			// END POSITION MUST BE EXPLICIT!
			inline size_t numChars() const noexcept {
				return End.BytePos > 0 ? End.CharPos - Start.CharPos : 0; }

			// Check if start position is at end of source
			inline bool startAtEnd() const noexcept { return Source->afterLast( Start ); }

			// Check if end position is at end of source
			inline bool endAtEnd() const noexcept { return Source->afterLast( End ); }

			// Check if the specified position is at (or beyond) the end of
			// this source reference
			inline bool atEnd( const Pos& pos ) const noexcept {
				return End.BytePos > 0 ? pos >= End : pos.BytePos >= Source->numBytesInSource(); }

			// Check if source matches exactly the specified string value.
			inline bool match( const eon::string& value ) const noexcept { return Source->match( Start, End, value ); }
			inline bool match( const char* value ) const noexcept { return Source->match( Start, End, value ); }

			// Get character at start position
			// Returns [eon::nochar] if at or beyond source end!
			inline char_t chr() const noexcept { return Source->chr( Start ); }

			// Get the referenced source area as a string
			// Returns empty if empty area (including implicit end position)
			inline string str() const noexcept { return Source->str( Start, End ); }

			// Get the referenced source area as bytes.
			// Returns empty if empty area (including implicit end position).
			inline std::string bytes() const noexcept { return Source->bytes( Start, End ); }

			// Access the source
			inline const Raw& source() const noexcept { return *Source; }
			inline Raw& source() noexcept { return *Source; }


			// Get start position as string on the format: "<line>:<pos>"
			inline string startStr() const { return Start.str(); }

			// Get end position as string on the format: "<line>:<pos>"
			inline string endStr() const { return End.str(); }


			// Compare two source references.
			int compare( const Ref& other ) const noexcept;

			inline friend bool operator<( const Ref& a, const Ref& b ) noexcept { return a.compare( b ) < 0; }
			inline friend bool operator<=( const Ref& a, const Ref& b ) noexcept { return a.compare( b ) <= 0; }
			inline friend bool operator>( const Ref& a, const Ref& b ) noexcept { return a.compare( b ) > 0; }
			inline friend bool operator>=( const Ref& a, const Ref& b ) noexcept { return a.compare( b ) >= 0; }
			inline friend bool operator==( const Ref& a, const Ref& b ) noexcept { return a.compare( b ) == 0; }
			inline friend bool operator!=( const Ref& a, const Ref& b ) noexcept { return a.compare( b ) != 0; }




			///////////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

			Raw* Source{ nullptr };
			Pos Start;
			Pos End;
		};
	}
}
