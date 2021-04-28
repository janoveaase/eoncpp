#pragma once
#include "Source.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Exception used when attempting to access a referenced source when no such
	  source exists.
	**************************************************************************/
	class NoSource : public std::exception
	{
	private:
		std::string Message;
	public:
		NoSource() : std::exception() {}
		inline NoSource( const char* message ) : std::exception() {
			Message = message; }
		inline NoSource( const std::string& message ) : std::exception() {
			Message = message; }
		inline NoSource( std::string&& message ) noexcept : std::exception() {
			Message = std::move( message ); }
		const char* what() const noexcept override { return Message.c_str(); }
	};




	/**************************************************************************
	  Eon Source Position Class - eon::sourcepos

	  A source position is a reference to a specific location within a source.
	  It includes a line number and an [eon::substring] marking the portion of
	  the source that is referenced.
	**************************************************************************/
	class sourcepos
	{
	public:
		//* Default construction, a non-reference
		sourcepos() = default;

		//* Construct as a copy of the 'other' source position
		inline sourcepos( const sourcepos& other ) noexcept { *this = other; }

		//* Construct for first characater of the specified 'source'
		inline sourcepos( const source& source ) noexcept {
			Line = 0; Area.begin() = source.data().begin();
			Area.end() = Area.begin() + 1; }

		//* Construct for the specified 'line' and 'start' position (with end =
		//* start + 1) 
		inline sourcepos( size_t line, const string_iterator& start )
			noexcept { Line = line; Area = substring( start, start + 1 ); }

		//* Construct for the specified 'line' and [eon::substring] 'area'
		inline sourcepos( size_t line, const substring& area ) noexcept {
			Line = line; Area = area; }

		//* Default destruction
		~sourcepos() = default;


		//* Assign as a copy of the 'other' source position
		inline sourcepos& operator=( const sourcepos& other ) noexcept {
			Line = other.Line; Area = other.Area; return *this; }

		//* Set the end of the source position reference
		inline void setEnd( const string_iterator& end ) noexcept {
			Area.end() = end; }


		//* Get the line number referenced
		inline size_t line() const noexcept { return Line; }

		//* Get the referenced area
		const substring& area() const noexcept { return Area; }

		//* Get the character number on the line
		inline size_t pos( const source& source ) const noexcept {
			return Line < source.numLines() ? Area.begin() - source.line(
				Line ).begin() : 0; }

		//* Get the number of characters referenced
		inline size_t numChars() const noexcept { return Area.numChars(); }


		//* Check if 'this' source position is at the end of the 'source'
		bool atEnd( const source& source ) const noexcept;

		//* Get character at 'this' source position
		//* Returns 'eon::nochar' if invalid position for the 'source'.
		char_t chr() const noexcept { return *Area.begin(); }

		//* Move 'this' source position to the next character in the source
		//* Newline will cause line number to increment and position on line to
		//* be zero.
		//* NOTE: The number of characters will be ignored (and set to 1).
		//* NOTE: This method will check [atEnd] and return 'false' if at end.
		bool advance( const source& source ) noexcept;

	private:
		size_t Line{ 0 };
		substring Area;
	};




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
		sourceref() noexcept {}

		//* Copy another source reference
		sourceref( const sourceref& rhs ) { *this = rhs; }

		//* Construct for a source, locate to start of that source
		sourceref( const source& src ) { *this = src; }

		//* Construct for a source, specify position within that source
		inline sourceref( const source& src, const sourcepos& pos ) noexcept {
			Src = &src; Pos = pos; }




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
		char_t chr() const noexcept {
			return Src != nullptr ? Pos.chr() : nochar; }

		//* Get the full string referenced by 'this'
		substring str() const noexcept {
			return Src != nullptr ? Pos.area() : substring(); }

		//* Access the source
		//* WARNING: Throws [eon::NoSource] if no source has been set!
		const source& source() const {
			if( Src != nullptr ) return *Src; else throw NoSource(); }




	private:
		const eon::source* Src{ nullptr };
		sourcepos Pos;
	};

}
