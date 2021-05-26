#pragma once

#include <eonstring/String.h>


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
		  Types of elements
		**********************************************************************/
		enum class elmtype
		{
			plaintext_t,
			title_t,
			heading_t,
			paragraph_t,
			listing_t,
			definition_t,
			insert_t,
			exclamation_t,
			emphasize_t,
			anchor_t,
			link_t,
			table_t,

			document_t,

			undef
		};

		class element;
		using elementptr = std::shared_ptr<element>;

		/**********************************************************************
		  Eon Document Element Class - eon::doc::element

		  Super-class for formatting elements inside an EDOC file or string.
		**********************************************************************/
		class element
		{
			/******************************************************************
			  Construction
			******************************************************************/
		protected:

			//* Cannot construct directly
			inline element( elmtype type ) { Type = type; }

		public:

			virtual ~element() = default;




			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get type
			inline elmtype type() const noexcept { return Type; }
			inline bool isPlaintext() const noexcept {
				return Type == elmtype::plaintext_t; }
			inline bool isTitle() const noexcept {
				return Type == elmtype::title_t; }
			inline bool isHeading() const noexcept {
				return Type == elmtype::heading_t; }
			inline bool isParagraph() const noexcept {
				return Type == elmtype::paragraph_t; }
			inline bool isListing() const noexcept {
				return Type == elmtype::listing_t; }
			inline bool isDefinition() const noexcept {
				return Type == elmtype::definition_t; }
			inline bool isInsert() const noexcept {
				return Type == elmtype::insert_t; }
			inline bool isExclamation() const noexcept {
				return Type == elmtype::exclamation_t; }
			inline bool isEmphasize() const noexcept {
				return Type == elmtype::emphasize_t; }
			inline bool isAnchor() const noexcept {
				return Type == elmtype::anchor_t; }
			inline bool isLink() const noexcept {
				return Type == elmtype::link_t; }
			inline bool isTable() const noexcept {
				return Type == elmtype::table_t; }
			inline bool isDocument() const noexcept {
				return Type == elmtype::document_t; }

			struct str_param
			{
				size_t max_line_len{ 79 };
				size_t indentation{ 0 };
//				bool first_element{}
			};
			virtual string str( size_t max_line_len, size_t indent ) const = 0;

			string anchor( const string& prefix, const string& postfix ) const {
				return !Anchor.empty() ? prefix + "@" + Anchor + ":" + postfix
					: ""; }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			element& operator=( const element& ) = delete;
			inline element& operator=( element&& other ) noexcept {
				Type = other.Type; return *this; }

			inline void setAnchor( const string& anchor ) { Anchor = anchor; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		protected:
			elmtype Type{ elmtype::undef };
			string Anchor;
		};
	}
}
