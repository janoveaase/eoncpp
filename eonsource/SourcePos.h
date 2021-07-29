#pragma once
#include "Source.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
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
}
