#pragma once

#include "String.h"
#include "Action.h"
#include "OperatorAction.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerStringActions( scope::Global& scope );




	namespace actions
	{
		/**********************************************************************
		  Eon String actions
		**********************************************************************/

		class StringConstruct : public type::Action
		{
		public:
			inline StringConstruct() : Action( name_string, Type::instance, name_constructor, name_string, DynamicTuple(), {},
				source::Ref() ) {}
			virtual ~StringConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringCopyConstruct : public type::Action
		{
		public:
			inline StringCopyConstruct() : Action( name_string, Type::instance, name_constructor, name_string, DynamicTuple( {
				type::Attribute( name_other, name_string ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~StringCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class StringBoolConstruct : public type::Action
		{
		public:
			inline StringBoolConstruct() : Action( name_string, Type::instance, name_constructor, name_string, DynamicTuple( {
				type::Attribute( name_other, name_bool ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~StringBoolConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringByteConstruct : public type::Action
		{
		public:
			inline StringByteConstruct() : Action( name_string, Type::instance, name_constructor, name_string, DynamicTuple( {
				type::Attribute( name_other, name_byte ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~StringByteConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringCharConstruct : public type::Action
		{
		public:
			inline StringCharConstruct() : Action( name_string, Type::instance, name_constructor, name_string, DynamicTuple( {
				type::Attribute( name_other, name_char ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~StringCharConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringIntConstruct : public type::Action
		{
		public:
			inline StringIntConstruct() : Action( name_string, Type::instance, name_constructor, name_string, DynamicTuple( {
				type::Attribute( name_other, name_int ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~StringIntConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringShortConstruct : public type::Action
		{
		public:
			inline StringShortConstruct() : Action( name_string, Type::instance, name_constructor, name_string,
				DynamicTuple( { type::Attribute( name_other, name_short ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~StringShortConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringLongConstruct : public type::Action
		{
		public:
			inline StringLongConstruct() : Action( name_string, Type::instance, name_constructor, name_string, DynamicTuple( {
				type::Attribute( name_other, name_long ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~StringLongConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringFloatConstruct : public type::Action
		{
		public:
			inline StringFloatConstruct() : Action( name_string, Type::instance, name_constructor, name_string,
				DynamicTuple( { type::Attribute( name_other, name_float ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~StringFloatConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringLowConstruct : public type::Action
		{
		public:
			inline StringLowConstruct() : Action( name_string, Type::instance, name_constructor, name_string, DynamicTuple( {
				type::Attribute( name_other, name_low ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~StringLowConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringHighConstruct : public type::Action
		{
		public:
			inline StringHighConstruct() : Action( name_string, Type::instance, name_constructor, name_string, DynamicTuple( {
				type::Attribute( name_other, name_high ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~StringHighConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringIndexConstruct : public type::Action
		{
		public:
			inline StringIndexConstruct() : Action( name_string, Type::instance, name_constructor, name_string,
				DynamicTuple( { type::Attribute( name_other, name_index ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~StringIndexConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringNameConstruct : public type::Action
		{
		public:
			inline StringNameConstruct() : Action( name_string, Type::instance, name_constructor, name_string,
				DynamicTuple( { type::Attribute( name_other, name_name ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~StringNameConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class StringCmp : public OperatorAction
		{
		public:
			inline StringCmp() : OperatorAction( name_string, opcode::cmp, name_int, DynamicTuple( {
				type::Attribute( name_other, name_string ) } ), {}, source::Ref() ) {}
			virtual ~StringCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringPlus : public OperatorAction
		{
		public:
			inline StringPlus() : OperatorAction( name_string, opcode::plus, name_string, DynamicTuple( {
				type::Attribute( name_other, name_string ) } ), {}, source::Ref() ) {}
			virtual ~StringPlus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringMinusString : public OperatorAction
		{
		public:
			inline StringMinusString() : OperatorAction( name_string, opcode::minus, name_string, DynamicTuple( {
				type::Attribute( name_other, name_string ) } ), {}, source::Ref() ) {}
			virtual ~StringMinusString() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringMultiplyInt : public OperatorAction
		{
		public:
			inline StringMultiplyInt() : OperatorAction( name_string, opcode::multiply, name_string, DynamicTuple( {
				type::Attribute( name_other, name_int ) } ), {}, source::Ref() ) {}
			virtual ~StringMultiplyInt() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringMultiplyIndex : public OperatorAction
		{
		public:
			inline StringMultiplyIndex() : OperatorAction( name_string, opcode::multiply, name_string, DynamicTuple( {
				type::Attribute( name_other, name_index ) } ), {}, source::Ref() ) {}
			virtual ~StringMultiplyIndex() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
