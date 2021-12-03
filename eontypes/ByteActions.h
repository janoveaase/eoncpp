#pragma once

#include "Byte.h"
#include "Action.h"
#include "Bool.h"
#include "Char.h"
#include "Int.h"
#include <eonscopes/Scope.h>


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
			inline ByteConstruct() : Action( name_byte, Type::instance, name_construct, name_byte, DynamicTuple() ) {}
			virtual ~ByteConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ByteCopyConstruct : public type::Action
		{
		public:
			inline ByteCopyConstruct( scope::Global& scope ) : Action( name_byte, Type::instance, name_construct, name_byte,
				DynamicTuple( { type::Attribute( name_other, new ByteInstance() ) } ), { name_memory_denied } ) {}
			virtual ~ByteCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class ByteCmp : public type::Action
		{
		public:
			inline ByteCmp( scope::Global& scope ) : Action( name_byte, opcode::cmp, name_int, DynamicTuple(
				{ type::Attribute( name_other, new ByteInstance() ) } ) ) {}
			virtual ~ByteCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
