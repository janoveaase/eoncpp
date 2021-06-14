#pragma once
#include "Node.h"
#include <eontuple/Value.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::expr' namespace encloses all hidden expression functionality
	**************************************************************************/
	namespace expr
	{
		/**********************************************************************
		  Eon Operand Node Class - eon::expr::operandnode
		**********************************************************************/
		class operandnode : public node
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			operandnode() = default;
			inline operandnode( const operandnode& other ) noexcept {
				*this = other; }
			inline operandnode( operandnode&& other ) noexcept {
				*this = std::move( other ); }
			inline operandnode( const tup::valueptr& value )
				: node( tup::operators::code::value ) { Value = value; }

			virtual ~operandnode() = default;




			/******************************************************************
			  Modifier Methods
			******************************************************************/
		public:

			inline operandnode& operator=( const operandnode& other )
				noexcept { Value = other.Value; Type = other.Type;
					return *this; }
			inline operandnode& operator=( operandnode&& other ) noexcept {
				Value = std::move( other.Value ); *static_cast<node*>( this )
					= std::move( other ); return *this; }




			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline const tup::valueptr& value() const noexcept {
				return Value; }

			string str() const override;

			inline string postorderStr() const override { return str(); }
			inline string inorderStr() const override { return str(); }

			//* Evaluate expression
			tup::valueptr evaluate( tup::variables& vars ) const override;

			inline nodeptr copy() const override { return nodeptr(
				new operandnode( Value ? Value->copy() : Value ) ); }

			bool equal( const nodeptr& other, const tup::variables& vars )
				const noexcept override;




			/******************************************************************
			  Evaluator Methods for operands
			******************************************************************/
		public:

			inline tup::valueptr evaluate( tup::operators::code op_type,
				tup::variables& vars ) const {
				return Value->evaluate( op_type, vars ); }
			inline tup::valueptr evaluate( tup::operators::code op_type,
				const operandnode& operand2, tup::variables& vars ) const {
				return Value->evaluate( op_type, operand2.Value, vars ); }
			inline tup::valueptr evaluate( tup::operators::code op_type,
				const operandnode& operand2, const operandnode& operand3,
				tup::variables& vars ) const { return Value->evaluate(
					op_type, operand2.Value, operand3.Value, vars ); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			tup::valueptr Value;
		};
	}
}
