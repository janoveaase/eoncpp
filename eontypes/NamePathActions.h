#pragma once

#include "NamePath.h"
#include "Action.h"
#include "OperatorAction.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerNamePathActions( scope::Global& scope );




	namespace actions
	{
		/**********************************************************************
		  Eon NamePath actions
		**********************************************************************/

		class NamePathConstruct : public type::Action
		{
		public:
			inline NamePathConstruct() : Action( name_namepath, Type::instance, name_constructor, name_namepath,
				DynamicTuple(), {}, source::Ref() ) {}
			virtual ~NamePathConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class NamePathCopyConstruct : public type::Action
		{
		public:
			inline NamePathCopyConstruct() : Action( name_namepath, Type::instance, name_constructor, name_namepath,
				DynamicTuple( { type::Attribute( name_other, name_namepath ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~NamePathCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class NamePathCmp : public OperatorAction
		{
		public:
			inline NamePathCmp() : OperatorAction( name_namepath, opcode::cmp, name_int, DynamicTuple( {
				type::Attribute( name_other, name_namepath ) } ), {}, source::Ref() ) {}
			virtual ~NamePathCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
