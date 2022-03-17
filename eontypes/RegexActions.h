#pragma once

#include "Regex.h"
#include "Action.h"
#include "OperatorAction.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerRegexActions( scope::Global& scope );




	namespace actions
	{
		/**********************************************************************
		  Eon Regex actions
		**********************************************************************/

		class RegexConstruct : public type::Action
		{
		public:
			inline RegexConstruct() : Action( name_regex, Type::instance, name_constructor, name_regex, DynamicTuple(), {},
				source::Ref() ) {}
			virtual ~RegexConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class RegexCopyConstruct : public type::Action
		{
		public:
			inline RegexCopyConstruct() : Action( name_regex, Type::instance, name_constructor, name_regex, DynamicTuple( {
				type::Attribute( name_other, name_regex ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~RegexCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class RegexStringConstruct : public type::Action
		{
		public:
			inline RegexStringConstruct() : Action( name_regex, Type::instance, name_constructor, name_regex, DynamicTuple( {
				type::Attribute( name_other, name_string ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~RegexStringConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class RegexCmp : public OperatorAction
		{
		public:
			inline RegexCmp() : OperatorAction( name_regex, opcode::cmp, name_int, DynamicTuple( {
				type::Attribute( name_other, name_regex ) } ), {}, source::Ref() ) {}
			virtual ~RegexCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
