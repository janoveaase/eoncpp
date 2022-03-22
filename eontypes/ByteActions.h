#pragma once

#include "Byte.h"
#include "Action.h"
#include "OperatorAction.h"
#include "Bool.h"
#include "Char.h"
#include "Integer.h"
#include "Scope.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerByteActions( scope::Global& scope );




	namespace actions
	{
		/**********************************************************************
		  Eon byte actions
		**********************************************************************/

		class ByteConstruct : public type::Action
		{
		public:
			inline ByteConstruct() : Action( name_byte, Type::instance, name_constructor, name_byte, DynamicTuple(), {},
				source::Ref() ) {}
			virtual ~ByteConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ByteCopyConstruct : public type::Action
		{
		public:
			inline ByteCopyConstruct() : Action( name_byte, Type::instance, name_constructor, name_byte, DynamicTuple( {
				type::Attribute( name_other, new ByteInstance() ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~ByteCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class ByteCmp : public OperatorAction
		{
		public:
			inline ByteCmp() : OperatorAction( name_byte, opcode::cmp, name_int, DynamicTuple( {
				type::Attribute( name_other, new ByteInstance() ) } ), {}, source::Ref() ) {}
			virtual ~ByteCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
