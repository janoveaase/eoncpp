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
		  Eon Tuple String Value Class - eon::tup::stringval

		  This is a specialization of [eon::tup::value].
		**********************************************************************/
		class stringval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline stringval() : value( basic_type::string_t ) {}
			inline stringval( const string& value )
				: value( basic_type::string_t ) { Val = value; }
			inline stringval( string&& value ) noexcept
				: value( basic_type::string_t ) { Val = std::move( value ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline const string& string_value() const override { return Val; }

			//* Check if equal to another value of the same type
			inline bool equal( const valueptr other ) const noexcept override {
				return other && other->isString()
					&& Val == other->string_value(); }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new stringval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level,
				perm format = perm::allow_oneliner | perm::allow_multiliner )
				const noexcept override { pos_on_line
					+= Val.numChars() + 2; return "\"" + Val.escape() + "\""; }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Get modifiable value
			inline string& string_value() override { return Val; }

			//* Clear value
			inline void clear() noexcept override { Val.clear(); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			eon::string Val;
		};
	}
}
