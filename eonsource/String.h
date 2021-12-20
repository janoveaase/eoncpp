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
		  Eon String Source Class - eon::source::String

		  A subclass of [eon::source::Raw], holding a string
		**********************************************************************/
		class String : public Raw
		{
		public:
			/******************************************************************
			  Construction
			******************************************************************/

			//* Default constructor for empty source
			String() = default;

			//* Construct for a string (that we take ownership over). A name
			//* (for reference purposes) is required and it cannot be empty!
			//* NOTE: The source data can be reclaimed later!
			//* WARNING: Throws [eon::source::BadName] if the name is empty!
			inline String( const string& name, string&& source_data ) {
				if( name.empty() ) throw BadName(); Name = name; Data = std::move( source_data ); }


			//* Default destruction
			virtual ~String() = default;




			/******************************************************************
			  Modifier Methods
			******************************************************************/

			//* Reclaim the source data string
			//* This will effectively clear 'this' source
			inline string&& reclaim() noexcept { Name.clear(); return std::move( Data ); }




			/******************************************************************
			  Read-only Methods
			******************************************************************/

			//* Get number of bytes in source
			inline size_t numBytes() const noexcept override { return Data.numBytes(); }

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
				return Data.substr( start, end ) == value.substr(); }
			inline bool match( const char* value, Pos start, Pos end ) const noexcept override {
				return Data.substr( start, end ) == substring( value ); }

			//* Get characater at specified position
			//* Returns [eon::nochar] if at or beyond source end!
			inline char_t chr( const Pos& pos ) noexcept override {
				return *string::iterator( Data.c_str(), Data.numBytes(), Data.numChars(),
					Data.c_str() + pos.bytePos(), pos.charPos() ); }

			//* Get string at specified area
			//* Returns empty string if not a valid area or the entire area is
			//* outside the scope of the source!
			string str( Pos start, Pos end ) noexcept override;



		private:
			string Data;
			std::vector<size_t> LineLengths;
		};
	}
}