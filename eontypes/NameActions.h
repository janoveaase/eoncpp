#pragma once

#include "Name.h"
#include "Action.h"
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
			inline NameConstruct() : Action( name_name,  Type::instance, name_construct, name_name, DynamicTuple() ) {}
			virtual ~NameConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class NameCopyConstruct : public type::Action
		{
		public:
			inline NameCopyConstruct( scope::Global& scope ) : Action( name_name,  Type::instance, name_construct, name_name,
				DynamicTuple( { type::Attribute( name_other, name_name ) } ), { name_memory_denied } ) {}
			virtual ~NameCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class NameCmp : public type::Action
		{
		public:
			inline NameCmp( scope::Global& scope ) : Action( name_name,  opcode::cmp, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_name ) } ) ) {}
			virtual ~NameCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
