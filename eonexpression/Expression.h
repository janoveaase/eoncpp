#pragma once
#include <eontuple/Value.h>
#include <eontokenizer/TokenParser.h>
#include <eontuple/Operators.h>
#include <eontuple/Variables.h>
#include "Node.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Exception thrown when there is an error in an expression
	EONEXCEPT( BadExpression );


	namespace tup
	{
		class value;
		using valueptr = std::shared_ptr<value>;
		class variables;
	}
	namespace expr
	{
		class node;
		using nodeptr = std::shared_ptr<node>;
	}


	/**************************************************************************
	  Eon Expression Class - eon::expression

	  This class can convert bytes to and from hex digits, but also store
	  binary in the form of hex digits.
	**************************************************************************/
	class expression
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		expression() = default;
		inline expression( const expression& other ) { *this = other; }
		inline expression( expression&& other ) noexcept {
			*this = std::move( other ); }
		inline expression( string&& expr, tup::variables& vars ) {
			_parse( std::move( expr ), vars ); }

		virtual ~expression() = default;




		/**********************************************************************
		  Modifier Methods
		**********************************************************************/
	public:

		expression& operator=( const expression& other );
		inline expression& operator=( expression&& other ) noexcept {
			Root = std::move( other.Root ); return *this; }

		void clear() noexcept { Root = expr::nodeptr(); }




		/**********************************************************************
		  Read-only Methods
		**********************************************************************/
	public:

		inline bool empty() const noexcept { return !Root; }

		// Get expression as postfix string
		string postfixStr() const;

		// Get expression as infix string
		string infixStr() const;

		// Evaluate expression
		tup::valueptr evaluate() const;
		tup::valueptr evaluate( tup::variables& vars )
			const;


		bool equal( const expression& other, const tup::variables& vars )
			const noexcept;




		/**********************************************************************
		  Helpers
		**********************************************************************/
	private:

		// Parse expression
		void _parse( string&& expr, tup::variables& vars );

		void _processToken( tokenparser& parser, tup::variables& vars,
			tup::operators::code& last_type,
			std::stack<tup::operators::code>& op_stack,
			std::stack<expr::nodeptr>& tree_stack );

		expr::nodeptr _parseValue( tokenparser& parser, tup::variables& vars );



		
		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:
		expr::nodeptr Root;
	};
}
