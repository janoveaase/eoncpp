#pragma once
#include <eonstring/String.h>
#include <eontuple/Operators.h>
#include <eontuple/Variables.h>


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
			inline node( node&& other ) noexcept {
				*this = std::move( other ); }
			node( tup::operators::code node_type ) { Type = node_type; }

			virtual ~node() = default;




			/******************************************************************
			  Modifier Methods
			******************************************************************/
		public:

			node& operator=( const node& ) = delete;
			inline node& operator=( node&& other ) noexcept {
				Type = other.Type;
				other.Type = tup::operators::code::undef;
				Children = std::move( other.Children ); return *this; }

			inline void addChild( const nodeptr& child ) {
				Children.push_back( child ); }




			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline tup::operators::code type() const noexcept { return Type; }
			inline bool isOperator() const noexcept {
				return Type != tup::operators::code::undef
					&& Type != tup::operators::code::value; }

			inline const std::vector<nodeptr>& children() const noexcept {
				return Children; }

			//* Get this node only as a string
			virtual string str() const = 0;

			//* Do postorder traversal, obtaining each node and sub-tree as a
			//* string
			virtual string postorderStr() const = 0;

			//* Do inorder traversal, obtaining each node and sub-tree as a
			//* string
			virtual string inorderStr() const = 0;

			//* Evaluate expression
			virtual tup::valueptr evaluate( tup::variables& vars ) const = 0;


			//* Get a copy
			virtual nodeptr copy() const = 0;

			//* Check if equal
			virtual bool equal( const nodeptr& other,
				const tup::variables& vars ) const noexcept = 0;




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		protected:
			tup::operators::code Type{ tup::operators::code::undef };
			std::vector<nodeptr> Children;
		};
	}
}
