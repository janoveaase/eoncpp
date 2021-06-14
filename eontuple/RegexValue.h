#pragma once

#include "Value.h"
#include <eonregex/RegEx.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::tup' namespace is for specific Æon Tuple Format elements
	**************************************************************************/
	namespace tup
	{
		/**********************************************************************
		  Eon Tuple Regex Value Class - eon::tup::regexval

		  This is a specialization of [eon::tup::value].
		**********************************************************************/
		class regexval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline regexval() : value( basic_type::regex_t ) {}
			inline regexval( const regex& val )
				: value( basic_type::regex_t ) { Val = val; }
			inline regexval( regex&& val ) noexcept
				: value( basic_type::regex_t ) { Val = std::move( val ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline bool softBool( variables& vars ) const override {
				return !Val.empty(); }
			inline const string& softString( variables& vars ) const override {
				static string str; str = "?" + Val.str(); return str; }
			inline const regex& hardRegex() const override { return Val; }
			inline const regex& softRegex( variables& vars ) const override {
				return Val; }

			inline int hardCompare( const valueptr& other ) const override {
				return Val.str().compare( other->hardRegex().str() ); }
			inline int softCompare( const valueptr& other, variables& vars )
				const override { return Val.str().compare(
					other->softRegex( vars ).str() ); }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new regexval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level,
				perm format = perm::allow_oneliner | perm::allow_multiliner )
				const noexcept override { pos_on_line += Val.str().numChars()
					+ 1; return "?" + Val.str(); }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Clear value
			inline void clear() noexcept override { Val.clear(); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			eon::regex Val;
		};
	}
}
