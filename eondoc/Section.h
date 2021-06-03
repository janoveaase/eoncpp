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
		//* Section types
		enum class sectiontype
		{
			text,
			emphasized,
			link,
			space,
			newline
		};



		/**********************************************************************
		  Eon Section Class - eon::doc::section

		  Paragraphs consists of sections (text, either plain, emphasized,
		  etc.)
		**********************************************************************/
		class section
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline section() = default;
			inline section( sectiontype type, string&& text ) {
				Type = type; Text = std::move( text ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline sectiontype type() const noexcept { return Type; }
			inline const string& text() const noexcept { return Text; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		protected:
			sectiontype Type{ sectiontype::text };
			string Text;
		};
	}
}
