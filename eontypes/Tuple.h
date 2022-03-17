#pragma once

#include "BasicTuple.h"



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Main Tuple Type - eon::Tuple

	  A meta data object is a tuple for which all attributes must be set at
	  instantiation, and their number, types and names cannot change after.
	**************************************************************************/
	class Tuple : public type::BasicTuple
	{
		/**********************************************************************
		  Tuple Construction
		**********************************************************************/
	public:

		// Construct an empty tuple
		inline Tuple() : type::BasicTuple( name_plain, source::Ref() ) {}

		// Construct tuple of unnamed attributes based on values only
		inline Tuple( std::initializer_list<type::Object*> values, source::Ref source )
			: type::BasicTuple( name_plain, values, source ) {}

		// Construct tuple for the specified attribute elements
		inline Tuple( std::initializer_list<type::Attribute> attributes, source::Ref source )
			: type::BasicTuple( name_plain, attributes, source ) {}

		// Copy other tuple
		inline Tuple( const Tuple& other ) : type::BasicTuple( other ) {}

		// Take ownership of other tuple
		inline Tuple( Tuple&& other ) noexcept : type::BasicTuple( std::move( other ) ) {}
		inline Tuple( type::BasicTuple&& other ) noexcept : type::BasicTuple( std::move( other ) ) {}

		// Destruct tuple
		virtual ~Tuple() = default;



		// Delete 'this' object
		virtual void die() override { delete this; }


		// Copy
		inline type::Object* copy() override { return new Tuple( *this ); }
		inline Tuple& operator=( const Tuple& other ) noexcept {
			*static_cast<type::BasicTuple*>( this ) = other; return *this; }

		// Transfer ownership
		inline Tuple& operator=( Tuple&& other ) noexcept {
			*static_cast<type::BasicTuple*>( this ) = std::move( other ); return *this; }


		virtual void callDestructor() override { this->~Tuple(); }




		/**********************************************************************
		  Basic Access
		**********************************************************************/
	public:

		static inline name_t tupleType() noexcept { return name_plain; }
	};
}
