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
		// Eon Byte parser class - eon::parser::ByteParser.
		//
		// Parses a literal byte value.
		//
		class ByteParser
		{
			///////////////////////////////////////////////////////////////////
			//
			// Public API
			//
		public:

			// Parse literal byte value.
			// Parameters:
			//   state : Parser state for which the next token is the literal byte value.
			// Returns: Node to be part of an expression.
			expression::Node parse( State& state );
		};
	}
}
