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
			inline floatval( double val ) noexcept
				: value( basic_type::float_t ) { Val = val; }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline bool softBool( variables& vars ) const override {
				return Val != 0.0; }
			inline int64_t softInt( variables& vars ) const override {
				return static_cast<int64_t>( Val ); }
			inline double hardFloat() const override { return Val; }
			inline double softFloat( variables& vars ) const override {
				return Val; }
			inline const string& softString( variables& vars ) const override {
				static string str; str = string( Val ); return str; }

			inline int hardCompare( const valueptr& other ) const override {
				auto o = other->hardFloat();
				return Val < o ? -1 : Val == o ? 0 : 1; }
			inline int softCompare( const valueptr& other, variables& vars )
				const override { auto o = other->softFloat( vars );
					return Val < o ? -1 : Val == o ? 0 : 1; }

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
