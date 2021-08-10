#pragma once

#include <eonname/NameRef.h>
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
		  Eon Reference Value Class - eon::vars::refval

		  This is a specialization of [eon::vars::value].
		**********************************************************************/
		class refval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline refval() : value( type_code::ref_t ) {}
			inline refval( const nameref& val, valueptr context ) : value( type_code::ref_t ) {
				Val = val; Context = context; }
			inline refval( nameref&& val, valueptr context ) noexcept: value( type_code::ref_t ) {
				Val = std::move( val ); Context = context; }



			/******************************************************************
			  Factory
			******************************************************************/
		public:

			static inline valueptr create( const nameref& value, valueptr context ) {
				return valueptr( new refval( value, context ) ); }
			static inline valueptr create( nameref&& value, valueptr context ) noexcept {
				return valueptr( new refval( std::move( value ), context ) ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline const nameref& actualReference() const override { return Val; }
			inline string convertString() const override { return Val.str(); }
			inline nameref convertReference() const override { return Val; }

			valueptr context() const noexcept { return Context; }

			//* Get read-only value
			inline bool targetBool( variables& vars ) const override {
				auto found = vars.get( actualReference(), Context );
				if( found ) return found->targetBool( vars ); throw NotFound(); }
			inline char_t targetChar( variables& vars ) const {
				auto found = vars.get( actualReference(), Context );
				if( found ) return found->targetChar( vars ); throw NotFound(); }
			inline int64_t targetInt( variables& vars ) const {
				auto found = vars.get( actualReference(), Context );
				if( found ) return found->targetInt( vars ); throw NotFound(); }
			inline double targetFloat( variables& vars ) const {
				auto found = vars.get( actualReference(), Context );
				if( found ) return found->targetFloat( vars ); throw NotFound(); }

			inline valueptr target( variables& vars, valueptr self ) const override {
				return Target ? Target : vars.get( actualReference(), Context ); }
			inline valueptr target() const noexcept { return Target; }

			inline name_t targetName( variables& vars ) const override {
				auto found = vars.get( actualReference(), Context );
				if( found ) return found->targetName( vars ); throw NotFound(); }
			inline const string& targetString( variables& vars ) const override {
				auto found = vars.get( actualReference(), Context );
				if( found ) return found->targetString( vars ); throw NotFound(); }
			inline const std::vector<string>& targetRaw( variables& vars ) const override {
				auto found = vars.get( actualReference(), Context );
				if( found ) return found->targetRaw( vars ); throw NotFound(); }
			inline const hex& targetBinary( variables& vars ) const override {
				auto found = vars.get( actualReference(), Context );
				if( found ) return found->targetBinary( vars ); throw NotFound(); }
			inline const regex& targetRegex( variables& vars ) const override {
				auto found = vars.get( actualReference(), Context );
				if( found ) return found->targetRegex( vars ); throw NotFound(); }
			inline const nameref& targetReference( variables& vars ) const override { return Val; }
			inline name_t targetVariable( variables& vars ) const override {
				auto found = vars.get( actualReference(), Context );
				if( found ) return found->targetVariable( vars ); throw NotFound(); }
			inline const expression& targetExpression( variables& vars ) const override {
				auto found = vars.get( actualReference(), Context );
				if( found ) return found->targetExpression( vars ); throw NotFound(); }

			inline const tuple& targetTuple( variables& vars ) const override {
				auto found = vars.get( actualReference(), Context );
				if( found ) return found->targetTuple( vars ); throw NotFound(); }
			inline const tuple& targetMeta( variables& vars ) const override {
				auto found = vars.get( actualReference(), Context );
				if( found ) return found->targetMeta( vars ); throw NotFound(); }
			inline const tuple& targetFunction( variables& vars ) const override {
				auto found = vars.get( actualReference(), Context );
				if( found ) return found->targetFunction( vars ); throw NotFound(); }

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new refval( Val, Context ) ); }


			//* Write value to string
			inline string str( size_t& pos_on_line, size_t indentation_level )
				const noexcept override { auto s = Val.str();
					pos_on_line += s.numChars() + 1; return "@" + s; }


			//* Compare
			inline int actualCompare( const valueptr& other ) const override {
				return other->isReference() ? Val.compare( other->actualReference() ) : value::actualCompare( other ); }
			int targetCompare( const valueptr& other, variables& vars ) const override;



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Set value
			//* Throws [eon::vars::WrongType] if the value to set does not
			//* match 'this'.
			inline void setActual( valueptr new_value ) override {
				if( new_value && new_value->isReference() ) Val = new_value->actualReference(); else throw WrongType(); }

			//* Clear value
			inline void clear() noexcept override { Val.clear(); }

			//* Set context tuple
			inline void context( const valueptr& tcont ) noexcept { Context = tcont; }

			//* Set target
			inline void target( valueptr target ) noexcept { Target = target; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			nameref Val;
			valueptr Context;
			valueptr Target;
		};
	}
}
