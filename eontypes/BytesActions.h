#pragma once

#include "Bytes.h"
#include "Action.h"
#include "OperatorAction.h"
#include "Scope.h"


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
			inline BytesConstruct() : Action( name_bytes, Type::instance, name_constructor, name_bytes, DynamicTuple(), {},
				source::Ref() ) {}
			virtual ~BytesConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesCopyConstruct : public type::Action
		{
		public:
			inline BytesCopyConstruct() : Action( name_bytes, Type::instance, name_constructor, name_bytes, DynamicTuple( {
				type::Attribute( name_other, name_bytes ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BytesCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class BytesBoolConstruct : public type::Action
		{
		public:
			inline BytesBoolConstruct() : Action( name_bytes, Type::instance, name_constructor, name_bytes, DynamicTuple( {
				type::Attribute( name_other, name_bool ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BytesBoolConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesByteConstruct : public type::Action
		{
		public:
			inline BytesByteConstruct() : Action( name_bytes, Type::instance, name_constructor, name_bytes, DynamicTuple( {
				type::Attribute( name_other, name_byte ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BytesByteConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesCharConstruct : public type::Action
		{
		public:
			inline BytesCharConstruct() : Action( name_bytes, Type::instance, name_constructor, name_bytes, DynamicTuple( {
				type::Attribute( name_other, name_char ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BytesCharConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesIntConstruct : public type::Action
		{
		public:
			inline BytesIntConstruct() : Action( name_bytes, Type::instance, name_constructor, name_bytes, DynamicTuple( {
				type::Attribute( name_other, name_int ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BytesIntConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesShortConstruct : public type::Action
		{
		public:
			inline BytesShortConstruct() : Action( name_bytes, Type::instance, name_constructor, name_bytes, DynamicTuple( {
				type::Attribute( name_other, name_short ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BytesShortConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesLongConstruct : public type::Action
		{
		public:
			inline BytesLongConstruct() : Action( name_bytes, Type::instance, name_constructor, name_bytes, DynamicTuple( {
				type::Attribute( name_other, name_long ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BytesLongConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesFloatConstruct : public type::Action
		{
		public:
			inline BytesFloatConstruct() : Action( name_bytes, Type::instance, name_constructor, name_bytes, DynamicTuple( {
				type::Attribute( name_other, name_float ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BytesFloatConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesLowConstruct : public type::Action
		{
		public:
			inline BytesLowConstruct() : Action( name_bytes, Type::instance, name_constructor, name_bytes, DynamicTuple( {
				type::Attribute( name_other, name_low ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BytesLowConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesHighConstruct : public type::Action
		{
		public:
			inline BytesHighConstruct() : Action( name_bytes, Type::instance, name_constructor, name_bytes, DynamicTuple( {
				type::Attribute( name_other, name_high ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BytesHighConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesIndexConstruct : public type::Action
		{
		public:
			inline BytesIndexConstruct() : Action( name_bytes, Type::instance, name_constructor, name_bytes, DynamicTuple( {
				type::Attribute( name_other, name_index ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BytesIndexConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class BytesCmp : public OperatorAction
		{
		public:
			inline BytesCmp() : OperatorAction( name_bytes, opcode::cmp, name_int, DynamicTuple( {
				type::Attribute( name_other, name_bytes ) } ), {}, source::Ref() ) {}
			virtual ~BytesCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BytesPlus : public OperatorAction
		{
		public:
			inline BytesPlus() : OperatorAction( name_bytes, opcode::plus, name_bytes, DynamicTuple( {
				type::Attribute( name_other, name_bytes ) } ), {}, source::Ref() ) {}
			virtual ~BytesPlus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
