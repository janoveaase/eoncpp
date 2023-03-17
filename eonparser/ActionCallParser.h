#pragma once

#include "TupleParserBase.h"
#include <eonexpression/Expression.h>



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
		// Eon Action call parser class - eon::parser::ActionCallParser.
		//
		// Parses an action call.
		//
		class ActionCallParser : public TupleParserBase
		{
			///////////////////////////////////////////////////////////////////
			//
			// Public API
			//
		public:

			// Parse action call.
			// Parameters:
			//   state : Parser state for which the next token is the action call.
			//   scope : Scope under which the call is to be parsed.
			// Returns: Node to be part of an expression.
			expression::Node parse( State& state, Tuple& scope );




			///////////////////////////////////////////////////////////////////
			//
			// Helpers
			//
		private:

			expression::Node _parse( State& state, Tuple& scope, name_t action_name, source::Ref source );

			struct CallArgs
			{
				std::vector<Expression> Args;
				TypeTuple ArgTypes;
			};

			CallArgs _parseActionCallArgs( State& state, Tuple& scope );
			void _parseArgument( State& state, Tuple& scope, CallArgs& args );

			expression::Node _processConstructorCall(
				State& state, Tuple& scope, name_t action_name, CallArgs& args, source::Ref source );
		};
	}
}
