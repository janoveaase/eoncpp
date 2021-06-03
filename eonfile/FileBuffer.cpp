#include "FileBuffer.h"

#ifndef EON_WINDOWS
#	define _close close
#	define _read read
#	define _write write
#endif

namespace eon
{
	string errnoMessage()
	{
		char buffer[ 80 ];
		_strerror_s<80>( buffer, NULL );
		return string( buffer );
	}




	filebuffer& filebuffer::operator=( filebuffer&& other ) noexcept
	{
		close();
		FPath = std::move( other.FPath );
		Handle = other.Handle; other.Handle = -1;
		Mode = other.Mode;
		Buffer = other.Buffer; other.Buffer = nullptr;
		BufferCapacity = other.BufferCapacity; other.BufferCapacity = 4096;
		BufferSize = other.BufferSize; other.BufferSize = 0;
		BufPos = other.BufPos; other.BufPos = 0;
		Eof = other.Eof;
		return *this;
	}




	void filebuffer::open()
	{
		close();
		int mode = _O_BINARY;
		int sharing = 0;
		int perm = _S_IREAD;
		switch( Mode )
		{
			case FileMode::input:
				mode |= _O_RDONLY;
				sharing |= _SH_DENYNO;
				break;
			case FileMode::output:
				mode |= _O_CREAT | _O_TRUNC | _O_WRONLY;
				sharing |= _SH_DENYWR;
				perm |= _S_IWRITE;
				break;
		}
		auto error = _sopen_s( &Handle, FPath.str().c_str(),
			mode, sharing, perm );
		if( error != 0 )
			throw FileOpenError( "File: " + FPath.stdstr() + "\nError: "
				+ errnoMessage().stdstr() );
		Buffer = new char[ BufferCapacity ];
		BufferSize = 0;
		BufPos = 0;
		Eof = false;
	}

	void filebuffer::close()
	{
		if( Handle != -1 )
		{
			if( Mode == FileMode::output )
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
				size = string_iterator::bytesToUnicode(
					Buffer + BufPos, Buffer + BufferSize, chr );
				BufPos += size;
				break;
			}
			return size > 0;
		}
		return false;
	}




	void filebuffer::write( byte_t byte )
	{
		if( BufferSize - BufPos < 1 )
			flush();
		*( Buffer + BufPos ) = byte;
		++BufPos;
	}
	void filebuffer::write( char_t chr )
	{
		if( BufferSize - BufPos < 4 )
			flush();
		string bytes{ chr };
		memcpy( Buffer + BufPos, bytes.c_str(), bytes.numBytes() );
		BufPos += bytes.numBytes();
	}

	void filebuffer::flush()
	{
		if( BufPos == 0 )
			return;
		auto bytes = _write( Handle, Buffer, static_cast<int>( BufPos ) );
		if( bytes < 0 )
			throw FileIOError( "File: " + FPath.stdstr()
				+ "\nError: " + errnoMessage().stdstr() );
		BufPos = 0;
	}




	void filebuffer::_readBuffer()
	{
		auto bytes = _read( Handle, Buffer, BufferCapacity );
		if( bytes >= 0 )
			BufferSize = bytes;
		else
			throw FileIOError( "File: " + FPath.stdstr() + "\nError: "
				+ errnoMessage().stdstr() );
		BufPos = 0;
	}
}
