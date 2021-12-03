#pragma once

#include "String.h"
#include "Action.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace actions
	{
		/**********************************************************************
		  Eon generic place-holder operator action
		**********************************************************************/

		class OperatorAction : public type::Action
		{
		public:
			inline OperatorAction( type::operators::code code ) : Action(
				name_operator, Type::_operator, name_operator, no_name, DynamicTuple() ) { Code = code; }
			virtual ~OperatorAction() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;

		private:
			type::operators::code Code{ type::operators::code::undef };
		};
	}
}
