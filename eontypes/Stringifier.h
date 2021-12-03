#pragma once

#include <eonstring/String.h>
#include <vector>
#include <algorithm>



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace type
	{
		//* Automatic spacing between "words" added to the output string
		enum class Spacing
		{
			none,		//* Do not add spaces automatically
			once,		//* Add before next word, then switch to none
			always		//* Add before every new word (except first)
		};




		/**********************************************************************
		  A stringifier helps turn structural data into string/text format
		**********************************************************************/
		class Stringifier
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			//* Construct new stringifier
			//* If initial_indent is not zero, then indent first line to this
			//* level and set new lines to be indented at this level + 1.
			inline Stringifier( size_t initial_indent = 0 ) { if( initial_indent > 0 ) {
				Output = string( initial_indent * 2, SpaceChr ); Indentation = initial_indent + 1; } }




			/******************************************************************
			  Settings
			******************************************************************/
		public:

			//* Ensure that new lines are indented
			inline void indentNewLines( bool set = true ) noexcept { IndentNewLines = set; }
			inline bool indentNewLines() const noexcept { return IndentNewLines; }

			//* Level of indentation when indenting new lines
			inline void indentation( size_t level ) noexcept { Indentation = level; }
			inline void addIndent() noexcept { ++Indentation; }
			inline void reduceIndent() noexcept { if( Indentation > 0 ) --Indentation; }
			inline size_t indentation() const noexcept { return Indentation; }

			//* Automatically add a space between "words"
			inline void spacingOnce() noexcept { Space = Spacing::once; }
			inline void spacingAlways() noexcept { Space = Spacing::always; }
			inline void noSpacing() noexcept { Space = Spacing::none; }




			/******************************************************************
			  Stringifying
			******************************************************************/
		public:

			//* Add another "word" to the output string
			inline Stringifier& addWord( const string& word ) { _indent(); _addSpacing(); Output += word; return *this; }
			inline Stringifier& addWord( name_t name ) { return addWord( *name ); }

			//* Add spaces to the output string
			//* Automatic spacing "once" will be reset to "none"!
			inline Stringifier& addSpace( size_t spaces = 1 ) {
				_indent(); Output += string( spaces, SpaceChr ); _resetSpacing(); return *this; }

			//* Start a new line in the output string
			//* Will not add if the current line is empty!
			//* Automatic spacing "once" will be reset to "none"!
			//* The new line will not be indented until you add a "word" or
			//* "space" element!
			inline Stringifier& newLine() { _resetSpacing(); Output += NewlineChr; return *this; }

			//* Add raw string
			//* Will not auto-indent new lines!
			//* Will not heed spacing setting, neither before nor after!
			//* Automatic spacing "once" will be reset to "none"!
			inline Stringifier& addRaw( const string& str ) { Raw = true; Output += str; _resetSpacing(); return *this; }

			//* Clear the "raw" marker so that normal spacing will apply to the
			//* next "word" added
			inline void resetRaw() noexcept { Raw = false; }

			//* Set the "raw" marker so that next "word" does not get a space
			//* in front of it, even it spacing is on.
			inline void markRaw() noexcept { Raw = true; }




			/******************************************************************
			  Tuple Stack

			  The tuple stack helps keep track of what is being output. For
			  example, if a meta data tuple is being output, and it contains
			  a sub-tuple that is also meta data, we don't want to add the "M"
			  in front of the sub!
			******************************************************************/
		public:

			//* Register a new tuple being output
			inline void pushTuple( name_t tuple_type ) { TupleStack.push_back( tuple_type ); }

			//* Check if the tuple stack contains a type
			inline bool inTuple( name_t tuple_type ) const noexcept {
				return std::find( TupleStack.begin(), TupleStack.end(), tuple_type ) != TupleStack.end(); }

			//* Pop the current tuple type from the stack
			inline void popTuple() { if( !TupleStack.empty() ) TupleStack.erase( TupleStack.end() - 1 ); }




			/******************************************************************
			  Output
			******************************************************************/
		public:

			const string& output() const noexcept { return Output; }
			string&& output() noexcept { return std::move( Output ); }




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		private:

			inline void _indent() { if( Indentation > 0 && ( Output.empty() || *Output.last() == NewlineChr ) )
				Output += string( Indentation * 2, SpaceChr ); }

			// Add space only if:
			//   1. Spacing is set
			//   2. Not after raw element
			//   3. Not if first in string
			//   4. Not after space
			void _addSpacing() { if( Space != Spacing::none && !Raw && !Output.empty()
				&& *Output.last() != SpaceChr ) Output += " "; _resetSpacing(); }
			void _resetSpacing() noexcept { if( Space == Spacing::once ) Space = Spacing::none; Raw = false; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

			string Output;
			bool IndentNewLines{ false };
			size_t Indentation{ 0 };
			Spacing Space{ Spacing::none };
			bool Raw{ false };
			std::vector<name_t> TupleStack;
		};
	}
}
