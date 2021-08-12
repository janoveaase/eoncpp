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
		  Eon Tuple Boolean Value Class - eon::vars::boolval

		  This is a specialization of [eon::vars::value].
		**********************************************************************/
		class boolval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline boolval() : value( type_code::bool_t ) {}
			inline boolval( bool val ) noexcept : value( type_code::bool_t ) { Val = val; }



			/******************************************************************
			  Factory
			******************************************************************/
		public:

			static inline valueptr create( bool value ) { return valueptr( new boolval( value ) ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline bool actualBool() const override { return Val; }
			inline bool targetBool( variables& vars ) const override { return Val; }
			inline bool convertBool() const override { return Val; }
			inline char_t convertChar() const override { return Val ? '1' : '\0'; }
			inline int64_t convertInt() const override { return Val ? 1 : 0; }
			inline double convertFloat() const override { return Val ? 1.0 : 0.0; }
			inline name_t convertName() const override { return Val ? name_true : name_false; }
			inline string convertString() const override { return Val ? "true" : ""; }

			//* Get an identical copy
			inline valueptr copy() const override { return valueptr( new boolval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level ) const noexcept override {
				pos_on_line += Val ? 4 : 5; return Val ? "true" : "false"; }


			//* Compare
			inline int actualCompare( const valueptr& other ) const override {
				return other->isBool() ? ( Val < other->actualBool() ? -1 : Val == other->actualBool() ? 0 : 1 )
					: value::actualCompare( other ); }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Set value
			//* Throws [eon::vars::WrongType] if the value to set does not
			//* match 'this'.
			inline void setActual( valueptr new_value ) override {
				if( new_value && new_value->isBool() ) Val = new_value->actualBool(); else throw WrongType(); }

			//* Clear value
			inline void clear() noexcept override { Val = false; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			bool Val{ false };
		};
	}
}
