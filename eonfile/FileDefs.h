#pragma once

#include <eonpath/Path.h>
#include <fcntl.h>
#ifdef EON_WINDOWS
#	include <io.h>
#endif


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Exception throw when unable to open file
	EONEXCEPT( FileOpenError );

	//* Exception thrown when trying to read a file opened for output, or write
	//* to a file opened for input or read or write to a file that has not been
	//* opened.
	EONEXCEPT( FileIOError );

	//* End of file marker
	static const char_t eof{ nochar };


	//* File open modes
	//* A file can be opened for input or for output.
	enum class FileMode : uint8_t
	{
		input,
		output
	};
}
