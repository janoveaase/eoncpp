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
			inline const path& ref_value() const override { return Val; }

			//* Check if equal to another value of the same type
			inline bool equal( const valueptr other ) const noexcept override {
				return other && other->isRef() && Val == other->ref_value(); }

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

			//* Get modifiable value
			inline path& ref_value() override { return Val; }

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
