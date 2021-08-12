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
		  Eon Tuple Name Value Class - eon::vars::nameval

		  This is a specialization of [eon::vars::value].
		**********************************************************************/
		class nameval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline nameval() : value( type_code::name_t ) {}
			inline nameval( name_t val ) noexcept : value( type_code::name_t ) { Val = val; }



			/******************************************************************
			  Factory
			******************************************************************/
		public:

			static inline valueptr create( name_t value ) { return valueptr( new nameval( value ) ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline name_t actualName() const override { return Val; }
			inline name_t targetName( variables& vars ) const override { return Val; }
			inline bool convertBool() const override {
				return Val == name_true || Val == name_yes || Val == name_active || Val == name_on; }
			inline name_t convertName() const override { return Val; }
			inline string convertString() const override { return *Val; }
			inline name_t convertVariable() const override { return Val; }

			//* Get an identical copy
			inline valueptr copy() const { return valueptr( new nameval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level ) const noexcept override {
				pos_on_line += Val->numChars(); return *Val; }


			//* Compare
			inline int actualCompare( const valueptr& other ) const override {
				return other->isName() ? ( Val < other->actualName() ? -1 : Val == other->actualName() ? 0 : 1 )
					: value::actualCompare( other ); }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Set value
			//* Throws [eon::vars::WrongType] if the value to set does not
			//* match 'this'.
			inline void setActual( valueptr new_value ) override {
				if( new_value && new_value->isName() ) Val = new_value->actualName(); else throw WrongType(); }

			//* Clear value
			inline void clear() noexcept override { Val = no_name; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			name_t Val{ no_name };
		};
	}
}
