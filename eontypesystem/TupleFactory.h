#pragma once

#include "Tuple.h"



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality.
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// The 'eon::scope' class encloses scope factory functions.
	//
	// Note that scopes are a special type of tuples!
	//
	class scope
	{
	public:

		// Access the one an only global scope.
		static Tuple& global();

		// Access scope for named package.
		static Tuple& package( name_t package_name );

		// Create a new normal scope.
		static Tuple normal( Tuple& parent_scope, std::initializer_list<AttributePair> attributes );
	};




	///////////////////////////////////////////////////////////////////////////
	//
	// The 'eon::cache' clase encloses cache factory functions.
	//
	// Note that caches are a special type of tuples!
	// Caches are created and can only exist in the global scope!
	//
	class cache
	{
	public:

		// Create a cache with specified permissions.
		// Throws [eon::type::DuplicateName] if another cache with the same name already exists in the global scope!
		static Tuple& create( name_t cache_name, TuplePerm permissions );

		// Create a cache with specified (initial) attributes and specified permissions.
		// Throws [eon::type::DuplicateName] if another cache with the same name already exists in the global scope!
		static Tuple& create( name_t cache_name, std::initializer_list<AttributePair> attributes, TuplePerm permissions );

		// Get cache with specified name.
		// Throws [eon::type::NotFound] if no cace exists with that name!
		static Tuple& get( name_t cache_name );
	};




	///////////////////////////////////////////////////////////////////////////
	//
	// The 'eon::unit' class encloses Æon unit factory functions.
	//
	class unit
	{
	public:

		// Create a new unit with specified attributes.
		// Throws [eon::type::DuplicateName] if another unit with the same name already exists in the global scope!
		static Tuple& create( name_t unit_name, std::initializer_list<AttributePair> attributes );

		// Get unit with specified name.
		// Throws [eon::type::NotFound] if no unit exists with that name!
		static Tuple& get( name_t unit_name );
	};




	///////////////////////////////////////////////////////////////////////////
	//
	// The 'eon::tuple' class encloses normal tuple factory functions.
	//
	class tuple
	{
	public:

		// Create a static tuple.
		// Once created, attributes can be modified but not added or removed!
		static inline Tuple newStatic( std::initializer_list<AttributePair> attributes ) {
			return Tuple( name_static, attributes ); }

		// Create an optional tuple.
		// Fixed number and type of attributes, but attributes can be without value.
		// Requires the 'access' construct/operator to access attributes.
		static inline Tuple newOptional( TypeTuple optional_tuple_attributes ) {
			return Tuple( optional_tuple_attributes ); }

		// Create a dynamic tuple.
		// Once created, attributes can be modified, added, and removed!
		static inline Tuple newDynamic(
			std::initializer_list<AttributePair> attributes = std::initializer_list<AttributePair>() ) {
				return Tuple( name_dynamic, attributes ); }

		// Create a data tuple.
		// Once created, attributes can be modified, added, and removed!
		static inline Tuple newData(
			std::initializer_list<AttributePair> attributes = std::initializer_list<AttributePair>() ) {
				return Tuple( name_data, attributes ); }

		EONEXCEPT( InvalidTupleType );

		// Create a tupe of the specified type.
		// Throws [InvalidTupleType] if tuple type is not static, dynamic, or data!
		static Tuple newCustom(
			name_t tupletype, std::initializer_list<AttributePair> attributes = std::initializer_list<AttributePair>() );

		// Create a tupe of the specified type.
		// Throws [InvalidTupleType] if tuple type is not static, dynamic, or data!
		static Tuple newCustom( name_t tupletype, std::vector<AttributePair> attributes );
	};
}
