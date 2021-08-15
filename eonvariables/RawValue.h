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
		  Eon Tuple Raw string Value Class - eon::vars::rawval

		  This is a specialization of [eon::vars::value].
		**********************************************************************/
		class rawval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline rawval() : value( type_code::raw_t ) {}
			inline rawval( const vector<string>& val ) : value( type_code::raw_t ) { Val = val; }
			inline rawval( vector<string>&& val ) noexcept : value( type_code::raw_t ) { Val = std::move( val ); }



			/******************************************************************
			  Factory
			******************************************************************/
		public:

			static inline valueptr create() { return valueptr( new rawval() ); }
			static inline valueptr create( const vector<string>& value ) { return valueptr( new rawval( value ) ); }
			static inline valueptr create( vector<string>&& value ) noexcept {
				return valueptr( new rawval( std::move( value ) ) ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline const vector<string>& actualRaw() const override { return Val; }
			inline const vector<string>& targetRaw( variables& vars ) const override { return Val; }
			inline string convertString() const override { return string( "\n" ).join( Val.begin(), Val.end() ); }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new rawval( Val ) ); }


			//* Write value to string
			string str( size_t& pos_on_line, size_t indentation_level )
				const noexcept override;


			//* Compare
			int actualCompare( const valueptr& other ) const override;



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Set value
			//* Throws [eon::vars::WrongType] if the value to set does not
			//* match 'this'.
			inline void setActual( valueptr new_value ) override {
				if( new_value && new_value->isRaw() ) Val = new_value->actualRaw(); else throw WrongType(); }

			inline vector<string>& raw_value() override { return Val; }

			//* Clear value
			inline void clear() noexcept override { Val.clear(); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			vector<string> Val;
		};
	}
}
