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
		  Eon Document Element Heading Class - eon::doc::heading

		  This is a specialization of [eon::doc::element].
		**********************************************************************/
		class heading : public element
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline heading( size_t level = 1 )
				: element( elmtype::heading_t ) { Level = level; }
			inline heading( string&& heading, size_t level = 1 )
				: element( elmtype::heading_t ) {
					Text = std::move( heading ); Level = level; }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline size_t level() const noexcept { return Level; }

			inline string str( size_t max_line_len, size_t indent )
				const override {
				return ( Level == 1 ? ">> " : ">> " + string( Level - 1, '>' )
					+ " " ) + Text + "\n"; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			size_t Level{ 1 };
			string Text;
		};
	}
}
