#pragma once

#include "Short.h"
#include "Action.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerShortActions( scope::Global& scope );




	namespace actions
	{
		/**********************************************************************
		  Eon short integer actions
		**********************************************************************/

		class ShortConstruct : public type::Action
		{
		public:
			inline ShortConstruct() : Action( name_short, Type::instance, name_construct, name_short, DynamicTuple() ) {}
			virtual ~ShortConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ShortCopyConstruct : public type::Action
		{
		public:
			inline ShortCopyConstruct( scope::Global& scope ) : Action(
				name_short, Type::instance, name_construct, name_short,
				DynamicTuple( { type::Attribute( name_other, name_short ) } ), { name_memory_denied } ) {}
			virtual ~ShortCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class ShortBoolConstruct : public type::Action
		{
		public:
			inline ShortBoolConstruct( scope::Global& scope ) : Action(
				name_short, Type::instance, name_construct, name_short,
				DynamicTuple( { type::Attribute( name_other, name_bool ) } ), { name_memory_denied } ) {}
			virtual ~ShortBoolConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ShortByteConstruct : public type::Action
		{
		public:
			inline ShortByteConstruct( scope::Global& scope ) : Action(
				name_short, Type::instance, name_construct, name_short,
				DynamicTuple( { type::Attribute( name_other, name_byte ) } ), { name_memory_denied } ) {}
			virtual ~ShortByteConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ShortCharConstruct : public type::Action
		{
		public:
			inline ShortCharConstruct( scope::Global& scope ) : Action(
				name_short, Type::instance, name_construct, name_short,
				DynamicTuple( { type::Attribute( name_other, name_char ) } ), { name_memory_denied } ) {}
			virtual ~ShortCharConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ShortIntConstruct : public type::Action
		{
		public:
			inline ShortIntConstruct( scope::Global& scope ) : Action(
				name_short, Type::instance, name_construct, name_short,
				DynamicTuple( { type::Attribute( name_other, name_int ) } ), { name_memory_denied } ) {}
			virtual ~ShortIntConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ShortLongConstruct : public type::Action
		{
		public:
			inline ShortLongConstruct( scope::Global& scope ) : Action(
				name_short, Type::instance, name_construct, name_short,
				DynamicTuple( { type::Attribute( name_other, name_long ) } ), { name_memory_denied } ) {}
			virtual ~ShortLongConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ShortFloatConstruct : public type::Action
		{
		public:
			inline ShortFloatConstruct( scope::Global& scope ) : Action(
				name_short, Type::instance, name_construct, name_short,
				DynamicTuple( { type::Attribute( name_other, name_float ) } ), { name_memory_denied } ) {}
			virtual ~ShortFloatConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ShortLowConstruct : public type::Action
		{
		public:
			inline ShortLowConstruct( scope::Global& scope ) : Action(
				name_short, Type::instance, name_construct, name_short,
				DynamicTuple( { type::Attribute( name_other, name_low ) } ), { name_memory_denied } ) {}
			virtual ~ShortLowConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ShortHighConstruct : public type::Action
		{
		public:
			inline ShortHighConstruct( scope::Global& scope ) : Action(
				name_short, Type::instance, name_construct, name_short,
				DynamicTuple( { type::Attribute( name_other, name_high ) } ), { name_memory_denied } ) {}
			virtual ~ShortHighConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ShortIndexConstruct : public type::Action
		{
		public:
			inline ShortIndexConstruct( scope::Global& scope ) : Action(
				name_short, Type::instance, name_construct, name_short,
				DynamicTuple( { type::Attribute( name_other, name_index ) } ), { name_memory_denied } ) {}
			virtual ~ShortIndexConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class ShortCmp : public type::Action
		{
		public:
			inline ShortCmp( scope::Global& scope ) : Action( name_short, opcode::cmp, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_short ) } ) ) {}
			virtual ~ShortCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ShortPlus : public type::Action
		{
		public:
			inline ShortPlus( scope::Global& scope ) : Action( name_short, opcode::plus, name_short, DynamicTuple(
				{ type::Attribute( name_other, name_short ) } ) ) {}
			virtual ~ShortPlus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ShortMinus : public type::Action
		{
		public:
			inline ShortMinus( scope::Global& scope ) : Action( name_short, opcode::minus, name_short, DynamicTuple(
				{ type::Attribute( name_other, name_short ) } ) ) {}
			virtual ~ShortMinus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ShortMultiply : public type::Action
		{
		public:
			inline ShortMultiply( scope::Global& scope ) : Action( name_short, opcode::multiply, name_short, DynamicTuple(
				{ type::Attribute( name_other, name_short ) } ) ) {}
			virtual ~ShortMultiply() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ShortDivide : public type::Action
		{
		public:
			inline ShortDivide( scope::Global& scope ) : Action( name_short, opcode::divide, name_short, DynamicTuple(
				{ type::Attribute( name_other, name_short ) } ) ) {}
			virtual ~ShortDivide() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class ShortElementIndex : public type::Action
		{
		public:
			inline ShortElementIndex( scope::Global& scope ) : Action( name_short, opcode::element, name_short, DynamicTuple(
				{ type::Attribute( name_pos, name_index ) } ) ) {}
			virtual ~ShortElementIndex() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ShortElementInt : public type::Action
		{
		public:
			inline ShortElementInt( scope::Global& scope ) : Action( name_short, opcode::element, name_short, DynamicTuple(
				{ type::Attribute( name_pos, name_int ) } ) ) {}
			virtual ~ShortElementInt() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class ShortSlice : public type::Action
		{
		public:
			inline ShortSlice( scope::Global& scope ) : Action( name_short, opcode::slice, name_short, DynamicTuple( {
				type::Attribute( name::get( "first" ), name_int ),
				type::Attribute( name::get( "last" ), name_int ) } ) ) {}
			virtual ~ShortSlice() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class ShortBitNot : public type::Action
		{
		public:
			inline ShortBitNot() : Action( name_short, opcode::bit_not, name_short, DynamicTuple() ) {}
			virtual ~ShortBitNot() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ShortBitAnd : public type::Action
		{
		public:
			inline ShortBitAnd( scope::Global& scope ) : Action( name_short, opcode::bit_and, name_short, DynamicTuple(
				{ type::Attribute( name_other, name_short ) } ) ) {}
			virtual ~ShortBitAnd() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ShortBitOr : public type::Action
		{
		public:
			inline ShortBitOr( scope::Global& scope ) : Action( name_short, opcode::bit_or, name_short, DynamicTuple(
				{ type::Attribute( name_other, name_short ) } ) ) {}
			virtual ~ShortBitOr() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ShortBitXor : public type::Action
		{
		public:
			inline ShortBitXor( scope::Global& scope ) : Action( name_short, opcode::bit_xor, name_short, DynamicTuple(
				{ type::Attribute( name_other, name_short ) } ) ) {}
			virtual ~ShortBitXor() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ShortLShift: public type::Action
		{
		public:
			inline ShortLShift( scope::Global& scope ) : Action( name_short, opcode::bit_lsh, name_short, DynamicTuple(
				{ type::Attribute( name_other, name_index ) } ) ) {}
			virtual ~ShortLShift() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ShortRShift : public type::Action
		{
		public:
			inline ShortRShift( scope::Global& scope ) : Action( name_short, opcode::bit_rsh, name_short, DynamicTuple(
				{ type::Attribute( name_other, name_index ) } ) ) {}
			virtual ~ShortRShift() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
