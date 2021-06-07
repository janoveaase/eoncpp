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
		  Eon Tuple Meta data Value Class - eon::tup::metaval

		  This is a specialization of [eon::tup::value].
		**********************************************************************/
		class metaval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline metaval() : value( basic_type::meta_t ) {}
			inline metaval( const tuple& val )
				: value( basic_type::meta_t ) { Val = val; }
			inline metaval( tuple&& val ) noexcept
				: value( basic_type::meta_t ) { Val = std::move( val ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline const tuple& meta_value() const override { return Val; }

			//* Check if equal to another value of the same type
			inline bool equal( const valueptr other ) const noexcept override {
				return other && other->isMeta()
					&& Val == other->meta_value(); }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new metaval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level,
				perm format = perm::allow_oneliner | perm::allow_multiliner )
				const noexcept override { auto s = Val.str( pos_on_line,
					indentation_level, tup::perm::allow_oneliner );
					pos_on_line += 2; return "<" + s + ">"; }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Get modifiable value
			inline tuple& meta_value() override { return Val; }

			//* Claim ownership of the tuple
			inline tuple&& claim() noexcept { return std::move( Val ); }

			//* Clear value
			inline void clear() noexcept override { Val.clear(); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			tuple Val;
		};
	}
}
