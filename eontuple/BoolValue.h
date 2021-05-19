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
		  Eon Tuple Boolean Value Class - eon::tup::boolval

		  This is a specialization of [eon::tup::value].
		**********************************************************************/
		class boolval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline boolval() : value( basic_type::bool_t ) {}
			inline boolval( bool value ) noexcept
				: value( basic_type::bool_t ) { Val = value; }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline bool bool_value() const override { return Val; }

			//* Check if equal to another value of the same type
			inline bool equal( const valueptr other ) const noexcept override {
				return other && other->isBool()
					&& Val == other->bool_value(); }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new boolval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level,
				bool named ) const noexcept override {
				pos_on_line += Val ? 4 : 5; return Val ? "true" : "false"; }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Get modifiable value
			inline bool& bool_value() override { return Val; }

			//* Clear value
			inline void clear() noexcept override { Val = false; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			bool Val{ false };
		};
	}
}
