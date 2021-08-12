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
		  Eon Tuple Float Value Class - eon::vars::floatval

		  This is a specialization of [eon::vars::value].
		**********************************************************************/
		class floatval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline floatval() : value( type_code::float_t ) {}
			inline floatval( double val ) noexcept : value( type_code::float_t ) { Val = val; }



			/******************************************************************
			  Factory
			******************************************************************/
		public:

			static inline valueptr create( double value ) { return valueptr( new floatval( value ) ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline double actualFloat() const override { return Val; }
			inline double targetFloat( variables& vars ) const override { return Val; }
			inline bool convertBool() const override { return Val != 0.0; }
			inline char_t convertChar() const override { return static_cast<char_t>( Val ); }
			inline int64_t convertInt() const override { return static_cast<int64_t>( Val ); }
			inline double convertFloat() const override { return Val; }
			inline string convertString() const override { return string( Val ); }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new floatval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_leve )
				const noexcept override { string val( Val );
					pos_on_line += val.numChars(); return val; }


			//* Compare
			inline int actualCompare( const valueptr& other ) const override {
				return other->isFloat() ? ( Val < other->actualFloat() ? -1 : Val == other->actualFloat() ? 0 : 1 )
					: value::actualCompare( other ); }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Set value
			//* Throws [eon::vars::WrongType] if the value to set does not
			//* match 'this'.
			inline void setActual( valueptr new_value ) override {
				if( new_value && new_value->isFloat() ) Val = new_value->actualFloat(); else throw WrongType(); }

			//* Clear value
			inline void clear() noexcept override { Val = 0.0; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			double Val{ 0.0 };
		};
	}
}
