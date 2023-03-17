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
		// Eon Name path parser class - eon::parser::NamePathParser.
		//
		// Parses a literal name path value.
		//
		class NamePathParser
		{
			///////////////////////////////////////////////////////////////////
			//
			// Public API
			//
		public:

			// Parse literal name path value.
			// Parameters:
			//   state : Parser state for which the next token is the literal name path value.
			// Returns: Node to be part of an expression.
			expression::Node parse( State& state );
		};
	}
}
