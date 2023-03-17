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
		// Eon Path parser class - eon::parser::PathParser.
		//
		// Parses a literal path value.
		//
		class PathParser
		{
			///////////////////////////////////////////////////////////////////
			//
			// Public API
			//
		public:

			// Parse literal path value.
			// Parameters:
			//   state : Parser state for which the next token is the literal path value.
			// Returns: Node to be part of an expression.
			expression::Node parse( State& state );
		};
	}
}
