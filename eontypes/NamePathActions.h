#pragma once

#include "NamePath.h"
#include "Action.h"
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
			inline NamePathConstruct() : Action( name_namepath, Type::instance, name_construct, name_namepath, DynamicTuple() ) {}
			virtual ~NamePathConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class NamePathCopyConstruct : public type::Action
		{
		public:
			inline NamePathCopyConstruct( scope::Global& scope ) : Action( name_namepath, 
				Type::instance, name_construct, name_namepath, DynamicTuple(
					{ type::Attribute( name_other, name_namepath ) } ), { name_memory_denied } ) {}
			virtual ~NamePathCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class NamePathCmp : public type::Action
		{
		public:
			inline NamePathCmp( scope::Global& scope ) : Action( name_namepath,  opcode::cmp, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_namepath ) } ) ) {}
			virtual ~NamePathCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
