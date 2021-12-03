#pragma once

#include "Bool.h"
#include "Action.h"
#include <eonscopes/Scope.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerBoolActions( scope::Global& scope );


	namespace actions
	{
		/**********************************************************************
		  Eon boolean actions
		**********************************************************************/

		class BoolConstruct : public type::Action
		{
		public:
			inline BoolConstruct() : Action( name_bool, Type::instance, name_construct, name_bool, DynamicTuple() ) {}
			virtual ~BoolConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolCopyConstruct : public type::Action
		{
		public:
			inline BoolCopyConstruct( scope::Global& scope ) : Action( name_bool, Type::instance, name_construct, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_bool ) } ), { name_memory_denied } ) {}
			virtual ~BoolCopyConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class BoolByteConstruct : public type::Action
		{
		public:
			inline BoolByteConstruct( scope::Global& scope ) : Action( name_bool, Type::instance, name_construct, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_byte ) } ), { name_memory_denied } ) {}
			virtual ~BoolByteConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolCharConstruct : public type::Action
		{
		public:
			inline BoolCharConstruct( scope::Global& scope ) : Action( name_bool, Type::instance, name_construct, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_char ) } ), { name_memory_denied } ) {}
			virtual ~BoolCharConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolIntConstruct : public type::Action
		{
		public:
			inline BoolIntConstruct( scope::Global& scope ) : Action( name_bool, Type::instance, name_construct, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_int ) } ),
				{ name_memory_denied } ) {}
			virtual ~BoolIntConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolShortConstruct : public type::Action
		{
		public:
			inline BoolShortConstruct( scope::Global& scope ) : Action( name_bool, Type::instance, name_construct, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_short ) } ), { name_memory_denied } ) {}
			virtual ~BoolShortConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolLongConstruct : public type::Action
		{
		public:
			inline BoolLongConstruct( scope::Global& scope ) : Action( name_bool, Type::instance, name_construct, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_long ) } ), { name_memory_denied } ) {}
			virtual ~BoolLongConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolFloatConstruct : public type::Action
		{
		public:
			inline BoolFloatConstruct( scope::Global& scope ) : Action( name_bool, Type::instance, name_construct, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_float ) } ), { name_memory_denied } ) {}
			virtual ~BoolFloatConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolLowConstruct : public type::Action
		{
		public:
			inline BoolLowConstruct( scope::Global& scope ) : Action( name_bool, Type::instance, name_construct, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_low ) } ), { name_memory_denied } ) {}
			virtual ~BoolLowConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolHighConstruct : public type::Action
		{
		public:
			inline BoolHighConstruct( scope::Global& scope ) : Action( name_bool, Type::instance, name_construct, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_high ) } ), { name_memory_denied } ) {}
			virtual ~BoolHighConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolIndexConstruct : public type::Action
		{
		public:
			inline BoolIndexConstruct( scope::Global& scope ) : Action( name_bool, Type::instance, name_construct, name_bool,
				DynamicTuple( { type::Attribute( name_other, name_index ) } ), { name_memory_denied } ) {}
			virtual ~BoolIndexConstruct() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};

		class BoolCmp : public type::Action
		{
		public:
			inline BoolCmp( scope::Global& scope ) : Action( name_bool, opcode::cmp, name_int, DynamicTuple(
				{ type::Attribute( name_other, name_bool ) } ) ) {}
			virtual ~BoolCmp() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolNot : public type::Action
		{
		public:
			inline BoolNot() : Action( name_bool, opcode::not, name_bool, DynamicTuple() ) {}
			virtual ~BoolNot() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolAnd : public type::Action
		{
		public:
			inline BoolAnd( scope::Global& scope ) : Action( name_bool, opcode::and, name_bool, DynamicTuple(
				{ type::Attribute( name_other, name_bool ) } ) ) {}
			virtual ~BoolAnd() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolOr : public type::Action
		{
		public:
			inline BoolOr( scope::Global& scope ) : Action( name_bool, opcode::or, name_bool, DynamicTuple(
				{ type::Attribute( name_other, name_bool ) } ) ) {}
			virtual ~BoolOr() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
		class BoolXor : public type::Action
		{
		public:
			inline BoolXor( scope::Global& scope ) : Action( name_bool, opcode::xor, name_bool, DynamicTuple(
				{ type::Attribute( name_other, name_bool ) } ) ) {}
			virtual ~BoolXor() = default;
			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
		};
	}
}
