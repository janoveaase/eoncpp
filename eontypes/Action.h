#pragma once
#include "TypeSystem.h"
#include "DynamicTuple.h"
#include "Operators.h"
#include "Node.h"
#include "Name.h"
#include <eonscopes/Scope.h>


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

			// Construct action
			Action( const TypeTuple& type, type::operators::code op_code, const TypeTuple& return_type,
				DynamicTuple arguments, std::initializer_list<name_t> raises = {} );
			Action( const TypeTuple& type, type::operators::code op_code, name_t return_type,
				DynamicTuple arguments, std::initializer_list<name_t> raises = {} );
			Action( name_t type, type::operators::code op_code, const TypeTuple& return_type,
				DynamicTuple arguments, std::initializer_list<name_t> raises = {} );
			Action( name_t type, type::operators::code op_code, name_t return_type,
				DynamicTuple arguments, std::initializer_list<name_t> raises = {} );

			Action( const TypeTuple& type, actions::Type action_type, name_t name, const TypeTuple& return_type,
				DynamicTuple arguments, std::initializer_list<name_t> raises = {} );
			Action( const TypeTuple& type, actions::Type action_type, name_t name, name_t return_type,
				DynamicTuple arguments, std::initializer_list<name_t> raises = {} );
			Action( name_t type, actions::Type action_type, name_t name, const TypeTuple& return_type,
				DynamicTuple arguments, std::initializer_list<name_t> raises = {} );
			Action( name_t type, actions::Type action_type, name_t name, name_t return_type,
				DynamicTuple arguments, std::initializer_list<name_t> raises = {} );
			// TODO: Add support for tests!

			virtual ~Action() = default;


			// Not required for actions
			void callDestructor() {}
			inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }

			virtual type::Object* copy( scope::Scope& scope ) override {
				throw AccessDenied( "Cannot copy action object!" ); }
			virtual void str( type::Stringifier& str ) const override {
				str.addWord( Name ); str.addRaw( ":" ); str.newLine(); }
			inline name_t generalType() const noexcept override { return name_action; }
			inline actions::Type actionType() const noexcept { return ActionType; }

			// Get the name of the action
			inline name_t name() const noexcept { return Name; }

			// Get return type
			inline const TypeTuple& returnType() const noexcept { return ReturnType; }

			// Get arguments
			inline const DynamicTuple& arguments() const noexcept { return Arguments; }

			inline type::operators::code opCode() const noexcept { return OpCode; }

			inline type::Precedence inputPrecedence() const noexcept { return InputPrecedence; }
			inline type::Precedence stackPrecedence() const noexcept { return StackPrecedence; }

			inline bool rightToLeft() const noexcept { return InputPrecedence > StackPrecedence; }
			inline bool leftToRight() const noexcept { return InputPrecedence < StackPrecedence; }

			//* Execute the action
			virtual Object* execute( scope::Scope& scope, Node& action_node ) = 0;

		protected:
			//* Get operand as specified type
			//* Throws [eon::WrongType] if not the specified type
			template<typename T>
			T* _operand( scope::Scope& scope, Node& action_node, size_t arg_no )
			{
				auto& child = action_node.child( arg_no );
				Object* value{ nullptr };
				if( child.isAction() )
					value = child.action().execute( scope, child );
				else if( child.isValue() )
					value = child.value();
				else
					value = ( (NameType*)scope.find( name_name ) )->instantiate( child.name() );

				if( typeid( T ) != typeid( value ) )
					throw WrongType();
				return (T*)value;
			}




			/**********************************************************************
			  Helpers
			**********************************************************************/
		private:

			name_t _makeSignature( actions::Type type, name_t name, name_t returntype, const DynamicTuple& arguments ) const;

			void _create( const TypeTuple& type, type::operators::code op_code, const TypeTuple& return_type, DynamicTuple arguments,
				std::initializer_list<name_t> raises );
			void _create( const TypeTuple& type, actions::Type action_type, name_t name, const TypeTuple& return_type,
				DynamicTuple arguments, std::initializer_list<name_t> raises );

			void _initPrefixFirstAction();
			void _initPrefixLastAction();




			/**********************************************************************
			  Attributes
			**********************************************************************/
		protected:

			TypeTuple InstanceType;
			actions::Type ActionType{ actions::Type::undef };
			name_t Name{ no_name };
			TypeTuple ReturnType;
			DynamicTuple Arguments;
			std::unordered_set<name_t> Raises;
			type::operators::code OpCode{ type::operators::code::undef };
			type::Precedence InputPrecedence{ type::Precedence::none };
			type::Precedence StackPrecedence{ type::Precedence::none };
		};
	}
}
