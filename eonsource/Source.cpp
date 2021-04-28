#include "Source.h"
#include <fstream>


namespace eon
{
	bool source::load( const string& file_path )
	{
		if( file_path.empty() )
			return false;
		std::ifstream file( file_path.wstr() );
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
