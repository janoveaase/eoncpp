#pragma once

#include "Tuple.h"



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// The 'eon::scope' namespace encloses scope factory functions
	//
	// Note that scopes are a special type of tuples!
	//
	namespace scope
	{
		// Create/access global scope
		// First time creates, all calls return the same tuple!
		Tuple& global();

		// Create/access a package scope
		// One package scope per package, first time creates, all calls return the same tuple!
		Tuple& package( name_t package_name );

		// Create a new normal scope
		Tuple normal( Tuple& parent_scope, std::initializer_list<AttributePair> attributes );
	}




	///////////////////////////////////////////////////////////////////////////
	//
	// The 'eon::cache' namespace encloses cache factory functions
	//
	// Note that caches are a special type of tuples!
	// Caches are created and can only exist in the global scope!
	//
	namespace cache
	{
		// Create a cache with specified permissions
		// Throws:
		//   - eon::type::DuplicateName : Another cache with the same name already exists in the global scope
		Tuple& cache( name_t name, TuplePerm permissions );

		// Create a cache with specified (initial) attributes and specified permissions
		// Throws:
		//   - eon::type::DuplicateName : Another cache with the same name already exists in the global scope
		Tuple& cache( name_t name, std::initializer_list<AttributePair> attributes, TuplePerm permissions );

		// Access cache with the specified name
		// Throws:
		//   - eon::type::NotFound : No such cache
		Tuple& cache( name_t name );
	}




	///////////////////////////////////////////////////////////////////////////
	//
	// The 'eon::construct' namespace encloses Æon construct factory functions
	//
	namespace construct
	{
		// Create a unit
		// Throws eon::type::DuplicateName if another unit with the same name already exists in the global scope
		Tuple& unit( name_t name, std::initializer_list<AttributePair> attributes );

		// Access unit
		// Throws eon::type::NotFound if no such unit
		Tuple& unit( name_t name );
	}




	///////////////////////////////////////////////////////////////////////////
	//
	// The 'eon::tuple' namespace encloses normal tuple factory functions
	//
	namespace tuple
	{
		// Create a static tuple
		// Once created, attributes can be modified but not added or removed!
		inline Tuple statc( std::initializer_list<AttributePair> attributes ) { return Tuple( name_static, attributes ); }

		// Create an optional tuple
		// Fixed number and type of attributes, but attributes can be without value.
		// Requires the 'access' construct/operator to access attributes.
		inline Tuple optional( TypeTuple optional_tuple_attributes ) { return Tuple( optional_tuple_attributes ); }

		// Create a dynamic tuple
		// Once created, attributes can be modified, added, and removed!
		inline Tuple dynamic( std::initializer_list<AttributePair> attributes = std::initializer_list<AttributePair>() ) {
			return Tuple( name_dynamic, attributes ); }

		// Create a data tuple
		// Once created, attributes can be modified, added, and removed!
		inline Tuple data( std::initializer_list<AttributePair> attributes = std::initializer_list<AttributePair>() ) {
			return Tuple( name_data, attributes ); }
	}
}
