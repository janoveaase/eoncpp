#pragma once
#include <eontypes/DataTuple.h>
#include <eonsource/String.h>
#include <eonsource/File.h>
#include <eonsource/SourceReporter.h>
#include <eontokenizer/Tokenizer.h>
#include <eontokenizer/ReTokenizer.h>
#include <eonfilesys/Path.h>
#include <eonregex/RegEx.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon Data Tuple Class - eon::edt

	  Load, save, and serialize DataTuple objects in EDT format + validate.
	**************************************************************************/
	class edt
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		edt() = default;
		virtual ~edt() = default;




		/**********************************************************************
		  Operations
		**********************************************************************/
	public:

		//* Save a data tuple to file
		void save( const DataTuple& tuple, const path& file );

		//* Load a data tuple from file
		DataTuple load( const path& file );

		//* Serialize a date tuple (convert it into a string and back again)
		void toStr( const DataTuple& tuple, std::string& str );
		DataTuple fromStr( const std::string& str );


		//* Run validation on a data tuple
		//* Parameters:
		//*   target : Data tuple to validate
		//*   source : Validation tuple
		//*   issues : Data tuple in which to store issues when validation fails
		//* Returns true if valid and false if not.
		bool validate( const DataTuple& target, const DataTuple& source, DataTuple* issues );




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
	};
};
