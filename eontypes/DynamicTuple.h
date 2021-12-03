#pragma once

#include "BasicTuple.h"



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  DynamicTuple class - eon::DynamicTuple
	**************************************************************************/
	class DynamicTuple : public type::BasicTuple
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		// Construct an empty tuple
		inline DynamicTuple() : type::BasicTuple( name_dynamic ) {}

		// Construct tuple of unnamed attributes based on values only
		inline DynamicTuple( std::initializer_list<Object*> values ) : type::BasicTuple( name_dynamic, values ) {}

		// Construct tuple for the specified attribute elements
		inline DynamicTuple( std::initializer_list<type::Attribute> attributes )
			: type::BasicTuple( name_dynamic, attributes ) {}

		// Copy other tuple
		inline DynamicTuple( const DynamicTuple& other ) : type::BasicTuple( other ) {}

		// Take ownership of other tuple
		inline DynamicTuple( DynamicTuple&& other ) noexcept : type::BasicTuple( std::move( other ) ) {}

		// Destruct tuple
		virtual ~DynamicTuple() = default;



		// Delete 'this' object
		inline void die() override { delete this; }


		// Copy
		inline DynamicTuple& operator=( const DynamicTuple& other ) {
			*static_cast<type::BasicTuple*>( this ) = other; return *this; }

		// Transfer ownership
		inline DynamicTuple& operator=( DynamicTuple&& other ) noexcept {
			*static_cast<type::BasicTuple*>( this ) = std::move( other ); return *this; }


		inline void callDestructor() override { this->~DynamicTuple(); }




		/**********************************************************************
		  Basic Access
		**********************************************************************/
	public:

		//* Get the C++ type
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }

		//* Create a copy of 'this'
		inline type::Object* copy( scope::Scope& scope ) override { return new DynamicTuple( *this ); }
	};
}
