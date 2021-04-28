#include "FileSource.h"


FileSource::FileSource( const String& filename ) : Source()
{
	Name = filename;
	File file( filename );
	file.openIn();
	while( !file.eof() )
		Lines.push_back( file.readLine() + "\n" );
}
