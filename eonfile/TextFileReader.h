#pragma once

#include "FileBuffer.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon TextFileReader Class - eon::textfilereader

	  Open and read text files (UTF-8) with line endings always converted to \n
	  regardless of platform.
	**************************************************************************/
	class textfilereader
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:
		textfilereader() = default;
		inline textfilereader( const textfilereader& ) = delete;
		inline textfilereader( textfilereader&& other ) noexcept {
			*this = std::move( other ); }

		inline textfilereader( const path& file_path ) {
			Buffer = filebuffer( file_path, FileMode::input ); }

		~textfilereader() { close(); }


		textfilereader& operator=( const textfilereader& ) = delete;
		inline textfilereader& operator=( textfilereader&& other ) noexcept {
			Buffer = std::move( other.Buffer ); }




		/**********************************************************************
		  Opening and Closing
		**********************************************************************/
	public:

		//* Open file for reading
		//* Throws [eon::FileOpenError] if unable to open.
		inline void open() { Buffer.open(); }

		//* Close the textfilereader
		inline void close() { Buffer.close(); }



		/******************************************************************
		  Operations
		******************************************************************/
	public:

		//* Read the next UTF-8 character
		//* Returns [eon::eof] if you try to read past the end!
		char_t read();

		//* Read a number of UTF-8 characters
		//* If the file ends before 'max_chars' has been read, the size of the
		//* returned string will reflect that. (It will be empty if already at
		//* end of file.)
		string read( size_t max_chars );

		//* Read everything up to the end of the line - or end of file
		//* (whichever comes first), reposition to the start of the next line.
		//* The newline character is skipped.
		string readLine();


		//* Check if end-of-file marker has been reached
		inline bool eof() const noexcept { return Buffer.eof(); }




		///////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:
		filebuffer Buffer;
	};
}
