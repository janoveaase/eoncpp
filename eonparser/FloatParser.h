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
		// Eon Floating point parser class - eon::parser::FloatParser.
		//
		// Parses a literal floating point value.
		//
		class FloatParser
		{
			///////////////////////////////////////////////////////////////////
			//
			// Public API
			//
		public:

			// Parse literal floating point value.
			// Parameters:
			//   state : Parser state for which the next token is the literal floating point value.
			// Returns: Node to be part of an expression.
			expression::Node parse( State& state );




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		private:

			expression::Node _floatValue( State& state, flt_t value );
			expression::Node _highValue( State& state, high_t value );
		};
	}
}
