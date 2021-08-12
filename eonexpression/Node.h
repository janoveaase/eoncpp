#pragma once
#include <eonstring/String.h>
#include <eonvariables/Operators.h>
#include <eonvariables/Variables.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace tup
	{
		class value;
		using valueptr = std::shared_ptr<value>;
		class variables;
	}

	/**************************************************************************
	  The 'eon::expr' namespace encloses all hidden expression functionality
	**************************************************************************/
	namespace expr
	{
		//* Exception thrown when dividing by zero
		EONEXCEPT( DivisionByZero );




		/**********************************************************************
		  Eon Node Class - eon::expr::node

		  Expressions are organized in trees, where each branch is a operator
		  and each leaf is a value.
		**********************************************************************/
		class node;
		using nodeptr = std::shared_ptr<node>;
		class node
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			node() = default;
			node( const node& ) = delete;
			inline node( node&& other ) noexcept { *this = std::move( other ); }
			node( vars::operators::code node_type ) { Type = node_type; }

			virtual ~node() = default;




			/******************************************************************
			  Modifier Methods
			******************************************************************/
		public:

			node& operator=( const node& ) = delete;
			inline node& operator=( node&& other ) noexcept {
				Type = other.Type; other.Type = vars::operators::code::undef;
				Children = std::move( other.Children ); return *this; }

			inline void addChild( const nodeptr& child ) {Children.push_back( child ); }

			// Register a tuple as context to references that might exist in this
			// expression
			virtual void setContext( vars::valueptr context ) = 0;




			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline vars::operators::code type() const noexcept { return Type; }
			inline bool isOperator() const noexcept {
				return Type != vars::operators::code::undef && Type != vars::operators::code::value; }

			inline const std::vector<nodeptr>& children() const noexcept { return Children; }

			//* Get this node only as a string
			virtual string str() const = 0;

			//* Do postorder traversal, obtaining each node and sub-tree as a
			//* string
			virtual string postorderStr() const = 0;

			//* Do inorder traversal, obtaining each node and sub-tree as a
			//* string
			virtual string inorderStr() const = 0;

			//* Evaluate expression, accept lack of value as bool false
			vars::valueptr evaluate( vars::variables& vars ) const;

			//* Evaluate expression, return void valueptr if no value
			virtual vars::valueptr evaluateAccurate( vars::variables& vars ) const = 0;


			//* Get a copy
			virtual nodeptr copy() const = 0;

			//* Check if equal
			virtual bool equal( const nodeptr& other ) const noexcept = 0;




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		protected:
			vars::operators::code Type{ vars::operators::code::undef };
			std::vector<nodeptr> Children;
		};
	}
}
