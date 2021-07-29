#pragma once
#include "DocNode.h"
#include <eonfilesys/FileSys.h>
#include <eonregex/RegEx.h>
#include <eonof/Eof.h>
#include <queue>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon Parser Class - eon::parser

	  The parser will read one or more EOF files, and given a file or directory
	  + pattern, will parse the file(s) and generate output, all according to
	  the EOF files.
	**************************************************************************/
	class parser
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		parser() = default;
		parser( const std::vector<string>& args ) noexcept;

		//* Default destruction
		virtual ~parser() = default;




		/**********************************************************************
		  Execution
		**********************************************************************/
	public:

		int run() noexcept;




		/**********************************************************************
		  Helpers
		**********************************************************************/
	private:

		void _showUsage();
		void _parseArgs( const std::vector<string>& args );
		void _parseInput( const substring& input );

		int _run();

		void _validateDocs();
		void _findDocument();
		void _showInfo();

		void _parse( const file& input_file );
		void _parse( source& src );
		bool _parseStructure( tokenparser& tp, const tuple& structure );

		bool _parseExplicit( tokenparser& tp, tup::valueptr element );
		bool _match( tokenparser& tp, char_t val );
		bool _match( tokenparser& tp, int64_t val );
		bool _match( tokenparser& tp, double val );
		bool _match( tokenparser& tp, name_t val );
		bool _match( tokenparser& tp, const string& val );
		bool _executeMeta( tokenparser& tp, const tuple& val );
		bool _executeMatch( tokenparser& tp, const tuple& val, const tup::valueptr& argument, bool optional );
		bool _executeSkip( tokenparser& tp, const tuple& val, const tup::valueptr& argument, bool optional );
		bool _matchAlternates( tokenparser& tp, const tuple& alternates );
		bool _matchReferenced( tokenparser& tp, const tup::path& ref );
		bool _skipAll( tokenparser& tp, const tuple& elements );

		bool _loop( tokenparser& tp, const tupleptr metadata,
			const tuple& structure );

		void _handleIncludeInput( const tuple& locate );
		void _execute( name_t action, const tuple& args, const tuple& block );
		void _executeIf( const tuple& args, const tuple& block );




	private:
		file App;
		std::set<directory> InputDirs;
//		std::queue<std::pair<path, regex>> ScanDirs;
		std::queue<file> InputFiles;
		directory OutputDir;
		eonof Docs;
		tuple* Doc{ nullptr };

		enum class statecode
		{
			normal,
			error,
			usage
		};
		statecode State{ statecode::normal };
	};
};
