#pragma once
#include <eontypes/DataTuple.h>
#include <eonsource/String.h>
#include <eonsource/File.h>
#include <eonsource/SourceReporter.h>
#include <eontokenizer/Tokenizer.h>
#include <eontokenizer/ReTokenizer.h>
#include <eonfilesys/Path.h>
#include <eonfilesys/File.h>
#include <eonregex/RegEx.h>


///////////////////////////////////////////////////////////////////////////////
//
//  The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	// Eon Data Tuple Class - eon::edt
	//
	// Load, save, and serialize DataTuple objects in EDT format + validate.
	//
	class edt
	{
		//////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		edt() = default;
		virtual ~edt() = default;




		//////////////////////////////////////////////////////////////////////
		//
		// Operations
		//
	public:

		// Save a data tuple as EDT to file
		inline void save( const DataTuple& tuple, const path& file ) {
			string str; toStr( tuple, str ); eon::file target( file ); target.save( str ); }

		// Load EDT from file into data tuple
		inline DataTuple load( const path& file ) { eon::file source( file ); return fromStr( source.loadText() ); }

		// "Save" data tuple as EDT to string
		inline void toStr( const DataTuple& tuple, string& str ) { _toStr( tuple, str, 0, false ); }

		// "Load" EDT from string into data tuple
		DataTuple fromStr( const string& str );


		// Run validation on a data tuple
		// Parameters:
		//   target : Data tuple to validate
		//   source : Validation tuple
		//   issues : Data tuple in which to store issues when validation fails
		// Returns true if valid and false if not.
		bool validate( const DataTuple& target, const DataTuple& source, DataTuple* issues );




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		void _toStr( const DataTuple& tuple, string& str, int_t indentation_level, bool unnamed_tuple );




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:
	};
};
