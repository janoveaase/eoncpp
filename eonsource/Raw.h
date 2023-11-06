#pragma once
#include "SourcePos.h"
#include <eonstring/String.h>
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
		// Exception used when attempting to set a source with an empty name -
		// or name a file that does not exist or cannot be opened for reading.
		EONEXCEPT( BadName );


		// Exception used when attempting to move beyond the end of the source
		EONEXCEPT( EndOfSource );




		///////////////////////////////////////////////////////////////////////
		//
		// Eon Raw Source Class - eon::source::raw
		//
		// Super-class for various source types, typically to be parsed/scanned.
		//
		class Raw
		{
			///////////////////////////////////////////////////////////////////
			//
			// Read-only Methods
			//
		public:

			inline const string& sourceName() const noexcept { return Name; };

			virtual size_t numBytesInSource() const noexcept = 0;

			// Given a base position and an offset (in characaters, not bytes!),
			// get a source::Pos object for that offset.
			// If 'offset_chars' refers to a character before start or after
			// end of source, it will be adjusted to fit.
			// Returns 'base_position' if 'offset_chars' is adjusted to zero!
			virtual Pos getPosAtOffset( Pos base_position, int offset_chars = 1 ) = 0;

			// Check if specified position is after the last element in the source.
			inline bool afterLast( const Pos& pos ) const noexcept { return pos.BytePos >= numBytesInSource(); }

			// Check if specified position is at or after start and before or at end of the source.
			inline bool isValid( const Pos& pos ) const noexcept { return pos.BytePos <= numBytesInSource(); }

			// Check if the portion of the source between 'start' and 'end' matches the specified string 'value'.
			virtual bool match( const Pos& start, const Pos& end, const eon::string& value ) const noexcept = 0;

			// Check if the portion of the source between 'start' and 'end' matches the specified string 'value'.
			virtual bool match( const Pos& start, const Pos& end, const char* value ) const noexcept = 0;

			// Get characater at specified position
			// Returns [eon::nochar] if at or beyond source end!
			virtual char_t chr( const Pos& pos ) noexcept = 0;

			// Get byte at specified position
			// Returns -1 if at or beyond source end!
			virtual int byte( index_t pos ) noexcept = 0;

			// Convert the specified portion of 'this' source into a string value.
			// The returned string will be empty if the portion is invalid or
			// entirely outside the scope of the source.
			virtual string str( Pos start, Pos end ) noexcept = 0;

			// Convert the specified portion of 'this' source into a bytes value.
			// The returned std::string will be empty if the portion is invalid
			// or entirely outside the scope of the source.
			virtual std::string bytes( Pos start, Pos end ) noexcept = 0;




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		EON_PROTECTED:

			string Name;
			std::vector<size_t> NumCharsOnLines;
		};
	}
}