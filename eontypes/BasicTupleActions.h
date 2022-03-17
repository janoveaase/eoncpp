#pragma once

#include "BasicTuple.h"
#include "Action.h"
#include "OperatorAction.h"
#include "Integer.h"
#include "Index.h"
#include "TypeTuple.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerBasicTupleActions( scope::Global& scope );




	namespace actions
	{
		/**********************************************************************
		  Eon BasicTuple actions
		**********************************************************************/

		template<typename T1, typename T2>
		class TupleCmp : public OperatorAction
		{
		public:
			inline TupleCmp() : OperatorAction( T1::tupleType(), opcode::cmp, name_int, DynamicTuple( {
				type::Attribute( name_other, T2::tupleType() ) } ), {}, source::Ref() ) {}
			virtual ~TupleCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto& arg1 = *_operand<T1>( scope, action_node, 0 );
				auto& arg2 = *_operand<T2>( scope, action_node, 1 );
				auto rval = new IntegerInstance<int_t>();
				rval->value( arg1.compare( arg2 ) );
				return rval;
			}
		};

		template<typename T1, typename T2>
		class TupleAssign : public OperatorAction
		{
		public:
			inline TupleAssign() : OperatorAction( T1::tupleType(), opcode::assign, T1::tupleType(), DynamicTuple( {
				type::Attribute( name_other, T2::tupleType() ) } ), {}, source::Ref() ) {}
			virtual ~TupleAssign() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto& arg1 = *_operand<T1>( scope, action_node, 0 );
				auto& arg2 = *_operand<T2>( scope, action_node, 1 );
				if( arg2.generalType() != name_reference && arg2.generalType() != name_modify )
					arg1 = std::move( arg2 );
				else
					*(type::BasicTuple*)&arg1 = *(type::BasicTuple*)&arg2;
				return &arg1;
			}
		};

		template<typename T, typename A, typename R>
		class TuplePlus : public OperatorAction
		{
		public:
			inline TuplePlus() : OperatorAction( TypeTuple::tuple( T::tupleType() ), opcode::plus,
				TypeTuple::tuple( R::tupleType() ), DynamicTuple( { type::Attribute( name_other,
					TypeTuple::tuple( A::tupleType() ) ) } ), {}, source::Ref() ) {}
			virtual ~TuplePlus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto& arg1 = *_operand<T>( scope, action_node, 0 );
				auto& arg2 = *_operand<A>( scope, action_node, 1 );
				auto rval = new R();
				*rval += arg1;
				*rval += arg2;
				rval->finalize();
				return rval;
			}
		};

		template<typename T, typename A, typename R>
		class TupleMinus : public OperatorAction
		{
		public:
			inline TupleMinus() : OperatorAction( TypeTuple::tuple( T::tupleType() ), opcode::plus,
				TypeTuple::tuple( R::tupleType() ), DynamicTuple( { type::Attribute( name_other,
					TypeTuple::tuple( A::tupleType() ) ) } ), {}, source::Ref() ) {}
			virtual ~TupleMinus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto& arg1 = *_operand<T>( scope, action_node, 0 );
				auto& arg2 = *_operand<A>( scope, action_node, 1 );
				auto rval = new R();
				*rval += arg1;
				*rval -= arg2;
				rval->finalize();
				return rval;
			}
		};

		template<typename T>
		class TupleElementIndex : public OperatorAction
		{
		public:
			inline TupleElementIndex() : OperatorAction( TypeTuple::tuple( T::tupleType() ), opcode::element,
				TypeTuple::tuple( name_dynamic ), DynamicTuple( { type::Attribute( name_pos, name_index ) } ), {},
				source::Ref() ) {}
			virtual ~TupleElementIndex() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto& arg1 = *_operand<T>( scope, action_node, 0 );
				auto arg2 = _operand<IndexInstance>( scope, action_node, 1 );
				return arg1.at( arg2->value() ).value();
			}
		};

		template<typename T>
		class TupleElementInt : public OperatorAction
		{
		public:
			inline TupleElementInt() : OperatorAction( TypeTuple::tuple( T::tupleType() ), opcode::element,
				TypeTuple::tuple( name_dynamic ), DynamicTuple( { type::Attribute( name_pos, name_int ) } ), {},
				source::Ref() ) {}
			virtual ~TupleElementInt() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto& arg1 = *_operand<T>( scope, action_node, 0 );
				auto arg2 = _operand<IntegerInstance<int_t>>( scope, action_node, 1 );
				auto pos = arg2->value() > 0 ? static_cast<index_t>( arg2->value() )
					: arg1.numAttributes() - static_cast<index_t>( -arg2->value() );
				return arg1.at( pos ).value();
			}
		};

		template<typename T>
		class TupleElementName : public OperatorAction
		{
		public:
			inline TupleElementName() : OperatorAction( TypeTuple::tuple( T::tupleType() ), opcode::element,
				TypeTuple::tuple( name_dynamic ), DynamicTuple( { type::Attribute( name_pos, name_name ) } ), {},
				source::Ref() ) {}
			virtual ~TupleElementName() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto& arg1 = *_operand<T>( scope, action_node, 0 );
				auto arg2 = _operand<NameInstance>( scope, action_node, 1 );
				return arg1.at( arg2->value() ).value();
			}
		};

		template<typename T>
		class TupleSlice : public OperatorAction
		{
		public:
			inline TupleSlice() : OperatorAction( TypeTuple::tuple( T::tupleType() ), opcode::slice,
				TypeTuple::tuple( name_dynamic ), DynamicTuple( { type::Attribute( eon::name( "first" ), name_int ),
					type::Attribute( eon::name( "last" ), name_int ) } ), {}, source::Ref() ) {}
			virtual ~TupleSlice() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override
			{
				auto& arg1 = *_operand<T>( scope, action_node, 0 );
				auto arg2 = _operand<IntegerInstance<int_t>>( scope, action_node, 1 );
				auto arg3 = _operand<IntegerInstance<int_t>>( scope, action_node, 2 );
				auto first = arg2->value() > 0 ? static_cast<index_t>( arg2->value() )
					: arg1.numAttributes() - static_cast<index_t>( -arg2->value() );
				auto last = arg3->value() > 0 ? static_cast<index_t>( arg3->value() )
					: arg1.numAttributes() - static_cast<index_t>( -arg3->value() );
				auto rval = new DynamicTuple();
				if( first < last )
				{
					for( auto i = first; i <= last; ++i )
						*rval += arg1.at( i ).value();
				}
				else if( first > last )
				{
					for( auto i = last; i >= first; --i )
						*rval += arg1.at( i ).value();
				}
				return rval;
			}
		};
	}
}
