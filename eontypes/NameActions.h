#pragma once

#include "Name.h"
#include "Action.h"
#include "OperatorAction.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerNameActions( scope::Global& scope );




	namespace actions
	{
		/**********************************************************************
		  Eon name actions
		**********************************************************************/

		class NameConstruct : public type::Action
		{
		public:
			inline NameConstruct() : Action( name_name, Type::instance, name_constructor, name_name, DynamicTuple(), {},
				source::Ref() ) {}
			virtual ~NameConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class NameCopyConstruct : public type::Action
		{
		public:
			inline NameCopyConstruct() : Action( name_name,  Type::instance, name_constructor, name_name, DynamicTuple( {
				type::Attribute( name_other, name_name ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~NameCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class NameCmp : public OperatorAction
		{
		public:
			inline NameCmp() : OperatorAction( name_name, opcode::cmp, name_int, DynamicTuple( {
				type::Attribute( name_other, name_name ) } ), {}, source::Ref() ) {}
			virtual ~NameCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
