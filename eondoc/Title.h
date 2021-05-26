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
		  Eon Document Element Title Class - eon::doc::title

		  This is a specialization of [eon::doc::element].
		**********************************************************************/
		class title : public element
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline title() : element( elmtype::title_t ) {}
			inline title( string&& title ) : element( elmtype::title_t ) {
				Text = std::move( title ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline string str( size_t max_line_len, size_t indent )
				const override { return ">> " + Text + " <<\n"; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			string Text;
		};
	}
}
