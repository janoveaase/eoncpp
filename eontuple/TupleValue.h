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
			inline tupleval( const tuple& val, bool pattern = false )
				: value( basic_type::tuple_t ) { Val = val; Pattern = pattern; }
			inline tupleval( tuple&& val, bool pattern = false ) noexcept
				: value( basic_type::tuple_t ) {
				Val = std::move( val ); Pattern = pattern; }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline bool softBool( variables& vars ) const override 
			{ return !Val.empty(); }
			inline const string& softString( variables& vars ) const override {
				static string str; str = Val.str(); return str; }
			inline const tuple& hardTuple() const override { return Val; }
			inline const tuple& softTuple( variables& vars ) const override {
				return Val; }
			inline const tuple& softMeta( variables& vars ) const override {
				return Val; }

			inline int hardCompare( const valueptr& other ) const override {
				auto& o = other->hardTuple();
				return Val < o ? -1 : Val == o ? 0 : 1; }
			inline int softCompare( const valueptr& other, variables& vars )
				const override { auto& o = other->softTuple( vars );
					return Val < o ? -1 : Val == o ? 0 : 1; }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new tupleval( Val ) ); }


			//* Write value to string
			string str( size_t& pos_on_line, size_t indentation_level )
				const noexcept override;



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

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
