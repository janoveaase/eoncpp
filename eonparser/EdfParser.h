#pragma once

#include "TupleParserBase.h"



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
		// Eon Data Format parser class - eon::parser::EdfParser.
		//
		// Parses input in EDF format.
		//
		class EdfParser : public TupleParserBase
		{
			///////////////////////////////////////////////////////////////////
			//
			// Public API
			//
		public:

			// Parse data tuple provided as EDF.
			// Parameters:
			//   state : Parser state for which the next token is the first in the EDF document.
			//   scope : Scope under which the EDF is to be parsed.
			// Returns: Node to be part of an expression.
			expression::Node parse( State& state, Tuple& scope );

			// Parse data tuple provided as EDF.
			// Parameters:
			//   state : Parser state for which the next token is the first in the EDF document.
			//   scope : Scope under which the EDF is to be parsed.
			// Returns: Raw data tuple.
			Tuple parseRaw( State& state, Tuple& scope );




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		PRIVATE:

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

			class ParserData
			{
			public:
				ParserData() = delete;
				ParserData( parser::State& state, Tuple& scope );

				inline bool isNamedAttribute() {
					return tokens().viewed().is( name_name ) && tokens().exists()
						&& tokens().peekAhead().isOneOf( { symbol_assign, symbol_colon } ); }

				inline TokenParser& tokens() { return State->Tokens; }

				inline void indent() {
					Indentations.push( Indentation::normal( tokens().viewed().str().numChars() ) ); tokens().forward(); }
				void forceIndent() { Indentations.push( Indentation::forced( Indentations.top().Value + 2 ) ); }
				bool atIncreasedIndentation( index_t offset = 0 ) const noexcept;
				bool atReducedIndentation( index_t offset = 0 ) const noexcept;
				bool atEndOfBlock() const {
					return State->Tokens.atEnd() || ( State->Tokens.viewed().is( name_indentation )
						&& State->Tokens.viewed().str().numChars() < Indentations.bottom().Value ); }
				inline void unindent() { if( !Indentations.empty() ) Indentations.pop(); }

				// Check if we have a sequence where the current token type is repeated,
				// either without separation or with newline and possibly indentation.
				bool haveSequence( IndentationMatch match_indentation ) const noexcept;

				void recordAttributeName();
				void recordNameValueSeparator();
				inline name_t attributeName() const noexcept { return AttributeName; }
				inline void clearAttributeName() noexcept { AttributeName = no_name; }

				inline void addSubtuple() { Data.top()->addTuple( AttributeName, tuple::newData() ); }
				inline void pushLastSubtuple() {
					Data.push( &Data.top()->attribute( Data.top()->numAttributes() - 1 ).value<Tuple>() );
					AttributeName = no_name; }
				inline void popSubtuple() { Data.pop(); }
				inline Tuple& tuple() { return *Data.top(); }

				inline void reportMissingSingletonValue() {
					Errors = true; State->Report->error(
						"A value must follow '=' for named singleton attributes!", NameSepSource ); }

				inline parser::State& state() noexcept { return *State; }
				inline Tuple& scope() noexcept { return *Scope; }

				bool ensureNewLine();

				void error( eon::string&& message, source::Ref* source = nullptr );

				inline bool hasErrors() const noexcept { return Errors; }
				inline Tuple&& root() noexcept { return std::move( Root ); }
				inline const source::Ref& source() const noexcept { return Source; }

			private:
				parser::State* State{ nullptr };
				Tuple* Scope{ nullptr };
				Tuple Root{ tuple::newData() };
				stack<Tuple*> Data{ &Root };
				source::Ref Source;
				stack<Indentation> Indentations;
				name_t AttributeName{ no_name };
				source::Ref NameSource, NameSepSource;
				bool Errors{ false };
			};

			enum class NewlinePolicy
			{
				ignore,
				stop
			};

			void _parse( ParserData& data, NewlinePolicy policy );

			void _parseAttribute( ParserData& data );
			void _parseDashAttribute( ParserData& data );
			void _parseNamedAttribute( ParserData& data );
			void _parseNamedSingleton( ParserData& data );
			void _skipLegalNamedSingletonWhitespaces( ParserData& data );
			bool _hasLegalValue( ParserData& data );
			void _parseNamedSubtuple( ParserData& data );
			bool _parseSubtupleValue( ParserData& data );

			void _parseUnnamedAttributes( ParserData& data );
			void _parseValue( ParserData& data );
			inline void _parseBoolValue( ParserData& data ) {
				data.tuple().add( data.attributeName(), data.tokens().viewed().str() == "true" ); }
			inline void _parseByteValue( ParserData& data ) {
				data.tuple().add( data.attributeName(), static_cast<byte_t>( *data.tokens().viewed().str().begin() ) ); }
			inline void _parseCharValue( ParserData& data ) {
				data.tuple().add( data.attributeName(), static_cast<char_t>( *data.tokens().viewed().str().begin() ) ); }
			void _parseIntValue( ParserData& data );
			void _parseFloatValue( ParserData& data );
			void _parseNameValue( ParserData& data );
			void _parseBytesValue( ParserData& data );
			void _parseStringValue( ParserData& data );
			inline void _parseRegexValue( ParserData& data ) {
				data.tuple().add( data.attributeName(), regex( data.tokens().viewed().str() ) ); }
			inline void _parseNamepathValue( ParserData& data ) {
				data.tuple().add( data.attributeName(), namepath( data.tokens().viewed().str() ) ); }
			inline void _parsePathValue( ParserData& data ) {
				data.tuple().add( data.attributeName(), path( data.tokens().viewed().str() ) ); }
			void _parseExpressionValue( ParserData& data );
			void _parseTypeTuple( ParserData& data );
			void _parseSubtuple( ParserData& data );
		};
	}
}
