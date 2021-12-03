#pragma once

#include "BasicTuple.h"



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  DataTuple class - eon::DataTuple

	  Data tuples can store the following types (only):
		bool, byte, char, long, high, name,
		string, bytes, regex, namepath, path,
		metadata, array, set, data
	
	  int and short will be automatically converted into long, and float and
	  low into high when added to the data tuple.
	**************************************************************************/
	class DataTuple : public type::BasicTuple
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		// Construct an empty tuple
		inline DataTuple() : type::BasicTuple( name_data ) {}

		// Construct tuple of unnamed attributes based on values only
		inline DataTuple( std::initializer_list<Object*> values ) : type::BasicTuple( name_data, values ) {}

		// Construct tuple for the specified attribute elements
		inline DataTuple( std::initializer_list<type::Attribute> attributes )
			: type::BasicTuple( name_data, attributes ) {}

		// Copy other tuple
		inline DataTuple( const DataTuple& other ) : type::BasicTuple( other ) {}

		// Take ownership of other tuple
		inline DataTuple( DataTuple&& other ) noexcept : type::BasicTuple( std::move( other ) ) {}
		inline DataTuple( type::BasicTuple&& other ) : type::BasicTuple( name_data, std::move( other ) ) { finalize(); }

		// Destruct tuple
		virtual ~DataTuple() = default;



		// Delete 'this' object
		inline void die() override { delete this; }


		// Copy
		inline DataTuple& operator=( const DataTuple& other ) {
			*static_cast<type::BasicTuple*>( this ) = other; return *this; }

		// Transfer ownership
		inline DataTuple& operator=( DataTuple&& other ) noexcept {
			*static_cast<type::BasicTuple*>( this ) = std::move( other ); return *this; }


		inline void callDestructor() override { this->~DataTuple(); }




		/**********************************************************************
		  Basic Access
		**********************************************************************/
	public:

		//* Get the C++ type
		inline std::type_index rawType() const noexcept override { return std::type_index( typeid( *this ) ); }

		//* Create a copy of 'this'
		inline type::Object* copy( scope::Scope& scope ) override { return new DataTuple( *this ); }
	};
}
