#pragma once

#include "Value.h"


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
		  Eon Tuple String Value Class - eon::vars::stringval

		  This is a specialization of [eon::vars::value].
		**********************************************************************/
		class stringval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline stringval() : value( type_code::string_t ) {}
			inline stringval( const string& val ) : value( type_code::string_t ) { Val = val; }
			inline stringval( string&& val ) noexcept : value( type_code::string_t ) { Val = std::move( val ); }



			/******************************************************************
			  Factory
			******************************************************************/
		public:

			static inline valueptr create( const string& value ) { return valueptr( new stringval( value ) ); }
			static inline valueptr create( string&& value ) noexcept {
				return valueptr( new stringval( std::move( value ) ) ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline const string& actualString() const override { return Val; }
			inline const string& targetString( variables& vars ) const override { return Val; }
			inline bool convertBool() const override { return !Val.empty(); }
			inline char_t convertChar() const override { return Val.empty() ? '\0' : *Val.begin(); }
			inline int64_t convertInt() const override { return Val.toInt64(); }
			inline double convertFloat() const override { return Val.toDouble(); }
			inline name_t convertName() const override { return name::get( Val ); }
			inline string convertString() const override { return Val; }
			inline nameref convertReference() const override { return nameref( Val ); }
			inline name_t convertVariable() const override  { return name::get( Val ); }

			//* Get an identical copy
			inline valueptr copy() const override { return valueptr( new stringval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level ) const noexcept override {
				pos_on_line += Val.numChars() + 2; return "\"" + Val.escape() + "\""; }


			//* Compare
			inline int actualCompare( const valueptr& other ) const override {
				return other->isString() ? Val.compare( other->actualString() ) : value::actualCompare( other ); }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Set value
			//* Throws [eon::vars::WrongType] if the value to set does not
			//* match 'this'.
			inline void setActual( valueptr new_value ) override {
				if( new_value && new_value->isString() ) Val = new_value->actualString(); else throw WrongType(); }

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
