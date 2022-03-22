#pragma once

#include "ExpressionObj.h"
#include "Action.h"
#include "OperatorAction.h"
#include "Scope.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerExpressionActions( scope::Global& scope );




	namespace actions
	{
		/**********************************************************************
		  Eon Expression actions
		**********************************************************************/

		class ExprConstruct : public type::Action
		{
		public:
			inline ExprConstruct() : Action( name_expression, Type::instance, name_constructor, name_expression,
				DynamicTuple(), {}, source::Ref() ) {}
			virtual ~ExprConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class ExprCopyConstruct : public type::Action
		{
		public:
			inline ExprCopyConstruct() : Action( name_expression, Type::instance, name_constructor, name_expression,
				DynamicTuple( { type::Attribute( name_other, name_expression ) } ), { name_memory_denied }, source::Ref() ) {}
			virtual ~ExprCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class ExprCmp : public OperatorAction
		{
		public:
			inline ExprCmp() : OperatorAction( name_expression, opcode::cmp, name_int, DynamicTuple( {
				type::Attribute( name_other, name_expression ) } ), {}, source::Ref() ) {}
			virtual ~ExprCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class ExprAssign : public OperatorAction
		{
		public:
			inline ExprAssign() : OperatorAction( name_expression, opcode::assign, name_expression,
				DynamicTuple( { type::Attribute( name_other, name_expression ) } ), {}, source::Ref() ) {}
			virtual ~ExprAssign() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
