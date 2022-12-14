#pragma once
#include <eonedoc/Edoc.h>
#include <eonedt/Edt.h>
#include <eonedoc/ToHtml.h>
#include <eonfilesys/FileSys.h>


///////////////////////////////////////////////////////////////////////////////
//
//  The 'eon' namespace encloses all public functionality
//
namespace doc
{
	using namespace eon;


	///////////////////////////////////////////////////////////////////////////
	//
	// Eon Document Generator Class - doc::Generator
	//
	// Application class that will convert .edoc files into .html.
	//
	class Generator
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
		// Only one constructor and it is called by the main function!
		//
	public:

		Generator() = delete;
		Generator( std::vector<string>&& args );




		///////////////////////////////////////////////////////////////////
		//
		// Operations
		//
	public:

		int run();




		///////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		void _scanSources( path sourcedir );

		void _generateHtml( DataTuple& dt, path source, path target );
		void _generateEdt( DataTuple& dt, path source, path target );




		///////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		std::set<path> Sources;
		path Target;
		string Title;
		bool Verbose{ true };
		bool SaveAsEDT{ false };
	};
}
