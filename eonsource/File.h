#pragma once
#include "Raw.h"
#include <fstream>


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
		  Eon File Source Class - eon::source::File

		  A subclass of [eon::source::Raw], using a file as source
		**********************************************************************/
		class File : public Raw
		{
		public:
			/******************************************************************
			  Construction
			******************************************************************/

			//* Default constructor for empty source
			inline File() = default;

			//* Construct for a named file (which must exist!)
			//* WARNING: Throws [eon::source::BadName] if the name is not for
			//*          an existing file that we can open for reading!
			File( const string& name );


			//* Default destruction
			virtual ~File() { if( Data ) Data.close(); };




			/******************************************************************
			  Read-only Methods
			******************************************************************/

			//* Get number of bytes in source
			inline size_t numBytes() const noexcept override { return NumBytes; }

			//* Get position a number of characters (not bytes!) ahead
			//* Throws [eon::source::EndOfSource] if already at the end
			//* Returns moved position - which may be less than num_characters
			//*         from the start point if end of source was reached.
			Pos push( Pos start_point, size_t num_characters ) override;

			//* Get position a number of characters (not bytes!) prior to the
			//* start point, but not at or before end point
			//* NOTE: The end point must be before the start point!
			//* Throws [eon::source::EndOfSource] if already at the end point
			//* Returns moved position - which may be less than num_characters
			//*         from the start point if end of source was reached.
			Pos pull( Pos start_point, Pos end_point, size_t num_characters ) override;

			//* Check if the specified portion of the source matches exactly
			//* with the given string value
			inline bool match( const eon::string& value, Pos start, Pos end ) const noexcept override {
				return ((File*)this)->str( start, end ) == value; }
			bool match( const char* value, Pos start, Pos end ) const noexcept override {
				return ( (File*)this )->str( start, end ) == value; }

			//* Get characater at specified position
			//* Returns [eon::nochar] if at or beyond source end!
			char_t chr( const Pos& pos ) noexcept override;

			//* Get string at specified area
			//* Returns empty string if not a valid area or the entire area is
			//* outside the scope of the source!
			string str( Pos start, Pos end ) noexcept override;



		private:
			std::ifstream Data;
			size_t NumBytes{ 0 };
		};
	}
}