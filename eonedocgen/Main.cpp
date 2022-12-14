#include "Generator.h"
#include <eonterminal/Terminal.h>
#include <eonfilesys/Path.h>

using namespace eon;


int showUsage( path exe );


int main( int argc, char* argv[] )
{
	std::vector<string> args;
	path exe = argv[ 0 ];
	for( int i = 0; i < argc; ++i )
	{
		string arg;
		for( auto c = (unsigned char*)argv[ i ]; *c != '\0'; ++c )
		{
			if( *c < 128 )
				arg << *c;
			else if( *c >= 160 )
			{
				char_t ch = *c;
				arg << ch;
			}
		}
		if( arg == "--help" )
			return showUsage( exe.base() );
		args.push_back( arg );
	}
	if( args.size() == 1 )
		return showUsage( exe.base() );
	if( args.rbegin()->startsWith( "--" ) )
		return showUsage( exe.base() );

	try
	{
		doc::Generator generator( std::move( args ) );
		return generator.run();
	}
	catch( ... )
	{
		return showUsage( exe.base() );
	}
}

int showUsage( path exe )
{
	term << style::strong << "Usage:" << style::normal << " " << style::yellow << exe.str() << style::normal;
	term << " " << style::weak << "[" << style::normal << "options" << style::weak << "]" << style::normal;
	term << " " << style::weak << "<" << style::magenta << "source" << style::weak << ">" << style::normal;
	term << " " << style::weak << "[<" << style::cyan << "target" << style::weak << ">]" << style::normal;
	term << "\n\n";
	term << "Options:\n";
	term << "  " << style::yellow << "--help" << style::normal << "        : Show usage/help text!\n";
	term << "  " << style::yellow << "--verbose" << style::normal << "     : Be verbose (default)!\n";
	term << "  " << style::yellow << "--silent" << style::normal << "      : Be silent (no output)!\n";
	term << "  " << style::yellow << "--title=" << style::weak << "<" << style::yellow << "text" << style::weak << ">"
		<< style::normal << ": Use this " << style::yellow << "text" << style::normal << " as title for the site!\n";
	term << "  " << style::yellow << "--edt" << style::normal << "         : Save as " << style::yellow << "edt"
		<< style::normal << " rather than " << style::yellow << "html" << style::normal << " when writing to "
		<< style::cyan << "target" << style::normal << ".\n";
	term << "\n";
	term << "To use a file as source, specify " << style::magenta << "source" << style::normal << " with " << style::yellow
		<< ".edoc" << style::normal << " extension.\n";
	term << "To generate for all " << style::yellow << ".edoc" << style::normal << " within a directory, make "
		<< style::magenta << "source" << style::normal << " end in a slash.\n";
	term << "If " << style::cyan << "target" << style::normal
		<< " ends in a slash then output file(s) will have the same base\n";
	term << " name as source, except extension will be switched from " << style::yellow << ".edoc" << style::normal
		<< " to " << style::yellow << ".html" << style::normal << ".\n";
	term << "If " << style::magenta << "source" << style::normal << " ends with a slash then " << style::cyan << "target"
		<< style::normal << " must also!\n";
	term << "If " << style::cyan << "target" << style::normal
		<< " is not specified, then HTML file(s) will be created in the\n";
	term << "directory of the " << style::magenta << "source" << style::normal << ".\n";
	return 0;
}
