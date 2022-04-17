#pragma once

#include "TypeSystem.h"
#include "Scope.h"
#include <eonstring/Stringifier.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Register type and actions in the global scope
	void registerTypeTupleObj( scope::Global& scope );


	/**************************************************************************
	 Eon Type Tuple object
	**************************************************************************/
	class TypeTupleObj : public type::Object
	{
	public:
		TypeTupleObj() : Object( name_typetuple, source::Ref() ) {}
		TypeTupleObj( const EonType& value, source::Ref source ) : Object( name_typetuple, source ) { Value = value; }
		TypeTupleObj( const EonType& value ) : Object( name_typetuple, source::Ref() ) { Value = value; }
		~TypeTupleObj() = default;



		//* Get the C++ type
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }

		//* General type of object (instance, typedef, action, tuple, etc.)
		inline name_t generalType() const noexcept override { return name_typetuple; }

		//* Objects that should be deleted upon exiting scope should call
		//* "delete this" in the implementation of this method. Those
		//* that are mere references should not.
		void die() override {}

		//* This method must be overridden to call the object's own
		//* destructor! For example, if the derived object type is "Item",
		//* then "callDestructor" should call "this->~Item();".
		void callDestructor() override {}

		//* Create a copy of 'this'
		inline Object* copy() override { return new TypeTupleObj( Value, source() ); }

		//* Get object as string representation
		inline void str( Stringifier& str ) const { Value.str( str ); }

	private:
		EonType Value;
	};
}
