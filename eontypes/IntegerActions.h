#pragma once

#include "Integer.h"
#include "Action.h"
#include "OperatorAction.h"
#include "Bool.h"
#include "Byte.h"
#include "Char.h"
#include "Floatingpt.h"
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
		  Eon integer actions
		**********************************************************************/

		template<typename int_type>
		class IntegerConstruct : public type::Action
		{
		public:
			inline IntegerConstruct() : Action( type::Handler::mapType<int_type>(), Type::instance, name_constructor,
				type::Handler::mapType<int_type>(), DynamicTuple(), {}, source::Ref() ) {}
			virtual ~IntegerConstruct() = default;
			void die() override {}
			inline Object* execute( scope::Scope& scope, type::Node& action_node ) override {
				return new IntegerInstance<int_type>(); }
		};

		template<typename int_type>
		class IntegerBoolConstruct : public type::Action
		{
		public:
			inline IntegerBoolConstruct() : Action( type::Handler::mapType<int_type>(), Type::instance, name_constructor,
				type::Handler::mapType<int_type>(), DynamicTuple( { type::Attribute( name_other, name_bool ) } ),
				{ name_memory_denied }, source::Ref() ) {}
			virtual ~IntegerBoolConstruct() = default;
			void die() override {}
			inline Object* execute( scope::Scope& scope, type::Node& action_node ) override {
				auto arg1 = _operand<BoolInstance>( scope, action_node, 0 );
				return new IntegerInstance<int_type>( arg1->value(), arg1->source() ); }
		};
		template<typename int_type>
		class IntegerByteConstruct : public type::Action
		{
		public:
			inline IntegerByteConstruct() : Action( type::Handler::mapType<int_type>(), Type::instance, name_constructor,
				type::Handler::mapType<int_type>(), DynamicTuple( { type::Attribute( name_other, name_byte ) } ),
				{ name_memory_denied }, source::Ref() ) {}
			virtual ~IntegerByteConstruct() = default;
			void die() override {}
			inline Object* execute( scope::Scope& scope, type::Node& action_node ) override {
				auto arg1 = _operand<ByteInstance>( scope, action_node, 0 );
				return new IntegerInstance<int_type>( arg1->value(), arg1->source() ); }
		};
		template<typename int_type>
		class IntegerCharConstruct : public type::Action
		{
		public:
			inline IntegerCharConstruct() : Action( type::Handler::mapType<int_type>(), Type::instance, name_constructor,
				type::Handler::mapType<int_type>(), DynamicTuple( { type::Attribute( name_other, name_char ) } ),
				{ name_memory_denied }, source::Ref() ) {}
			virtual ~IntegerCharConstruct() = default;
			void die() override {}
			inline Object* execute( scope::Scope& scope, type::Node& action_node ) override {
				auto arg1 = _operand<CharInstance>( scope, action_node, 0 );
				return new IntegerInstance<int_type>( static_cast<int_type>( arg1->value() ), arg1->source() ); }
		};
		template<typename int_type, typename input_type>
		class IntegerIntegerConstruct : public type::Action
		{
		public:
			inline IntegerIntegerConstruct() : Action( type::Handler::mapType<int_type>(), Type::instance,
				name_constructor, type::Handler::mapType<int_type>(), DynamicTuple( { type::Attribute( name_other,
					name_short ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~IntegerIntegerConstruct() = default;
			void die() override {}
			inline Object* execute( scope::Scope& scope, type::Node& action_node ) override {
				auto arg1 = _operand<IntegerInstance<input_type>>( scope, action_node, 0 );
				return new IntegerInstance<int_type>( static_cast<int_type>( arg1->value() ), arg1->source() ); }
		};
		template<typename int_type, typename input_type>
		class IntegerFloatingptConstruct : public type::Action
		{
		public:
			inline IntegerFloatingptConstruct() : Action( type::Handler::mapType<int_type>(), Type::instance,
				name_constructor, type::Handler::mapType<int_type>(), DynamicTuple( { type::Attribute( name_other,
					name_float ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~IntegerFloatingptConstruct() = default;
			void die() override {}
			inline Object* execute( scope::Scope& scope, type::Node& action_node ) override {
				auto arg1 = _operand<FloatingptInstance<input_type>>( scope, action_node, 0 );
				return new IntegerInstance<int_type>( static_cast<int_type>( arg1->value() ), arg1->source() ); }
		};
		template<typename int_type>
		class IntegerIndexConstruct : public type::Action
		{
		public:
			inline IntegerIndexConstruct() : Action( type::Handler::mapType<int_type>(), Type::instance, name_constructor,
				type::Handler::mapType<int_type>(), DynamicTuple( { type::Attribute( name_other, name_index ) } ),
				{ name_memory_denied }, source::Ref() ) {}
			virtual ~IntegerIndexConstruct() = default;
			void die() override {}
			inline Object* execute( scope::Scope& scope, type::Node& action_node ) override {
				auto arg1 = _operand<IndexInstance>( scope, action_node, 0 );
				return new IntegerInstance<int_type>( static_cast<int_type>( arg1->value() ), arg1->source() );}
		};

		template<typename int_type>
		class IntegerAssign : public OperatorAction
		{
		public:
			inline IntegerAssign() : OperatorAction( type::Handler::mapType<int_type>(), opcode::assign, name_int,
				DynamicTuple( { type::Attribute( name_other, type::Handler::mapType<int_type>() ) } ), {},
				source::Ref() ) {}
			virtual ~IntegerAssign() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<IntegerInstance<int_type>>( scope, action_node, 0 );
				auto arg2 = _operand<IntegerInstance<int_type>>( scope, action_node, 1 );
				arg1->value( arg2->value() );
				return arg1;
			}
		};
		template<typename int_type>
		class IntegerPlusAssign : public OperatorAction
		{
		public:
			inline IntegerPlusAssign() : OperatorAction( type::Handler::mapType<int_type>(), opcode::plus_assign, name_int,
				DynamicTuple( { type::Attribute( name_other, type::Handler::mapType<int_type>() ) } ), {},
				source::Ref() ) {}
			virtual ~IntegerPlusAssign() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<IntegerInstance<int_type>>( scope, action_node, 0 );
				auto arg2 = _operand<IntegerInstance<int_type>>( scope, action_node, 1 );
				arg1->value( arg1->value() + arg2->value() );
				return arg1;
			}
		};
		template<typename int_type>
		class IntegerMinusAssign : public OperatorAction
		{
		public:
			inline IntegerMinusAssign() : OperatorAction( type::Handler::mapType<int_type>(), opcode::minus_assign,
				name_int, DynamicTuple( { type::Attribute( name_other, type::Handler::mapType<int_type>() ) } ), {},
				source::Ref() ) {}
			virtual ~IntegerMinusAssign() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<IntegerInstance<int_type>>( scope, action_node, 0 );
				auto arg2 = _operand<IntegerInstance<int_type>>( scope, action_node, 1 );
				arg1->value( arg1->value() - arg2->value() );
				return arg1;
			}
		};
		template<typename int_type>
		class IntegerMultAssign : public OperatorAction
		{
		public:
			inline IntegerMultAssign() : OperatorAction( type::Handler::mapType<int_type>(), opcode::multiply_assign,
				name_int, DynamicTuple( { type::Attribute( name_other, type::Handler::mapType<int_type>() ) } ), {},
				source::Ref() ) {}
			virtual ~IntegerMultAssign() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<IntegerInstance<int_type>>( scope, action_node, 0 );
				auto arg2 = _operand<IntegerInstance<int_type>>( scope, action_node, 1 );
				arg1->value( arg1->value() * arg2->value() );
				return arg1;
			}
		};
		template<typename int_type>
		class IntegerDivideAssign : public OperatorAction
		{
		public:
			inline IntegerDivideAssign() : OperatorAction( type::Handler::mapType<int_type>(), opcode::divide_assign, name_int,
				DynamicTuple( { type::Attribute( name_other, type::Handler::mapType<int_type>() ) } ), {},
				source::Ref() ) {}
			virtual ~IntegerDivideAssign() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg2 = _operand<IntegerInstance<int_type>>( scope, action_node, 1 );
				if( arg2->value() == 0 )
					throw DivisionByZero();
				auto arg1 = _operand<IntegerInstance<int_type>>( scope, action_node, 0 );
				arg1->value( arg1->value() / arg2->value() );
				return arg1;
			}
		};

		template<typename int_type>
		class IntegerCmp : public OperatorAction
		{
		public:
			inline IntegerCmp() : OperatorAction( type::Handler::mapType<int_type>(), opcode::cmp, name_int, DynamicTuple( {
				type::Attribute( name_other, type::Handler::mapType<int_type>() ) } ), {}, source::Ref() ) {}
			virtual ~IntegerCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<IntegerInstance<int_type>>( scope, action_node, 0 );
				auto arg2 = _operand<IntegerInstance<int_type>>( scope, action_node, 1 );
				auto rval = new IntegerInstance<int_t>();
				rval->value( arg1->value() < arg2->value() ? -1 : arg2->value() < arg1->value() ? 1 : 0 );
				return rval;
			}
		};

		template<typename int_type>
		class IntegerPlus : public OperatorAction
		{
		public:
			inline IntegerPlus() : OperatorAction( type::Handler::mapType<int_type>(), opcode::plus,
				type::Handler::mapType<int_type>(), DynamicTuple( { type::Attribute( name_other,
					type::Handler::mapType<int_type>() ) } ), {}, source::Ref() ) {}
			virtual ~IntegerPlus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<IntegerInstance<int_type>>( scope, action_node, 0 );
				auto arg2 = _operand<IntegerInstance<int_type>>( scope, action_node, 1 );
				auto rval = new IntegerInstance<int_type>();
				rval->value( arg1->value() + arg2->value() );
				return rval;
			}
		};
		template<typename int_type>
		class IntegerMinus : public OperatorAction
		{
		public:
			inline IntegerMinus() : OperatorAction( type::Handler::mapType<int_type>(), opcode::minus,
				type::Handler::mapType<int_type>(), DynamicTuple( { type::Attribute( name_other,
					type::Handler::mapType<int_type>() ) } ), {}, source::Ref() ) {}
			virtual ~IntegerMinus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<IntegerInstance<int_type>>( scope, action_node, 0 );
				auto arg2 = _operand<IntegerInstance<int_type>>( scope, action_node, 1 );
				auto rval = new IntegerInstance<int_type>();
				rval->value( arg1->value() - arg2->value() );
				return rval;
			}
		};
		template<typename int_type>
		class IntegerMultiply : public OperatorAction
		{
		public:
			inline IntegerMultiply() : OperatorAction( type::Handler::mapType<int_type>(), opcode::multiply,
				type::Handler::mapType<int_type>(), DynamicTuple( { type::Attribute( name_other,
					type::Handler::mapType<int_type>() ) } ), {}, source::Ref() ) {}
			virtual ~IntegerMultiply() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<IntegerInstance<int_type>>( scope, action_node, 0 );
				auto arg2 = _operand<IntegerInstance<int_type>>( scope, action_node, 1 );
				auto rval = new IntegerInstance<int_type>();
				rval->value( arg1->value() * arg2->value() );
				return rval;
			}
		};
		template<typename int_type>
		class IntegerDivide : public OperatorAction
		{
		public:
			inline IntegerDivide() : OperatorAction( type::Handler::mapType<int_type>(), opcode::divide,
				type::Handler::mapType<int_type>(), DynamicTuple( { type::Attribute( name_other,
					type::Handler::mapType<int_type>() ) } ), {}, source::Ref() ) {}
			virtual ~IntegerDivide() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg2 = _operand<IntegerInstance<int_type>>( scope, action_node, 1 );
				if( arg2->value() == 0 )
					throw DivisionByZero();
				auto arg1 = _operand<IntegerInstance<int_type>>( scope, action_node, 0 );
				auto rval = new IntegerInstance<int_type>();
				rval->value( arg1->value() / arg2->value() );
				return rval;
			}
		};

		template<typename int_type>
		class IntegerElementIndex : public OperatorAction
		{
		public:
			inline IntegerElementIndex() : OperatorAction( type::Handler::mapType<int_type>(), opcode::element,
				type::Handler::mapType<int_type>(), DynamicTuple( { type::Attribute( name_pos, name_index ) } ), {},
				source::Ref() ) {}
			virtual ~IntegerElementIndex() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<IntegerInstance<int_type>>( scope, action_node, 0 );
				auto arg2 = _operand<IndexInstance>( scope, action_node, 1 );
				if( arg2->value() >= sizeof( int_t ) )
					throw type::NotFound();
				auto rval = new IntegerInstance<int_type>();
				rval->value( arg1->value() & ( static_cast<int_type>( 1 ) << ( arg2->value() - 1 ) ) ? 1 : 0 );
				return rval;
			}
		};
		template<typename int_type>
		class IntegerElementInt : public OperatorAction
		{
		public:
			inline IntegerElementInt() : OperatorAction( type::Handler::mapType<int_type>(), opcode::element,
				type::Handler::mapType<int_type>(), DynamicTuple( { type::Attribute( name_pos,
					type::Handler::mapType<int_type>() ) } ), {}, source::Ref() ) {}
			virtual ~IntegerElementInt() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<IntegerInstance<int_type>>( scope, action_node, 0 );
				auto arg2 = _operand<IntegerInstance<int_t>>( scope, action_node, 1 );
				index_t pos = arg2->value() > 0 ? static_cast<index_t>( arg2->value() )
					: sizeof( int_type ) - static_cast<index_t>( -arg2->value() );
				if( pos >= sizeof( int_type ) )
					throw type::NotFound();
				auto rval = new IntegerInstance<int_type>();
				rval->value( arg1->value() & ( static_cast<int_type>( 1 ) << ( pos - 1 ) ) ? 1 : 0 );
				return rval;
			}
		};

		template<typename int_type>
		class IntegerSlice : public OperatorAction
		{
		public:
			inline IntegerSlice() : OperatorAction( type::Handler::mapType<int_type>(), opcode::slice,
				type::Handler::mapType<int_type>(), DynamicTuple( { type::Attribute( eon::name( "first" ),
					type::Handler::mapType<int_type>() ), type::Attribute( eon::name( "last" ),
						type::Handler::mapType<int_type>() ) } ), {}, source::Ref() ) {}
			virtual ~IntegerSlice() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<IntegerInstance<int_type>>( scope, action_node, 0 );
				auto arg2 = _operand<IntegerInstance<int_t>>( scope, action_node, 1 );
				auto arg3 = _operand<IntegerInstance<int_t>>( scope, action_node, 2 );
				auto rval = new IntegerInstance<int_type>();
				auto v1 = arg2->value(), v2 = arg3->value();
				if( v1 < 0 )
					v1 = 32 + v1;
				if( v2 < 0 )
					v2 = 32 + v2;
				auto low = v1 < v2 ? v1 : v2;
				auto high = v1 > v2 ? v1 : v2;
				// TODO: Verify that v1 and v2 are in range!
				if( low > 0 && high <= 32 )
					rval->value( ( arg1->value() << ( 32 - high ) ) >> low );
				return rval;
			}
		};

		template<typename int_type>
		class IntegerBitNot : public OperatorAction
		{
		public:
			inline IntegerBitNot() : OperatorAction( type::Handler::mapType<int_type>(), opcode::bit_not,
				type::Handler::mapType<int_type>(), DynamicTuple(), {}, source::Ref() ) {}
			virtual ~IntegerBitNot() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<IntegerInstance<int_type>>( scope, action_node, 0 );
				auto rval = new IntegerInstance<int_type>();
				rval->value( ~arg1->value() );
				return rval;
			}
		};
		template<typename int_type>
		class IntegerBitAnd : public OperatorAction
		{
		public:
			inline IntegerBitAnd() : OperatorAction( type::Handler::mapType<int_type>(), opcode::bit_and,
				type::Handler::mapType<int_type>(), DynamicTuple( { type::Attribute( name_other,
					type::Handler::mapType<int_type>() ) } ), {}, source::Ref() ) {}
			virtual ~IntegerBitAnd() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<IntegerInstance<int_type>>( scope, action_node, 0 );
				auto arg2 = _operand<IntegerInstance<int_type>>( scope, action_node, 1 );
				auto rval = new IntegerInstance<int_type>();
				rval->value( arg1->value() & arg2->value() );
				return rval;
			}
		};
		template<typename int_type>
		class IntegerBitOr : public OperatorAction
		{
		public:
			inline IntegerBitOr() : OperatorAction( type::Handler::mapType<int_type>(), opcode::bit_or,
				type::Handler::mapType<int_type>(), DynamicTuple( { type::Attribute( name_other,
					type::Handler::mapType<int_type>() ) } ), {}, source::Ref() ) {}
			virtual ~IntegerBitOr() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<IntegerInstance<int_type>>( scope, action_node, 0 );
				auto arg2 = _operand<IntegerInstance<int_type>>( scope, action_node, 1 );
				auto rval = new IntegerInstance<int_type>();
				rval->value( arg1->value() | arg2->value() );
				return rval;
			}
		};
		template<typename int_type>
		class IntegerBitXor : public OperatorAction
		{
		public:
			inline IntegerBitXor() : OperatorAction( type::Handler::mapType<int_type>(), opcode::bit_xor,
				type::Handler::mapType<int_type>(), DynamicTuple( { type::Attribute( name_other,
					type::Handler::mapType<int_type>() ) } ), {}, source::Ref() ) {}
			virtual ~IntegerBitXor() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<IntegerInstance<int_type>>( scope, action_node, 0 );
				auto arg2 = _operand<IntegerInstance<int_type>>( scope, action_node, 1 );
				auto rval = new IntegerInstance<int_type>();
				rval->value( arg1->value() ^ arg2->value() );
				return rval;
			}
		};
		template<typename int_type>
		class IntegerLShift : public OperatorAction
		{
		public:
			inline IntegerLShift() : OperatorAction( type::Handler::mapType<int_type>(), opcode::bit_lsh,
				type::Handler::mapType<int_type>(), DynamicTuple( { type::Attribute( name_other, name_index ) } ), {},
				source::Ref() ) {}
			virtual ~IntegerLShift() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<IntegerInstance<int_type>>( scope, action_node, 0 );
				auto arg2 = _operand<IntegerInstance<int_type>>( scope, action_node, 1 );
				auto rval = new IntegerInstance<int_type>();
				rval->value( arg1->value() << arg2->value() );
				return rval;
			}
		};
		template<typename int_type>
		class IntegerRShift : public OperatorAction
		{
		public:
			inline IntegerRShift() : OperatorAction( type::Handler::mapType<int_type>(), opcode::bit_rsh,
				type::Handler::mapType<int_type>(), DynamicTuple( { type::Attribute( name_other, name_index ) } ), {},
				source::Ref() ) {}
			virtual ~IntegerRShift() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto arg1 = _operand<IntegerInstance<int_type>>( scope, action_node, 0 );
				auto arg2 = _operand<IntegerInstance<int_type>>( scope, action_node, 1 );
				auto rval = new IntegerInstance<int_type>();
				rval->value( arg1->value() >> arg2->value() );
				return rval;
			}
		};
	}




	//* Register type and actions in the global scope
	template<typename int_type>
	void registerIntegerActions( scope::Global& scope ) {
		scope.addAction( name_constructor, new actions::IntegerConstruct<int_type>() );
		scope.addAction( name_constructor, new actions::IntegerBoolConstruct<int_type>() );
		scope.addAction( name_constructor, new actions::IntegerByteConstruct<int_type>() );
		scope.addAction( name_constructor, new actions::IntegerCharConstruct<int_type>() );		
		scope.addAction( name_constructor, new actions::IntegerIntegerConstruct<int_type, int_t>() );
		scope.addAction( name_constructor, new actions::IntegerIntegerConstruct<int_type, short_t>() );
		scope.addAction( name_constructor, new actions::IntegerIntegerConstruct<int_type, long_t>() );
		scope.addAction( name_constructor, new actions::IntegerFloatingptConstruct<int_type, flt_t>() );
		scope.addAction( name_constructor, new actions::IntegerFloatingptConstruct<int_type, low_t>() );
		scope.addAction( name_constructor, new actions::IntegerFloatingptConstruct<int_type, high_t>() );
		scope.addAction( name_constructor, new actions::IntegerIndexConstruct<int_type>() );
		scope.addOperator( type::operators::code::assign, new actions::IntegerAssign<int_type>() );
		scope.addOperator( type::operators::code::plus_assign, new actions::IntegerPlusAssign<int_type>() );
		scope.addOperator( type::operators::code::minus_assign, new actions::IntegerMinusAssign<int_type>() );
		scope.addOperator( type::operators::code::multiply_assign, new actions::IntegerMultAssign<int_type>() );
		scope.addOperator( type::operators::code::divide_assign, new actions::IntegerDivideAssign<int_type>() );
		scope.addOperator( type::operators::code::cmp, new actions::IntegerCmp<int_type>() );
		scope.addOperator( type::operators::code::plus, new actions::IntegerPlus<int_type>() );
		scope.addOperator( type::operators::code::minus, new actions::IntegerMinus<int_type>() );
		scope.addOperator( type::operators::code::multiply, new actions::IntegerMultiply<int_type>() );
		scope.addOperator( type::operators::code::divide, new actions::IntegerDivide<int_type>() );
		scope.addOperator( type::operators::code::element, new actions::IntegerElementIndex<int_type>() );
		scope.addOperator( type::operators::code::element, new actions::IntegerElementInt<int_type>() );
		scope.addOperator( type::operators::code::slice, new actions::IntegerSlice<int_type>() );
		scope.addOperator( type::operators::code::bit_not, new actions::IntegerBitNot<int_type>() );
		scope.addOperator( type::operators::code::bit_and, new actions::IntegerBitAnd<int_type>() );
		scope.addOperator( type::operators::code::bit_or, new actions::IntegerBitOr<int_type>() );
		scope.addOperator( type::operators::code::bit_xor, new actions::IntegerBitXor<int_type>() );
		scope.addOperator( type::operators::code::bit_lsh, new actions::IntegerLShift<int_type>() );
		scope.addOperator( type::operators::code::bit_rsh, new actions::IntegerRShift<int_type>() );
	}
}
