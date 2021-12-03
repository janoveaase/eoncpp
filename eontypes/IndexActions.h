#pragma once

#include "Index.h"
#include "Action.h"
#include <eonscopes/Scope.h>


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
			inline IndexConstruct() : Action( name_index, Type::instance, name_construct, name_index, DynamicTuple() ) {}
			virtual ~IndexConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexCopyConstruct : public type::Action
		{
		public:
			inline IndexCopyConstruct( scope::Global& scope ) : Action(
				name_index, Type::instance, name_construct, name_index,
				DynamicTuple( { type::Attribute( name_other, name_index ) } ), { name_memory_denied } ) {}
			virtual ~IndexCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class IndexBoolConstruct : public type::Action
		{
		public:
			inline IndexBoolConstruct( scope::Global& scope ) : Action(
				name_index, Type::instance, name_construct, name_index,
				DynamicTuple( { type::Attribute( name_other, name_bool ) } ), { name_memory_denied } ) {}
			virtual ~IndexBoolConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexByteConstruct : public type::Action
		{
		public:
			inline IndexByteConstruct( scope::Global& scope ) : Action(
				name_index, Type::instance, name_construct, name_index,
				DynamicTuple( { type::Attribute( name_other, name_byte ) } ), { name_memory_denied } ) {}
			virtual ~IndexByteConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexCharConstruct : public type::Action
		{
		public:
			inline IndexCharConstruct( scope::Global& scope ) : Action(
				name_index, Type::instance, name_construct, name_index,
				DynamicTuple( { type::Attribute( name_other, name_char ) } ), { name_memory_denied } ) {}
			virtual ~IndexCharConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexIntConstruct : public type::Action
		{
		public:
			inline IndexIntConstruct( scope::Global& scope ) : Action(
				name_index, Type::instance, name_construct, name_index,
				DynamicTuple( { type::Attribute( name_other, name_int ) } ), { name_memory_denied } ) {}
			virtual ~IndexIntConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexShortConstruct : public type::Action
		{
		public:
			inline IndexShortConstruct( scope::Global& scope ) : Action(
				name_index, Type::instance, name_construct, name_index,
				DynamicTuple( { type::Attribute( name_other, name_short ) } ), { name_memory_denied } ) {}
			virtual ~IndexShortConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexLongConstruct : public type::Action
		{
		public:
			inline IndexLongConstruct( scope::Global& scope ) : Action(
				name_index, Type::instance, name_construct, name_index,
				DynamicTuple( { type::Attribute( name_other, name_long ) } ), { name_memory_denied } ) {}
			virtual ~IndexLongConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexFloatConstruct : public type::Action
		{
		public:
			inline IndexFloatConstruct( scope::Global& scope ) : Action(
				name_index, Type::instance, name_construct, name_index,
				DynamicTuple( { type::Attribute( name_other, name_float ) } ), { name_memory_denied } ) {}
			virtual ~IndexFloatConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexLowConstruct : public type::Action
		{
		public:
			inline IndexLowConstruct( scope::Global& scope ) : Action(
				name_index, Type::instance, name_construct, name_index,
				DynamicTuple( { type::Attribute( name_other, name_low ) } ), { name_memory_denied } ) {}
			virtual ~IndexLowConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexHighConstruct : public type::Action
		{
		public:
			inline IndexHighConstruct( scope::Global& scope ) : Action(
				name_index, Type::instance, name_construct, name_index,
				DynamicTuple( { type::Attribute( name_other, name_high ) } ), { name_memory_denied } ) {}
			virtual ~IndexHighConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class IndexCmp : public type::Action
		{
		public:
			inline IndexCmp( scope::Global& scope ) : Action( name_index, opcode::cmp, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_index ) } ) ) {}
			virtual ~IndexCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexPlus : public type::Action
		{
		public:
			inline IndexPlus( scope::Global& scope ) : Action( name_index, opcode::plus, name_index, DynamicTuple(
				{ type::Attribute( name_other, name_index ) } ) ) {}
			virtual ~IndexPlus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexMinus : public type::Action
		{
		public:
			inline IndexMinus( scope::Global& scope ) : Action( name_index, opcode::minus, name_index, DynamicTuple(
				{ type::Attribute( name_other, name_index ) } ) ) {}
			virtual ~IndexMinus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexMultiply : public type::Action
		{
		public:
			inline IndexMultiply( scope::Global& scope ) : Action( name_index, opcode::multiply, name_index, DynamicTuple(
				{ type::Attribute( name_other, name_index ) } ) ) {}
			virtual ~IndexMultiply() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexDivide : public type::Action
		{
		public:
			inline IndexDivide( scope::Global& scope ) : Action( name_index, opcode::divide, name_index, DynamicTuple(
				{ type::Attribute( name_other, name_index ) } ) ) {}
			virtual ~IndexDivide() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class IndexElementIndex : public type::Action
		{
		public:
			inline IndexElementIndex( scope::Global& scope ) : Action( name_index, opcode::element, name_index, DynamicTuple(
				{ type::Attribute( name_pos, name_index ) } ) ) {}
			virtual ~IndexElementIndex() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexElementInt : public type::Action
		{
		public:
			inline IndexElementInt( scope::Global& scope ) : Action( name_index, opcode::element, name_index, DynamicTuple(
				{ type::Attribute( name_pos, name_int ) } ) ) {}
			virtual ~IndexElementInt() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class IndexSlice : public type::Action
		{
		public:
			inline IndexSlice( scope::Global& scope ) : Action( name_index, opcode::slice, name_index, DynamicTuple( {
				type::Attribute( name::get( "first" ), name_int ),
				type::Attribute( name::get( "last" ), name_int ) } ) ) {}
			virtual ~IndexSlice() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class IndexBitNot : public type::Action
		{
		public:
			inline IndexBitNot() : Action( name_index, opcode::bit_not, name_index, DynamicTuple() ) {}
			virtual ~IndexBitNot() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexBitAnd : public type::Action
		{
		public:
			inline IndexBitAnd( scope::Global& scope ) : Action( name_index, opcode::bit_and, name_index, DynamicTuple(
				{ type::Attribute( name_other, name_index ) } ) ) {}
			virtual ~IndexBitAnd() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexBitOr : public type::Action
		{
		public:
			inline IndexBitOr( scope::Global& scope ) : Action( name_index, opcode::bit_or, name_index, DynamicTuple(
				{ type::Attribute( name_other, name_index ) } ) ) {}
			virtual ~IndexBitOr() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexBitXor : public type::Action
		{
		public:
			inline IndexBitXor( scope::Global& scope ) : Action( name_index, opcode::bit_xor, name_index, DynamicTuple(
				{ type::Attribute( name_other, name_index ) } ) ) {}
			virtual ~IndexBitXor() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexLShift: public type::Action
		{
		public:
			inline IndexLShift( scope::Global& scope ) : Action( name_index, opcode::bit_lsh, name_index, DynamicTuple(
				{ type::Attribute( name_other, name_index ) } ) ) {}
			virtual ~IndexLShift() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class IndexRShift : public type::Action
		{
		public:
			inline IndexRShift( scope::Global& scope ) : Action( name_index, opcode::bit_rsh, name_index, DynamicTuple(
				{ type::Attribute( name_other, name_index ) } ) ) {}
			virtual ~IndexRShift() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
