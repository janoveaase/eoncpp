#pragma once
#include <eontokenizer/TokenParser.h>
#include "Node.h"
#include <stack>


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
		inline expression( expression&& other ) noexcept { *this = std::move( other ); }
		inline expression( string&& expr, vars::variables& vars ) { _parse( std::move( expr ), vars ); }

		virtual ~expression() = default;




		/**********************************************************************
		  Modifier Methods
		**********************************************************************/
	public:

		expression& operator=( const expression& other );
		inline expression& operator=( expression&& other ) noexcept {
			Roots = std::move( other.Roots ); return *this; }

		void clear() noexcept { Roots.clear(); }

		// Register a tuple as context to references that might exist in this
		// expression
		void setContext( vars::valueptr context );




		/**********************************************************************
		  Read-only Methods
		**********************************************************************/
	public:

		inline bool empty() const noexcept { return Roots.empty(); }

		// Get expression as postfix string
		string postfixStr() const;

		// Get expression as infix string
		string infixStr() const;

		// Evaluate expression
		vars::valueptr evaluate() const;
		vars::valueptr evaluate( vars::variables& vars ) const;


		//* Check for equality in expressions
		bool equal( const expression& other ) const noexcept;




		/**********************************************************************
		  Helpers
		**********************************************************************/
	private:

		// Parse expression
		void _parse( string&& expr, vars::variables& vars );

		void _processToken( tokenparser& parser, vars::variables& vars, vars::operators::code& last_type,
			std::stack<vars::operators::code>& op_stack, std::stack<expr::nodeptr>& tree_stack );

		expr::nodeptr _parseValue( tokenparser& parser, vars::variables& vars );



		
		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:
		std::vector<expr::nodeptr> Roots;
	};
}
