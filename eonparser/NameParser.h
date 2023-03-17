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
		// Eon Name parser class - eon::parser::NameParser.
		//
		// Parses a literal name path value.
		//
		class NameParser
		{
			///////////////////////////////////////////////////////////////////
			//
			// Public API
			//
		public:

			// Parse literal name value.
			// Parameters:
			//   state : Parser state for which the next token is the literal name value.
			// Returns: Node to be part of an expression.
			expression::Node parse( State& state );
		};
	}
}
