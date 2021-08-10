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
		  Eon Tuple Variable Value Class - eon::vars::variableval

		  This is a specialization of [eon::vars::value].
		**********************************************************************/
		class variableval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline variableval() : value( type_code::var_t ) {}
			inline variableval( name_t val ) : value( type_code::var_t ) { Val = val; }



			/******************************************************************
			  Factory
			******************************************************************/
		public:

			static inline valueptr create( name_t value ) { return valueptr( new variableval( value ) ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline name_t actualVariable() const override { return Val; }
			inline name_t convertName() const override { return Val; }
			inline string convertString() const override { return *Val; }
			inline name_t convertVariable() const override { return Val; }

			inline valueptr target( variables& vars, valueptr self ) const override {
				return vars.get( Val ); }

			inline bool targetBool( variables& vars ) const override {
				auto found = vars.get( actualVariable() );
				if( found ) return found->targetBool( vars ); throw NotFound(); }
			inline char_t targetChar( variables& vars ) const {
				auto found = vars.get( actualVariable() );
				if( found ) return found->targetChar( vars ); throw NotFound(); }
			inline int64_t targetInt( variables& vars ) const {
				auto found = vars.get( actualVariable() );
				if( found ) return found->targetInt( vars ); throw NotFound(); }
			inline double targetFloat( variables& vars ) const {
				auto found = vars.get( actualVariable() );
				if( found ) return found->targetFloat( vars ); throw NotFound(); }

			inline name_t targetName( variables& vars ) const override {
				auto found = vars.get( actualVariable() );
				if( found ) return found->targetName( vars ); throw NotFound(); }
			inline const string& targetString( variables& vars ) const override {
				auto found = vars.get( actualVariable() );
				if( found ) return found->targetString( vars ); throw NotFound(); }
			inline const std::vector<string>& targetRaw( variables& vars ) const override {
				auto found = vars.get( actualVariable() );
				if( found ) return found->targetRaw( vars ); throw NotFound(); }
			inline const hex& targetBinary( variables& vars ) const override {
				auto found = vars.get( actualVariable() );
				if( found ) return found->targetBinary( vars ); throw NotFound(); }
			inline const regex& targetRegex( variables& vars ) const override {
				auto found = vars.get( actualVariable() );
				if( found ) return found->targetRegex( vars ); throw NotFound(); }
			inline const nameref& targetReference( variables& vars ) const override {
				auto found = vars.get( actualVariable() );
				if( found ) return found->targetReference( vars ); throw NotFound(); }
			inline name_t targetVariable( variables& vars ) const override { return Val; }
			inline const expression& targetExpression( variables& vars ) const override {
				auto found = vars.get( actualVariable() );
				if( found ) return found->targetExpression( vars ); throw NotFound(); }

			inline const tuple& targetTuple( variables& vars ) const override {
				auto found = vars.get( actualVariable() );
				if( found ) return found->targetTuple( vars ); throw NotFound(); }
			inline const tuple& targetMeta( variables& vars ) const override {
				auto found = vars.get( actualVariable() );
				if( found ) return found->targetMeta( vars ); throw NotFound(); }
			inline const tuple& targetFunction( variables& vars ) const override {
				auto found = vars.get( actualVariable() );
				if( found ) return found->targetFunction( vars ); throw NotFound(); }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new variableval( Val ) ); }


			//* Write value to string
			string str( size_t& pos_on_line, size_t indentation_level )
				const noexcept override {
				pos_on_line += Val->numChars(); return *Val; }


			//* Compare
			int actualCompare( const valueptr& other ) const override;
			int targetCompare( const valueptr& other, variables& vars ) const override;



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Set value
			//* Throws [eon::vars::WrongType] if the value to set does not
			//* match 'this'.
			inline void setActual( valueptr new_value ) override {
				if( new_value && new_value->isVariable() ) Val = new_value->actualVariable(); else throw WrongType(); }

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
