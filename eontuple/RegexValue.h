#pragma once

#include "Value.h"
#include <eonregex/RegEx.h>


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
		  Eon Tuple Regex Value Class - eon::tup::regexval

		  This is a specialization of [eon::tup::value].
		**********************************************************************/
		class regexval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline regexval() : value( basic_type::regex_t ) {}
			inline regexval( const regex& val )
				: value( basic_type::regex_t ) { Val = val; }
			inline regexval( regex&& val ) noexcept
				: value( basic_type::regex_t ) { Val = std::move( val ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline const regex& regex_value() const override { return Val; }

			//* Check if equal to another value of the same type
			inline bool equal( const valueptr other ) const noexcept override {
				return other && other->isRegex()
					&& Val.raw() == other->regex_value().raw(); }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new regexval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level,
				perm format = perm::allow_oneliner | perm::allow_multiliner )
				const noexcept override { pos_on_line += Val.raw().numChars()
					+ 1; return "?" + Val.raw(); }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Get modifiable value
			inline regex& regex_value() override { return Val; }

			//* Clear value
			inline void clear() noexcept override { Val.clear(); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			eon::regex Val;
		};
	}
}
