#pragma once

#include "ParserState.h"
#include <eonexpression/ExpressionNode.h>



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
		// Eon Char parser class - eon::parser::CharParser.
		//
		// Parses a literal char_t value.
		//
		class CharParser
		{
			///////////////////////////////////////////////////////////////////
			//
			// Public API
			//
		public:

			// Parse literal char_t value.
			// Parameters:
			//   state : Parser state for which the next token is the literal char_t value.
			// Returns: Node to be part of an expression.
			expression::Node parse( State& state );
		};
	}
}
