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
		// Eon Type tuple parser class - eon::parser::TypeTupleParser.
		//
		// Parses a literal type tuple value.
		//
		class TypeTupleParser : public TupleParserBase
		{
			///////////////////////////////////////////////////////////////////
			//
			// Public API
			//
		public:

			// Parse literal type tuple value.
			// Parameters:
			//   state : Parser state for which the next token is the literal type tuple value.
			//   scope : Scope under which the tuple is to be parsed.
			// Returns: Node to be part of an expression.
			expression::Node parse( State& state, Tuple& scope );

			// Parse literal type tuple value.
			// Parameters:
			//   state : Parser state for which the next token is the literal type tuple value.
			//   scope : Scope under which the tuple is to be parsed.
			// Returns: Type tuple object.
			inline TypeTuple parseRaw( State& state, Tuple& scope ) { return _parseDetails( state ); }




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		PRIVATE:

			TypeTuple _parseDetails( State& state );
			void _parseAttributes( State& state, TypeTuple& ttuple );
			void _parseAttributeValue( State& state, name_t attrib_name, TypeTuple& ttuple );
			void _parseTupleAttributeValue( State& state, name_t attrib_name, TypeTuple& ttuple );
		};
	}
}
