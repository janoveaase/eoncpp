#pragma once

#include "TypeDefinitions.h"
#include "TypeTuple.h"
#include "Stringifier.h"



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace scope
	{
		class Scope;
	}




	/**************************************************************************
	  The 'eon::type' namespace encloses all hidden type functionality
	**************************************************************************/
	namespace type
	{
		//* Base class for all objects of all types
		class Object
		{
		public:
			Object() = delete;
			inline Object( name_t type ) { Type = type; }
			inline Object( TypeTuple type ) { Type = type; }

			//* Get the type name
			inline const TypeTuple& type() const noexcept { return Type; }

			//* Get the C++ type
			virtual std::type_index rawType() const noexcept = 0;

			//* General type of object (instance, typedef, action, tuple, etc.)
			virtual name_t generalType() const noexcept = 0;

			//* Objects that should be deleted upon exiting scope should call
			//* "delete this" in the implementation of this method. Those
			//* that are mere references should not.
			virtual void die() = 0;

			//* This method must be overridden to call the object's own
			//* destructor! For example, if the derived object type is "Item",
			//* then "callDestructor" should call "this->~Item();".
			virtual void callDestructor() = 0;

			//* Create a copy of 'this'
			virtual Object* copy( scope::Scope& scope ) = 0;

			//* Create a new object that consumes 'this'
			//* For primitives, this is the same as copy
			virtual Object* consume( scope::Scope& scope ) { return copy( scope ); }

			//* Get object as string representation
			virtual void str( Stringifier& str ) const = 0;


		protected:
			TypeTuple Type;
		};




		class Instance;
		class Node;

		//* Base class for definition objects
		class Definition : public Object
		{
		public:
			Definition() = delete;
			inline Definition( name_t name ) noexcept : Object( name ) {}

			//* Identify this as a 'definition'
			virtual name_t generalType() const noexcept override { return name_definition; }

			//* Create a new instance of this type
			//* If 'memory_start' is nullptr, then create in heap memory,
			//* otherwise create at the given memory position!
			virtual Instance* instantiate( Node* args = nullptr ) const = 0;
		};




		//* Base class for instance objects - created by calling "instantiate"
		//* method of Definition derived objects.
		class Instance : public Object
		{
		public:
			Instance() = delete;
			inline Instance( name_t type ) : Object( type ) {}

			//* Identify this as an 'instance'
			inline name_t generalType() const noexcept override { return name_instance; }

			//* Get a full copy of the instance
			virtual Instance* copy() const = 0;

			//* Access the raw value
			virtual void* rawValue() const noexcept = 0;

			//* Compare with another instance
			virtual int compare( const Instance& other ) const noexcept = 0;
		};




		class Action;

		//* Dynamically growable TypeID*->Action* cache
		class DynamicActionMap
		{
		public:
			DynamicActionMap() = default;
			~DynamicActionMap();

			inline void add( name_t signature, Action* action ) { ensureFree( signature ); Actions[ signature ] = action; }
			void ensureFree( name_t signature ) noexcept;

			inline const Action* at( name_t signature ) const noexcept {
				auto found = Actions.find( signature ); return found != Actions.end() ? found->second : nullptr; }
			inline Action* at( name_t signature ) noexcept {
				auto found = Actions.find( signature ); return found != Actions.end() ? found->second : nullptr; }

		private:
			std::map<name_t, Action*> Actions;
		};




		//* Base class for type definitions
		//* These can have actions!
		class TypeDef : public Definition
		{
		public:
			TypeDef() = delete;
			inline TypeDef( name_t name ) : Definition( name ) {};

			//* Identify this as an 'instance'
			inline name_t generalType() const noexcept override { return name_type; }

			// Add an action
			inline void addAction( Action* action ) {}

			// Access the actions
			DynamicActionMap& actions() noexcept { return Actions; }

		private:
			DynamicActionMap Actions;
		};




/*		//* Base class for type definitions involving templates
		class TemplateTypeDef : public TypeDef
		{
		public:
			TemplateTypeDef() = delete;
			inline TemplateTypeDef( name_t name, TypeID* template_types ) : TypeDef( name ) {
				TemplateTypes = template_types; }
			virtual ~TemplateTypeDef() { if( TemplateTypes ) delete TemplateTypes; }

			// Get template types for the type - if any
			inline const TypeID* templateTypes() const noexcept { return TemplateTypes; }

		private:
			TypeID* TemplateTypes{ nullptr };
		};*/




		//* Base class for enum objects
		class Enum : public Definition
		{
		public:
			Enum() = delete;
			inline Enum( name_t name, uint8_t unit_size ) : Definition( name ) {
				UnitSize = unit_size; }

			//* Identify this as an 'instance'
			inline name_t generalType() const noexcept override { return name_enum; }

			// Get number of bytes required to store an enum value
			inline uint8_t unitSize() const noexcept { return UnitSize; }

		private:
			uint8_t UnitSize{ sizeof( int ) };
		};




		//* Wrapper for type tuple
		class TypeTupleObject : public Object
		{
		public:
			TypeTupleObject() : Object( name_typetuple ) {}
			TypeTupleObject( TypeTuple&& value ) : Object( name_typetuple ) { Value = std::move( value ); }
			virtual ~TypeTupleObject() = default;

			//* Get the C++ type
			inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }

			//* General type of object (instance, typedef, action, tuple, etc.)
			inline name_t generalType() const noexcept { return name_typetuple; }

			//* Objects that should be deleted upon exiting scope should call
			//* "delete this" in the implementation of this method. Those
			//* that are mere references should not.
			void die() override {}

			//* This method must be overridden to call the object's own
			//* destructor! For example, if the derived object type is "Item",
			//* then "callDestructor" should call "this->~Item();".
			inline void callDestructor() override { this->~TypeTupleObject(); }

			//* Create a copy of 'this'
			Object* copy( scope::Scope& scope ) override { return new TypeTupleObject( TypeTuple( Value ) ); }

			//* Create a new object that consumes 'this'
			//* For primitives, this is the same as copy
			inline Object* consume( scope::Scope& scope ) override { return new TypeTupleObject( std::move( Value ) ); }

			//* Get object as string representation
			inline void str( Stringifier& str ) const override { return Value.str( str ); }

		private:
			TypeTuple Value;
		};
	}
}
