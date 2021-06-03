#include "TextFileReader.h"

namespace eon
{
	char_t textfilereader::read()
	{
		char_t chr;
		while( !Buffer.eof() )
		{
			if( !Buffer.read( chr ) )
				break;
			if( chr != CReturnChr )
				return chr;
		}
		return eon::eof;
	}
	string textfilereader::read( size_t max_chars )
	{
		string str;
		char_t chr;
		for( size_t i = 0; i < max_chars && Buffer.read( chr ); ++i )
		{
			if( chr == CReturnChr )
				continue;
			str += chr;
		}
		return str;
	}
	string textfilereader::readLine()
	{
		string str;
		char_t chr;
		for( size_t i = 0; Buffer.read( chr ) && chr != NewlineChr; ++i )
		{
			if( chr == CReturnChr )
				continue;
			str += chr;
		}
		return str;
	}
}
