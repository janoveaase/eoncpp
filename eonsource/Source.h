#pragma once
#include <eonstring/String.h>
#include <eonexcept/Exception.h>
#include <vector>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Exception used when attempting to set a source with an empty name.
	**************************************************************************/
	EONEXCEPT( EmptyName );




	/**************************************************************************
	  Eon Source Class - eon::source

	  A source string or file (UTF-8), typically used as part of parsing.
	**************************************************************************/
	class source
	{
	public:
		/**********************************************************************
		  Construction
		**********************************************************************/

		//* Default constructor for empty source
		source() = default;

		//* Construct for a specific name (file name or string name) and take
		//* ownership of the source data
		//* NOTE: The source data can be reclaimed later
		//* WARNING: Throws [eon::EmptyName] if the name is empty!
		inline source( const string& name, string&& data ) {
			if( name.empty() ) throw EmptyName();
			Name = name; Data = std::move( data ); splitLines(); }

		//* Construct by taking ownership of the source data from another
		//* source object
		inline source( source&& other ) noexcept {
			*this = std::move( other ); }


		//* Default destruction
		virtual ~source() = default;



		/**********************************************************************
		  Modifier Methods
		**********************************************************************/

		//* Take ownership of the source data from another source object
		//* WARNING: The source data of 'this' object (if any) will be lost
		inline source& operator=( source&& other ) noexcept {
			Name = std::move( other.Name ); Data = std::move( other.Data );
			Lines = std::move( other.Lines ); return *this; }

		//* Clear this source (its data will be lost)
		inline void clear() noexcept {
			Name.clear(); Data.clear(); Lines.clear(); }


		//* Load source from file
		//* Returns 'true' if successfully loaded, 'false' if empty or non-
		//* existing file path.
		//* WARNING: The source data of 'this' object (if any) will be lost
		bool load( const string& file_path );


		//* Reclaim the source
		//* This will effectively clear 'this' source
		inline string&& reclaim() noexcept {
			Name.clear(); Lines.clear(); return std::move( Data ); }
		


		/**********************************************************************
		  Read-only Methods
		**********************************************************************/

		//* Get source name
		inline const string& name() const noexcept { return Name; }

		//* Get number of lines
		inline size_t numLines() const noexcept { return Lines.size(); }

		//* Get a line
		//* Returns invalid substring if number is too high.
		inline substring line( size_t num ) const noexcept {
			return num < Lines.size() ? Lines[ num ] : substring(); }

		//* Access the entire source string data
		inline const string& data() const noexcept { return Data; }




	private:
		inline void splitLines() {
			Lines = Data.splitSequential<std::vector<substring>>( '\n' ); }

	private:
		string Name;
		string Data;
		std::vector<substring> Lines;
	};
}