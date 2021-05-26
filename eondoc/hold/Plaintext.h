#pragma once

#include "Element.h"


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
		  Eon Document Element Plaintext Class - eon::doc::plaintext

		  This is a specialization of [eon::doc::element], a super-class for
		  elements containing a single string of text.
		**********************************************************************/
		class plaintext : public element
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline plaintext() : element( elmtype::plaintext_t ) {}
			inline plaintext( const string& text )
				: element( elmtype::plaintext_t ) { Words
					= text.splitSequential<std::vector<string>>( SpaceChr ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline const std::vector<string>& words() const noexcept { return Words; }

			string str( size_t max_line_len, size_t indent ) const override;




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		protected:
			std::vector<string> Words;
		};
	}
}
