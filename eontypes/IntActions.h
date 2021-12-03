#pragma once

#include "Int.h"
#include "Action.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerIntActions( scope::Global& scope );




	namespace actions
	{
		/**********************************************************************
		  Eon integer actions
		**********************************************************************/

		class IntConstruct : public type::Action
		{
		public:
			inline IntConstruct() : Action( name_int, Type::instance, name_construct, name_int, DynamicTuple() ) {}
			virtual ~IntConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IntCopyConstruct : public type::Action
		{
		public:
			inline IntCopyConstruct( scope::Global& scope ) : Action( name_int, Type::instance, name_construct, name_int,
				DynamicTuple( { type::Attribute( name_other, name_int ) } ), { name_memory_denied } ) {}
			virtual ~IntCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class IntBoolConstruct : public type::Action
		{
		public:
			inline IntBoolConstruct( scope::Global& scope ) : Action( name_int, Type::instance, name_construct, name_int,
				DynamicTuple( { type::Attribute( name_other, name_bool ) } ), { name_memory_denied } ) {}
			virtual ~IntBoolConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IntByteConstruct : public type::Action
		{
		public:
			inline IntByteConstruct( scope::Global& scope ) : Action( name_int, Type::instance, name_construct, name_int,
				DynamicTuple( { type::Attribute( name_other, name_byte ) } ), { name_memory_denied } ) {}
			virtual ~IntByteConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IntCharConstruct : public type::Action
		{
		public:
			inline IntCharConstruct( scope::Global& scope ) : Action( name_int, Type::instance, name_construct, name_int,
				DynamicTuple( { type::Attribute( name_other, name_char ) } ),{ name_memory_denied } ) {}
			virtual ~IntCharConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IntShortConstruct : public type::Action
		{
		public:
			inline IntShortConstruct( scope::Global& scope ) : Action( name_int, Type::instance, name_construct, name_int,
				DynamicTuple( { type::Attribute( name_other, name_short ) } ), { name_memory_denied } ) {}
			virtual ~IntShortConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IntLongConstruct : public type::Action
		{
		public:
			inline IntLongConstruct( scope::Global& scope ) : Action( name_int, Type::instance, name_construct, name_int,
				DynamicTuple( { type::Attribute( name_other, name_long ) } ), { name_memory_denied } ) {}
			virtual ~IntLongConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IntFloatConstruct : public type::Action
		{
		public:
			inline IntFloatConstruct( scope::Global& scope ) : Action( name_int, Type::instance, name_construct, name_int,
				DynamicTuple( { type::Attribute( name_other, name_float ) } ), { name_memory_denied } ) {}
			virtual ~IntFloatConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IntLowConstruct : public type::Action
		{
		public:
			inline IntLowConstruct( scope::Global& scope ) : Action( name_int, Type::instance, name_construct, name_int,
				DynamicTuple( { type::Attribute( name_other, name_low ) } ), { name_memory_denied } ) {}
			virtual ~IntLowConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IntHighConstruct : public type::Action
		{
		public:
			inline IntHighConstruct( scope::Global& scope ) : Action( name_int, Type::instance, name_construct, name_int,
				DynamicTuple( { type::Attribute( name_other, name_high ) } ), { name_memory_denied } ) {}
			virtual ~IntHighConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IntIndexConstruct : public type::Action
		{
		public:
			inline IntIndexConstruct( scope::Global& scope ) : Action( name_int, Type::instance, name_construct, name_int,
				DynamicTuple( { type::Attribute( name_other, name_index ) } ), { name_memory_denied } ) {}
			virtual ~IntIndexConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class IntCmp : public type::Action
		{
		public:
			inline IntCmp( scope::Global& scope ) : Action( name_int, opcode::cmp, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_int ) } ) ) {}
			virtual ~IntCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IntPlus : public type::Action
		{
		public:
			inline IntPlus( scope::Global& scope ) : Action( name_int, opcode::plus, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_int ) } ) ) {}
			virtual ~IntPlus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IntMinus : public type::Action
		{
		public:
			inline IntMinus( scope::Global& scope ) : Action( name_int, opcode::minus, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_int ) } ) ) {}
			virtual ~IntMinus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IntMultiply : public type::Action
		{
		public:
			inline IntMultiply( scope::Global& scope ) : Action( name_int, opcode::multiply, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_int ) } ) ) {}
			virtual ~IntMultiply() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IntDivide : public type::Action
		{
		public:
			inline IntDivide( scope::Global& scope ) : Action( name_int, opcode::divide, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_int ) } ) ) {}
			virtual ~IntDivide() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class IntElementIndex : public type::Action
		{
		public:
			inline IntElementIndex( scope::Global& scope ) : Action( name_int, opcode::element, name_int, DynamicTuple(
				{ type::Attribute( name_pos, name_index ) } ) ) {}
			virtual ~IntElementIndex() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IntElementInt : public type::Action
		{
		public:
			inline IntElementInt( scope::Global& scope ) : Action( name_int, opcode::element, name_int, DynamicTuple(
				{ type::Attribute( name_pos, name_int ) } ) ) {}
			virtual ~IntElementInt() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class IntSlice : public type::Action
		{
		public:
			inline IntSlice( scope::Global& scope ) : Action( name_int, opcode::slice, name_int, DynamicTuple( {
				type::Attribute( name::get( "first" ), name_int ),
				type::Attribute( name::get( "last" ), name_int ) } ) ) {}
			virtual ~IntSlice() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class IntBitNot : public type::Action
		{
		public:
			inline IntBitNot() : Action( name_int, opcode::bit_not, name_int, DynamicTuple() ) {}
			virtual ~IntBitNot() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IntBitAnd : public type::Action
		{
		public:
			inline IntBitAnd( scope::Global& scope ) : Action( name_int, opcode::bit_and, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_int ) } ) ) {}
			virtual ~IntBitAnd() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IntBitOr : public type::Action
		{
		public:
			inline IntBitOr( scope::Global& scope ) : Action( name_int, opcode::bit_or, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_int ) } ) ) {}
			virtual ~IntBitOr() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IntBitXor : public type::Action
		{
		public:
			inline IntBitXor( scope::Global& scope ) : Action( name_int, opcode::bit_xor, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_int ) } ) ) {}
			virtual ~IntBitXor() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IntLShift: public type::Action
		{
		public:
			inline IntLShift( scope::Global& scope ) : Action( name_int, opcode::bit_lsh, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_index ) } ) ) {}
			virtual ~IntLShift() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IntRShift : public type::Action
		{
		public:
			inline IntRShift( scope::Global& scope ) : Action( name_int, opcode::bit_rsh, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_index ) } ) ) {}
			virtual ~IntRShift() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
