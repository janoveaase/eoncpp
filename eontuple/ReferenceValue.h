#pragma once

#include "Value.h"
#include "Path.h"


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
		  Eon Tuple Reference Value Class - eon::tup::refval

		  This is a specialization of [eon::tup::value].
		**********************************************************************/
		class refval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline refval() : value( basic_type::ref_t ) {}
			inline refval( const path& val )
				: value( basic_type::ref_t ) { Val = val; }
			inline refval( path&& val ) noexcept
				: value( basic_type::ref_t ) { Val = std::move( val ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline bool softBool( variables& vars ) const override {
				return !Val.empty(); }
			inline const string& softString( variables& vars ) const override {
				static string str; str = Val.str(); return str; }
			inline const path& hardRef() const override { return Val; }
			inline const path& softRef( variables& vars ) const override {
				return Val; }

			inline int hardCompare( const valueptr& other ) const override {
				auto& o = other->hardRef();
				return Val < o ? -1 : Val == o ? 0 : 1; }
			inline int softCompare( const valueptr& other, variables& vars )
				const override { auto& o = other->softRef( vars );
					return Val < o ? -1 : Val == o ? 0 : 1; }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new refval( Val ) ); }

			//* Get target
			inline valueptr target() const noexcept { return Target; }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level,
				perm format = perm::allow_oneliner | perm::allow_multiliner )
				const noexcept override { auto s = Val.str();
					pos_on_line += s.numChars() + 1; return "@" + s; }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Clear value
			inline void clear() noexcept override { Val.clear(); }

			//* Set target
			inline void target( valueptr target ) noexcept { Target = target; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			path Val;
			valueptr Target;
		};
	}
}
