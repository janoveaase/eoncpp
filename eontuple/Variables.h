#pragma once

#include "Value.h"
#include <unordered_map>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::tup' namespace is for specific Æon tuple elements
	**************************************************************************/
	namespace tup
	{
		class value;
		using valueptr = std::shared_ptr<value>;


		/**********************************************************************
		  Eon Variables Class - eon::tup::variables

		  Uses name_t as keys for accessing valueptr objects.
		**********************************************************************/
		class variables
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			variables() = default;
			virtual ~variables() = default;



			/******************************************************************
			  Accessing variables

			  WARNING: This is not thread safe! No two threads should ever
			           access the same variable - or even variables object -
					   at the same time!
			******************************************************************/
		public:

			// Get a variable
			// Returns 'false' if not found - or void
			valueptr get( name_t name ) const noexcept;

			// Set or create a variable
			void set( name_t name, const valueptr& value );




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			std::unordered_map<name_t, valueptr> Vars;
		};
	}
}
