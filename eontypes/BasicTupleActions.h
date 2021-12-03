#pragma once

#include "BasicTuple.h"
#include "Action.h"
#include "Int.h"
#include "Index.h"
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

		class BasicTupleCmp : public type::Action
		{
		public:
			inline BasicTupleCmp( scope::Global& scope ) : Action( name_dynamic, opcode::cmp, name_int, DynamicTuple( 
				{ type::Attribute( name_other, name_dynamic ) } ) ) {}
			virtual ~BasicTupleCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class BasicTuplePlusDynamic : public type::Action
		{
		public:
			inline BasicTuplePlusDynamic( scope::Global& scope ) : Action(
				name_dynamic, opcode::plus, name_dynamic, DynamicTuple(
				{ type::Attribute( name_other, name_dynamic ) } ) ) {}
			virtual ~BasicTuplePlusDynamic() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BasicTuplePlusTuple : public type::Action
		{
		public:
			inline BasicTuplePlusTuple( scope::Global& scope ) : Action(
				name_dynamic, opcode::plus, name_dynamic, DynamicTuple(
				{ type::Attribute( name_other, name_tuple ) } ) ) {}
			virtual ~BasicTuplePlusTuple() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BasicTupleMinusDynamic : public type::Action
		{
		public:
			inline BasicTupleMinusDynamic( scope::Global& scope ) : Action(
				name_dynamic, opcode::minus, name_dynamic, DynamicTuple(
				{ type::Attribute( name_other, name_dynamic ) } ) ) {}
			virtual ~BasicTupleMinusDynamic() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BasicTupleMinusTuple : public type::Action
		{
		public:
			inline BasicTupleMinusTuple( scope::Global& scope ) : Action(
				name_dynamic, opcode::minus, name_dynamic, DynamicTuple(
				{ type::Attribute( name_other, name_tuple ) } ) ) {}
			virtual ~BasicTupleMinusTuple() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class BasicTupleElementIndex : public type::Action
		{
		public:
			inline BasicTupleElementIndex( scope::Global& scope ) : Action(
				name_dynamic, opcode::element, name_dynamic, DynamicTuple(
				{ type::Attribute( name_pos, name_index ) } ) ) {}
			virtual ~BasicTupleElementIndex() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BasicTupleElementInt : public type::Action
		{
		public:
			inline BasicTupleElementInt( scope::Global& scope ) : Action(
				name_dynamic, opcode::element, name_dynamic, DynamicTuple(
				{ type::Attribute( name_pos, name_int ) } ) ) {}
			virtual ~BasicTupleElementInt() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BasicTupleElementName : public type::Action
		{
		public:
			inline BasicTupleElementName( scope::Global& scope ) : Action(
				name_dynamic, opcode::element, name_dynamic, DynamicTuple(
				{ type::Attribute( name_pos, name_name ) } ) ) {}
			virtual ~BasicTupleElementName() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class BasicTupleSlice : public type::Action
		{
		public:
			inline BasicTupleSlice( scope::Global& scope ) : Action(
				name_dynamic, opcode::slice, name_dynamic, DynamicTuple( {
				type::Attribute( name::get( "first" ), name_int ),
				type::Attribute( name::get( "last" ), name_int ) } ) ) {}
			virtual ~BasicTupleSlice() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
