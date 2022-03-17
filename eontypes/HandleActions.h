#pragma once

#include "Handle.h"
#include "Action.h"
#include "OperatorAction.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerHandleActions( scope::Global& scope );




	namespace actions
	{
		/**********************************************************************
		  Eon Handle actions
		**********************************************************************/

		class HandleConstruct : public type::Action
		{
		public:
			inline HandleConstruct() : Action( name_handle, Type::instance, name_constructor, name_handle, DynamicTuple(), {},
				source::Ref() ) {}
			virtual ~HandleConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class HandleCopyConstruct : public type::Action
		{
		public:
			inline HandleCopyConstruct() : Action( name_handle, Type::instance, name_constructor, name_handle, DynamicTuple( {
				type::Attribute( name_other, name_handle ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~HandleCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class HandleCmp : public OperatorAction
		{
		public:
			inline HandleCmp() : OperatorAction( name_handle, opcode::cmp, name_int, DynamicTuple( {
				type::Attribute( name_other, name_handle ) } ), {}, source::Ref() ) {}
			virtual ~HandleCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
