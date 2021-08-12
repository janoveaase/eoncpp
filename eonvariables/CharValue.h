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
		  Eon Tuple Character Value Class - eon::vars::charval

		  This is a specialization of [eon::vars::value].
		**********************************************************************/
		class charval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline charval() : value( type_code::char_t ) {}
			inline charval( char_t val ) noexcept : value( type_code::char_t ) { Val = val; }



			/******************************************************************
			  Factory
			******************************************************************/
		public:

			static inline valueptr create( char_t value ) { return valueptr( new charval( value ) ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline char_t actualChar() const override { return Val; }
			inline char_t targetChar( variables& vars ) const override { return Val; }
			inline bool convertBool() const override { return Val != '\0'; }
			inline char_t convertChar() const override { return Val; }
			inline int64_t convertInt() const override { return static_cast<int64_t>( Val ); }
			inline double convertFloat() const override { return static_cast<double>( Val ); }
			inline string convertString() const override { return string( Val ); }

			//* Get an identical copy
			inline valueptr copy() const override { return valueptr( new charval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level ) const noexcept override {
				pos_on_line += 2; return "'" + ( Val == '"' ? string( Val ) : string( Val ).escape() ) + "'"; }


			//* Compare
			inline int actualCompare( const valueptr& other ) const override {
				return other->isChar() ? ( Val < other->actualChar() ? -1 : Val == other->actualChar() ? 0 : 1 )
					: value::actualCompare( other ); }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Set value
			//* Throws [eon::vars::WrongType] if the value to set does not
			//* match 'this'.
			inline void setActual( valueptr new_value ) override {
				if( new_value && new_value->isChar() ) Val = new_value->actualChar(); else throw WrongType(); }

			//* Clear value
			inline void clear() noexcept override { Val = 0; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			char_t Val{ 0 };
		};
	}
}
