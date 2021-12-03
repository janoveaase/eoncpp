#pragma once
#include "Raw.h"


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
		  Exception used when attempting to access a referenced source when no
		  such source exists.
		**********************************************************************/
		EONEXCEPT( NoSource );




		/**********************************************************************
		  Eon Source Reference Class - eon::source::Ref

		  A source reference is a reference to a source, it includes a
		  source::Pos marking the start of the referenced area and another that
		  marks the end. The end is not inclusive (one character past the last
		  charcater of the reference).

		  Because we in the case of file input do not read the entire file from
		  start to end beforehand, we do not know the exact number of UTF-8
		  characters in the file. (Not until we have worked our way through it
		  to the end.) Because of this, initial (default) end position for a
		  source reference will only have the byte count of the source, not the
		  character count. This is called an 'implicit' end position, as
		  opposed to an 'explicit' where the character count is also known.
		  The effect is that some methods will not work when end position is
		  implicit. Each affected method will have information about a this in
		  their comments!
		**********************************************************************/
		class Ref
		{
		public:

			/******************************************************************
			  Construction
			******************************************************************/

			//* Default constructor, no source reference
			//* WARNING: This is just a place holder for class attributes!
			//*          Using a source reference without a source will result
			//*          in undefined behavior!
			Ref() noexcept = default;

			//* Copy another source reference
			inline Ref( const Ref& rhs ) { *this = rhs; }

			//* Construct for a source, reference the entire source
			inline Ref( Raw& src ) { *this = src; }

			//* Construct for a source, reference from specified start to the
			//* end of the source
			inline Ref( Raw& src, Pos start ) noexcept { Source = &src; Start = start; End.BytePos = Source->numBytes(); }

			//* Construct for a source, reference the specified area
			inline Ref( Raw& src, Pos start, Pos end ) noexcept { Source = &src; Start = start; End = end; }


			//* Default destructor
			~Ref() = default;




			/******************************************************************
			  Modifier Methods
			******************************************************************/

			//* Assign a copy the 'other' source reference
			inline Ref& operator=( const Ref& other ) noexcept {
				Source = other.Source; Start = other.Start; End = other.End; return *this; }

			//* Assign to a (new) source, reference the entire source
			inline Ref& operator=( Raw& src ) noexcept {
				Source = &src; Start = Pos(); End.BytePos = Source->numBytes(); return *this; }


			//* Push (move forward) start position a number of characters
			//* If moving past end, end position will follow start!
			//* Returns true if moved one or more characters
			inline bool pushStart( size_t num_characters = 1 ) noexcept {
				try { Start = Source->push( Start, num_characters ); if( End < Start ) End = Start; return true; }
				catch( ... ) {} return false; }

			//* Pull (move backward) start position a number of characters
			//* Will not move if already as start of source!
			//* Returns true if moved one or more characters
			inline bool pullStart( size_t num_characters = 1 ) noexcept {
				try { Start = Source->pull( Start, Pos(), num_characters ); return true; } catch( ... ) {} return false; }

			//* Pull (move backward) start position to start of current line
			//* Will not move if already at start of line!
			//* Returns true if moved one or more characters
			bool pullStartOfLine() noexcept;

			//* Move start position to end position
			//* END POSITION MUST BE EXPLICIT!
			inline void startToEnd() noexcept { if( End.charPos() > 0 ) { Start = End; } }

			//* Set start position to specified value
			//* If new start is beyond the current end, the end position will
			//* be moved to the new start!
			//* Returns true if set, won't set to after end!
			inline bool start( Pos pos ) noexcept {
				if( !Source->atEnd( pos ) ) { Start = pos; if( End < Start ) End = Start; return true; } return false; }

			//* Push (move forward) end position a number of characters
			//* END POSITION MUST BE EXPLICIT!
			//* Returns true if moved one or more characters
			inline bool pushEnd( size_t num_characters = 1 ) noexcept {
				try { End = Source->push( End, num_characters ); return true; } catch( ... ) {} return false; }

			//* Push (move forward) end position to end of line
			//* Will not move if already at end of line!
			//* Returns true if moved one or more characters
			bool pushEndOfLine() noexcept;

			//* Pull (move backward) end position a number of characters
			//* END POSITION MUST BE EXPLICIT!
			//* Will not move beyond current start position!
			//* Returns true if moved one or more characters
			inline bool pullEnd( size_t num_characters = 1 ) noexcept {
				try { End = Source->pull( End, Start, num_characters ); return true; } catch( ... ) {} return false; }

			//* Send end position to specified value
			//* Will not set to after end of source nor before current start
			//* position!
			//* Returns true if set
			inline bool end( Pos pos ) noexcept {
				if( pos >= Start && !Source->beyondEnd( pos ) ) { End = pos; return true; } return false; }





			/******************************************************************
			  Read-only Methods
			******************************************************************/

			//* Check if source reference is empty (end == start)
			inline bool empty() const noexcept { return numBytes() == 0; }

			//* Check if source reference refers to something
			inline operator bool() const { return numBytes() > 0; }

			//* Check if end position is explicit
			inline bool explicitEnd() const noexcept { return End.CharPos > 0 && End.BytePos > 0; }

			//* Get source name
			inline const string& name() const noexcept { return Source->name(); }

			//* Get start position
			inline const Pos& start() const noexcept { return Start; }

			//* Get end position
			inline const Pos& end() const noexcept { return End; }

			//* Get last position (just before end)
			//* END POSITION MUST BE EXPLICIT!
			inline Pos last() {
				return End == Start || End.bytePos() == Start.bytePos() + 1 ? Start : Source->pull( End, Start, 1 ); }

			//* Get number of bytes in reference area (including newlines and
			//* carriage return, if any).
			inline size_t numBytes() const noexcept { return End.bytePos() - Start.bytePos(); }

			//* Get number of characters in reference area (including newlines
			//* and carriage return, if any)
			//* END POSITION MUST BE EXPLICIT!
			inline size_t numChars() const noexcept {
				return End.bytePos() > 0 ? End.charPos() - Start.charPos() : 0; }

			//* Check if start position is at end of source
			inline bool startAtEnd() const noexcept { return Source->atEnd( Start ); }

			//* Check if end position is at end of source
			inline bool endAtEnd() const noexcept { return Source->atEnd( End ); }

			//* Check if the specified position is at (or beyond) the end of
			//* this source reference
			inline bool atEnd( const Pos& pos ) const noexcept {
				return End.bytePos() > 0 ? pos >= End : pos.bytePos() >= Source->numBytes(); }

			//* Get character at start position
			//* Returns [eon::nochar] if at or beyond source end!
			inline char_t chr() const noexcept { return Source->chr( Start ); }

			//* Get the referenced source area as a string
			//* Returns empty if empty area (including implicit end position)
			inline string str() const noexcept { return Source->str( Start, End ); }

			//* Access the source
			inline const Raw& source() const noexcept { return *Source; }
			inline Raw& source() noexcept { return *Source; }


			//* Get start position as string on the format: "<line>:<pos>"
			inline string startStr() const { return Start.str(); }

			//* Get end position as string on the format: "<line>:<pos>"
			inline string endStr() const { return End.str(); }




		private:
			Raw* Source{ nullptr };
			Pos Start, End;
		};
	}
}
