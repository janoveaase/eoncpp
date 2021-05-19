#include "BinaryFileReader.h"

namespace eon
{
	std::string binaryfilereader::read( size_t max_bytes )
	{
		std::string str;
		byte_t byte;
		for( size_t i = 0; i < max_bytes && Buffer.read( byte ); ++i )
			str += byte;
		return str;
	}
}
