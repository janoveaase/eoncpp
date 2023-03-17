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
		// Eon Regular expressions parser class - eon::parser::RegexParser.
		//
		// Parses a literal regex value.
		//
		class RegexParser
		{
			///////////////////////////////////////////////////////////////////
			//
			// Public API
			//
		public:

			// Parse literal regex value.
			// Parameters:
			//   state : Parser state for which the next token is the literal regex value.
			// Returns: Node to be part of an expression.
			expression::Node parse( State& state );
		};
	}
}
