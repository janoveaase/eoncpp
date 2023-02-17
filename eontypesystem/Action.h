#pragma once

#include "Definitions.h"
#include "TypeTuple.h"
#include "Attribute.h"
#include <eonsource/SourceReporter.h>



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// The 'type' namespace enclosing all background type functionality
	//
	namespace type
	{
		// Action return types
		enum class sig_t : uint8_t
		{
			undef,
			_normal,
			_break,
			_continue,
			_return,
			_raise
		};


		// Action exection argument object
		class ActionExeArgs
		{
		public:
			ActionExeArgs() = delete;
			inline ActionExeArgs( Tuple& scope ) noexcept { Scope = &scope; }

			Tuple& scope() noexcept { return *Scope; }
			stack<Attribute>& values() noexcept { return Values; }

		private:
			Tuple* Scope{ nullptr };
			stack<Attribute> Values;
		};



		///////////////////////////////////////////////////////////////////////
		//
		// Super-class for action objects
		//
		class Action
		{
		public:
			Action() = delete;
			Action( TypeTuple signature ) { Signature = signature; }
			virtual ~Action() = default;

			// Record a source for this action.
			inline void source( source::Ref source ) noexcept { Source = source; }

			// Get source for this action (will be void for built-in actions).
			inline const source::Ref& source() const noexcept { return Source; }

			inline const TypeTuple& signature() const noexcept { return Signature; }

			// Test that action can be executed, report issues if not.
			virtual bool canExecute( ActionExeArgs& args, source::Reporter& reporter ) const { return true; }

			// Execute action and return a signal (normally sig_t::norm).
			virtual sig_t operator()( ActionExeArgs& args ) const = 0;

		private:
			TypeTuple Signature;
			source::Ref Source;
		};
	}
}
