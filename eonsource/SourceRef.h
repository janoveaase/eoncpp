#pragma once
#include "SourcePos.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Exception used when attempting to access a referenced source when no such
	  source exists.
	**************************************************************************/
	EONEXCEPT( NoSource );




	/**************************************************************************
	  Eon Source Reference Class - eon::sourceref

	  A source reference is a reference to a source, a line in that source and
	  a position on that line. There is an optional number-of-characters
	  attribute that is used to make a source reference span multiple
	  characaters.

	  NOTE: Position and number of characters is in pure characters, meaning 1
	        tab = 1 character pos.
	**************************************************************************/
	class sourceref
	{
	public:

		/**********************************************************************
		  Construction
		**********************************************************************/

		//* Default constructor, no source reference
		sourceref() noexcept = default;

		//* Copy another source reference
		inline sourceref( const sourceref& rhs ) { *this = rhs; }

		//* Construct for a source, locate to start of that source
		inline sourceref( const source& src ) { *this = src; }

		//* Construct for a source, specify position within that source
		inline sourceref( const source& src, const sourcepos& pos ) noexcept {
			Src = &src; Pos = pos; }


		//* Default destructor
		~sourceref() = default;




		/**********************************************************************
		  Modifier Methods
		**********************************************************************/

		//* Assign a copy the 'other' source reference
		inline sourceref& operator=( const sourceref& other ) noexcept {
			Src = other.Src; Pos = other.Pos; return *this; }

		//* Assign to a (new) source, locate to start of that source
		inline sourceref& operator=( const source& src ) noexcept {
			Src = &src; Pos = sourcepos( *Src ); return *this; }

		//* Assign a new position to this source reference
		inline sourceref& operator=( const sourcepos& pos ) noexcept {
			Pos = pos; return *this; }


		//* Set end of the source reference
		inline void setEnd( const string_iterator& end ) noexcept {
			Pos.setEnd( end ); }


		//* Move 'this' source reference's position to the next character in
		//* the source
		//* Newline will cause line number to increment and position on line to
		//* be zero.
		//* NOTE: The number of characters will be ignored (and set to 1).
		//* NOTE: This method will check [atEnd] and return 'false' if at end.
		inline bool advance() noexcept { return Pos.advance( *Src ); }




		/**********************************************************************
		  Read-only Methods
		**********************************************************************/

		//* Check if 'this' source reference references an actual source
		inline operator bool() const { return Src != nullptr; }

		//* Get source name
		//* Returns an empty string if no source attached
		inline const string& name() const noexcept {
			return Src != nullptr ? Src->name() : string::Empty; }

		//* Get position within source
		inline const sourcepos& pos() const noexcept { return Pos; }

		//* Check if 'this' source reference refers to the of the source
		//* Returns 'true' if no source!
		inline bool atEnd() const noexcept {
			return Src != nullptr ? Pos.atEnd( *Src ) : true; }

		//* Get character at current position
		//* Returns 'eon::nochar' if invalid source reference
		inline char_t chr() const noexcept {
			return Src != nullptr ? Pos.chr() : nochar; }

		//* Get the full string referenced by 'this'
		inline substring str() const noexcept {
			return Src != nullptr ? Pos.area() : substring(); }

		//* Access the source
		//* WARNING: Throws [eon::NoSource] if no source has been set!
		inline const eon::source& source() const {
			if( Src != nullptr ) return *Src; else throw NoSource(); }


		//* Get the full source reference in text form
		//* Format: "<source name>:<line>:<pos>[-<pos>]"
		//* (The last part of the range is excluded if only 1 character)
		string textRefRange() const;

		//* Get position reference in text form
		//* Format: "<source name>:<line>:<pos>"
		string textRefPos() const;

		//* Get line reference in text form
		//* Format: "<source name>:<line>"
		string textRefLine() const;




	private:
		const eon::source* Src{ nullptr };
		sourcepos Pos;
	};

}
