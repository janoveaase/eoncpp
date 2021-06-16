#include "Parser.h"


namespace eon
{
	parser::parser( const std::vector<string>& args ) noexcept
	{
		try
		{
			_parseArgs( args );
		}
		catch( exception& e )
		{
			std::cerr << "ERROR: " << e.details().stdstr() << "\n";
			State = statecode::error;
		}
	}




	int parser::run() noexcept
	{
		switch( State )
		{
			case statecode::error:
				return 4;
			case statecode::usage:
				_showUsage();
				return 0;
			default:
				try
				{
					return _run();
				}
				catch( exception& e )
				{
					std::cerr << "ERROR: " << e.details().stdstr() << "\n";
					return 4;
				}
		}
	}



	void parser::_showUsage()
	{
		std::cout << "Eon Parser Usage\n";
		std::cout << App.base() + " <input> <output> <how>\n";
		std::cout << "  input=<source>  : The source can be a file path, a directory path,\n";
		std::cout << "                    (ending in a slahs), a directory path (optionally\n";
		std::cout << "                    ending in slash) followed by '::' and a regular\n";
		std::cout << "                    expression identifying files within it, or or a\n";
		std::cout << "                    comma-separated list of any combinations of the\n";
		std::cout << "                    above. Directories witout a pattern won't be\n";
		std::cout << "                    scanned, but will be available for EOF speci-\n";
		std::cout << "                    fied scanning.\n";
		std::cout << "  output=<dir>    : Directory where to write output file(s).\n";
		std::cout << "  how=<eof>       : Path to an EOF file that defines how to do the\n";
		std::cout << "                    parsing.\n";
	}
	void parser::_parseArgs( const std::vector<string>& args )
	{
		App = args[ 0 ];
		for( size_t i = 1; i < args.size(); ++i )
		{
			auto& arg = args[ i ];
			if( arg == "--help" || arg == "-h" || arg == "help" )
				State = statecode::usage;
			else if( arg.startsWith( "input=" ) )
				_parseInput( arg.substr( arg.begin() + 6 ) );
			else if( arg.startsWith( "output=" ) )
				OutputDir = arg.substr( arg.begin() + 7 );
			else if( arg.startsWith( "how=" ) )
				Docs.load( path( arg.substr( arg.begin() + 4 ) ) );
			else
			{
				std::cerr << "Unknown argument: " << arg << "\n";
				State = statecode::error;
			}
		}
	}
	void parser::_parseInput( const substring& input )
	{
		auto sources = input.splitSequential<std::vector<string>>( ',' );
		for( auto& source : sources )
		{
			auto pos = source.findFirst( "::" );
			if( pos )
			{
				path dir = source.substr( source.begin(), pos.begin() );
				dir.noEndSlash();
				regex pattern = source.substr( pos.end() );
				InputDirs.insert( dir );
				ScanDirs.push( std::make_pair(
					std::move( dir ), std::move( pattern ) ) );
			}
			else
			{
				path item = source;
				if( item.str().endsWith( "/" ) )
					InputDirs.insert( item.noEndSlash() );
				else
					InputFiles.push( std::move( item ) );
			}
		}
	}

	int parser::_run()
	{
		return 0;
	}
}
