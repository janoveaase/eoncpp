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
		//* Insert types
		enum instype
		{
			example_t,
			code_t,
			quote_t,
			insert_t
		};
		extern string mapInsType( instype type );


		/**********************************************************************
		  Eon Document Element Insert Class - eon::doc::insert

		  This is a specialization of [eon::doc::element].
		**********************************************************************/
		class insert : public element
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline insert( instype ins_type, string&& heading,
				std::vector<string>&& lines ) : element( elmtype::insert_t ) {
				InsType = ins_type; Heading = std::move( heading );
				Lines = std::move( lines ); }
			inline insert( instype ins_type, string&& type_info,
				string&& heading, std::vector<string>&& lines )
				: element( elmtype::insert_t ) { InsType = ins_type; TypeInfo
					= std::move( type_info ); Heading = std::move( heading );
					Lines = std::move( lines ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline instype insertType() const noexcept { return InsType; }
			inline const string& typeInfo() const noexcept {
				return TypeInfo; }
			inline const string& heading() const noexcept { return Heading; }
			inline const std::vector<string>& lines() const noexcept {
				return Lines; }

			string str( size_t max_line_len, size_t indent ) const override;




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			instype InsType{ instype::example_t };
			string TypeInfo;
			string Heading;
			std::vector<string> Lines;
		};
	}
}
