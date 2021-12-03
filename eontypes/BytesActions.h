#pragma once

#include "Bytes.h"
#include "Action.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerBytesActions( scope::Global& scope );




	namespace actions
	{
		/**********************************************************************
		  Eon Bytes actions
		**********************************************************************/

		class BytesConstruct : public type::Action
		{
		public:
			inline BytesConstruct() : Action( name_bytes, Type::instance, name_construct, name_bytes, DynamicTuple() ) {}
			virtual ~BytesConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesCopyConstruct : public type::Action
		{
		public:
			inline BytesCopyConstruct( scope::Global& scope ) : Action(
				name_bytes, Type::instance, name_construct, name_bytes, DynamicTuple(
					{ type::Attribute( name_other, name_bytes ) } ), { name_memory_denied } ) {}
			virtual ~BytesCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class BytesBoolConstruct : public type::Action
		{
		public:
			inline BytesBoolConstruct( scope::Global& scope ) : Action(
				name_bytes, Type::instance, name_construct, name_bytes, DynamicTuple(
					{ type::Attribute( name_other, name_bool ) } ), { name_memory_denied } ) {}
			virtual ~BytesBoolConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesByteConstruct : public type::Action
		{
		public:
			inline BytesByteConstruct( scope::Global& scope ) : Action(
				name_bytes, Type::instance, name_construct, name_bytes, DynamicTuple(
					{ type::Attribute( name_other, name_byte ) } ), { name_memory_denied } ) {}
			virtual ~BytesByteConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesCharConstruct : public type::Action
		{
		public:
			inline BytesCharConstruct( scope::Global& scope ) : Action(
				name_bytes, Type::instance, name_construct, name_bytes, DynamicTuple(
					{ type::Attribute( name_other, name_char ) } ), { name_memory_denied } ) {}
			virtual ~BytesCharConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesIntConstruct : public type::Action
		{
		public:
			inline BytesIntConstruct( scope::Global& scope ) : Action(
				name_bytes, Type::instance, name_construct, name_bytes, DynamicTuple(
					{ type::Attribute( name_other, name_int ) } ), { name_memory_denied } ) {}
			virtual ~BytesIntConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesShortConstruct : public type::Action
		{
		public:
			inline BytesShortConstruct( scope::Global& scope ) : Action(
				name_bytes, Type::instance, name_construct, name_bytes, DynamicTuple(
					{ type::Attribute( name_other, name_short ) } ), { name_memory_denied } ) {}
			virtual ~BytesShortConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesLongConstruct : public type::Action
		{
		public:
			inline BytesLongConstruct( scope::Global& scope ) : Action(
				name_bytes, Type::instance, name_construct, name_bytes, DynamicTuple(
					{ type::Attribute( name_other, name_long ) } ), { name_memory_denied } ) {}
			virtual ~BytesLongConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesFloatConstruct : public type::Action
		{
		public:
			inline BytesFloatConstruct( scope::Global& scope ) : Action(
				name_bytes, Type::instance, name_construct, name_bytes, DynamicTuple(
					{ type::Attribute( name_other, name_float ) } ), { name_memory_denied } ) {}
			virtual ~BytesFloatConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesLowConstruct : public type::Action
		{
		public:
			inline BytesLowConstruct( scope::Global& scope ) : Action(
				name_bytes, Type::instance, name_construct, name_bytes, DynamicTuple(
					{ type::Attribute( name_other, name_low ) } ), { name_memory_denied } ) {}
			virtual ~BytesLowConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesHighConstruct : public type::Action
		{
		public:
			inline BytesHighConstruct( scope::Global& scope ) : Action(
				name_bytes, Type::instance, name_construct, name_bytes, DynamicTuple(
					{ type::Attribute( name_other, name_high ) } ), { name_memory_denied } ) {}
			virtual ~BytesHighConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesIndexConstruct : public type::Action
		{
		public:
			inline BytesIndexConstruct( scope::Global& scope ) : Action(
				name_bytes, Type::instance, name_construct, name_bytes, DynamicTuple(
					{ type::Attribute( name_other, name_index ) } ), { name_memory_denied } ) {}
			virtual ~BytesIndexConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class BytesCmp : public type::Action
		{
		public:
			inline BytesCmp( scope::Global& scope ) : Action( name_bytes, opcode::cmp, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_bytes ) } ) ) {}
			virtual ~BytesCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesPlus : public type::Action
		{
		public:
			inline BytesPlus( scope::Global& scope ) : Action( name_bytes, opcode::plus, name_bytes, DynamicTuple(
				{ type::Attribute( name_other, name_bytes ) } ) ) {}
			virtual ~BytesPlus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
