#pragma once
#include "DocNode.h"
#include <eonpath/Path.h>
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




	private:
		path App;
		std::set<path> InputDirs;
		std::queue<std::pair<path, regex>> ScanDirs;
		std::queue<path> InputFiles;
		path OutputDir;
		eonof Docs;

		enum class statecode
		{
			normal,
			error,
			usage
		};
		statecode State{ statecode::normal };
	};
};
