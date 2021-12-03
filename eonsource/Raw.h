#pragma once
#include "SourcePos.h"
#include <eonstring/String.h>


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
		  Exception used when attempting to set a source with an empty name -
		  or name a file that does not exist or cannot be opened for reading.
		**********************************************************************/
		EONEXCEPT( BadName );


		/**********************************************************************
		  Exception used when attempting to move beyond the end of the source
		**********************************************************************/
		EONEXCEPT( EndOfSource );




		/**********************************************************************
		  Eon Raw Source Class - eon::source::raw

		  Super-class for various source types, typically to be parsed/scanned
		**********************************************************************/
		class Raw
		{
		public:
			/******************************************************************
			  Construction
			******************************************************************/

			//* Default constructor for empty source
			Raw() = default;


			//* Default destruction
			virtual ~Raw() = default;



			/******************************************************************
			  Read-only Methods
			******************************************************************/

			//* Get source name
			inline const string& name() const noexcept { return Name; };

			//* Get number of bytes in source
			virtual size_t numBytes() const noexcept = 0;

			//* Get position a number of characters (not bytes!) ahead
			//* Throws [eon::source::EndOfSource] if already at the end
			//* Returns moved position - which may be less than num_characters
			//*         from the start point if end of source was reached.
			virtual Pos push( Pos start_point, size_t num_characters ) = 0;

			//* Get position a number of characters (not bytes!) prior to the
			//* start point, but not at or before end point
			//* NOTE: The end point must be before the start point!
			//* Throws [eon::source::EndOfSource] if already at the end point
			//* Returns moved position - which may be less than num_characters
			//*         from the start point if end of source was reached.
			virtual Pos pull( Pos start_point, Pos end_point, size_t num_characters ) = 0;

			//* Check if specified position is at (or beyond) the end of source
			//* End of source is defined to be one byte beyond last character!
			inline bool atEnd( const Pos& pos ) const noexcept { return pos.bytePos() >= numBytes(); }

			//* Check if specified position is beyond the end of the source
			inline bool beyondEnd( const Pos& pos ) const noexcept { return pos.bytePos() > numBytes(); }

			//* Get characater at specified position
			//* Returns [eon::nochar] if at or beyond source end!
			virtual char_t chr( const Pos& pos ) noexcept = 0;

			//* Get string at specified area
			//* Returns empty string if not a valid area or the entire area is
			//* outside the scope of the source!
			virtual string str( Pos start, Pos end ) noexcept = 0;



		protected:
			string Name;
			std::vector<size_t> LineLengths;	// In number of characters
		};
	}
}