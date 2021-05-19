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
			inline nameval( name_t value ) noexcept
				: value( basic_type::name_t ) { Val = value; }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline name_t name_value() const override { return Val; }

			//* Check if equal to another value of the same type
			inline bool equal( const valueptr other ) const noexcept {
				return other && other->isName()
					&& Val == other->name_value(); }

			//* Get an identical copy
			inline valueptr copy() const {
				return valueptr( new nameval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level,
				bool named ) const noexcept override {
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
