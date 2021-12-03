#pragma once
#include "Tuple.h"
#include "TypeSpec.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	class instance;

	/**************************************************************************
	  The 'eon::tup' namespace encloses all hidden tuple functionality
	**************************************************************************/
	namespace tup
	{
		class varscope;

		/**********************************************************************
		  Eon Action Class - eon::tup::action

		  Interface class for actions
		**********************************************************************/
		class action
		{
			/******************************************************************
			  Read-only methods
			******************************************************************/
		public:

			const actionspec* type() const noexcept { return Type; }
			name_t name() const noexcept { return Name; }




			/******************************************************************
			  Interface
			******************************************************************/
		public:

			virtual instance* operator()( instance* me, varscope* scope, tuple* args = nullptr ) = 0;




			///////////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		protected:

			const actionspec* Type{ nullptr };
			name_t Name{ no_name };
		};
	}
}
