#pragma once

#include "Index.h"
#include "Action.h"
#include "OperatorAction.h"
#include "Scope.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerIndexActions( scope::Global& scope );




	namespace actions
	{
		/**********************************************************************
		  Eon index integer actions
		**********************************************************************/

		class IndexConstruct : public type::Action
		{
		public:
			inline IndexConstruct() : Action( name_index, Type::instance, name_constructor, name_index, DynamicTuple(), {},
				source::Ref() ) {}
			virtual ~IndexConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexCopyConstruct : public type::Action
		{
		public:
			inline IndexCopyConstruct() : Action( name_index, Type::instance, name_constructor, name_index, DynamicTuple( {
				type::Attribute( name_other, name_index ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~IndexCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class IndexBoolConstruct : public type::Action
		{
		public:
			inline IndexBoolConstruct() : Action( name_index, Type::instance, name_constructor, name_index, DynamicTuple( {
				type::Attribute( name_other, name_bool ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~IndexBoolConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexByteConstruct : public type::Action
		{
		public:
			inline IndexByteConstruct() : Action( name_index, Type::instance, name_constructor, name_index, DynamicTuple( {
				type::Attribute( name_other, name_byte ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~IndexByteConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexCharConstruct : public type::Action
		{
		public:
			inline IndexCharConstruct() : Action( name_index, Type::instance, name_constructor, name_index, DynamicTuple( {
				type::Attribute( name_other, name_char ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~IndexCharConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexIntConstruct : public type::Action
		{
		public:
			inline IndexIntConstruct() : Action( name_index, Type::instance, name_constructor, name_index, DynamicTuple( {
				type::Attribute( name_other, name_int ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~IndexIntConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexShortConstruct : public type::Action
		{
		public:
			inline IndexShortConstruct() : Action( name_index, Type::instance, name_constructor, name_index, DynamicTuple( {
				type::Attribute( name_other, name_short ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~IndexShortConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexLongConstruct : public type::Action
		{
		public:
			inline IndexLongConstruct() : Action( name_index, Type::instance, name_constructor, name_index, DynamicTuple( {
				type::Attribute( name_other, name_long ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~IndexLongConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexFloatConstruct : public type::Action
		{
		public:
			inline IndexFloatConstruct() : Action( name_index, Type::instance, name_constructor, name_index, DynamicTuple( {
				type::Attribute( name_other, name_float ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~IndexFloatConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexLowConstruct : public type::Action
		{
		public:
			inline IndexLowConstruct() : Action( name_index, Type::instance, name_constructor, name_index, DynamicTuple( {
				type::Attribute( name_other, name_low ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~IndexLowConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexHighConstruct : public type::Action
		{
		public:
			inline IndexHighConstruct() : Action( name_index, Type::instance, name_constructor, name_index, DynamicTuple( {
				type::Attribute( name_other, name_high ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~IndexHighConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class IndexCmp : public OperatorAction
		{
		public:
			inline IndexCmp() : OperatorAction( name_index, opcode::cmp, name_int, DynamicTuple( {
				type::Attribute( name_other, name_index ) } ), {}, source::Ref() ) {}
			virtual ~IndexCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexPlus : public OperatorAction
		{
		public:
			inline IndexPlus() : OperatorAction( name_index, opcode::plus, name_index, DynamicTuple( {
				type::Attribute( name_other, name_index ) } ), {}, source::Ref() ) {}
			virtual ~IndexPlus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexMinus : public OperatorAction
		{
		public:
			inline IndexMinus() : OperatorAction( name_index, opcode::minus, name_index, DynamicTuple( {
				type::Attribute( name_other, name_index ) } ), {}, source::Ref() ) {}
			virtual ~IndexMinus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexMultiply : public OperatorAction
		{
		public:
			inline IndexMultiply() : OperatorAction( name_index, opcode::multiply, name_index, DynamicTuple( {
				type::Attribute( name_other, name_index ) } ), {}, source::Ref() ) {}
			virtual ~IndexMultiply() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexDivide : public OperatorAction
		{
		public:
			inline IndexDivide() : OperatorAction( name_index, opcode::divide, name_index, DynamicTuple( {
				type::Attribute( name_other, name_index ) } ), {}, source::Ref() ) {}
			virtual ~IndexDivide() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class IndexElementIndex : public OperatorAction
		{
		public:
			inline IndexElementIndex() : OperatorAction( name_index, opcode::element, name_index, DynamicTuple( {
				type::Attribute( name_pos, name_index ) } ), {}, source::Ref() ) {}
			virtual ~IndexElementIndex() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexElementInt : public OperatorAction
		{
		public:
			inline IndexElementInt() : OperatorAction( name_index, opcode::element, name_index, DynamicTuple( {
				type::Attribute( name_pos, name_int ) } ), {}, source::Ref() ) {}
			virtual ~IndexElementInt() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class IndexSlice : public OperatorAction
		{
		public:
			inline IndexSlice() : OperatorAction( name_index, opcode::slice, name_index, DynamicTuple( {
				type::Attribute( eon::name( "first" ), name_int ), type::Attribute( eon::name( "last" ), name_int ) } ), {},
				source::Ref() ) {}
			virtual ~IndexSlice() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class IndexBitNot : public OperatorAction
		{
		public:
			inline IndexBitNot() : OperatorAction( name_index, opcode::bit_not, name_index, DynamicTuple(), {},
				source::Ref() ) {}
			virtual ~IndexBitNot() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexBitAnd : public OperatorAction
		{
		public:
			inline IndexBitAnd() : OperatorAction( name_index, opcode::bit_and, name_index, DynamicTuple( {
				type::Attribute( name_other, name_index ) } ), {}, source::Ref() ) {}
			virtual ~IndexBitAnd() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexBitOr : public OperatorAction
		{
		public:
			inline IndexBitOr() : OperatorAction( name_index, opcode::bit_or, name_index, DynamicTuple( {
				type::Attribute( name_other, name_index ) } ), {}, source::Ref() ) {}
			virtual ~IndexBitOr() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexBitXor : public OperatorAction
		{
		public:
			inline IndexBitXor() : OperatorAction( name_index, opcode::bit_xor, name_index, DynamicTuple( {
				type::Attribute( name_other, name_index ) } ), {}, source::Ref() ) {}
			virtual ~IndexBitXor() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexLShift: public OperatorAction
		{
		public:
			inline IndexLShift() : OperatorAction( name_index, opcode::bit_lsh, name_index, DynamicTuple( {
				type::Attribute( name_other, name_index ) } ), {}, source::Ref() ) {}
			virtual ~IndexLShift() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexRShift : public OperatorAction
		{
		public:
			inline IndexRShift() : OperatorAction( name_index, opcode::bit_rsh, name_index, DynamicTuple( {
				type::Attribute( name_other, name_index ) } ), {}, source::Ref() ) {}
			virtual ~IndexRShift() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
