#pragma once

#include "Element.h"
#include "Section.h"


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
		  Eon Document Element Paragraph Class - eon::doc::paragraph

		  This is a specialization of [eon::doc::element].
		**********************************************************************/
		class paragraph : public element
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline paragraph() : element( elmtype::paragraph_t ) {}



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			virtual void add( section&& sect ) {
				Sections.push_back( std::move( sect ) ); }

			// Have trailing whitespace-sections removed
			void trimTrailing();



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			string str( size_t max_line_len, size_t indent ) const override;




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		protected:
			std::vector<section> Sections;
		};
	}
}
