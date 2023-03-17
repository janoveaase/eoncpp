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
		// Eon Tuple parser class - eon::parser::TupleParser.
		//
		// Parses a literal tuple value.
		//
		class TupleParser : public TupleParserBase
		{
			///////////////////////////////////////////////////////////////////
			//
			// Public API
			//
		public:

			// Parse literal tuple value.
			// Parameters:
			//   state : Parser state for which the next token is the literal tuple value.
			//   scope : Scope under which the tuple is to be parsed.
			// Returns: Node to be part of an expression.
			expression::Node parse( State& state, Tuple& scope );




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		PRIVATE:

			std::vector<AttributePair> _parseAttributes( State& state, Tuple& scope );
			void _parseAttribute( State& state, Tuple& scope, std::vector<AttributePair>& attributes );
		};
	}
}
