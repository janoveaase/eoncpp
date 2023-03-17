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
		// Eon Integer parser class - eon::parser::IntParser.
		//
		// Parses a literal integer value.
		//
		class IntParser
		{
			///////////////////////////////////////////////////////////////////
			//
			// Public API
			//
		public:

			// Parse literal integer value.
			// Parameters:
			//   state : Parser state for which the next token is the literal integer value.
			// Returns: Node to be part of an expression.
			expression::Node parse( State& state );




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		private:

			expression::Node _intValue( State& state, int_t value );
			expression::Node _longValue( State& state, long_t value );
		};
	}
}
