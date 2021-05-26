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
		  Eon Document Element Block Class - eon::doc::block

		  This is a specialization of [eon::doc::element], and a super-class
		  for elements that can contain other elements.
		**********************************************************************/
		class block : public element
		{
			/******************************************************************
			  Construction
			******************************************************************/
		protected:

			//* Cannot construct directly
			inline block( elmtype type ) : element( type ) {}

			block( const block& ) = delete;
			inline block( block&& other ) noexcept : element( other.Type ) {
				*this = std::move( other ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline const std::vector<elementptr>& elements() const noexcept {
				return Elms; }

			string str( size_t max_line_len, size_t indent ) const override {
				string s; for( auto& elm : Elms ) s += elm->str( max_line_len,
					indent ); return s; }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			block& operator=( const block& ) = delete;
			inline block& operator=( block&& other ) noexcept {
				Elms = std::move( other.Elms ); *static_cast<element*>( this )
					= std::move( other ); return *this; }

			virtual void add( const elementptr& element ) {
				Elms.push_back( element ); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		protected:
			std::vector<elementptr> Elms;
		};
	}
}
