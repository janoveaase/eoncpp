#pragma once

#include "FileBuffer.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon BinaryFileWriter Class - eon::binaryfilewriter

	  Open and write binary files - with no modifications to the raw byte data.
	**************************************************************************/
	class binaryfilewriter
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:
		binaryfilewriter() = default;
		inline binaryfilewriter( const binaryfilewriter& ) = delete;
		inline binaryfilewriter( binaryfilewriter&& other ) noexcept {
			*this = std::move( other ); }

		inline binaryfilewriter( const path& file_path ) {
			Buffer = filebuffer( file_path, FileMode::output ); }

		~binaryfilewriter() { close(); }


		binaryfilewriter& operator=( const binaryfilewriter& ) = delete;
		inline binaryfilewriter& operator=( binaryfilewriter&& other ) noexcept {
			Buffer = std::move( other.Buffer ); return *this; }




		/**********************************************************************
		  Opening and Closing
		**********************************************************************/
	public:

		//* Open file for writing
		//* Throws [eon::FileOpenError] if unable to open.
		inline void open() { Buffer.open(); }

		//* Close the binaryfilewriter
		inline void close() { Buffer.close(); }



		/******************************************************************
		  Operations
		******************************************************************/
	public:

		//* Write a new raw byte
		inline void write( byte_t byte ) { Buffer.write( byte ); }

		//* Write a sequence of raw bytes
		inline void write( const std::string& str ) {
			for( auto byte : str ) write( byte ); }
		inline void write( const char* str ) {
			for( auto byte = str; *byte != '\0'; ++byte ) write( *byte ); }


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
