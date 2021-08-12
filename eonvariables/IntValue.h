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
		  Eon Tuple Integer Value Class - eon::vars::intval

		  This is a specialization of [eon::vars::value].
		**********************************************************************/
		class intval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline intval() : value( type_code::int_t ) {}
			inline intval( int64_t val ) noexcept : value( type_code::int_t ) { Val = val; }



			/******************************************************************
			  Factory
			******************************************************************/
		public:

			static inline valueptr create( int64_t value ) { return valueptr( new intval( value ) ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline int64_t actualInt() const override { return Val; }
			inline int64_t targetInt( variables& vars ) const override { return Val; }
			inline bool convertBool() const override { return Val != 0; }
			inline char_t convertChar() const override { return static_cast<char_t>( Val ); }
			inline int64_t convertInt() const override { return Val; }
			inline double convertFloat() const override { return static_cast<double>( Val ); }
			inline string convertString() const override { return string( Val ); }

			//* Get an identical copy
			inline valueptr copy() const override { return valueptr( new intval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level ) const noexcept override {
				string val( Val ); pos_on_line += val.numChars(); return val; }


			//* Compare
			inline int actualCompare( const valueptr& other ) const override {
				return other->isInt() ? ( Val < other->actualInt() ? -1 : Val == other->actualInt() ? 0 : 1 )
					: value::actualCompare( other ); }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Set value
			//* Throws [eon::vars::WrongType] if the value to set does not
			//* match 'this'.
			inline void setActual( valueptr new_value ) override {
				if( new_value && new_value->isInt() ) Val = new_value->actualInt(); else throw WrongType(); }

			//* Clear value
			inline void clear() noexcept override { Val = 0; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			int64_t Val{ 0 };
		};
	}
}
