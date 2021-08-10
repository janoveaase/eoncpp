#pragma once

#include "Value.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::vars' namespace is for specific Æon Variables elements
	**************************************************************************/
	namespace vars
	{
		/**********************************************************************
		  Eon Function Class - eon::vars::function

		  This is an interface class for built-in functions that can be used
		  in expressions.
		**********************************************************************/
		class function
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			function() = default;
			virtual ~function() = default;



			/******************************************************************
			  Function methods
			******************************************************************/
		public:

			//* Get argument requirements
			virtual const tuple* args() const noexcept = 0;

			//* Execute/run function
			virtual valueptr execute( variables& varcache, const tuple& args ) const = 0;
		};
		using functionptr = std::shared_ptr<function>;
	}
}
