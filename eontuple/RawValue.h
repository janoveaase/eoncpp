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
		  Eon Tuple Raw string Value Class - eon::tup::rawval

		  This is a specialization of [eon::tup::value].
		**********************************************************************/
		class rawval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline rawval() : value( basic_type::raw_t ) {}
			inline rawval( const std::vector<string>& val )
				: value( basic_type::raw_t ) { Val = val; }
			inline rawval( std::vector<string>&& val ) noexcept
				: value( basic_type::raw_t ) { Val = std::move( val ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline const std::vector<string>& raw_value() const override {
				return Val; }

			//* Check if equal to another value of the same type
			inline bool equal( const valueptr other ) const noexcept override {
				return other && other->isRaw() && Val == other->raw_value(); }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new rawval( Val ) ); }


			//* Write value to string
			string str( size_t& pos_on_line, size_t indentation_level,
				perm format = perm::allow_oneliner | perm::allow_multiliner )
				const noexcept override;



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Get modifiable value
			inline std::vector<string>& raw_value() override { return Val; }

			//* Clear value
			inline void clear() noexcept override { Val.clear(); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			std::vector<string> Val;
		};
	}
}
