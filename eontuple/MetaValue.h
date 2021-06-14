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
			inline bool softBool( variables& vars ) const override {
				return !Val.empty(); }
			inline const string& softString( variables& vars ) const override {
				static string str; str = Val.str(); return str; }
			inline const tuple& softTuple( variables& vars ) const override {
				return Val; }
			inline const tuple& hardMeta() const override { return Val; }
			inline const tuple& softMeta( variables& vars ) const override {
				return Val; }

			inline int hardCompare( const valueptr& other ) const override {
				auto& o = other->hardMeta();
				return Val < o ? -1 : Val == o ? 0 : 1; }
			inline int softCompare( const valueptr& other, variables& vars )
				const override { auto& o = other->softMeta( vars );
					return Val < o ? -1 : Val == o ? 0 : 1; }

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
