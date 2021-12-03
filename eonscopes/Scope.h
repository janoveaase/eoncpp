#pragma once

#include <eontypes/TypeSystem.h>
#include <typeindex>



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
		//* Exception thrown when trying to add an item to a scope where the
		//* name already exists!
		EONEXCEPT( DuplicateName );

		//* Exception thrown when trying to add an item to a scope were it is
		//* not supported!
		EONEXCEPT( Unsupported );


		/**********************************************************************
		  There are three types of scopes: global, action, and local.
		  
		  In the global scope (also named 'global'), we have groups, types,
		  enums and actions.

		  In action scopes (named for the actions they belong to), we have
		  instances, tuples, dynamic tuples, lambdas.
		  These are stored on the call stack.

		  In local scopes (unnamed), we have instances, tuples, dynamic tuples,
		  expressions, lambdas.
		  These scopes are also organized as a stack within action scopes, but
		  unlike action scopes, if an item does not exist in the current scope,
		  the next scope on the stack will be tried, and so on.
		**********************************************************************/
		class Scope
		{
		public:

			//* Add an item to this scope
			//* Throws DuplicateName if already existing!
			//* Throws Unsupported if scope does not support the item type!
			virtual void add( name_t name, type::Object* item ) = 0;

			//* Get an item, from this cope only!
			//* Returns nullptr if not found!
			virtual type::Object* get( name_t name ) const noexcept = 0;

			//* Find an item, search all accessible scopes!
			//* Returns nullptr if not found!
			virtual type::Object* find( name_t name ) const noexcept = 0;
		};

		class Global : public Scope
		{
		public:

			void add( name_t name, type::Object* item ) override;
			inline type::Object* get( name_t name ) const noexcept override {
				auto found = Items.find( name ); return found != Items.end() ? found->second : nullptr; }
			inline type::Object* find( name_t name ) const noexcept override { return get( name ); }

			//* Get a definition based on raw C++ type
			//* Returns nullptr if not found!
			template<typename T>
			type::Definition* definition() const noexcept {
				auto found = Definitions.find( std::type_index( typeid( T ) ) );
				return found != Definitions.end() ? found->second : nullptr; }

			//* Get all actions with the specified name.
			//* Returns nullptr if none!
			inline const std::vector<type::Action*>* actions( name_t name ) const noexcept {
				auto found = Actions.find( name ); return found != Actions.end() ? &found->second : nullptr; }

		private:

			// These are the items of this scope
			std::unordered_map<name_t, type::Object*> Items;

			// Definitions are a subset of Items
			std::unordered_map<std::type_index, type::Definition*> Definitions;

			// Actions, by their name only, a subset of Items
			std::unordered_map<name_t, std::vector<type::Action*>> Actions;
		};

		class _Local : public Scope
		{
		public:
			_Local() = delete;
			inline _Local( Global& global ) { Glob = &global; }

			inline Global& global() noexcept { return *Glob; }
			inline const Global& global() const noexcept { return *Glob; }

			inline type::Object* get( name_t name ) const noexcept override {
				auto found = Items.find( name ); return found != Items.end() ? found->second : nullptr; }
			void add( name_t name, type::Object* item ) override;

			virtual bool isAction() const noexcept { return false; }

		private:
			std::unordered_map<name_t, type::Object*> Items;
			Global* Glob{ nullptr };
		};

		class Action : public _Local
		{
		public:
			Action() = delete;
			Action( Global& global, name_t name, Action* previous = nullptr ) : _Local( global ) {
				Name = name; Prev = previous; }

			inline name_t name() const noexcept { return Name; }

			inline Action* previous() noexcept { return Prev; }
			inline const Action* previous() const noexcept { return Prev; }

			inline type::Object* find( name_t name ) const noexcept override {
				auto found = get( name ); return found ? found : global().get( name ); }

			inline bool isAction() const noexcept override { return true; }

		private:
			name_t Name{ no_name };
			Action* Prev{ nullptr };
		};

		class Local : public _Local
		{
		public:
			Local() = delete;
			Local( Global& global, _Local& parent ) : _Local( global ) { Parent = &parent; }

			_Local* parent() noexcept { return Parent; }
			
			inline type::Object* find( name_t name ) const noexcept override {
				auto found = get( name ); return found ? found : Parent ? Parent->find( name ) : global().get( name ); }

		private:
			_Local* Parent{ nullptr };
		};
	}
}
