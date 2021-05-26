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
		  Eon Document Element Listing Class - eon::doc::listing

		  This is a specialization of [eon::doc::block].
		**********************************************************************/
		class listing : public block
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline listing( bool numbered = false )
				: block( elmtype::listing_t ) { Numbered = numbered; }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline bool numbered() const noexcept { return Numbered; }

			string str( size_t max_line_len, size_t indent ) const override;




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			bool Numbered{ false };
		};
	}
}
