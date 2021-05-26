#pragma once

#include "Block.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::doc' namespace is for specific Æon Document Format elements
	**************************************************************************/
	namespace doc
	{
		/**********************************************************************
		  Eon Document Element Definition Class - eon::doc::definition

		  This is a specialization of [eon::doc::paragraph].
		**********************************************************************/
		class definition : public block
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline definition() : block( elmtype::definition_t ) {}
			inline definition( string&& what )
				: block( elmtype::definition_t ) { What = std::move( what ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline const string& what() const noexcept { return What; }

			string str( size_t max_line_len, size_t indent ) const override;




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		protected:
			string What;
		};
	}
}
