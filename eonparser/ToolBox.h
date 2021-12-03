#pragma once
#include <eonsource/String.h>
#include <eonsource/File.h>
#include <eonsource/SourceReporter.h>
#include <eontokenizer/Tokenizer.h>
#include <eontokenizer/ReTokenizer.h>
#include <eonfilesys/Path.h>
#include <eonregex/RegEx.h>
#include <eonscopes/Scope.h>
#include <eontypes/Node.h>
#include <eontypes/Operators.h>
#include <eontypes/Tuple.h>


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
				tuple,			// A plain tuple
				data_tuple,		// A data tuple
				meta_data,		// A meta data tuple
			};




			/******************************************************************
			  Construction
			******************************************************************/
		public:

			ToolBox() = default;
			virtual ~ToolBox() = default;

			inline void init( source::Ref source, source::Reporter& reporter, scope::Scope& scope ) {
				Source = source; Reporter = &reporter; Scope = &scope; _prepare(); }





			/******************************************************************
			  Tools
			******************************************************************/
		public:

			//* Get fully configured Tokenizer object
			Tokenizer& tokenizer();

			//* Get fully configured ReTokenizer object
			ReTokenizer& retok();

			//* Access the source
			inline const source::Ref& source() const noexcept { return Source; }

			//* Access the token parser
			inline TokenParser& parser() noexcept { return Parser; }

			//* Access the reporter
			inline source::Reporter& reporter() const noexcept { return *Reporter; }


			//* Check context
			inline Context context() const noexcept { return ContextStack.empty() ? Context::global : ContextStack.top(); }

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


			//* Access the operator stack
			inline std::stack<type::Action*>& opStack() noexcept { return OpStack; }

			//* Access the tree stack
			inline std::stack<type::Node>& treeStack() noexcept { return TreeStack; }

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

			// Prepare for parsing by tokenizing and re-tokenizing to get an
			// optimal token stream
			void _prepare();




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

			source::Ref Source;
			TokenParser Parser;
			source::Reporter* Reporter{ nullptr };
			std::stack<Context> ContextStack;

			std::stack<index_t> IndentStack;

			std::stack<type::Action*> OpStack;
			std::stack<type::Node> TreeStack;

			scope::Scope* Scope{ nullptr };
		};
	}
}
