#pragma once

#include <typeindex>
#include "BasicTuple.h"
#include "Reference.h"
#include "Tuple.h"
#include "Operators.h"



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace type
	{
		class Object;
		class Definition;
		class Action;
	}




	/**************************************************************************
	  The 'eon::scope' namespace encloses all hidden scopes functionality
	**************************************************************************/
	namespace scope
	{
		/**********************************************************************
		  There are three types of scopes: global, action, and local.
		  
		  In the (single) global scope (also named 'global'), we have caches,
		  groups, types, enums and actions.

		  In action scopes (named for the actions they belong to), we have
		  instances, tuples (all types), lambdas.
		  These scopes are stored on the call stack.

		  In local scopes (unnamed), we have instances, tuples (all types),
		  lambdas.
		  These scopes are also organized as a stack within action scopes, but
		  unlike action scopes, if an item does not exist in the current scope,
		  the next scope on the stack will be tried, and so on.

		  Note that all scopes are BasicTuple objects at heart!
		**********************************************************************/
		class Global;

		class Scope : public type::BasicTuple
		{
		public:
			Scope( name_t type, source::Ref source ) : BasicTuple( type, source ) {}

			//* Direct access to the global scope
			virtual Global& global() = 0;
			virtual const Global& global() const = 0;

			virtual Object* find( name_t name ) const noexcept { return exists( name ) ? at( name ).value() : nullptr; }

			inline name_t generalType() const noexcept override { return name_scope; }
			inline type::Object* copy() override { throw type::AccessDenied( "Cannot copy scope object!" ); }

			virtual void formalPrefix( Stringifier& str ) const override {}
			virtual void formalPostfix( Stringifier& str ) const override {}
		};

		class Global : public Scope
		{
		public:
			Global() : Scope( name_global, source::Ref() ) {}
			~Global();

			inline Global& global() override { return *this; }
			inline const Global& global() const override { return *this; }

			//* Add an action
			inline void addAction( name_t name, type::Action* action ) { Actions[ name ].push_back( action ); }

			//* Add an operator action
			inline void addOperator( type::operators::code op, type::Action* action ) {
				Actions[ compilerName( "$op_" + type::operators::mapCode( op ) ) ].push_back( action ); }

			//* Get all actions matching specified details
			std::list<type::Action*> getActions( name_t name, const EonType& inst_type, const EonType& args,
				const EonType& return_type ) const noexcept;
			std::list<type::Action*> getActions( name_t name, const EonType& inst_type, const EonType& args )
				const noexcept;

		private:
			std::unordered_map<name_t, std::list<type::Action*>> Actions;
		};

		class _Local : public Scope
		{
		public:
			_Local() = delete;
			inline _Local( Global& global, source::Ref source ) : Scope( name_local, source ) {
				*this += type::Attribute( new Modifiable( &global ) ); }

			const Global& global() const override { return *(Global*)at( name_global ).value(); }
			Global& global() override { return *(Global*)at( name_global ).value(); }

			virtual bool isAction() const noexcept { return false; }
		};

		class Action : public _Local
		{
		public:
			Action() = delete;
			Action( Global& global, type::TypeDef& obj, Tuple& args, name_t name, Action* previous = nullptr )
				: _Local( global, obj.source() ) { add( name_me, new Modifiable( &obj ) ); add( name_args,
					new Modifiable( &args ) ); Name = name; Prev = previous; }
			Action( Global& global, type::Instance& obj, Tuple& args, name_t name, Action* previous = nullptr )
				: _Local( global, obj.source() ) { add( name_me, new Modifiable( &obj ) ); add( name_args,
					new Modifiable( &args ) ); Name = name; Prev = previous; }

			inline name_t name() const noexcept { return Name; }

			inline Action* previous() noexcept { return Prev; }
			inline const Action* previous() const noexcept { return Prev; }

			inline type::Object* find( name_t name ) const noexcept override {
				auto found = Scope::find( name ); return found ? found : global().find( name ); }

			inline bool isAction() const noexcept override { return true; }

		private:
			name_t Name{ no_name };
			Action* Prev{ nullptr };
		};

		class Local : public _Local
		{
		public:
			Local() = delete;
			Local( Global& global, _Local& parent, source::Ref source ) : _Local( global, source ) { Parent = &parent; }

			_Local* parent() noexcept { return Parent; }
			
			inline type::Object* find( name_t name ) const noexcept override {
				auto found = Scope::find( name ); return found ? found : Parent ? Parent->find( name )
					: global().find( name ); }

		private:
			_Local* Parent{ nullptr };
		};
	}
}
