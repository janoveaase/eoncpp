#pragma once

#include "FileDefs.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
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
		inline filebuffer( filebuffer&& other ) noexcept {
			*this = std::move( other ); }

		inline filebuffer( const path& file_path, FileMode mode,
			int buffer_size = 4096 ) {
				FPath = file_path; Mode = mode; BufferCapacity = buffer_size; }

		~filebuffer() { close(); }


		filebuffer& operator=( const filebuffer& ) = delete;
		filebuffer& operator=( filebuffer&& other ) noexcept;




		/**********************************************************************
		  Read-only Methods
		**********************************************************************/
	public:

		inline const path& filePath() const noexcept { return FPath; }

		inline size_t bufferSize() const noexcept { return BufferCapacity; }

		inline bool eof() const noexcept { return Eof; }




		/**********************************************************************
		  Opening and Closing
		**********************************************************************/
	public:

		//* Open file
		//* Throws [eon::FileOpenError] if unable to open.
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
		path FPath;
		int Handle{ -1 };
		FileMode Mode{ FileMode::input };
		bool Eof{ false };
		char* Buffer{ nullptr };
		int BufferCapacity{ 4096 };
		int BufferSize{ 0 };
		size_t BufPos{ 0 };
	};
}
