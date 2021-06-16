#include "Parser.h"


int main( int argc, const char* argv[] )
{
	std::vector<eon::string> args;
	for( int i = 0; i < argc; ++i )
		args.push_back( eon::string( argv[ i ] ) );
	eon::parser parser( args );
	return parser.run();
}
