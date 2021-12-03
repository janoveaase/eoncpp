#pragma once

#include "Low.h"
#include "Action.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerLowActions( scope::Global& scope );




	namespace actions
	{
		using namespace scope;
		using namespace type;

		/**********************************************************************
		  Eon low precition float actions
		**********************************************************************/

		class LowConstruct : public type::Action
		{
		public:
			inline LowConstruct() : Action( name_low, Type::instance, name_construct, name_low, DynamicTuple() ) {}
			virtual ~LowConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LowCopyConstruct : public type::Action
		{
		public:
			inline LowCopyConstruct( scope::Global& scope ) : Action( name_low, Type::instance, name_construct, name_low,
				DynamicTuple( { type::Attribute( name_other, name_low ) } ), { name_memory_denied } ) {}
			virtual ~LowCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class LowBoolConstruct : public type::Action
		{
		public:
			inline LowBoolConstruct( scope::Global& scope ) : Action( name_low, Type::instance, name_construct, name_low,
				DynamicTuple( { type::Attribute( name_other, name_bool ) } ), { name_memory_denied } ) {}
			virtual ~LowBoolConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LowByteConstruct : public type::Action
		{
		public:
			inline LowByteConstruct( scope::Global& scope ) : Action( name_low, Type::instance, name_construct, name_low,
				DynamicTuple( { type::Attribute( name_other, name_byte ) } ), { name_memory_denied } ) {}
			virtual ~LowByteConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LowCharConstruct : public type::Action
		{
		public:
			inline LowCharConstruct( scope::Global& scope ) : Action( name_low, Type::instance, name_construct, name_low,
				DynamicTuple( { type::Attribute( name_other, name_char ) } ), { name_memory_denied } ) {}
			virtual ~LowCharConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LowIntConstruct : public type::Action
		{
		public:
			inline LowIntConstruct( scope::Global& scope ) : Action( name_low, Type::instance, name_construct, name_low,
				DynamicTuple( { type::Attribute( name_other, name_int ) } ), { name_memory_denied } ) {}
			virtual ~LowIntConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LowShortConstruct : public type::Action
		{
		public:
			inline LowShortConstruct( scope::Global& scope ) : Action( name_low, Type::instance, name_construct, name_low,
				DynamicTuple( { type::Attribute( name_other, name_short ) } ), { name_memory_denied } ) {}
			virtual ~LowShortConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LowLongConstruct : public type::Action
		{
		public:
			inline LowLongConstruct( scope::Global& scope ) : Action( name_low, Type::instance, name_construct, name_low,
				DynamicTuple( { type::Attribute( name_other, name_long ) } ), { name_memory_denied } ) {}
			virtual ~LowLongConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LowFloatConstruct : public type::Action
		{
		public:
			inline LowFloatConstruct( scope::Global& scope ) : Action( name_low, Type::instance, name_construct, name_low,
				DynamicTuple( { type::Attribute( name_other, name_float ) } ), { name_memory_denied } ) {}
			virtual ~LowFloatConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LowHighConstruct : public type::Action
		{
		public:
			inline LowHighConstruct( scope::Global& scope ) : Action( name_low, Type::instance, name_construct, name_low,
				DynamicTuple( { type::Attribute( name_other, name_high ) } ), { name_memory_denied } ) {}
			virtual ~LowHighConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LowIndexConstruct : public type::Action
		{
		public:
			inline LowIndexConstruct( scope::Global& scope ) : Action( name_low, Type::instance, name_construct, name_low,
				DynamicTuple( { type::Attribute( name_other, name_index ) } ), { name_memory_denied } ) {}
			virtual ~LowIndexConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class LowCmp : public type::Action
		{
		public:
			inline LowCmp( scope::Global& scope ) : Action( name_low, opcode::cmp, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_low ) } ) ) {}
			virtual ~LowCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class LowPlus : public type::Action
		{
		public:
			inline LowPlus( scope::Global& scope ) : Action( name_low, opcode::plus, name_low, DynamicTuple(
				{ type::Attribute( name_other, name_low ) } ) ) {}
			virtual ~LowPlus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LowMinus : public type::Action
		{
		public:
			inline LowMinus( scope::Global& scope ) : Action( name_low, opcode::minus, name_low, DynamicTuple(
				{ type::Attribute( name_other, name_low ) } ) ) {}
			virtual ~LowMinus() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LowMultiply : public type::Action
		{
		public:
			inline LowMultiply( scope::Global& scope ) : Action( name_low, opcode::multiply, name_low, DynamicTuple(
				{ type::Attribute( name_other, name_low ) } ) ) {}
			virtual ~LowMultiply() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class LowDivide : public type::Action
		{
		public:
			inline LowDivide( scope::Global& scope ) : Action( name_low, opcode::divide, name_low, DynamicTuple(
				{ type::Attribute( name_other, name_low ) } ) ) {}
			virtual ~LowDivide() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
