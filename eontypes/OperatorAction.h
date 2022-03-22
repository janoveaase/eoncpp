#pragma once

#include "String.h"
#include "Action.h"
#include "Scope.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace actions
	{
		/**********************************************************************
		  Eon operator action
		**********************************************************************/
		class OperatorAction : public type::Action
		{
		public:
			OperatorAction( const EonType& instance_type, type::operators::code op_code, const EonType& return_type,
				DynamicTuple arguments, std::initializer_list<name_t> raises, source::Ref source );

			OperatorAction( type::operators::code op_code, index_t num_args, source::Ref source );
			virtual ~OperatorAction() = default;


			void die() override {}
			Object* execute( scope::Scope& scope, type::Node& action_node ) override;
			inline index_t numArguments() const noexcept override { return NumArgs; }
			inline index_t numArgsWithoutDefaultValue() const noexcept override { return NumArgs; }

			inline type::operators::code opCode() const noexcept { return OpCode; }

			inline type::Precedence inputPrecedence() const noexcept override {
				return type::operators::inputPrecedence( OpCode ); }
			inline type::Precedence stackPrecedence() const noexcept override {
				return type::operators::stackPrecedence( OpCode ); }

			inline bool rightToLeft() const noexcept { return inputPrecedence() > stackPrecedence(); }
			inline bool leftToRight() const noexcept { return inputPrecedence() < stackPrecedence(); }

			static const string NamePrefix;

		private:
			type::operators::code OpCode{ type::operators::code::undef };
			index_t NumArgs{ 0 };
		};
	}
}
