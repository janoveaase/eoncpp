#include "FileBuffer.h"

#include <fcntl.h>
#ifdef EON_WINDOWS
#	include <io.h>
#else
#	include <errno.h>
#	include <sys/stat.h>
#	include <fcntl.h>
#	include <unistd.h>
#	define _close ::close
#	define _read ::read
#	define _write ::write
#endif

namespace eon
{
	string errnoMessage()
	{
#ifdef EON_WINDOWS
		char buffer[ 80 ];
		_strerror_s<80>( buffer, NULL );
#else
		const char* buffer = strerror( errno );
#endif
		return string( buffer );
	}




	filebuffer& filebuffer::operator=( filebuffer&& other ) noexcept
	{
		close();
		Path = std::move( other.Path );
		Handle = other.Handle; other.Handle = -1;
		Mode = other.Mode;
		Buffer = other.Buffer; other.Buffer = nullptr;
		BufferCapacity = other.BufferCapacity; other.BufferCapacity = 4096;
		BufferSize = other.BufferSize; other.BufferSize = 0;
		BufPos = other.BufPos; other.BufPos = 0;
		Eof = other.Eof;
		return *this;
	}




#ifdef EON_WINDOWS
	void filebuffer::open()
	{
		close();
		int mode = _O_BINARY;
		int sharing = 0;
		int perm = _S_IREAD;
		switch( Mode )
		{
			case filesys::mode::input:
				mode |= _O_RDONLY;
				sharing |= _SH_DENYNO;
				break;
			case filesys::mode::output:
				mode |= _O_CREAT | _O_TRUNC | _O_WRONLY;
				sharing |= _SH_DENYWR;
				perm |= _S_IWRITE;
				break;
		}
		auto error = _wsopen_s( &Handle, Path.str().wstr().c_str(), mode, sharing, perm );
		if( error != 0 )
			throw filesys::OpenError( "File: " + Path.str() + "\nError: " + errnoMessage().stdstr() );
		Buffer = new char[ BufferCapacity ];
		BufferSize = 0;
		BufPos = 0;
		Eof = false;
	}
#else
	void filebuffer::open()
	{
		close();
		int flags = 0;
		int perm = S_IRUSR | S_IWUSR;
		switch( Mode )
		{
			case filesys::mode::input:
				flags |= O_RDONLY;
				break;
			case filesys::mode::output:
				flags |= O_CREAT | O_TRUNC | O_WRONLY;
				break;
		}
		Handle = ::open( Path.c_str(), flags, perm );
		if( Handle == -1 )
			throw filesys::OpenError( "File: " + Path.str() + "\nError: " + errnoMessage().stdstr() );
		Buffer = new char[ BufferCapacity ];
		BufferSize = 0;
		BufPos = 0;
		Eof = false;
	}
#endif

	void filebuffer::close()
	{
		if( Handle != -1 )
		{
			if( Mode == filesys::mode::output )
				flush();
			_close( Handle );
			Handle = -1;
			if( Buffer != nullptr )
			{
				delete[] Buffer;
				Buffer = nullptr;
			}
		}
	}




	bool filebuffer::read( byte_t& byte )
	{
		if( BufPos == BufferSize )
		{
			if( BufferSize > 0 && BufferSize < BufferCapacity )
				return false;
			_readBuffer();
		}
		if( BufPos < BufferSize )
		{
			byte = *( Buffer + BufPos );
			++BufPos;
			return true;
		}
		return false;
	}
	bool filebuffer::read( char_t& chr )
	{
		if( BufPos == BufferSize )
		{
			if( BufferSize > 0 && BufferSize < BufferCapacity )
				return false;
			_readBuffer();
		}
		if( BufPos < BufferSize )
		{
			size_t size;
			while( true )
			{
				size = string_iterator::bytesToUnicode( Buffer + BufPos, Buffer + BufferSize, chr );
				BufPos += size;
				break;
			}
			return size > 0;
		}
		return false;
	}




	void filebuffer::write( byte_t byte )
	{
		if( BufPos + 1 >= BufferCapacity )
			flush();
		*( Buffer + BufPos ) = byte;
		++BufPos;
	}
	void filebuffer::write( char_t chr )
	{
		string bytes{ chr };
		if( BufPos + bytes.numBytes() >= BufferCapacity )
			flush();
		memcpy( Buffer + BufPos, bytes.c_str(), bytes.numBytes() );
		BufPos += bytes.numBytes();
	}

	void filebuffer::flush()
	{
		if( BufPos == 0 )
			return;
		auto bytes = _write( Handle, Buffer, static_cast<int>( BufPos ) );
		if( bytes < 0 )
			throw filesys::RWError( "File: " + Path.stdstr() + "\nError: " + errnoMessage().stdstr() );
		BufPos = 0;
	}




	void filebuffer::_readBuffer()
	{
		auto bytes = _read( Handle, Buffer, BufferCapacity );
		if( bytes >= 0 )
			BufferSize = bytes;
		else
			throw filesys::RWError( "File: " + Path.stdstr() + "\nError: " + errnoMessage().stdstr() );
		BufPos = 0;
	}
}
