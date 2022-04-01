#pragma once

#include <eonstring/String.h>
#include <eonexcept/Exception.h>
#include "Path.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace filesys
	{
		//* File open modes
		//* A file can be opened for input or for output.
		enum class mode : uint8_t
		{
			input,
			output
		};

		//* Exception thrown when unable to open file
		EONEXCEPT( OpenError );

		//* Exception thrown when unable to read or write
		EONEXCEPT( RWError );
	}



	/**************************************************************************
	  Eon File Buffer Class - eon::filebuffer

	  Handle the low level file access and buffering.
	  This is a utility class and should not be used directly. Use
	  [eon::TextFileReader], [eon::TextFileWriter], [eon::BinaryFileReader] or
	  [eon::BinaryFileWriter] instead!
	**************************************************************************/
	class filebuffer
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:
		filebuffer() = default;
		inline filebuffer( const filebuffer& ) = delete;
		inline filebuffer( filebuffer&& other ) noexcept { *this = std::move( other ); }

		inline filebuffer( const path& file_path, filesys::mode mode, int buffer_size = 4096 ) {
			Path = file_path; Mode = mode; BufferCapacity = buffer_size; }

		~filebuffer() { close(); }


		filebuffer& operator=( const filebuffer& ) = delete;
		filebuffer& operator=( filebuffer&& other ) noexcept;




		/**********************************************************************
		  Read-only Methods
		**********************************************************************/
	public:

		inline const path& filePath() const noexcept { return Path; }

		inline size_t bufferSize() const noexcept { return BufferCapacity; }

		inline bool eof() const noexcept { return Eof; }




		/**********************************************************************
		  Opening and Closing
		**********************************************************************/
	public:

		//* Open file
		//* Throws [eon::filesys::OpenError] if unable to open.
		void open();

		//* Close file
		void close();



		/**********************************************************************
		  Input

		  WARNING: These methods won't check if the file is open for reading!
		**********************************************************************/
	public:

		//* Read the next raw byte
		//* Returns false if end of file.
		bool read( byte_t& byte );

		//* Read the next UTF-8 character
		//* Returns false if end of file.
		bool read( char_t& chr );



		/**********************************************************************
		  Output

		  WARNING: These methods won't check if the file is open for writing!
		**********************************************************************/
	public:

		//* Write a new raw byte to the buffer
		void write( byte_t byte );

		//* Write a new UTF-8 characater to the buffer
		void write( char_t chr );


		//* Flush buffer to file
		void flush();




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:
		void _readBuffer();




		///////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:
		path Path;
		int Handle{ -1 };
		filesys::mode Mode{ filesys::mode::input };
		bool Eof{ false };
		char* Buffer{ nullptr };
		int BufferCapacity{ 4096 };
		int BufferSize{ 0 };
		size_t BufPos{ 0 };
	};
}
