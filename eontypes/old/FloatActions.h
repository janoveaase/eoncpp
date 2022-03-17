#pragma once

#include "Float.h"
#include "Action.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerFloatActions( scope::Global& scope );




	namespace actions
	{
		using namespace scope;
		using namespace type;

		/**********************************************************************
		  Eon float actions
		**********************************************************************/

		class FloatConstruct : public type::Action
		{
		public:
			inline FloatConstruct() : Action( name_float, Type::instance, name_construct, name_float, DynamicTuple() ) {}
			virtual ~FloatConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class FloatCopyConstruct : public type::Action
		{
		public:
			inline FloatCopyConstruct() : Action( name_float, Type::instance, name_construct, name_float, DynamicTuple( {
				type::Attribute( name_other, name_float ) } ), { name_memory_denied } ) {}
			virtual ~FloatCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class FloatBoolConstruct : public type::Action
		{
		public:
			inline FloatBoolConstruct() : Action( name_float, Type::instance, name_construct, name_float, DynamicTuple( {
				type::Attribute( name_other, name_bool ) } ), { name_memory_denied } ) {}
			virtual ~FloatBoolConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class FloatByteConstruct : public type::Action
		{
		public:
			inline FloatByteConstruct() : Action( name_float, Type::instance, name_construct, name_float, DynamicTuple( {
				type::Attribute( name_other, name_byte ) } ), { name_memory_denied } ) {}
			virtual ~FloatByteConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class FloatCharConstruct : public type::Action
		{
		public:
			inline FloatCharConstruct() : Action( name_float, Type::instance, name_construct, name_float, DynamicTuple( {
				type::Attribute( name_other, name_char ) } ), { name_memory_denied } ) {}
			virtual ~FloatCharConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class FloatIntConstruct : public type::Action
		{
		public:
			inline FloatIntConstruct() : Action( name_float, Type::instance, name_construct, name_float, DynamicTuple( {
				type::Attribute( name_other, name_int ) } ), { name_memory_denied } ) {}
			virtual ~FloatIntConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class FloatShortConstruct : public type::Action
		{
		public:
			inline FloatShortConstruct() : Action( name_float, Type::instance, name_construct, name_float, DynamicTuple( {
				type::Attribute( name_other, name_short ) } ), { name_memory_denied } ) {}
			virtual ~FloatShortConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class FloatLongConstruct : public type::Action
		{
		public:
			inline FloatLongConstruct() : Action( name_float, Type::instance, name_construct, name_float, DynamicTuple( {
				type::Attribute( name_other, name_long ) } ), { name_memory_denied } ) {}
			virtual ~FloatLongConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class FloatLowConstruct : public type::Action
		{
		public:
			inline FloatLowConstruct() : Action( name_float, Type::instance, name_construct, name_float, DynamicTuple( {
				type::Attribute( name_other, name_low ) } ), { name_memory_denied } ) {}
			virtual ~FloatLowConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class FloatHighConstruct : public type::Action
		{
		public:

			inline FloatHighConstruct() : Action( name_float, Type::instance, name_construct, name_float, DynamicTuple( {
				type::Attribute( name_other, name_high ) } ), { name_memory_denied } ) {}
			virtual ~FloatHighConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class FloatIndexConstruct : public type::Action
		{
		public:
			inline FloatIndexConstruct() : Action( name_float, Type::instance, name_construct, name_float, DynamicTuple( {
					type::Attribute( name_other, name_index ) } ), { name_memory_denied } ) {}
			virtual ~FloatIndexConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class FloatCmp : public type::Action
		{
		public:
			inline FloatCmp() : Action( name_float, opcode::cmp, name_int, DynamicTuple( {
				type::Attribute( name_other, name_float ) } ) ) {}
			virtual ~FloatCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class FloatPlus : public type::Action
		{
		public:
			inline FloatPlus() : Action( name_float, opcode::plus, name_float, DynamicTuple( {
				type::Attribute( name_other, name_float ) } ) ) {}
			virtual ~FloatPlus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class FloatMinus : public type::Action
		{
		public:
			inline FloatMinus() : Action( name_float, opcode::minus, name_float, DynamicTuple( {
				type::Attribute( name_other, name_float ) } ) ) {}
			virtual ~FloatMinus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class FloatMultiply : public type::Action
		{
		public:
			inline FloatMultiply() : Action( name_float, opcode::multiply, name_float, DynamicTuple( {
				type::Attribute( name_other, name_float ) } ) ) {}
			virtual ~FloatMultiply() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class FloatDivide : public type::Action
		{
		public:
			inline FloatDivide() : Action( name_float, opcode::divide, name_float, DynamicTuple( {
				type::Attribute( name_other, name_float ) } ) ) {}
			virtual ~FloatDivide() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
