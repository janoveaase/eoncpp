#pragma once

#include "Char.h"
#include "Action.h"
#include "OperatorAction.h"
#include "Scope.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerCharActions( scope::Global& scope );




	namespace actions
	{
		using namespace scope;
		using namespace type;

		/**************************************************************************
		  Eon char actions
		**************************************************************************/

		class CharConstruct : public type::Action
		{
		public:
			inline CharConstruct() : Action( name_char, Type::instance, name_constructor, name_char, DynamicTuple(), {},
				source::Ref() ) {}
			virtual ~CharConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class CharCopyConstruct : public type::Action
		{
		public:
			inline CharCopyConstruct() : Action( name_char, Type::instance, name_constructor, name_char, DynamicTuple( {
				type::Attribute( name_other, name_char ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~CharCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class CharByteConstruct : public type::Action
		{
		public:
			inline CharByteConstruct() : Action( name_char, Type::instance, name_constructor, name_char, DynamicTuple( {
				type::Attribute( name_other, name_byte ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~CharByteConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class CharCmp : public OperatorAction
		{
		public:
			inline CharCmp() : OperatorAction( name_char, opcode::cmp, name_int, DynamicTuple( {
				type::Attribute( name_other, name_char ) } ), {}, source::Ref() ) {}
			virtual ~CharCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
