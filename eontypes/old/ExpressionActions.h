#pragma once

#include "Expression.h"
#include "Action.h"
#include "Bool.h"
#include "Int.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register expresion actions in the global scope
	void registerExpressionActions();


	namespace actions
	{
		using namespace scope;
		using namespace type;

		/**********************************************************************
		  Eon expression actions
		**********************************************************************/

		class ExpressionConstruct : public Action
		{
		public:
			inline ExpressionConstruct() : Action( Type::instance, name_construct, name_expression, DynamicTuple(), {},
				false, Precedence::lower_ultra_hi, Precedence::lower_ultra_lo ) {}
			virtual ~ExpressionConstruct() = default;
			inline void selfDelete() override { delete this; }
			inline void execute( Scope& parent_scope, scope::Frame& frame ) override {}
		};
		class ExpressionCopyConstruct : public Action
		{
		public:
			inline ExpressionCopyConstruct() : Action( Type::instance, name_construct, name_expression, DynamicTuple(
				{ type::DynamicAttribute(  type::Qualifier::read, name_one,
					new ExpressionInstance( GLOBSCOPE.get( name_expression ), false ) ) } ),
				{ name_no_memory }, false, Precedence::lower_ultra_hi, Precedence::lower_ultra_lo ) {}
			virtual ~ExpressionCopyConstruct() = default;
			inline void selfDelete() override { delete this; }
			void execute( Scope& parent_scope, scope::Frame& frame ) override;
		};

		class ExpressionCmp : public Action
		{
		public:
			inline ExpressionCmp() : Action( Type::_operator, name_cmp, name_int,
				DynamicTuple( { type::DynamicAttribute( type::Qualifier::read, name_one,
					new ExpressionInstance( GLOBSCOPE.get( name_expression ), false ) ) } ),
				{}, false, Precedence::mid_hi, Precedence::mid_lo ) {}
			virtual ~ExpressionCmp() = default;
			void selfDelete() override { delete this; }
			void execute( Scope& parent_scope, scope::Frame& frame ) override;
		};

		class ExpressionCall : public Action
		{
		public:
			inline ExpressionCall() : Action( Type::_operator, name_not, name_bool, DynamicTuple(), {}, false,
				Precedence::lower_ultra_hi, Precedence::lower_ultra_lo ) {}
			virtual ~ExpressionCall() = default;
			void selfDelete() override { delete this; }
			void execute( Scope& parent_scope, scope::Frame& frame ) override;
		};
	}
}
