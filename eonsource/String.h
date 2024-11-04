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
		// Eon String Source Class - eon::source::String
		//
		// A subclass of [eon::source::Raw], holding a string
		//
		class String : public Raw
		{
			///////////////////////////////////////////////////////////////////
			//
			// Construction
			//
		public:

			// Default constructor for empty source
			String() = default;

			// Construct for a string (that we take ownership over). A name
			// (for reference purposes) is required and it cannot be empty!
			// NOTE: The source data can be reclaimed later!
			// WARNING: Throws [eon::source::BadName] if the name is empty!
			inline String( const string& name, string&& source_data ) : Raw( name ), Data( std::move( source_data ) ) {}




			///////////////////////////////////////////////////////////////////
			//
			// Modifier Methods
			//
		public:

			// Reclaim the source data string.
			// This will effectively clear 'this' source!
			inline string&& reclaim() noexcept { _clearName(); return std::move( Data ); }




			///////////////////////////////////////////////////////////////////
			//
			// Read-only Methods
			//
		public:

			inline size_t numBytesInSource() const noexcept override { return Data.numBytes(); }

			// Given a base position and an offset (in characaters, not bytes!),
			// get a source::Pos object for that offset.
			// If 'offset_chars' refers to a character before start or after
			// end of source, it will be adjusted to fit.
			// Returns 'base_position' if 'offset_chars' is adjusted to zero!
			Pos getPosAtOffset( Pos base_position, int offset_chars = 1 ) override;

			// Check if the portion of the source between 'start' and 'end' matches the specified string 'value'.
			inline bool match( const Pos& start, const Pos& end, const eon::string& value ) const noexcept override {
				return Data.substr( start.CharPos, end.CharPos - start.CharPos ) == value.substr(); }

			// Check if the portion of the source between 'start' and 'end' matches the specified string 'value'.
			bool match( const Pos& start, const Pos& end, const char* value ) const noexcept override {
				return Data.substr( start.CharPos, end.CharPos - start.CharPos ) == substring( value ); }

			// Get characater at specified position.
			// Returns [eon::nochar] if at or beyond source end!
			inline char_t chr( const Pos& pos ) noexcept override {
				return *string::iterator(
					Data.c_str(), Data.numBytes(), Data.numChars(), Data.c_str() + pos.BytePos, pos.CharPos ); }

			// Get byte at specified position
			// Returns -1 if at or beyond source end!
			inline int byte( index_t pos ) noexcept override { return pos < Data.numBytes() ? Data.byte( pos ) : -1; }

			// Get string at specified area.
			// Returns empty string if not a valid area or the entire area is
			// outside the scope of the source!
			string str( const Pos& start, const Pos& end ) noexcept override;

			// Get bytes at specified area.
			// Returns empty if not a valid area or the entire area is outside the scope of the source!
			std::string bytes( const Pos& start, const Pos& end ) noexcept override;




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		EON_PRIVATE:

			Pos _realEnd( const Pos& end ) const noexcept;

			void _forward( Pos& pos, index_t num_chars );
			void _backward( Pos& pos, index_t num_chars );

			void _scanString();




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		EON_PRIVATE:

			string Data;
			std::vector<size_t> NumCharsOnLines;
		};
	}
}