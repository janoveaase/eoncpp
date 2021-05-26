#pragma once

#include "Block.h"
#include <eonname/Name.h>


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
		//* Link types
		enum class linktype
		{
			internal_t,
			document_t,
			url_t
		};



		/**********************************************************************
		  Eon Document Element Link Class - eon::doc::link

		  This is a specialization of [eon::doc::block].
		**********************************************************************/
		class exclamation : public block
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline exclamation( linktype type )
				: block( elmtype::link_t ) { LinkType = type; }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline linktype linkType() const noexcept { return LinkType; }

			inline name_t protocol() const noexcept { return Protocol; }
			inline const string& document() const noexcept { return Document; }
			inline name_t tag() const noexcept { return Tag; }

			string str( size_t max_line_len, size_t indent ) const override;



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			inline void protocol( name_t name ) { Protocol = name; }
			inline void document( const string& name ) { Document = name; }
			inline void tag( name_t name ) { Tag = name; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			linktype LinkType{ linktype::internal_t };
			name_t Protocol{ no_name };
			string Document;
			name_t Tag{ no_name };
		};
	}
}
