#pragma once
#include <eonsource/String.h>
#include <eonsource/File.h>
#include <eonsource/SourceReporter.h>
#include <eontokenizer/Tokenizer.h>
#include <eontokenizer/ReTokenizer.h>
#include <eonfilesys/Path.h>
#include <eonregex/RegEx.h>
#include "ParseExpression.h"
#include "ToolBox.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon Parser Class - eon::Parser

	  Parse Æon code, Æon Dynamic Tuple Format (DTF), and/or Æon expression.
	**************************************************************************/
	class Parser
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		//* Construct a place-holder object
		Parser() = default;

		//* Construct for parsing a string source
/*		inline Parser( const string& source_name, string&& string_source, source::Reporter& reporter,
			scope::Scope& scope ) {
			source::String src( source_name, std::move( string_source ) );
			Tools = parser::ToolBox( std::vector<Element>&& elements, reporter, scope );
			Tools = parser::ToolBox( source::Ref( src ), reporter, scope ); }

		//* Construct for parsing a file source
		inline Parser( const path& file_path, source::Reporter& reporter, scope::Scope& scope ) {
			source::File src( file_path.str() ); Tools.init( source::Ref( src ), reporter, scope ); }
			*/
		virtual ~Parser() = default;




		/**********************************************************************
		  Preparations
		**********************************************************************/
	public:

		//* Parse the source from current position as Æon code, don't stop
		//* until end of source (or fatal error)

		//* Parse the source from current position as Æon Dynamic Tuple Format,
		//* stop at end of EDT (or fatal error)

		//* Parse the source from current position as Æon expression, stop at
		//* end of expression
		//* Can only occur in the following contexts: args, test, body, lambda,
		//* switcher, trait
		//* Returns true if parsed, false if not an expression or the
		//*         expression has errors. (Errors are reported!)
//		inline bool parseExpression( expression& result ) { return ParseExpression()( Tools, result ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		// Do the actual parsing




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		parser::ToolBox Tools;
	};
};
