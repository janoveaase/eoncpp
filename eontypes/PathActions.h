#pragma once

#include "Path.h"
#include "Action.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerPathActions( scope::Global& scope );




	namespace actions
	{
		/**********************************************************************
		  Eon Path actions
		**********************************************************************/

		class PathConstruct : public type::Action
		{
		public:
			inline PathConstruct() : Action( name_path, Type::instance, name_construct, name_path, DynamicTuple() ) {}
			virtual ~PathConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class PathCopyConstruct : public type::Action
		{
		public:
			inline PathCopyConstruct( scope::Global& scope ) : Action( name_path, Type::instance, name_construct, name_path,
				DynamicTuple( { type::Attribute( name_other, name_path ) } ), { name_memory_denied } ) {}
			virtual ~PathCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class PathStringConstruct : public type::Action
		{
		public:
			inline PathStringConstruct( scope::Global& scope ) : Action(
				name_path, Type::instance, name_construct, name_path,
				DynamicTuple( { type::Attribute( name_other, name_string ) } ), { name_memory_denied } ) {}
			virtual ~PathStringConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class PathCmp : public type::Action
		{
		public:
			inline PathCmp( scope::Global& scope ) : Action( name_path, opcode::cmp, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_path ) } ) ) {}
			virtual ~PathCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class PathPlus : public type::Action
		{
		public:
			inline PathPlus( scope::Global& scope ) : Action( name_path, opcode::plus, name_path, DynamicTuple(
				{ type::Attribute( name_other, name_path ) } ) ) {}
			virtual ~PathPlus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
