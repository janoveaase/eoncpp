#pragma once

#include "Value.h"
#include "Tuple.h"


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
		  Eon Tuple Tuple Value Class - eon::tup::tupleval

		  This is a specialization of [eon::tup::value].
		**********************************************************************/
		class tupleval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline tupleval() : value( basic_type::tuple_t ) {}
			inline tupleval( const tuple& value, bool pattern = false )
				: value( basic_type::tuple_t ) {
				Val = value; Pattern = pattern; }
			inline tupleval( tuple&& value, bool pattern = false ) noexcept
				: value( basic_type::tuple_t ) {
				Val = std::move( value ); Pattern = pattern; }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline const tuple& tuple_value() const override { return Val; }

			//* Check if equal to another value of the same type
			inline bool equal( const valueptr other ) const noexcept override {
				return other && other->isTuple()
					&& Val == other->tuple_value(); }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new tupleval( Val ) ); }


			//* Write value to string
			string str( size_t& pos_on_line, size_t indentation_level,
				perm format = perm::allow_oneliner | perm::allow_multiliner )
				const noexcept override;



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Get modifiable value
			inline tuple& tuple_value() override { return Val; }

			//* Claim ownership of the value
			inline tuple&& claim() { return std::move( Val ); }

			//* Clear value
			inline void clear() noexcept override { Val.clear(); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			tuple Val;
			bool Pattern{ false };
		};
	}
}
