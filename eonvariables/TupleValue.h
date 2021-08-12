#pragma once

#include <eontuple/Tuple.h>
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
		  Eon Tuple Tuple Value Class - eon::vars::tupleval

		  This is a specialization of [eon::vars::value].
		**********************************************************************/
		class tupleval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline tupleval() : value( type_code::tuple_t ) {}
			inline tupleval( const tuple& val, bool pattern = false ) : value( type_code::tuple_t ) {
				Val = val; Pattern = pattern; }
			inline tupleval( tuple&& val, bool pattern = false ) noexcept : value( type_code::tuple_t ) {
				Val = std::move( val ); Pattern = pattern; }



			/******************************************************************
			  Factory
			******************************************************************/
		public:

			static inline valueptr create() { return valueptr( new tupleval() ); }
			static inline valueptr create( const tuple& value ) { return valueptr( new tupleval( value ) ); }
			static inline valueptr create( tuple&& value ) noexcept {
				return valueptr( new tupleval( std::move( value ) ) ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline const tuple& actualTuple() const override { return Val; }
			inline const tuple& targetTuple( variables& vars ) const override { return Val; }

			//* Get an identical copy
			inline valueptr copy() const override { return valueptr( new tupleval( Val ) ); }


			//* Write value to string
			string str( size_t& pos_on_line, size_t indentation_level ) const noexcept override;


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
				if( new_value && new_value->isTuple() ) Val = new_value->actualTuple(); else throw WrongType(); }

			inline tuple& tuple_value() override { return Val; }

			//* Claim ownership of the value
			inline tuple&& claim() { return std::move( Val ); }

			//* Clear value
			inline void clear() noexcept override { Val.clear(); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			tuple Val;
			bool Pattern{ false };
		};
	}
}
