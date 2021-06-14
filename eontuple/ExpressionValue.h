#pragma once

#include "Value.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::tup' namespace is for specific Æon Tuple Format elements
	**************************************************************************/
	namespace tup
	{
		/**********************************************************************
		  Eon Tuple Expression Value Class - eon::tup::expressionval

		  This is a specialization of [eon::tup::value].
		**********************************************************************/
		class expressionval : public value
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline expressionval() : value( basic_type::expr_t ) {}
			inline expressionval( const expression& val )
				: value( basic_type::expr_t ) { Val = val; }
			inline expressionval( expression&& val )
				: value( basic_type::expr_t ) { Val = std::move( val ); }



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Get read-only value
			inline bool softBool( variables& vars ) const override {
				return !Val.empty(); }
			inline char_t softChar( variables& vars ) const override {
				return Val.evaluate( vars )->softChar( vars ); }
			inline int64_t softInt( variables& vars ) const override {
				return Val.evaluate( vars )->softInt( vars ); }
			inline double softFloat( variables& vars ) const override {
				return Val.evaluate( vars )->softFloat( vars ); }
			inline name_t softName( variables& vars ) const override {
				return Val.evaluate( vars )->softName( vars ); }
			inline const string& hardString() const override {
				static string str; str = Val.infixStr(); return str; }
			inline const string& softString( variables& vars ) const override {
				return Val.evaluate( vars )->softString( vars ); }
			inline const std::vector<string>& softRaw( variables& vars ) const
				override { return Val.evaluate( vars )->softRaw( vars ); }
			inline const hex& softBinary( variables& vars ) const override {
				return Val.evaluate( vars )->softBinary( vars ); }
			inline const regex& softRegex( variables& vars ) const override {
				return Val.evaluate( vars )->softRegex( vars ); }
			inline const path& softRef( variables& vars ) const override {
				return Val.evaluate( vars )->softRef( vars ); }
			inline const expression& hardExpression() const override {
				return Val; }
			inline const tuple& softTuple( variables& vars ) const override {
				return Val.evaluate( vars )->softTuple( vars ); }
			inline const tuple& softMeta( variables& vars ) const override {
				return Val.evaluate( vars )->softMeta( vars ); }


			inline int hardCompare( const valueptr& other ) const override {
				return Val.infixStr().compare(
					other->hardExpression().infixStr() ); }
			int softCompare( const valueptr& other, variables& vars )
				const override;

			//* Get an identical copy
			inline valueptr copy() const override {
				return valueptr( new expressionval( Val ) ); }


			//* Write value to string
			string str( size_t& pos_on_line, size_t indentation_level,
				perm format = perm::allow_oneliner | perm::allow_multiliner )
				const noexcept override { auto s = Val.infixStr(); pos_on_line
					+= s.numChars() + 2; return "(" + s + ")"; }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Clear value
			inline void clear() noexcept override { Val.clear(); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			expression Val;
		};
	}
}
