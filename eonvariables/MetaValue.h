#pragma once

#include "Value.h"
#include <eontuple/Tuple.h>


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
		  Eon Tuple Meta data Value Class - eon::vars::metaval

		  This is a specialization of [eon::vars::value].
		**********************************************************************/
		class metaval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline metaval() : value( type_code::meta_t ) {}
			inline metaval( const tuple& val ) : value( type_code::meta_t ) { Val = val; }
			inline metaval( tuple&& val ) noexcept : value( type_code::meta_t ) { Val = std::move( val ); }



			/******************************************************************
			  Factory
			******************************************************************/
		public:

			static inline valueptr create() { return valueptr( new metaval() ); }
			static inline valueptr create( const tuple& value ) { return valueptr( new metaval( value ) ); }
			static inline valueptr create( tuple&& value ) noexcept {
				return valueptr( new metaval( std::move( value ) ) ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline const tuple& actualMeta() const override { return Val; }
			inline const tuple& targetMeta( variables& vars ) const override { return Val; }

			//* Get an identical copy
			inline valueptr copy() const override { return valueptr( new metaval( Val ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level ) const noexcept override {
				auto s = Val.str( pos_on_line, indentation_level ); pos_on_line += 2; return "<" + s + ">"; }


			//* Compare
			inline int actualCompare( const valueptr& other ) const override {
				return other->isMeta() ? ( Val.equal( other->actualMeta() ) ? 0 : &Val < &other->actualMeta() ? -1 : 1 )
					: value::actualCompare( other ); }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Set value
			//* Throws [eon::vars::WrongType] if the value to set does not
			//* match 'this'.
			inline void setActual( valueptr new_value ) override {
				if( new_value && new_value->isMeta() ) Val = new_value->actualMeta(); else throw WrongType(); }

			inline tuple& meta_value() override { return Val; }

			//* Claim ownership of the tuple
			inline tuple&& claim() noexcept { return std::move( Val ); }

			//* Clear value
			inline void clear() noexcept override { Val.clear(); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			tuple Val;
		};
	}
}
