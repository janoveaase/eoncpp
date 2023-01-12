#pragma once

#include "Action.h"
#include "Operators.h"
#include "TypeTuple.h"



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	EON_NAME( variable );

	///////////////////////////////////////////////////////////////////////////
	//
	// The 'type' namespace enclosing all background type functionality
	//
	namespace type
	{
		struct TupleCopy : public Action {
			inline TupleCopy() : Action(
				TypeTuple::action( name_tuple, name_copy, name_operator, name_tuple, { name_tuple } ) ) {}
			bool canExecute( ActionExeArgs& args, source::Reporter& reporter ) const override;
			sig_t operator()( ActionExeArgs& args ) const override;
		};

		struct TupleMove : public Action {
			inline TupleMove() : Action(
				TypeTuple::action( name_tuple, name_take, name_operator, name_tuple, { name_tuple } ) ) {}
			bool canExecute( ActionExeArgs& args, source::Reporter& reporter ) const override;
			sig_t operator()( ActionExeArgs& args ) const override;
		};

		struct TupleAssign : public Action {
			inline TupleAssign() : Action(
				TypeTuple::action( name_tuple, name_assign, name_operator, name_tuple, { name_tuple } ) ) {}
			bool canExecute( ActionExeArgs& args, source::Reporter& reporter ) const override;
			sig_t operator()( ActionExeArgs& args ) const override;
		};

		void registerTupleActions();
	}
}
