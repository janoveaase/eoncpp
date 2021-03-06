#pragma once
#include "TypeSystem.h"
#include "DynamicTuple.h"
#include "Operators.h"
#include "Name.h"
#include "Scope.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	// General exception when trying to divide by zero
	EONEXCEPT( DivisionByZero );

	// Shortening the operator code reference
	using opcode = type::operators::code;




	/**************************************************************************
	  The 'eon::actions' namespace encloses all hidden actions functionality
	**************************************************************************/
	namespace actions
	{
		// Action types
		enum class Type
		{
			undef,
			instance,
			prefix_first,
			prefix_last,
			get,
			set,
			_operator,
			helper,
			type
		};
		string mapType( Type type );
	}




	namespace type
	{
		/**********************************************************************
		  Action Class - eon::type::Action

		  Base class for action objects
		**********************************************************************/
		class Action : public type::Object
		{
		public:
			Action() = delete;

			Action( const EonType& instance_type, actions::Type action_type, name_t name, const EonType& return_type,
				DynamicTuple arguments, std::initializer_list<name_t> raises, source::Ref source = source::Ref() );
			// TODO: Add support for tests!

			virtual ~Action() = default;


			// Not required for actions
			void callDestructor() override {}
			inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }

			virtual type::Object* copy() override { throw AccessDenied( "Cannot copy action object!" ); }
			virtual void str( Stringifier& str ) const override { str.pushWord( eon::str( Name ) ); }
			inline name_t generalType() const noexcept override { return name_action; }
			inline const EonType& instanceType() const noexcept { return InstanceType; }
			inline actions::Type actionType() const noexcept { return ActionType; }

			virtual type::Precedence inputPrecedence() const noexcept {
				return operators::inputPrecedence( operators::code::call ); }
			virtual type::Precedence stackPrecedence() const noexcept {
				return operators::stackPrecedence( operators::code::call ); }

			// Get the name of the action
			inline name_t name() const noexcept { return Name; }

			// Get return type
			inline const EonType& returnType() const noexcept { return ReturnType; }

			// Get arguments
			inline const DynamicTuple& arguments() const noexcept { return Arguments; }

			// Get number of arguments
			virtual index_t numArguments() const noexcept { return Arguments.numAttributes(); }

			// Get number of arguments without default values
			virtual index_t numArgsWithoutDefaultValue() const noexcept;

			//* Execute the action
			virtual Object* execute( scope::Scope& scope, Node& action_node ) = 0;

		protected:
			//* Get operand as specified type
			//* Throws [eon::WrongType] if not the specified type
			template<typename T>
			T* _operand( scope::Scope& scope, Node& action_node, size_t arg_no )
			{
				auto value = _childValue( scope, action_node, arg_no );

				if( typeid( T ) != typeid( value ) )
					throw WrongType();
				return (T*)value;
			}




			/**********************************************************************
			  Helpers
			**********************************************************************/
		private:

			static EonType _generateType( name_t name, const EonType& returntype, const DynamicTuple& arguments );

			void _create( const EonType& instance_type, actions::Type action_type, name_t name, const EonType& return_type,
				DynamicTuple arguments, std::initializer_list<name_t> raises );

			void _initPrefixFirstAction();
			void _initPrefixLastAction();

			Object* _childValue( scope::Scope& scope, Node& action_node, size_t arg_no );




			/**********************************************************************
			  Attributes
			**********************************************************************/
		protected:

			EonType InstanceType;
			actions::Type ActionType{ actions::Type::undef };
			name_t Name{ no_name };
			EonType ReturnType;
			DynamicTuple Arguments;
			std::unordered_set<name_t> Raises;
		};
	}
}
