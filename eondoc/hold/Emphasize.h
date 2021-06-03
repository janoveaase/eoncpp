#pragma once

#include "Plaintext.h"


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
		  Eon Document Element Emphasize Class - eon::doc::emphasize

		  This is a specialization of [eon::doc::plaintext].
		**********************************************************************/
		class emphasize : public plaintext
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline emphasize() : plaintext() { Type = elmtype::emphasize_t; }
			inline emphasize( string&& text ) : plaintext( std::move( text ) )
				{ Type = elmtype::emphasize_t; }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline string str( size_t max_line_len, size_t indent )
				const override { return "*" + plaintext::str(
					max_line_len - 2, indent ) + "*"; }
		};
	}
}
