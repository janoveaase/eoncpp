#pragma once
#include "Source.h"
#include <common/File.h>
#include <fstream>

/*
	A FileSource is a file with eon code
*/

class FileSource : public Source
{
public:
	SOURCE_SETUP( FileSource );
	FileSource( const String& filename );
};
