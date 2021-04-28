#include "Source.h"
#include <fstream>


namespace eon
{
	bool source::load( const string& file_path )
	{
		if( file_path.empty() )
			return false;
#ifdef EON_WINDOWS
		std::ifstream file( file_path.wstr() );
#else
		std::ifstream file( file_path.stdstr() );
#endif
		if( !file )
			return false;
		std::string data( std::istreambuf_iterator<char>( file ),
			std::istreambuf_iterator<char>() );
		Data = std::move( data );
		Name = file_path;
		Lines = Data.splitSequential<std::vector<substring>>( '\n' );
		return true;
	}
}
