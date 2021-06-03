#pragma once

#include "FileBuffer.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon BinaryFileReader Class - eon::binaryfilereader

	  Open and read binary files - with no modifications to the raw byte data.
	**************************************************************************/
	class binaryfilereader
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:
		binaryfilereader() = default;
		inline binaryfilereader( const binaryfilereader& ) = delete;
		inline binaryfilereader( binaryfilereader&& other ) noexcept {
			*this = std::move( other ); }

		inline binaryfilereader( const path& file_path ) {
			Buffer = filebuffer( file_path, FileMode::input ); }

		~binaryfilereader() { close(); }


		binaryfilereader& operator=( const binaryfilereader& ) = delete;
		inline binaryfilereader& operator=( binaryfilereader&& other ) noexcept {
			Buffer = std::move( other.Buffer ); }




		/**********************************************************************
		  Opening and Closing
		**********************************************************************/
	public:

		//* Open file for reading
		//* Throws [eon::FileOpenError] if unable to open.
		inline void open() { Buffer.open(); }

		//* Close the binaryfilereader
		inline void close() { Buffer.close(); }



		/******************************************************************
		  Operations
		******************************************************************/
	public:

		//* Read the next UTF-8 character
		//* Returns -1 if you try to read past the end!
		inline int read() { byte_t byte;
			if( Buffer.read( byte ) ) return byte; else return -1; }

		//* Read a number of bytes
		//* If the file ends before 'max_chars' has been read, the size of the
		//* returned string will reflect that. (It will be empty if already at
		//* end of file.)
		std::string read( size_t max_chars );


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
