#pragma once

#include "Path.h"
#include "Action.h"
#include "OperatorAction.h"
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
			inline PathConstruct() : Action( name_path, Type::instance, name_constructor, name_path, DynamicTuple(), {},
				source::Ref() ) {}
			virtual ~PathConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class PathCopyConstruct : public type::Action
		{
		public:
			inline PathCopyConstruct() : Action( name_path, Type::instance, name_constructor, name_path, DynamicTuple( {
				type::Attribute( name_other, name_path ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~PathCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class PathStringConstruct : public type::Action
		{
		public:
			inline PathStringConstruct() : Action( name_path, Type::instance, name_constructor, name_path, DynamicTuple( {
				type::Attribute( name_other, name_string ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~PathStringConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class PathCmp : public OperatorAction
		{
		public:
			inline PathCmp() : OperatorAction( name_path, opcode::cmp, name_int, DynamicTuple( {
				type::Attribute( name_other, name_path ) } ), {}, source::Ref() ) {}
			virtual ~PathCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class PathPlus : public OperatorAction
		{
		public:
			inline PathPlus() : OperatorAction( name_path, opcode::plus, name_path, DynamicTuple( {
				type::Attribute( name_other, name_path ) } ), {}, source::Ref() ) {}
			virtual ~PathPlus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
