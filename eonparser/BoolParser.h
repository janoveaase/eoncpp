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
		// Eon Boolean parser class - eon::parser::BoolParser.
		//
		// Parses a literal Boolean value.
		//
		class BoolParser
		{
			///////////////////////////////////////////////////////////////////
			//
			// Public API
			//
		public:

			// Parse literal Boolean value.
			// Parameters:
			//   state : Parser state for which the next token is the literal Boolean value.
			// Returns: Node to be part of an expression.
			expression::Node parse( State& state );
		};
	}
}
