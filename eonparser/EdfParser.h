#pragma once

#include "TupleParserBase.h"
#include "EdfParserData.h"



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

			enum class NewlinePolicy
			{
				ignore,
				stop
			};

			void _parse( EdfData& data, NewlinePolicy policy );

			void _parseAttribute( EdfData& data );
			void _parseDashAttribute( EdfData& data );
			void _parseNamedAttribute( EdfData& data );
			void _parseNamedSingleton( EdfData& data );
			void _skipLegalNamedSingletonWhitespaces( EdfData& data );
			bool _hasLegalValue( EdfData& data );
			void _parseNamedSubtuple( EdfData& data );
			bool _parseSubtupleValue( EdfData& data );

			void _parseUnnamedAttributes( EdfData& data );
			void _parseValue( EdfData& data );
			inline void _parseBoolValue( EdfData& data ) {
				data.tuple().add( data.attributeName(), data.tokens().viewed().str() == "true" ); }
			inline void _parseByteValue( EdfData& data ) {
				data.tuple().add( data.attributeName(), static_cast<byte_t>( *data.tokens().viewed().str().begin() ) ); }
			inline void _parseCharValue( EdfData& data ) {
				data.tuple().add( data.attributeName(), static_cast<char_t>( *data.tokens().viewed().str().begin() ) ); }
			void _parseIntValue( EdfData& data );
			void _parseFloatValue( EdfData& data );
			void _parseNameValue( EdfData& data );
			void _parseBytesValue( EdfData& data );
			void _parseStringValue( EdfData& data );
			inline void _parseRegexValue( EdfData& data ) {
				data.tuple().add( data.attributeName(), regex( data.tokens().viewed().str() ) ); }
			inline void _parseNamepathValue( EdfData& data ) {
				data.tuple().add( data.attributeName(), namepath( data.tokens().viewed().str() ) ); }
			inline void _parsePathValue( EdfData& data ) {
				data.tuple().add( data.attributeName(), path( data.tokens().viewed().str() ) ); }
			void _parseExpressionValue( EdfData& data );
			void _parseTypeTuple( EdfData& data );
			void _parseSubtuple( EdfData& data );

			inline bool _haveInitialRepeatedSequence( EdfData& data ) const noexcept {
				return data.nextIsRepeated() || ( data.nextIsNewline() && data.atOffsetIsGreaterIndentation( 2 )
					&& data.atOffsetIsRepeated( 3 ) ); }
			inline bool _haveSubsequentRepeatedSequence( EdfData& data ) const noexcept {
				return data.nextIsRepeated() || ( data.nextIsNewline() && data.atOffsetIsSameOrGreaterIndentation( 2 )
					&& data.atOffsetIsRepeated( 3 ) ); }
			inline void _skipToRepeated( EdfData& data ) const noexcept {
				auto type = data.tokens().viewed().type();
				for( data.tokens().forward(); !data.tokens().viewed().is( type ); data.tokens().forward() ); }
		};
	}
}
