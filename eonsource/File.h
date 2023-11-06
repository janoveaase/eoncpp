#pragma once
#include "Raw.h"
#include <fstream>


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
		// Eon File Source Class - eon::source::File
		//
		// A subclass of [eon::source::Raw], using a file as source
		//
		class File : public Raw
		{
		public:
			///////////////////////////////////////////////////////////////////
			//
			// Construction
			//

			// Default constructor for empty source
			inline File() = default;

			// Construct for a named file (which must exist!)
			// WARNING: Throws [eon::source::BadName] if the name is not for
			//          an existing file that we can open for reading!
			File( const string& name );


			// Default destruction
			virtual ~File() { if( Data ) Data.close(); };




			///////////////////////////////////////////////////////////////////
			//
			// Read-only Methods
			//
		public:

			inline size_t numBytesInSource() const noexcept override { return NumBytes; }

			// Given a base position and an offset (in characaters, not bytes!),
			// get a source::Pos object for that offset.
			// If 'offset_chars' refers to a character before start or after
			// end of source, it will be adjusted to fit.
			// Returns 'base_position' if 'offset_chars' is adjusted to zero!
			Pos getPosAtOffset( Pos base_position, int offset_chars = 1 ) override;

			// Check if the portion of the source between 'start' and 'end' matches the specified string 'value'.
			inline bool match( const Pos& start, const Pos& end, const eon::string& value ) const noexcept override {
				return ( (File*)this )->str( start, end ) == value; }

			// Check if the portion of the source between 'start' and 'end' matches the specified string 'value'.
			bool match( const Pos& start, const Pos& end, const char* value ) const noexcept override {
				return ( (File*)this )->str( start, end ) == value; }

			// Get characater at specified position
			// Returns [eon::nochar] if at or beyond source end!
			char_t chr( const Pos& pos ) noexcept override;

			// Get byte at specified position
			// Returns -1 if at or beyond source end!
			int byte( index_t pos ) noexcept override;

			// Get string at specified area
			// Returns empty string if not a valid area or the entire area is
			// outside the scope of the source!
			string str( Pos start, Pos end ) noexcept override;

			// Get bytes at specified area.
			// Returns empty if not a valid area or the entire area is outside the scope of the source!
			std::string bytes( Pos start, Pos end ) noexcept override;




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		EON_PRIVATE:

			void _forward( Pos& pos, index_t num_chars );
			void _backward( Pos& pos, index_t num_chars );

			void _scanFile();




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

			std::ifstream Data;
			size_t NumBytes{ 0 };
		};
	}
}