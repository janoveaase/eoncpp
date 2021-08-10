#pragma once

#include "Value.h"
#include <eonregex/RegEx.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::vars' namespace is for specific Æon Variables elements
	**************************************************************************/
	namespace vars
	{
		/**********************************************************************
		  Eon Tuple Regex Value Class - eon::vars::regexval

		  This is a specialization of [eon::vars::value].
		**********************************************************************/
		class regexval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline regexval() : value( type_code::regex_t ) {}
			inline regexval( const regex& val ) : value( type_code::regex_t ) { Val = val; }
			inline regexval( regex&& val ) noexcept : value( type_code::regex_t ) { Val = std::move( val ); }



			/******************************************************************
			  Factory
			******************************************************************/
		public:

			static inline valueptr create( const regex& value ) { return valueptr( new regexval( value ) ); }
			static inline valueptr create( regex&& value ) noexcept {
				return valueptr( new regexval( std::move( value ) ) ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline const regex& actualRegex() const override { return Val; }
			inline const regex& targetRegex( variables& vars ) const override { return Val; }
			inline string convertString() const override { return Val.str(); }

			//* Get an identical copy
			inline valueptr copy() const override { return valueptr( new regexval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level ) const noexcept override {
				pos_on_line += Val.str().numChars() + 1; return "?" + Val.str(); }


			//* Compare
			inline int actualCompare( const valueptr& other ) const override {
				return other->isRegex() ? Val.str().compare( other->actualRegex().str() ) : value::actualCompare( other ); }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Set value
			//* Throws [eon::vars::WrongType] if the value to set does not
			//* match 'this'.
			inline void setActual( valueptr new_value ) override {
				if( new_value && new_value->isRegex() ) Val = new_value->actualRegex(); else throw WrongType(); }

			//* Clear value
			inline void clear() noexcept override { Val.clear(); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			regex Val;
		};
	}
}
