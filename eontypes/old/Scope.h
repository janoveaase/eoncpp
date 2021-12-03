#pragma once

#include "Types.h"
#include <eonexcept/Exception.h>



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Exception thrown when trying to add a new item with the same name as an
	//* existing one
	EONEXCEPT( DuplicateNameError );

	//* Exception thrown when trying to access a type that doesn't exist
	EONEXCEPT( TypeNotFound );

	class instance;



	/**************************************************************************
	  The 'eon::tup' namespace encloses all hidden tuple functionality
	**************************************************************************/
	namespace tup
	{
		/**********************************************************************
		  Variable Scope Class - eon::tup::VarScope
		**********************************************************************/
		class VarScope
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			// Construct a place-holder scope
			VarScope() = default;

			// Construct a scope with stack memory
			VarScope( std::vector<Definition>&& types, std::unordered_map<name_t, Instance>&& instances );
			virtual ~VarScope();




			/******************************************************************
			  Types
			******************************************************************/
		public:




			/******************************************************************
			  Instances (variables)
			******************************************************************/
		public:



			/******************************************************************
			  Actions
			******************************************************************/
		public:




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		private:




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			byte_t* Memory{ nullptr };
			std::unordered_map<name_t, Definition*> Types;
			std::unordered_map<name_t, Action*> Actions;
			std::unordered_map<name_t, Instance*> Instances;
		};
	}
}
