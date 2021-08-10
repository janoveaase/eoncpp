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
		  Eon Binary Value Class - eon::vars::binaryval

		  This is a specialization of [eon::vars::value].
		**********************************************************************/
		class binaryval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline binaryval() : value( type_code::binary_t ) {}
			inline binaryval( const hex& val ) : value( type_code::binary_t ) { Val = val; }
			inline binaryval( hex&& val ) noexcept : value( type_code::binary_t ) { Val = std::move( val ); }



			/******************************************************************
			  Factory
			******************************************************************/
		public:

			static inline valueptr create() { return valueptr( new binaryval() ); }
			static inline valueptr create( const hex& value ) { return valueptr( new binaryval( value ) ); }
			static inline valueptr create( hex&& value ) noexcept {
				return valueptr( new binaryval( std::move( value ) ) ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline const hex& actualBinary() const override { return Val; }
			inline const hex& targetBinary( variables& vars ) const override { return Val; }

			//* Get an identical copy
			inline valueptr copy() const override { return valueptr( new binaryval( Val ) ); }


			//* Write value to string
			string str( size_t& pos_on_line, size_t indentation_level ) const noexcept override;


			//* Compare
			inline int actualCompare( const valueptr& other ) const override {
				return other->isBinary() ? Val.compare( other->actualBinary() ) : value::actualCompare( other ); }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Set value
			//* Throws [eon::vars::WrongType] if the value to set does not
			//* match 'this'.
			inline void setActual( valueptr new_value ) override {
				if( new_value && new_value->isBinary() ) Val = new_value->actualBinary(); else throw WrongType(); }

			inline hex& binary_value() override { return Val; }

			//* Clear value
			inline void clear() noexcept override { Val.clear(); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			hex Val;
		};
	}
}
