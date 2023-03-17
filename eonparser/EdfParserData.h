#pragma once

#include "ParserState.h"



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// The 'parser' namespace encloses all background parser functionality
	//
	namespace parser
	{
		///////////////////////////////////////////////////////////////////////
		//
		// EDF parser data class - eon::parser::EdfData.
		//
		// Information-carrying object with some functionality for use by EdfParser.
		//
		class EdfData
		{
			///////////////////////////////////////////////////////////////////
			//
			// Construction
			//
		public:

			// No default construction permitted!
			EdfData() = delete;

			// Construct using specified state and scope.
			EdfData( parser::State& state, Tuple& scope );





			///////////////////////////////////////////////////////////////////
			//
			// Token sequence
			//
		public:

			// Access the token sequence.
			inline TokenParser& tokens() { return State->Tokens; }

			// Check if the current token is the name of a named attribute.
			inline bool isNamedAttribute() {
				return tokens().viewed().is( name_name ) && tokens().exists()
					&& tokens().peekAhead().isOneOf( { symbol_assign, symbol_colon } ); }


			// Check if the next token has the same type as the current.
			inline bool nextIsRepeated() const noexcept { return atOffsetIsRepeated( 1 ); }

			// Check if the token at current position + specified offset has the same type as the current.
			inline bool atOffsetIsRepeated( index_t offset ) const noexcept {
				return State->Tokens.exists( static_cast<int>( offset ) ) && State->Tokens.peekAhead(
					static_cast<int>( offset ) ).is( State->Tokens.viewed().type() );}

			// Check if the next token is a newline.
			inline bool nextIsNewline() const noexcept {
				return State->Tokens.exists() && State->Tokens.peekAhead().is( name_newline ); }


			// If the next token is comma, then skip past it (it is optional).
			inline void skipOptionalComma() {
				if( !State->Tokens.atEnd() && State->Tokens.viewed().is( symbol_comma ) ) State->Tokens.forward(); }




			///////////////////////////////////////////////////////////////////
			//
			// Indentation
			//
			// Indentation levels are recorded in a stack. An increase in
			// indentation means we are entering a block, a decrease means we
			// are exiting a block.
			//
		public:

			// Record (push) indentation as seen in current token.
			inline void indent() {
				Indentations.push( Indentation::normal( tokens().viewed().str().numChars() ) ); tokens().forward(); }

			// Record (push) indentation as one beyond current level regardless of what it is or what the current token is.
			inline void forceIndent() { Indentations.push( Indentation::forced( Indentations.top().Value + 2 ) ); }

			// Check if the token at current position + specified offset is
			// indentation that is greater than currently recorded.
			// NOTE: If recorded offset level is "forced", then also allow for offset to be the same!
			bool atOffsetIsGreaterIndentation( index_t offset ) const noexcept;

			// Check if the token at current position + specified offset is
			// indentation that is greater than or equal to that currently recorded.
			bool atOffsetIsSameOrGreaterIndentation( index_t offset ) const noexcept;

			// Check if the token at current position + specified offset is
			// indentation that is lesser than currently recorded.
			bool atOffsetIsLowerIndentation( index_t offset = 0 ) const noexcept;

			// Check if indentation has dropped below the level automatically
			// recorded when started parsing the EDF data.
			// (I.e., we have reached the overall end of the EDF data.)
			bool atEndOfBlock() const {
				return State->Tokens.atEnd() || ( State->Tokens.viewed().is( name_indentation )
					&& State->Tokens.viewed().str().numChars() < Indentations.bottom().Value ); }

			// Drop (pop) currently recorded indentation level (revert to what it was before).
			inline void unindent() { if( !Indentations.empty() ) Indentations.pop(); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes parsing
			//
		public:

			// Record current token as attribute name (move past it).
			// Recording a new name will overwrite the old one!
			void recordAttributeName();

			// Record current token as attirbute name-value separator (move past it).
			// Recording a new separator will overwrite the old one!
			void recordNameValueSeparator();

			// Get the currently recorded attribute name.
			inline name_t attributeName() const noexcept { return AttributeName; }

			// Clear the currently recorded attribute name.
			inline void clearAttributeName() noexcept { AttributeName = no_name; }


			// Add a new sub-tuple to the current tuple being parsed.
			inline void addSubtuple() { Data.top()->addTuple( AttributeName, tuple::newData() ); }

			// Push the last added sub-tuple of the current tuple being parsed onto the current tuple stack.
			// (I.e., make it the new current tuple.)
			inline void pushLastSubtuple() {
				Data.push( &Data.top()->attribute( Data.top()->numAttributes() - 1 ).value<Tuple>() );
				AttributeName = no_name; }

			// Pop the current tuple from the current tuple stack, making its parent the new current.
			inline void popSubtuple() { Data.pop(); }

			// Get the current tuple being parsed.
			inline Tuple& tuple() { return *Data.top(); }


			// Mark it so that comma is legal (optional) or illegal as attribute separator.
			// (A stack will keep track of current setting.)
			inline void pushAllowComma( bool allow ) { AllowComma.push( allow ); }

			// Pop current setting from the allow-comma stack.
			// (Go back to how it was before our previous call to pushAllowComma.)
			inline void popAllowComma() noexcept { AllowComma.pop(); }

			// Check if comma is legal (optional) or illegal as attribute separator.
			inline bool allowComma() const noexcept { return AllowComma.empty() || AllowComma.top(); }




			///////////////////////////////////////////////////////////////////
			//
			// Errors and error handling
			//
		public:

			// Report error about named attribute missing value following '='.
			inline void reportMissingSingletonValue() {
				Errors = true; State->Report->error(
					"A value must follow '=' for named singleton attributes!", NameSepSource ); }

			// Make sure the current token is a newline, if not, then record an error.
			// Returns true if newline has been verified, false if not (and error is recorded).
			bool ensureNewLine();

			// Record an error.
			// Parameters:
			//   message : Error message.
			//   source  : Pointer to optional source, current token will be used if nullptr.
			void error( eon::string&& message, source::Ref* source = nullptr );




			///////////////////////////////////////////////////////////////////
			//
			// Misc.
			//
		public:

			// Access the state object.
			inline parser::State& state() noexcept { return *State; }

			// Access the scope object.
			inline Tuple& scope() noexcept { return *Scope; }


			// Check if one or more errors have been recorded.
			inline bool hasErrors() const noexcept { return Errors; }

			// Extract the root tuple parsed from EDF.
			// Call this at the end, to get final result.
			inline Tuple&& extractRoot() noexcept { return std::move( Root ); }

			// Get the overall source of the entire parsed tuple.
			source::Ref source() const noexcept;




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		PRIVATE:

			inline bool _atOffsetIs( index_t offset, name_t type ) const noexcept {
				return State->Tokens.exists( static_cast<int>( offset ) )
					&& State->Tokens.peekAhead( static_cast<int>( offset ) ).is( type ); }


			struct Indentation
			{
				static inline Indentation normal( index_t value ) { Indentation ind; ind.Value = value; return ind; }
				static inline Indentation forced( index_t value ) {
					Indentation ind; ind.Value = value; ind.Forced = true; return ind; }

				index_t Value{ 0 };
				bool Forced{ false };
			};

			enum class IndentationMatch
			{
				same_or_greater,
				indented
			};




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		PRIVATE:

			parser::State* State{ nullptr };
			Tuple* Scope{ nullptr };
			Tuple Root{ tuple::newData() };
			stack<Tuple*> Data{ &Root };
			source::Ref Source;
			stack<Indentation> Indentations;
			name_t AttributeName{ no_name };
			source::Ref NameSource, NameSepSource;
			stack<bool> AllowComma;
			bool Errors{ false };
		};
	}
}
