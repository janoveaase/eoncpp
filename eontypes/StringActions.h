#pragma once

#include "String.h"
#include "Action.h"
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
			inline StringConstruct() : Action( name_string, Type::instance, name_construct, name_string, DynamicTuple() ) {}
			virtual ~StringConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringCopyConstruct : public type::Action
		{
		public:
			inline StringCopyConstruct( scope::Global& scope ) : Action( name_string, Type::instance, name_construct, name_string,
				DynamicTuple( { type::Attribute( name_other, name_string ) } ), { name_memory_denied } ) {}
			virtual ~StringCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class StringBoolConstruct : public type::Action
		{
		public:
			inline StringBoolConstruct( scope::Global& scope ) : Action( name_string, Type::instance, name_construct, name_string,
				DynamicTuple( { type::Attribute( name_other, name_bool ) } ), { name_memory_denied } ) {}
			virtual ~StringBoolConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringByteConstruct : public type::Action
		{
		public:
			inline StringByteConstruct( scope::Global& scope ) : Action( name_string, Type::instance, name_construct, name_string,
				DynamicTuple( { type::Attribute( name_other, name_byte ) } ), { name_memory_denied } ) {}
			virtual ~StringByteConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringCharConstruct : public type::Action
		{
		public:
			inline StringCharConstruct( scope::Global& scope ) : Action( name_string, Type::instance, name_construct, name_string,
				DynamicTuple( { type::Attribute( name_other, name_char ) } ), { name_memory_denied } ) {}
			virtual ~StringCharConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringIntConstruct : public type::Action
		{
		public:
			inline StringIntConstruct( scope::Global& scope ) : Action( name_string, Type::instance, name_construct, name_string,
				DynamicTuple( { type::Attribute( name_other, name_int ) } ), { name_memory_denied } ) {}
			virtual ~StringIntConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringShortConstruct : public type::Action
		{
		public:
			inline StringShortConstruct( scope::Global& scope ) : Action( name_string, Type::instance, name_construct, name_string,
				DynamicTuple( { type::Attribute( name_other, name_short ) } ), { name_memory_denied } ) {}
			virtual ~StringShortConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringLongConstruct : public type::Action
		{
		public:
			inline StringLongConstruct( scope::Global& scope ) : Action( name_string, Type::instance, name_construct, name_string,
				DynamicTuple( { type::Attribute( name_other, name_long ) } ), { name_memory_denied } ) {}
			virtual ~StringLongConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringFloatConstruct : public type::Action
		{
		public:
			inline StringFloatConstruct( scope::Global& scope ) : Action( name_string, Type::instance, name_construct, name_string,
				DynamicTuple( { type::Attribute( name_other, name_float ) } ), { name_memory_denied } ) {}
			virtual ~StringFloatConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringLowConstruct : public type::Action
		{
		public:
			inline StringLowConstruct( scope::Global& scope ) : Action( name_string, Type::instance, name_construct, name_string,
				DynamicTuple( { type::Attribute( name_other, name_low ) } ), { name_memory_denied } ) {}
			virtual ~StringLowConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringHighConstruct : public type::Action
		{
		public:
			inline StringHighConstruct( scope::Global& scope ) : Action( name_string, Type::instance, name_construct, name_string,
				DynamicTuple( { type::Attribute( name_other, name_high ) } ), { name_memory_denied } ) {}
			virtual ~StringHighConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringIndexConstruct : public type::Action
		{
		public:
			inline StringIndexConstruct( scope::Global& scope ) : Action( name_string, Type::instance, name_construct, name_string,
				DynamicTuple( { type::Attribute( name_other, name_index ) } ), { name_memory_denied } ) {}
			virtual ~StringIndexConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class StringCmp : public type::Action
		{
		public:
			inline StringCmp( scope::Global& scope ) : Action( name_string, opcode::cmp, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_string ) } ), {} ) {}
			virtual ~StringCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringPlus : public type::Action
		{
		public:
			inline StringPlus( scope::Global& scope ) : Action( name_string, opcode::plus, name_string, DynamicTuple(
				{ type::Attribute( name_other, name_string ) } ), {} ) {}
			virtual ~StringPlus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringMinusString : public type::Action
		{
		public:
			inline StringMinusString( scope::Global& scope ) : Action( name_string, opcode::minus, name_string, DynamicTuple(
				{ type::Attribute( name_other, name_string ) } ), {} ) {}
			virtual ~StringMinusString() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringMultiplyInt : public type::Action
		{
		public:
			inline StringMultiplyInt( scope::Global& scope ) : Action( name_string, opcode::multiply, name_string, DynamicTuple(
				{ type::Attribute( name_other, name_int ) } ), {} ) {}
			virtual ~StringMultiplyInt() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class StringMultiplyIndex : public type::Action
		{
		public:
			inline StringMultiplyIndex( scope::Global& scope ) : Action( name_string, opcode::multiply, name_string, DynamicTuple(
				{ type::Attribute( name_other, name_index ) } ), {} ) {}
			virtual ~StringMultiplyIndex() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
