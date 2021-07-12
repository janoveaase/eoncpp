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
			inline bool softBool( variables& vars ) const override {
				return !Val.empty(); }
			inline const string& softString( variables& vars ) const override {
				static string str; str = "\n"; str.join( Val ); return str; }
			inline const std::vector<string>& hardRaw() const override {
				return Val; }
			inline const std::vector<string>& softRaw( variables& vars )
				const override { return Val; }

			inline int hardCompare( const valueptr& other ) const override {
				auto o = other->hardRaw();
				return Val < o ? -1 : Val == o ? 0 : 1; }
			inline int softCompare( const valueptr& other, variables& vars )
				const override { auto o = other->softRaw( vars );
					return Val < o ? -1 : Val == o ? 0 : 1; }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new rawval( Val ) ); }


			//* Write value to string
			string str( size_t& pos_on_line, size_t indentation_level )
				const noexcept override;



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

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
