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
		//* Exclamation types
		enum class excltype
		{
			tip_t,
			note_t,
			warning_t,

			undef
		};
		extern string mapExcltype( excltype type );
		extern excltype mapExcltype( const string& type );



		/**********************************************************************
		  Eon Document Element Exclamation Class - eon::doc::exclamation

		  This is a specialization of [eon::doc::element].
		**********************************************************************/
		class exclamation : public element
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline exclamation( excltype type )
				: element( elmtype::exclamation_t ) { XType = type;}
			inline exclamation( excltype type, const string& text )
				: element( elmtype::exclamation_t ) { XType = type; Words
					= text.splitSequential<std::vector<string>>( SpaceChr ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline excltype xType() const noexcept { return XType; }
			inline string text() const { return string( " " ).join( Words ); }
			inline const std::vector<string>& words() const noexcept {
				return Words; }

			string str( size_t max_line_len, size_t indent ) const override;




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			excltype XType{ excltype::note_t };
			std::vector<string> Words;
		};
	}
}
