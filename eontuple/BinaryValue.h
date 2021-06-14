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
			inline binaryval( const hex& val )
				: value( basic_type::binary_t ) { Val = val; }
			inline binaryval( hex&& val ) noexcept
				: value( basic_type::binary_t ) { Val = std::move( val ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline bool softBool( variables& vars ) const override {
				return !Val.empty(); }
			inline const hex& hardBinary() const override { return Val; }
			inline const hex& softBinary( variables& vars ) const override {
				return Val; }

			inline int hardCompare( const valueptr& other ) const override {
				auto& o = other->hardBinary();
				return Val < o ? -1 : Val == o ? 0 : 1; }
			inline int softCompare( const valueptr& other, variables& vars )
				const override { auto& o = other->softBinary( vars );
					return Val < o ? -1 : Val == o ? 0 : 1; }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new binaryval( Val ) ); }


			//* Write value to string
			string str( size_t& pos_on_line, size_t indentation_level,
				perm format = perm::allow_oneliner | perm::allow_multiliner )
				const noexcept override;



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

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
