#pragma once

#include "FileBuffer.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon TextFileWriter Class - eon::textfilewriter

	  Open and write text files (UTF-8) with line endings converted to \r\n on
	  Windows.
	**************************************************************************/
	class textfilewriter
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:
		textfilewriter() = default;
		inline textfilewriter( const textfilewriter& ) = delete;
		inline textfilewriter( textfilewriter&& other ) noexcept {
			*this = std::move( other ); }

		inline textfilewriter( const path& file_path ) {
			Buffer = filebuffer( file_path, FileMode::output ); }

		~textfilewriter() { close(); }


		textfilewriter& operator=( const textfilewriter& ) = delete;
		inline textfilewriter& operator=( textfilewriter&& other ) noexcept {
			Buffer = std::move( other.Buffer ); return *this; }




		/**********************************************************************
		  Opening and Closing
		**********************************************************************/
	public:

		//* Open file for writing
		//* Throws [eon::FileOpenError] if unable to open.
		inline void open() { Buffer.open(); }

		//* Close the textfilereader
		inline void close() { Buffer.close(); }



		/******************************************************************
		  Operations
		******************************************************************/
	public:

		//* Write a new UTF-8 characther
		void write( char_t chr );

		//* Write a UTF-8 string
		inline void write( const string& str ) {
			for( auto chr : str ) write( chr ); }

		//* Write a UTF-8 string and start a new line
		inline void writeLine( const string& str ) {
			write( str ); write( NewlineChr ); }


		//* Flush buffer to file
		inline void flush() { Buffer.flush(); }




		///////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:
		filebuffer Buffer;
	};
}
