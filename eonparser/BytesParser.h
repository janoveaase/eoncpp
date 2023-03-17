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
		// Eon Bytes parser class - eon::parser::BytesParser.
		//
		// Parses a literal bytes value.
		//
		class BytesParser
		{
			///////////////////////////////////////////////////////////////////
			//
			// Public API
			//
		public:

			// Parse literal bytes value.
			// Parameters:
			//   state : Parser state for which the next token is the literal bytes value.
			// Returns: Node to be part of an expression.
			expression::Node parse( State& state );
		};
	}
}
