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
		// Eon Tuple parser base class - eon::parser::TupleParserBase.
		//
		// Parent class of TupleParser, TypeTupleParser, and ActionCallParser.
		//
		class TupleParserBase
		{
			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		PROTECTED:

			name_t _parseAttributeName( State& state );
		};
	}
}
