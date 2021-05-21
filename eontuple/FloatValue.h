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
		  Eon Tuple Float Value Class - eon::tup::floatval

		  This is a specialization of [eon::tup::value].
		**********************************************************************/
		class floatval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline floatval() : value( basic_type::float_t ) {}
			inline floatval( double value ) noexcept
				: value( basic_type::float_t ) { Val = value; }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline double float_value() const override { return Val; }

			//* Check if equal to another value of the same type
			inline bool equal( const valueptr other ) const noexcept override {
				return other && other->isFloat()
					&& Val == other->float_value(); }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new floatval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_leve,
				perm format = perm::allow_oneliner | perm::allow_multiliner )
				const noexcept override { string val( Val );
					pos_on_line += val.numChars(); return val; }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Get modifiable value
			inline double& float_value() override { return Val; }

			//* Clear value
			inline void clear() noexcept override { Val = 0.0; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			double Val{ 0.0 };
		};
	}
}
