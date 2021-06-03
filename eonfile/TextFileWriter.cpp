#include "TextFileWriter.h"

namespace eon
{
	void textfilewriter::write( char_t chr )
	{
#ifdef EON_WINDOWS
		if( chr == NewlineChr )
			Buffer.write( CReturnChr );
#endif
		Buffer.write( chr );
	}
}
