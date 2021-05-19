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
		  Eon Tuple Binary Value Class - eon::tup::binaryval

		  This is a specialization of [eon::tup::value].
		**********************************************************************/
		class binaryval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline binaryval() : value( basic_type::binary_t ) {}
			inline binaryval( const hex& value )
				: value( basic_type::binary_t ) { Val = value; }
			inline binaryval( hex&& value ) noexcept
				: value( basic_type::binary_t ) { Val = std::move( value ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline const hex& binary_value() const override { return Val; }

			//* Check if equal to another value of the same type
			inline bool equal( const valueptr other ) const noexcept override {
				return other && other->isBinary()
					&& Val == other->binary_value(); }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new binaryval( Val ) ); }


			//* Write value to string
			string str( size_t& pos_on_line, size_t indentation_level,
				bool named ) const noexcept override;



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Get modifiable value
			inline hex& binary_value() override { return Val; }

			//* Clear value
			inline void clear() noexcept override { Val.clear(); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			hex Val;
		};
	}
}
