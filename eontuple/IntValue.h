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
		  Eon Tuple Integer Value Class - eon::tup::intval

		  This is a specialization of [eon::tup::value].
		**********************************************************************/
		class intval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline intval() : value( basic_type::int_t ) {}
			inline intval( int64_t val ) noexcept
				: value( basic_type::int_t ) { Val = val; }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline int64_t int_value() const override { return Val; }

			//* Check if equal to another value of the same type
			inline bool equal( const valueptr other ) const noexcept override {
				return other && other->isInt() && Val == other->int_value(); }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new intval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level,
				perm format = perm::allow_oneliner | perm::allow_multiliner )
				const noexcept override { string val( Val );
					pos_on_line += val.numChars(); return val; }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Get modifiable value
			inline int64_t& int_value() override { return Val; }

			//* Clear value
			inline void clear() noexcept override { Val = 0; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			int64_t Val{ 0 };
		};
	}
}
