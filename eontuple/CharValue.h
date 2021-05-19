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
		  Eon Tuple Character Value Class - eon::tup::charval

		  This is a specialization of [eon::tup::value].
		**********************************************************************/
		class charval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline charval() : value( basic_type::char_t ) {}
			inline charval( char_t value ) noexcept
				: value( basic_type::char_t ) { Val = value; }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline char_t char_value() const override { return Val; }

			//* Check if equal to another value of the same type
			inline bool equal( const valueptr other ) const noexcept override {
				return other && other->isChar()
					&& Val == other->char_value(); }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new charval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level,
				bool named ) const noexcept override { pos_on_line += 2;
					return "'" + string( Val ).escape() + "'"; }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Get modifiable value
			inline char_t& char_value() override { return Val; }

			//* Clear value
			inline void clear() noexcept override { Val = 0; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			char_t Val{ 0 };
		};
	}
}
