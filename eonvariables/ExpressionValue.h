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
		  Eon Tuple Expression Value Class - eon::vars::expressionval

		  This is a specialization of [eon::vars::value].
		**********************************************************************/
		class expressionval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline expressionval() : value( type_code::expr_t ) {}
			inline expressionval( const expression& val, bool braced ) : value( type_code::expr_t ) {
				Val = val; Braced = braced; }
			inline expressionval( expression&& val, bool braced ) : value( type_code::expr_t ) {
				Val = std::move( val ); Braced = braced; }



			/******************************************************************
			  Factory
			******************************************************************/
		public:

			static inline valueptr create( const expression& value, bool braced ) {
				return valueptr( new expressionval( value, braced ) ); }
			static inline valueptr create( expression&& value, bool braced ) {
				return valueptr( new expressionval( std::move( value ), braced ) ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline const expression& actualExpression() const override { return Val; }
			inline string convertString() const override { return Val.infixStr(); }

			inline valueptr target( variables& vars, valueptr self ) const override {
				return Val.evaluate( vars ); }

			inline bool targetBool( variables& vars ) const override { return Val.evaluate( vars )->targetBool( vars ); }
			inline char_t targetChar( variables& vars ) const { return Val.evaluate( vars )->targetChar( vars ); }
			inline int64_t targetInt( variables& vars ) const { return Val.evaluate( vars )->targetInt( vars ); }
			inline double targetFloat( variables& vars ) const { return Val.evaluate( vars )->targetFloat( vars ); }

			inline name_t targetName( variables& vars ) const override {
				return Val.evaluate( vars )->targetName( vars ); }
			inline const string& targetString( variables& vars ) const override {
				return Val.evaluate( vars )->targetString( vars ); }
			inline const std::vector<string>& targetRaw( variables& vars ) const override {
				return Val.evaluate( vars )->targetRaw( vars ); }
			inline const hex& targetBinary( variables& vars ) const override {
				return Val.evaluate( vars )->targetBinary( vars ); }
			inline const regex& targetRegex( variables& vars ) const override {
				return Val.evaluate( vars )->targetRegex( vars ); }
			inline const nameref& targetReference( variables& vars ) const override {
				return Val.evaluate( vars )->targetReference( vars ); }
			inline name_t targetVariable( variables& vars ) const override {
				return Val.evaluate( vars )->targetVariable( vars ); }
			inline const expression& targetExpression( variables& vars ) const override { return Val; }

			inline const tuple& targetTuple( variables& vars ) const override {
				return Val.evaluate( vars )->targetTuple( vars ); }
			inline const tuple& targetMeta( variables& vars ) const override {
				return Val.evaluate( vars )->targetMeta( vars ); }
			inline const tuple& targetFunction( variables& vars ) const override {
				return Val.evaluate( vars )->targetFunction( vars ); }

			//* Get an identical copy
			inline valueptr copy() const override { return valueptr( new expressionval( Val, Braced ) ); }


			//* Write value to string
			string str( size_t& pos_on_line, size_t indentation_level )
				const noexcept override { auto s = Val.infixStr(); pos_on_line
					+= s.numChars() + 3; return Braced ? "&(" + s + ")" : s; }


			//* Compare
			inline int actualCompare( const valueptr& other ) const override {
				return other->isExpression()
					? ( Val.equal( other->actualExpression() ) ? 0 : &Val < &other->actualExpression() ? -1 : 1 )
					: value::actualCompare( other ); }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Set value
			//* Throws [eon::vars::WrongType] if the value to set does not
			//* match 'this'.
			inline void setActual( valueptr new_value ) override {
				if( new_value && new_value->isExpression() ) Val = new_value->actualExpression(); else throw WrongType(); }

			//* Clear value
			inline void clear() noexcept override { Val.clear(); }

			inline expression& expression_value() { return Val; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			expression Val;
			bool Braced{ true };
		};
	}
}
