#pragma once

#include "Bool.h"
#include "Action.h"
#include "OperatorAction.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerBoolActions( scope::Global& scope );


	namespace actions
	{
		/**********************************************************************
		  Eon boolean actions
		**********************************************************************/

		class BoolConstruct : public type::Action
		{
		public:
			inline BoolConstruct() : Action( name_bool, Type::instance, name_constructor, name_bool, DynamicTuple(), {},
				source::Ref() ) {}
			virtual ~BoolConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolCopyConstruct : public type::Action
		{
		public:
			inline BoolCopyConstruct() : Action( name_bool, Type::instance, name_constructor, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_bool ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BoolCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class BoolByteConstruct : public type::Action
		{
		public:
			inline BoolByteConstruct() : Action( name_bool, Type::instance, name_constructor, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_byte ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BoolByteConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolCharConstruct : public type::Action
		{
		public:
			inline BoolCharConstruct() : Action( name_bool, Type::instance, name_constructor, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_char ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BoolCharConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolIntConstruct : public type::Action
		{
		public:
			inline BoolIntConstruct() : Action( name_bool, Type::instance, name_constructor, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_int ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BoolIntConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolShortConstruct : public type::Action
		{
		public:
			inline BoolShortConstruct() : Action( name_bool, Type::instance, name_constructor, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_short ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BoolShortConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolLongConstruct : public type::Action
		{
		public:
			inline BoolLongConstruct() : Action( name_bool, Type::instance, name_constructor, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_long ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BoolLongConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolFloatConstruct : public type::Action
		{
		public:
			inline BoolFloatConstruct() : Action( name_bool, Type::instance, name_constructor, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_float ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BoolFloatConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolLowConstruct : public type::Action
		{
		public:
			inline BoolLowConstruct() : Action( name_bool, Type::instance, name_constructor, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_low ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BoolLowConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolHighConstruct : public type::Action
		{
		public:
			inline BoolHighConstruct() : Action( name_bool, Type::instance, name_constructor, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_high ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BoolHighConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolIndexConstruct : public type::Action
		{
		public:
			inline BoolIndexConstruct() : Action( name_bool, Type::instance, name_constructor, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_index ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~BoolIndexConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class BoolCmp : public OperatorAction
		{
		public:
			inline BoolCmp() : OperatorAction( name_bool, opcode::cmp, name_int,
				DynamicTuple( { type::Attribute( name_other, name_bool ) } ), {}, source::Ref() ) {}
			virtual ~BoolCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolNot : public OperatorAction
		{
		public:
			inline BoolNot() : OperatorAction( name_bool, opcode::not, name_bool, DynamicTuple(), {}, source::Ref() ) {}
			virtual ~BoolNot() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolAnd : public OperatorAction
		{
		public:
			inline BoolAnd() : OperatorAction( name_bool, opcode::and, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_bool ) } ), {}, source::Ref() ) {}
			virtual ~BoolAnd() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolOr : public OperatorAction
		{
		public:
			inline BoolOr() : OperatorAction( name_bool, opcode::or, name_bool, DynamicTuple( {
				type::Attribute( name_other, name_bool ) } ), {}, source::Ref() ) {}
			virtual ~BoolOr() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolXor : public OperatorAction
		{
		public:
			inline BoolXor() : OperatorAction( name_bool, opcode::xor, name_bool, DynamicTuple( {
				type::Attribute( name_other, name_bool ) } ), {}, source::Ref() ) {}
			virtual ~BoolXor() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
