#pragma once

#include <eonname/Name.h>



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Eon primitives (not defined elsewhere)
	using int_t = long int;
	using pos_t = size_t;
	using flt_t = double;
	using flt32_t = float;
	using flt64_t = double;
	using flt128_t = long double;
	using handle_t = void*;




	/**************************************************************************
	  The 'eon::tup' namespace encloses all hidden tuple functionality
	**************************************************************************/
	namespace tup
	{
		// Base class for all objects of all types
		class Object
		{
		public:
			Object() = default;
			Object( bool heap_memory ) { HeapMemory = heap_memory; }

			// Method for self-deleting (from heap memory)
			virtual void selfDelete() = 0;


			// This method must be overridden to call the object's own
			// destructor! For example, if the derived object type is "String",
			// then "callDestructor" should call "this->~String();".
			virtual void callDestructor() = 0;

			// Get object as string
			virtual string str() const = 0;

			// Get number of bytes this object - or its instances - will occupy in stack memory
			virtual pos_t stackSize() const noexcept = 0;

		protected:
			bool HeapMemory{ false };
		};


		class Definition;
		class Instance;


		// Dynamically growable name_t->Object cache, objects existing in heap
		// memory only!
		class DynamicObjectMap
		{
		public:
			DynamicObjectMap() = default;
			~DynamicObjectMap() { for( auto& i : ObjectMap ) i.second->selfDelete(); }

			inline void add( name_t name, Object* object ) { ensureFree( name ); ObjectMap[ name ] = object; }
			inline void ensureFree( name_t name ) noexcept { auto found = ObjectMap.find( name );
				if( found != ObjectMap.end() ) { found->second->selfDelete(); ObjectMap.erase( found ); } }

			inline const Object* at( name_t name ) const noexcept {
				auto found = ObjectMap.find( name ); return found != ObjectMap.end() ? found->second : nullptr; }
			inline Object* at( name_t name ) noexcept {
				auto found = ObjectMap.find( name ); return found != ObjectMap.end() ? found->second : nullptr; }

		private:
			std::unordered_map<name_t, Object*> ObjectMap;
		};


		// Main tuple type
		class Tuple : public Object
		{
		public:
			struct Attribute
			{
				Attribute() = default;
				inline Attribute( const Definition* type ) noexcept { Type = type; }
				inline Attribute( const Definition* type, Instance* value ) noexcept { Type = type; Value = value; }
				inline Attribute( name_t name, const Definition* type ) noexcept { Name = name; Type = type; }
				inline Attribute( name_t name, const Definition* type, Instance* value ) noexcept {
					Name = name; Type = type; Value = value; }

				name_t Name{ no_name };
				const Definition* Type{ nullptr };
				Instance* Value{ nullptr };
			};

			// Construct an empty tuple
			Tuple() = default;

			// Construct a tuple at specified (stack) memory, or if nullptr, in
			// heap memory!
			Tuple( std::vector<Attribute>& attributes, byte_t* memory_start = nullptr );

			// Copy construct at specified (stack) memory, or if nullptr, in
			// heap memory!
			Tuple( const Tuple& other, byte_t* memory_start = nullptr );
			inline Tuple( Tuple&& other ) noexcept { *this = std::move( other ); }

			// Destruct tuple
			// Will call destructor of attributes, and, if allocated in heap
			// memory, delete that memory.
			virtual ~Tuple();

			// Delete 'this' object
			virtual void selfDelete() override { delete this; }


			// Copy
			inline Tuple& operator=( Tuple&& other ) noexcept {
				Memory = other.Memory; other.Memory = nullptr; Attributes = std::move( other.Attributes );
				NamedAttribs = std::move( other.NamedAttribs ); return *this; }


			// Access attributes count
			inline bool empty() const noexcept { return Attributes.empty(); }
			inline pos_t numAttributes() const noexcept { return Attributes.size(); }

			// Access attribute at specified position
			// Returns nullptr if invalid position
			inline Instance* attribute( pos_t pos ) const noexcept {
				return pos < Attributes.size() ? Attributes[ pos ] : nullptr; }

			// Access an attribute by name
			// Returns nullptr if no attribute with that name
			inline Instance* attribute( name_t name ) const noexcept {
				auto a = NamedAttribs.find( name ); return a != NamedAttribs.end() ? a->second.second : nullptr; }

			// Compare with another tuple
			int compare( const Tuple& other ) const noexcept;


			virtual void callDestructor() override { this->~Tuple(); }
			virtual string str() const override;
			virtual pos_t stackSize() const noexcept override;

		private:
			byte_t* Memory{ nullptr };
			std::vector<Instance*> Attributes;
			std::unordered_map<name_t, std::pair<pos_t, Instance*>> NamedAttribs;
		};


		// Base class for defintion objects
		class Definition : public Object
		{
		public:
			Definition() = delete;
			inline Definition( name_t name, bool heap_memory ) noexcept : Object( heap_memory ) { Name = name; }

			// Delete 'this' object
			virtual void selfDelete() override { delete this; }

			// Get the name of the definition
			inline name_t name() const noexcept { return Name; }

			// Create a new instance of this definition
			// If 'memory_start' is nullptr, then create in heap memory,
			// otherwise create at the given memory position!
			virtual Instance* instantiate( const Instance* default_value, byte_t* memory_start = nullptr ) const = 0;

			// Compare with other type
			virtual int compare( const Definition* other ) const noexcept {
				return Name < other->Name ? -1 : other->Name < Name ? 1 : 0; };

		private:
			name_t Name{ no_name };				// The type name
		};


		class Action;

		// Base class for type definitions
		class TypeDef : public Definition
		{
		public:
			TypeDef() = delete;
			inline TypeDef( name_t name, bool heap_memory ) : Definition( name, heap_memory ) {};

			// Delete 'this' object
			virtual void selfDelete() override { delete this; }

			// Add an action
			inline void addAction( Action* action ) {}

			// Access the actions
			DynamicObjectMap& actions() noexcept { return Actions; }

		private:
			DynamicObjectMap Actions;
		};


		// Base class for type definitions involving templates
		class TemplateTypeDef : public TypeDef
		{
		public:
			TemplateTypeDef() = delete;
			inline TemplateTypeDef( name_t name, const Tuple* template_types, bool heap_memory )
				: TypeDef( name, heap_memory ) { TemplateTypes = template_types; }

			// Delete 'this' object
			virtual void selfDelete() override { delete this; }

			// Compare with other type
			virtual int compare( const Definition* other ) const noexcept {
				auto cmp = Definition::compare( other ); if( cmp != 0 ) return cmp;
				return TemplateTypes->compare( *dynamic_cast<const TemplateTypeDef*>( other )->TemplateTypes ); }

			// Get template types for the type - if any
			inline const Tuple* templateTypes() const noexcept { return TemplateTypes; }

		private:
			const Tuple* TemplateTypes{ nullptr };
		};


		// Base class for enum objects
		class Enum : public Definition
		{
		public:
			Enum() = delete;
			inline Enum( name_t name, uint8_t unit_size, bool heap_memory ) : Definition( name, heap_memory ) {
				UnitSize = unit_size; }

			// Delete 'this' object
			virtual void selfDelete() override { delete this; }

			// Get number of bytes required to store an enum value
			inline uint8_t unitSize() const noexcept { return UnitSize; }

		private:
			uint8_t UnitSize{ sizeof( int ) };
		};


		// Base class for instance objects - created by calling "instantiate"
		// method of Definition derived objects.
		class Instance : public Object
		{
		public:
			Instance() = delete;
			inline Instance( const Definition* type, bool heap_memory ) : Object( heap_memory ) { Type = type; }

			// Delete 'this' object
			virtual void selfDelete() override { delete this; }

			// This method must be overridden to call the object's own
			// destructor! For example, if the derived object type is "String",
			// then "callDestructor" should call "this->~String();".
			virtual void callDestructor() override;

			// Get object as string
			virtual string str() const override;

			// Get number of bytes this object - or its instances - will occupy in stack memory
			virtual pos_t stackSize() const noexcept override;

			// Get the definition type object for this instance
			inline const Definition* type() const noexcept { return Type; }

			// Get a full copy of the instance
			virtual Instance* copy() const = 0;

			// Compare with another instance
			virtual int compare( const Instance& other ) const noexcept { return Type->compare( other.Type ); }


		protected:
			const Definition* Type{ nullptr };
		};
	}
}
