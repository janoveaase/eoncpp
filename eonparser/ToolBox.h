#pragma once
#include "Element.h"
#include <eonsource/String.h>
#include <eonsource/File.h>
#include <eonsource/SourceReporter.h>
#include <eonfilesys/Path.h>
#include <eonregex/RegEx.h>
#include <eontypes/Scope.h>
#include <eontypes/Node.h>
#include <eontypes/Operators.h>
#include <eontypes/Tuple.h>
#include <eonstack/Stack.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace parser
	{
		/**********************************************************************
		  Eon Parser ToolBox Class - eon::ToolBox

		  Data and tools used in parsing.
		**********************************************************************/
		class ToolBox
		{
			/******************************************************************
			  Definitions
			******************************************************************/
		public:

			//* Æon parser is context aware and will interpret tokens
			//* differently for different contexts.
			enum class Context
			{
				global,			// The global context
				definition,		// A definition
				extension,		// An enum extend section
				actiondefs,		// Definition of actions
				details,		// Action details section
				args,			// Action arguments detail
				test,			// Action test detail
				body,			// Action body
				lambda,			// A lambda implementation
				switcher,		// A switch section
				trait,			// A trait implementation

				expression,		// An expression
				type_tuple,		// A type tuple
				tuple,			// A tuple that is undecided as of yet
				plain_tuple,	// A plain tuple
				dynamic_tuple,	// A dynamic tuple
				data_tuple,		// A data tuple
				meta_data,		// A meta data tuple
			};




			/******************************************************************
			  Construction
			******************************************************************/
		public:

			ToolBox() = default;
			ToolBox( std::vector<Element>&& elements, source::Reporter& reporter, scope::Scope& scope ) {
				Elements = std::move( elements ); Reporter = &reporter; Scope = &scope; pushIndent( 0 ); }
			ToolBox( const ToolBox& ) = delete;
			ToolBox( ToolBox&& other ) noexcept { *this = std::move( other ); }

			virtual ~ToolBox() = default;


			ToolBox& operator=( const ToolBox& ) = delete;
			ToolBox& operator=( ToolBox&& other ) noexcept;





			/******************************************************************
			  Tools
			******************************************************************/
		public:

			//* Create a new ToolBox object for the same data, but with parsing
			//* details reset
			//* The new ToolBox object must be sync'd back when done!
			//* 'this' ToolBox cannot be used in the meantime!!
			ToolBox split();

			//* Sync back a previously split off ToolBox object
			void sync( ToolBox&& other ) noexcept;


			//* Access the reporter
			inline source::Reporter& reporter() const noexcept { return *Reporter; }


			//* Check context
			inline Context context() const noexcept { return ContextStack.empty() ? Context::global : ContextStack.top(); }
			inline bool underTupleContext() const noexcept {
				return ContextStack.size() > 1 && ContextStack.at( 1 ) == Context::tuple; }

			//* Push a new context onto the context stack
			inline void push( Context context ) { ContextStack.push( context ); }

			//* Pop the current context from stack
			inline void popContext() noexcept { ContextStack.pop(); }


			//* Push a new indentation level onto the indentation stack
			inline void pushIndent( index_t level ) { IndentStack.push( level ); }

			//* Access the current indentation level
			inline index_t indentLevel() const noexcept { return IndentStack.top(); }

			//* Pop the current indentation level
			inline void popIndent() noexcept { IndentStack.pop(); }


			//* Make sure there is a current element
			inline operator bool() const noexcept { return CurElm < Elements.size(); }

			//* Access the current element
			inline const Element& current() const { return Elements[ CurElm ]; }

			//* Consume the object value of current element
			//* WARNING: No checking is done to see if it is an existing object value!
			inline type::Object* consumeObject() { return Elements[ CurElm ].consumeObjValue(); }

			//* Check if there is an element a number of steps ahead
			inline bool exists( int_t steps = 1 ) const noexcept {
				return steps < 0 ? CurElm >= -steps : CurElm + steps < Elements.size(); }

			//* Look ahead a number of elements
			inline const Element& peek( int_t steps = 1 ) const { return Elements[ CurElm + steps ]; }

			//* Move current element marker forward
			inline void forward( index_t steps = 1 ) noexcept { CurElm += steps; }

			//* Move current element marker backward
			inline void backward( index_t steps = 1 ) noexcept { if( steps >= CurElm ) CurElm = 0; else CurElm -= steps; }

			//* Access the last element
			inline const Element& last() const { return Elements[ Elements.size() - 1 ]; }


			//* Access the operator stack
			inline stack<type::Action*>& opStack() noexcept { return OpStack; }

			//* Access the tree stack
			inline stack<type::Node>& treeStack() noexcept { return TreeStack; }

			//* Check if operator and tree stacks are ready a new expression
			inline bool ready() const noexcept { return OpStack.empty() && TreeStack.empty(); }

			//* Reset the operator and tree stacks
			void reset() noexcept;


			//* Access the current scope
			inline scope::Scope& scope() noexcept { return *Scope; }




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		private:





			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

			source::Reporter* Reporter{ nullptr };

			std::vector<Element> Elements;
			size_t CurElm{ 0 };

			stack<Context> ContextStack;
			stack<index_t> IndentStack;

			stack<type::Action*> OpStack;
			stack<type::Node> TreeStack;

			scope::Scope* Scope{ nullptr };
		};
	}
}
