#pragma once

#include "Long.h"
#include "Action.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerLongActions( scope::Global& scope );




	namespace actions
	{
		/**********************************************************************
		  Eon long integer actions
		**********************************************************************/

		class LongConstruct : public type::Action
		{
		public:
			inline LongConstruct() : Action( name_long, Type::instance, name_construct, name_long, DynamicTuple() ) {}
			virtual ~LongConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LongCopyConstruct : public type::Action
		{
		public:
			inline LongCopyConstruct( scope::Global& scope ) : Action( name_long, Type::instance, name_construct, name_long,
				DynamicTuple( { type::Attribute( name_other, name_long ) } ), { name_memory_denied } ) {}
			virtual ~LongCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class LongBoolConstruct : public type::Action
		{
		public:
			inline LongBoolConstruct( scope::Global& scope ) : Action( name_long, Type::instance, name_construct, name_long,
				DynamicTuple( { type::Attribute( name_other, name_bool ) } ), { name_memory_denied } ) {}
			virtual ~LongBoolConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LongByteConstruct : public type::Action
		{
		public:
			inline LongByteConstruct( scope::Global& scope ) : Action( name_long, Type::instance, name_construct, name_long,
				DynamicTuple( { type::Attribute( name_other, name_byte ) } ), { name_memory_denied } ) {}
			virtual ~LongByteConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LongCharConstruct : public type::Action
		{
		public:
			inline LongCharConstruct( scope::Global& scope ) : Action( name_long, Type::instance, name_construct, name_long,
				DynamicTuple( { type::Attribute( name_other, name_char ) } ), { name_memory_denied } ) {}
			virtual ~LongCharConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LongShortConstruct : public type::Action
		{
		public:
			inline LongShortConstruct( scope::Global& scope ) : Action( name_long, Type::instance, name_construct, name_long,
				DynamicTuple( { type::Attribute( name_other, name_short ) } ), { name_memory_denied } ) {}
			virtual ~LongShortConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LongIntConstruct : public type::Action
		{
		public:
			inline LongIntConstruct( scope::Global& scope ) : Action( name_long, Type::instance, name_construct, name_long,
				DynamicTuple( { type::Attribute( name_other, name_int ) } ), { name_memory_denied } ) {}
			virtual ~LongIntConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LongFloatConstruct : public type::Action
		{
		public:
			inline LongFloatConstruct( scope::Global& scope ) : Action( name_long, Type::instance, name_construct, name_long,
				DynamicTuple( { type::Attribute( name_other, name_float ) } ), { name_memory_denied } ) {}
			virtual ~LongFloatConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LongLowConstruct : public type::Action
		{
		public:
			inline LongLowConstruct( scope::Global& scope ) : Action( name_long, Type::instance, name_construct, name_long,
				DynamicTuple( { type::Attribute( name_other, name_low ) } ), { name_memory_denied } ) {}
			virtual ~LongLowConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LongHighConstruct : public type::Action
		{
		public:
			inline LongHighConstruct( scope::Global& scope ) : Action( name_long, Type::instance, name_construct, name_long,
				DynamicTuple( { type::Attribute( name_other, name_high ) } ), { name_memory_denied } ) {}
			virtual ~LongHighConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LongIndexConstruct : public type::Action
		{
		public:
			inline LongIndexConstruct( scope::Global& scope ) : Action( name_long, Type::instance, name_construct, name_long,
				DynamicTuple( { type::Attribute( name_other, name_index ) } ), { name_memory_denied } ) {}
			virtual ~LongIndexConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class LongCmp : public type::Action
		{
		public:
			inline LongCmp( scope::Global& scope ) : Action( name_long, opcode::cmp, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_long ) } ) ) {}
			virtual ~LongCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LongPlus : public type::Action
		{
		public:
			inline LongPlus( scope::Global& scope ) : Action( name_long, opcode::plus, name_long, DynamicTuple(
				{ type::Attribute( name_other, name_long ) } ) ) {}
			virtual ~LongPlus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LongMinus : public type::Action
		{
		public:
			inline LongMinus( scope::Global& scope ) : Action( name_long, opcode::minus, name_long, DynamicTuple(
				{ type::Attribute( name_other, name_long ) } ) ) {}
			virtual ~LongMinus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LongMultiply : public type::Action
		{
		public:
			inline LongMultiply( scope::Global& scope ) : Action( name_long, opcode::multiply, name_long, DynamicTuple(
				{ type::Attribute( name_other, name_long ) } ) ) {}
			virtual ~LongMultiply() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LongDivide : public type::Action
		{
		public:
			inline LongDivide( scope::Global& scope ) : Action( name_long, opcode::divide, name_long, DynamicTuple(
				{ type::Attribute( name_other, name_long ) } ) ) {}
			virtual ~LongDivide() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class LongElementIndex : public type::Action
		{
		public:
			inline LongElementIndex( scope::Global& scope ) : Action( name_long, opcode::element, name_long, DynamicTuple(
				{ type::Attribute( name_pos, name_index ) } ) ) {}
			virtual ~LongElementIndex() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LongElementInt : public type::Action
		{
		public:
			inline LongElementInt( scope::Global& scope ) : Action( name_long, opcode::element, name_long, DynamicTuple(
				{ type::Attribute( name_pos, name_int ) } ) ) {}
			virtual ~LongElementInt() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class LongSlice : public type::Action
		{
		public:
			inline LongSlice( scope::Global& scope ) : Action( name_long, opcode::slice, name_long, DynamicTuple( {
				type::Attribute( name::get( "first" ), name_int ),
				type::Attribute( name::get( "last" ), name_int ) } ) ) {}
			virtual ~LongSlice() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class LongBitNot : public type::Action
		{
		public:
			inline LongBitNot() : Action( name_long, opcode::bit_not, name_long, DynamicTuple() ) {}
			virtual ~LongBitNot() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LongBitAnd : public type::Action
		{
		public:
			inline LongBitAnd( scope::Global& scope ) : Action( name_long, opcode::bit_and, name_long, DynamicTuple(
				{ type::Attribute( name_other, name_long ) } ) ) {}
			virtual ~LongBitAnd() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LongBitOr : public type::Action
		{
		public:
			inline LongBitOr( scope::Global& scope ) : Action( name_long, opcode::bit_or, name_long, DynamicTuple(
				{ type::Attribute( name_other, name_long ) } ) ) {}
			virtual ~LongBitOr() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LongBitXor : public type::Action
		{
		public:
			inline LongBitXor( scope::Global& scope ) : Action( name_long, opcode::bit_xor, name_long, DynamicTuple(
				{ type::Attribute( name_other, name_long ) } ) ) {}
			virtual ~LongBitXor() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LongLShift: public type::Action
		{
		public:
			inline LongLShift( scope::Global& scope ) : Action( name_long, opcode::bit_lsh, name_long, DynamicTuple(
				{ type::Attribute( name_other, name_index ) } ) ) {}
			virtual ~LongLShift() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LongRShift : public type::Action
		{
		public:
			inline LongRShift( scope::Global& scope ) : Action( name_long, opcode::bit_rsh, name_long, DynamicTuple(
				{ type::Attribute( name_other, name_index ) } ) ) {}
			virtual ~LongRShift() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
