#pragma once

#include "Action.h"
#include "Operators.h"
#include "TypeTupleFactory.h"



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
				typetuple::newAction(
					name_tuple, name_copy, name_operator ).returns( name_tuple ).arguments( { name_tuple } ) ) {}
			bool canExecute( ActionExeArgs& args, source::Reporter& reporter ) const override;
			sig_t operator()( ActionExeArgs& args ) const override;
		};

		struct TupleMove : public Action {
			inline TupleMove() : Action(
				typetuple::newAction(
					name_tuple, name_take, name_operator ).returns( name_tuple ).arguments( { name_tuple } ) ) {}
			bool canExecute( ActionExeArgs& args, source::Reporter& reporter ) const override;
			sig_t operator()( ActionExeArgs& args ) const override;
		};

		struct TupleAssign : public Action {
			inline TupleAssign() : Action(
				typetuple::newAction(
					name_tuple, name_assign, name_operator ).returns( name_tuple ).arguments( { name_tuple } ) ) {}
			bool canExecute( ActionExeArgs& args, source::Reporter& reporter ) const override;
			sig_t operator()( ActionExeArgs& args ) const override;
		};

		void registerTupleActions();
	}
}
