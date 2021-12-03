#pragma once

#include "Regex.h"
#include "Action.h"
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
			inline RegexConstruct() : Action( name_regex, Type::instance, name_construct, name_regex, DynamicTuple() ) {}
			virtual ~RegexConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class RegexCopyConstruct : public type::Action
		{
		public:
			inline RegexCopyConstruct( scope::Global& scope ) : Action( name_regex, Type::instance, name_construct, name_regex,
				DynamicTuple( { type::Attribute( name_other, name_regex ) } ), { name_memory_denied } ) {}
			virtual ~RegexCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class RegexStringConstruct : public type::Action
		{
		public:
			inline RegexStringConstruct( scope::Global& scope ) : Action( name_regex, Type::instance, name_construct, name_regex,
				DynamicTuple( { type::Attribute( name_other, name_string ) } ), { name_memory_denied } ) {}
			virtual ~RegexStringConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class RegexCmp : public type::Action
		{
		public:
			inline RegexCmp( scope::Global& scope ) : Action( name_regex, opcode::cmp, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_regex ) } ) ) {}
			virtual ~RegexCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
