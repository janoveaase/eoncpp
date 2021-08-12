#pragma once

#include "Function.h"


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
		  Eon Tuple Variable Value Class - eon::vars::functionval

		  This is a specialization of [eon::vars::value] - and of
		  [eon::vars::function].
		**********************************************************************/
		class functionval : public value, public function
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline functionval() : value( type_code::function_t ) {}
			inline functionval( const tuple& val ) : value( type_code::function_t ) { Val = val; }
			inline functionval( tuple&& val ) noexcept : value( type_code::function_t ) { Val = std::move( val ); }



			/******************************************************************
			  Factory
			******************************************************************/
		public:

			static inline valueptr create() { return valueptr( new functionval() ); }
			static inline valueptr create( const tuple& value ) { return valueptr( new functionval( value ) ); }
			static inline valueptr create( tuple&& value ) noexcept {
				return valueptr( new functionval( std::move( value ) ) ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline const tuple& actualFunction() const override { return Val; }
			inline const tuple& targetFunction( variables& vars ) const override { return Val; }

			//* Get an identical copy
			inline valueptr copy() const override { return valueptr( new functionval( Val ) ); }


			//* Write value to string
			string str( size_t& pos_on_line, size_t indentation_level ) const noexcept override {
				return Val.str( pos_on_line, indentation_level ); }


			//* Compare
			inline int actualCompare( const valueptr& other ) const override {
				return other->isFunction()
					? ( Val.equal( other->actualFunction() ) ? 0 : &Val < &other->actualFunction() ? -1 : 1 )
					: value::actualCompare( other ); }


			//* Get argument requirements
			inline const tuple* args() const noexcept {
				return Meta && Meta->exists( name_args ) && Meta->at( name_args )->isTuple()
					? &Meta->at( name_args )->actualTuple() : nullptr; }

			//* Execute function
			valueptr execute( variables& varcache, const tuple& args ) const override;



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Set value
			//* Throws [eon::vars::WrongType] if the value to set does not
			//* match 'this'.
			inline void setActual( valueptr new_value ) override {
				if( new_value && new_value->isFunction() ) Val = new_value->actualFunction(); else throw WrongType(); }

			//* Set meta data from key
			inline void setMeta( tuple* meta_data ) noexcept { Meta = meta_data; }

			//* Access modifyable tuple of expressions
			inline tuple& function_value() override { return Val; }

			//* Clear value
			inline void clear() noexcept override { Val.clear(); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

			tuple Val;
			tuple* Meta{ nullptr };		// Same as for the key
		};
	}
}
