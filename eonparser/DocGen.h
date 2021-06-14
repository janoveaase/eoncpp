#pragma once
#include "DocNode.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon Documentation Generator Class - eon::docgen

	  The generator will scan through source files of supported languages and
	  generate HTML documentation.

	  Language support is specified in ETF files.
	**************************************************************************/
	class docgen
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		//* Default construction, invalid tokenizer
		docgen() = default;

		//* Default destruction
		virtual ~docgen() = default;




	private:
		docnode Root; 
	};
};
