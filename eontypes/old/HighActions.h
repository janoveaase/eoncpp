#pragma once

#include "High.h"
#include "Action.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerHighActions( scope::Global& scope );




	namespace actions
	{
		using namespace scope;
		using namespace type;

		/**********************************************************************
		  Eon high precicition float actions
		**********************************************************************/

		class HighConstruct : public type::Action
		{
		public:
			inline HighConstruct() : Action( name_high, Type::instance, name_construct, name_high, DynamicTuple() ) {}
			virtual ~HighConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class HighCopyConstruct : public type::Action
		{
		public:
			inline HighCopyConstruct() : Action( name_high, Type::instance, name_construct, name_high, DynamicTuple( {
				type::Attribute( name_other, name_high ) } ), { name_memory_denied } ) {}
			virtual ~HighCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class HighBoolConstruct : public type::Action
		{
		public:
			inline HighBoolConstruct() : Action( name_high, Type::instance, name_construct, name_high, DynamicTuple( {
				type::Attribute( name_other, name_bool ) } ), { name_memory_denied } ) {}
			virtual ~HighBoolConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class HighByteConstruct : public type::Action
		{
		public:
			inline HighByteConstruct() : Action( name_high, Type::instance, name_construct, name_high, DynamicTuple( {
				type::Attribute( name_other, name_byte ) } ), { name_memory_denied } ) {}
			virtual ~HighByteConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class HighCharConstruct : public type::Action
		{
		public:
			inline HighCharConstruct() : Action( name_high, Type::instance, name_construct, name_high, DynamicTuple( {
				type::Attribute( name_other, name_char ) } ), { name_memory_denied } ) {}
			virtual ~HighCharConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class HighIntConstruct : public type::Action
		{
		public:
			inline HighIntConstruct() : Action( name_high, Type::instance, name_construct, name_high, DynamicTuple( {
				type::Attribute( name_other, name_int ) } ), { name_memory_denied } ) {}
			virtual ~HighIntConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class HighShortConstruct : public type::Action
		{
		public:
			inline HighShortConstruct() : Action( name_high, Type::instance, name_construct, name_high, DynamicTuple( {
				type::Attribute( name_other, name_short ) } ), { name_memory_denied } ) {}
			virtual ~HighShortConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class HighLongConstruct : public type::Action
		{
		public:
			inline HighLongConstruct() : Action( name_high, Type::instance, name_construct, name_high, DynamicTuple( {
				type::Attribute( name_other, name_long ) } ), { name_memory_denied } ) {}
			virtual ~HighLongConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class HighLowConstruct : public type::Action
		{
		public:
			inline HighLowConstruct() : Action( name_high, Type::instance, name_construct, name_high, DynamicTuple( {
				type::Attribute( name_other, name_low ) } ), { name_memory_denied } ) {}
			virtual ~HighLowConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class HighFloatConstruct : public type::Action
		{
		public:
			inline HighFloatConstruct() : Action( name_high, Type::instance, name_construct, name_high, DynamicTuple( {
				type::Attribute( name_other, name_float ) } ), { name_memory_denied } ) {}
			virtual ~HighFloatConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class HighIndexConstruct : public type::Action
		{
		public:
			inline HighIndexConstruct() : Action( name_high, Type::instance, name_construct, name_high, DynamicTuple( {
				type::Attribute( name_other, name_index ) } ), { name_memory_denied } ) {}
			virtual ~HighIndexConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class HighCmp : public type::Action
		{
		public:
			inline HighCmp() : Action( name_high, opcode::cmp, name_int, DynamicTuple( {
				type::Attribute( name_other, name_high ) } ) ) {}
			virtual ~HighCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class HighPlus : public type::Action
		{
		public:
			inline HighPlus() : Action( name_high, opcode::plus, name_high, DynamicTuple( {
				type::Attribute( name_other, name_high ) } ) ) {}
			virtual ~HighPlus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class HighMinus : public type::Action
		{
		public:
			inline HighMinus() : Action( name_high, opcode::minus, name_high, DynamicTuple( {
				type::Attribute( name_other, name_high ) } ) ) {}
			virtual ~HighMinus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class HighMultiply : public type::Action
		{
		public:
			inline HighMultiply() : Action( name_high, opcode::multiply, name_high, DynamicTuple( {
				type::Attribute( name_other, name_high ) } ) ) {}
			virtual ~HighMultiply() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class HighDivide : public type::Action
		{
		public:
			inline HighDivide() : Action( name_high, opcode::divide, name_high, DynamicTuple( {
				type::Attribute( name_other, name_high ) } ) ) {}
			virtual ~HighDivide() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
