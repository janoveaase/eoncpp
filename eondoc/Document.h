#pragma once

#include <eonstring/String.h>
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
		class element;
		using elementptr = std::shared_ptr<element>;



		/**********************************************************************
		  Eon Document Class - eon::doc::document

		  This is a specialization of [eon::doc::block].
		**********************************************************************/
		class document : public block
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline document() : block( elmtype::document_t ) {}

			document( const document& ) = delete;
			inline document( document&& other ) noexcept
				: block( elmtype::document_t ) { *this = std::move( other ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline bool hasAnchor( const string& anchor ) const noexcept {
				return Anchors.find( anchor ) != Anchors.end(); }
			inline elementptr anchor( const string& name ) const noexcept {
				auto found = Anchors.find( name ); return found
					!= Anchors.end() ? found->second : elementptr(); }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			document& operator=( const document& ) = delete;
			inline document& operator=( document&& other ) noexcept {
				Anchors = std::move( other.Anchors ); *static_cast<block*>(
					this ) = std::move( other ); return *this; }

			inline void add( const elementptr& element ) override {
				block::add( element ); }
			inline void add( elementptr element, string&& anchor ) {
				Elms.push_back( element ); setAnchor( std::move( anchor ),
					element ); }

			inline void setAnchor( string&& anchor, elementptr elm ) {
				if( !anchor.empty() ) { elm->setAnchor( anchor );
					Anchors[ std::move( anchor ) ] = elm; } }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			std::map<string, elementptr> Anchors;
		};
	}
}
