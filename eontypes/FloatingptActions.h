#pragma once

#include "Floatingpt.h"
#include "Action.h"
#include "OperatorAction.h"
#include "Bool.h"
#include "Byte.h"
#include "Char.h"
#include "Integer.h"
#include "Index.h"
#include "Scope.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace actions
	{
		/**********************************************************************
		  Eon float actions
		**********************************************************************/

		template<typename float_type>
		class FloatingptConstruct : public type::Action
		{
		public:
			inline FloatingptConstruct() : Action( type::Handler::mapType<float_type>(), Type::instance, name_constructor,
				type::Handler::mapType<float_type>(), DynamicTuple(), {}, source::Ref() ) {}
			virtual ~FloatingptConstruct() = default;
			void die() override {}
			inline Object* execute( scope::Scope& scope, type::Node& action_node ) override {
				return new FloatingptInstance<float_type>(); }
		};

		template<typename float_type>
		class FloatingptBoolConstruct : public type::Action
		{
		public:
			inline FloatingptBoolConstruct() : Action( type::Handler::mapType<float_type>(), Type::instance, name_constructor,
				type::Handler::mapType<float_type>(), DynamicTuple( { type::Attribute( name_other, name_bool ) } ),
				{ name_memory_denied }, source::Ref() ) {}
			virtual ~FloatingptBoolConstruct() = default;
			void die() override {}
			inline Object* execute( scope::Scope& scope, type::Node& action_node ) override {
				auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
				return new FloatingptInstance<float_type>( arg1->value(), arg1->source() ); }
		};
		template<typename float_type>
		class FloatingptByteConstruct : public type::Action
		{
		public:
			inline FloatingptByteConstruct() : Action( type::Handler::mapType<float_type>(), Type::instance, name_constructor,
				type::Handler::mapType<float_type>(), DynamicTuple( { type::Attribute( name_other, name_byte ) } ),
				{ name_memory_denied }, source::Ref() ) {}
			virtual ~FloatingptByteConstruct() = default;
			void die() override {}
			inline Object* execute( scope::Scope& scope, type::Node& action_node ) override {
				auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
				return new FloatingptInstance<float_type>( arg1->value(), arg1->source() ); }
		};
		template<typename float_type>
		class FloatingptCharConstruct : public type::Action
		{
		public:
			inline FloatingptCharConstruct() : Action( type::Handler::mapType<float_type>(), Type::instance, name_constructor,
				type::Handler::mapType<float_type>(), DynamicTuple( { type::Attribute( name_other, name_char ) } ),
				{ name_memory_denied }, source::Ref() ) {}
			virtual ~FloatingptCharConstruct() = default;
			void die() override {}
			inline Object* execute( scope::Scope& scope, type::Node& action_node ) override {
				auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
				return new FloatingptInstance<float_type>( static_cast<float_type>( arg1->value() ), arg1->source() ); }
		};
		template<typename float_type, typename input_type>
		class FloatingptIntegerConstruct : public type::Action
		{
		public:
			inline FloatingptIntegerConstruct() : Action( type::Handler::mapType<float_type>(), Type::instance,
				name_constructor, type::Handler::mapType<float_type>(), DynamicTuple( { type::Attribute( name_other,
					name_short ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~FloatingptIntegerConstruct() = default;
			void die() override {}
			inline Object* execute( scope::Scope& scope, type::Node& action_node ) override {
				auto arg1 = _operand<IntegerInstance<input_type>>( scope, action_node, 0 );
				return new FloatingptInstance<float_type>( static_cast<float_type>( arg1->value() ), arg1->source() ); }
		};
		template<typename float_type, typename input_type>
		class FloatingptFloatingptConstruct : public type::Action
		{
		public:
			inline FloatingptFloatingptConstruct() : Action( type::Handler::mapType<float_type>(), Type::instance,
				name_constructor, type::Handler::mapType<float_type>(), DynamicTuple( { type::Attribute( name_other,
					 name_float ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~FloatingptFloatingptConstruct() = default;
			void die() override {}
			inline Object* execute( scope::Scope& scope, type::Node& action_node ) override {
				auto arg1 = _operand<FloatingptInstance<input_type>>( scope, action_node, 0 );
				return new FloatingptInstance<float_type>( static_cast<float_type>( arg1->value() ), arg1->source() ); }
		};
		template<typename float_type>
		class FloatingptIndexConstruct : public type::Action
		{
		public:
			inline FloatingptIndexConstruct() : Action( type::Handler::mapType<float_type>(), Type::instance,
				name_constructor, type::Handler::mapType<float_type>(), DynamicTuple( { type::Attribute( name_other,
					name_index ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~FloatingptIndexConstruct() = default;
			void die() override {}
			inline Object* execute( scope::Scope& scope, type::Node& action_node ) override {
				auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
				return new FloatingptInstance<float_type>( static_cast<float_type>( arg1->value() ), arg1->source() ); }
		};

		template<typename float_type>
		class FloatingptCmp : public OperatorAction
		{
		public:
			inline FloatingptCmp() : OperatorAction( type::Handler::mapType<float_type>(), opcode::cmp, name_int,
				DynamicTuple( { type::Attribute( name_other, type::Handler::mapType<float_type>() ) } ), {},
				source::Ref() ) {}
			virtual ~FloatingptCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<FloatingptInstance<float_type>>( scope, action_node, 0 );
				auto arg2 = _operand<FloatingptInstance<float_type>>( scope, action_node, 1 );
				auto rval = new IntegerInstance<int_t>();
				rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
				return rval;
			}
		};

		template<typename float_type>
		class FloatingptPlus : public OperatorAction
		{
		public:
			inline FloatingptPlus() : OperatorAction( type::Handler::mapType<float_type>(), opcode::plus,
				type::Handler::mapType<float_type>(), DynamicTuple( { type::Attribute( name_other,
					type::Handler::mapType<float_type>() ) } ), {}, source::Ref() ) {}
			virtual ~FloatingptPlus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<FloatingptInstance<float_type>>( scope, action_node, 0 );
				auto arg2 = _operand<FloatingptInstance<float_type>>( scope, action_node, 1 );
				auto rval = new FloatingptInstance<float_type>();
				rval->value( arg1->value() + arg2->value() );
				return rval;
			}
		};
		template<typename float_type>
		class FloatingptMinus : public OperatorAction
		{
		public:
			inline FloatingptMinus() : OperatorAction( type::Handler::mapType<float_type>(), opcode::minus,
				type::Handler::mapType<float_type>(), DynamicTuple( { type::Attribute( name_other,
					type::Handler::mapType<float_type>() ) } ), {}, source::Ref() ) {}
			virtual ~FloatingptMinus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<FloatingptInstance<float_type>>( scope, action_node, 0 );
				auto arg2 = _operand<FloatingptInstance<float_type>>( scope, action_node, 1 );
				auto rval = new FloatingptInstance<float_type>();
				rval->value( arg1->value() - arg2->value() );
				return rval;
			}
		};
		template<typename float_type>
		class FloatingptMultiply : public OperatorAction
		{
		public:
			inline FloatingptMultiply() : OperatorAction( type::Handler::mapType<float_type>(), opcode::multiply,
				type::Handler::mapType<float_type>(), DynamicTuple( { type::Attribute( name_other,
					type::Handler::mapType<float_type>() ) } ), {}, source::Ref() ) {}
			virtual ~FloatingptMultiply() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<FloatingptInstance<float_type>>( scope, action_node, 0 );
				auto arg2 = _operand<FloatingptInstance<float_type>>( scope, action_node, 1 );
				auto rval = new FloatingptInstance<float_type>();
				rval->value( arg1->value() * arg2->value() );
				return rval;
			}
		};
		template<typename float_type>
		class FloatingptDivide : public OperatorAction
		{
		public:
			inline FloatingptDivide() : OperatorAction( type::Handler::mapType<float_type>(), opcode::divide,
				type::Handler::mapType<float_type>(), DynamicTuple( { type::Attribute( name_other,
					type::Handler::mapType<float_type>() ) } ), {}, source::Ref() ) {}
			virtual ~FloatingptDivide() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg2 = _operand<FloatingptInstance<float_type>>( scope, action_node, 1 );
				if( arg2->value() == 0.0 )
					throw DivisionByZero();
				auto arg1 = _operand<FloatingptInstance<float_type>>( scope, action_node, 0 );
				auto rval = new FloatingptInstance<float_type>();
				rval->value( arg1->value() / arg2->value() );
				return rval;
			}
		};
	}




	//* Register type and actions in the global scope
	template<typename float_type>
	void registerFloatingptActions( scope::Global& scope )
	{
		scope.addAction( name_constructor, new actions::FloatingptConstruct<float_type>() );
		scope.addAction( name_constructor, new actions::FloatingptBoolConstruct<float_type>() );
		scope.addAction( name_constructor, new actions::FloatingptByteConstruct<float_type>() );
		scope.addAction( name_constructor, new actions::FloatingptCharConstruct<float_type>() );
		scope.addAction( name_constructor, new actions::FloatingptIntegerConstruct<float_type, int_t>() );
		scope.addAction( name_constructor, new actions::FloatingptIntegerConstruct<float_type, short_t>() );
		scope.addAction( name_constructor, new actions::FloatingptIntegerConstruct<float_type, long_t>() );
		scope.addAction( name_constructor, new actions::FloatingptIntegerConstruct<float_type, flt_t>() );
		scope.addAction( name_constructor, new actions::FloatingptIntegerConstruct<float_type, low_t>() );
		scope.addAction( name_constructor, new actions::FloatingptIntegerConstruct<float_type, high_t>() );
		scope.addAction( name_constructor, new actions::FloatingptIndexConstruct<float_type>() );
		scope.addOperator( type::operators::code::cmp, new actions::FloatingptCmp<float_type>() );
		scope.addOperator( type::operators::code::plus, new actions::FloatingptPlus<float_type>() );
		scope.addOperator( type::operators::code::minus, new actions::FloatingptMinus<float_type>() );
		scope.addOperator( type::operators::code::multiply, new actions::FloatingptMultiply<float_type>() );
		scope.addOperator( type::operators::code::divide, new actions::FloatingptDivide<float_type>() );
	}
}
