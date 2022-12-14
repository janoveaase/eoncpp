#include "Generator.h"
#include <eonterminal/Terminal.h>


namespace doc
{
	Generator::Generator( std::vector<string>&& args )
	{
		auto arg = args.begin();
		for( ++arg; arg->startsWith( "--" ); ++arg )
		{
			if( arg->startsWith( "--title=" ) )
				Title = arg->substr( arg->begin() + 8 );
			else if( *arg == "--verbose" )
				Verbose = true;
			else if( *arg == "--silent" )
				Verbose = false;
			else if( *arg == "--edt" )
				SaveAsEDT = true;
			else
			{
				term << style::error << "ERROR:" << style::normal << " Unknown option " << style::yellow << *arg
					<< style::normal << "!\n";
				throw 1;	// Throw anything - just get us out of here!
			}
		}
		if( arg->endsWith( '/' ) || arg->endsWith( '\\' ) )
			_scanSources( *arg );
		else
		{
			Sources.insert( *arg );
			Target = Sources.begin()->parent();
		}
		if( ++arg != args.end() )
		{
			if( arg->endsWith( '/' ) || arg->endsWith( '\\' ) )
				Target = *arg;
			else if( Sources.size() == 1 )
				Target = *arg;
			else
			{
				term << style::error << "ERROR:" << style::normal << " " << style::cyan << "Target" << style::normal
					<< "cannot be a file path when " << style::magenta << "source" << style::normal << " is a directory!\n";
				throw 1;	// Throw anything - just get us out of here!
			}
		}
	}




	int Generator::run()
	{
		path target_dir;
		try
		{
			if( Target.str().endsWith( '/' ) )
			{
				directory( Target ).ensureExists();
				target_dir = Target;
				if( Verbose )
					term << "Using " << style::cyan << "target" << style::normal<< " directory " << style::yellow
					<< target_dir.str() << style::normal << "!\n";
			}
			else
				target_dir = Target.parent();
		}
		catch( exception& e )
		{
			term << style::error << "ERROR:" << style::normal << " Cannot create or access " << style::cyan << "target"
				<< style::normal << " directory " << style::yellow << Target.str() << style::normal << "!\n";
			term << style::right_value << e.details() << style::normal << "\n";
			return 4;
		}

		type::Handler::init();
		for( auto& source : Sources )
		{
			if( Verbose )
				term << "Parsing " << style::magenta << "source" << style::normal << " file: " << style::yellow
				<< source.str() << style::normal << "\n";
			DataTuple dt;
			try
			{
				dt = edoc().parse( file( source ) );
			}
			catch( exception& e )
			{
				term << style::error << "ERROR:" << style::normal << " Failed to parse " << style::magenta << "source"
					<< style::normal << " file " << style::yellow << source.str() << style::normal << "!\n";
				term << style::right_value << e.details() << style::normal << "\n";
				return 4;
			}

			path target;
			if( Target.str().endsWith( '/' ) )
				target = target_dir / ( string( source.base() ).slice( 0, -6 ) );
			else
				target = Target;

			if( SaveAsEDT )
				_generateEdt( dt, source, target );
			else
				_generateHtml( dt, source, target );
		}
		return 0;
	}




	void Generator::_scanSources( path sourcedir )
	{
		directory dir{ sourcedir };
		for( auto& elm : dir )
		{
			if( elm.isFile() && elm.str().lower().endsWith( ".edoc" ) )
				Sources.insert( elm );
		}
	}

	void Generator::_generateHtml( DataTuple& dt, path source, path target )
	{
		if( Verbose )
			term << "  Generating HTML\n";
		try
		{
			file target_file{ target.ext( "html" ) };
			target_file.save( ToHtml().makeDocument( dt, Title ) );
			if( Verbose )
				term << "  Created " << style::cyan << "target" << style::normal << " file: " << style::yellow
				<< target_file.fpath().str() << style::normal << "!\n";
		}
		catch( exception& e )
		{
			term << style::error << "ERROR:" << style::normal << " Failed to generate " << style::cyan << "target"
				<< style::normal << " file " << style::yellow << source.str() << style::normal << "!\n";
			term << style::right_value << e.details() << style::normal << "\n";
		}
	}
	
	void Generator::_generateEdt( DataTuple& dt, path source, path target )
	{
		if( Verbose )
			term << "  Generating EDT\n";
		try
		{
			target.ext( "edt" );
			edt( dt ).save( target );
			if( Verbose )
				term << "  Created " << style::cyan << "target" << style::normal << " file: " << style::yellow
				<< target.str() << style::normal << "!\n";
		}
		catch( exception& e )
		{
			term << style::error << "ERROR:" << style::normal << " Failed to generate " << style::cyan << "target"
				<< style::normal << " file " << style::yellow << source.str() << style::normal << "!\n";
			term << style::right_value << e.details() << style::normal << "\n";
		}
	}
}
