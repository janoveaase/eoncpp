#pragma once

#include "Value.h"


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
		  Eon Tuple Name Value Class - eon::tup::nameval

		  This is a specialization of [eon::tup::value].
		**********************************************************************/
		class nameval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline nameval() : value( basic_type::name_t ) {}
			inline nameval( name_t val ) noexcept
				: value( basic_type::name_t ) { Val = val; }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline bool softBool( variables& vars ) const override { return !(
				Val == no_name || Val == name_false || Val == name_no
				|| Val == name_off || Val == name_negative
				|| Val == name_cancel || Val == name_abort ); }
			inline name_t hardName() const override { return Val; }
			inline name_t softName( variables& vars ) const override {
				return Val; }
			inline const string& softString( variables& vars ) const override {
				static string str; return Val == no_name ? str : *Val; }

			inline int hardCompare( const valueptr& other ) const override {
				auto o = other->hardName();
				return Val < o ? -1 : Val == o ? 0 : 1; }
			inline int softCompare( const valueptr& other, variables& vars )
				const override { auto o = other->softName( vars );
					return Val < o ? -1 : Val == o ? 0 : 1; }

			//* Get an identical copy
			inline valueptr copy() const {
				return valueptr( new nameval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level )
				const noexcept override {
					pos_on_line += Val->numChars(); return *Val; }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Clear value
			inline void clear() noexcept override { Val = no_name; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			name_t Val{ no_name };
		};
	}
}
