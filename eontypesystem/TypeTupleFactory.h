#pragma once

#include "Definitions.h"
#include "TypeTuple.h"



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// The 'eon::typetuple' class encloses type tuple factory functions.
	//
	class typetuple
	{
	public:

		// Create type for a static tuple without attributes.
		static inline TypeTuple newStatic() { return _new( name_static ); }

		// Create type for a static tuple with (all) attributes defined in a type-tuple.
		static inline TypeTuple newStatic( TypeTuple&& attributes ) {
			auto typet = _new( name_static ); typet.set( name_value, std::move( attributes ) ); return typet; }

		// Create type for a static tuple, optionally with name attributes.
		static inline TypeTuple newStatic( std::initializer_list<name_t> names ) {
			auto typet = _new( name_static ); return _populate( typet, names ); }

		//// Create type for a static tuple with explicit attributes.
		//static inline TypeTuple newStatic( std::initializer_list<TypeTuple> attributes ) {
		//	auto typet = _new( name_static ); return _populate( typet, attributes ); }

		// Create type for a static tuple with named attributes.
		static inline TypeTuple newStatic( std::initializer_list<std::pair<name_t, TypeTuple>> attributes ) {
			auto typet = _new( name_static ); return attributes.size() > 0 ? _populate( typet, attributes ) : typet; }

		// Create type for a dynamic tuple.
		static inline TypeTuple newDynamic() { return _new( name_dynamic ); }

		// Create type for a data tuple.
		static inline TypeTuple newData() { return _new( name_data ); }


		// Create type for an expression.
		static TypeTuple newExpression( TypeTuple&& return_type );


		// Create type for a lambda.
		static TypeTuple newLambda( TypeTuple&& return_type );


		// Create type for an action without arguments and return type.
		static TypeTuple newAction( name_t type_name, name_t action_name, name_t category );


		// Convert list of unnamed type tuples into a type tuple where each element is an attribute.
		static TypeTuple convert( std::initializer_list<TypeTuple> attributes );

		// Convert list of named type tuples into a type tuple where each element is an attribute.
		static TypeTuple convert( std::initializer_list<std::pair<name_t, TypeTuple>> attributes );




	private:

		static TypeTuple _new( name_t tuple_type );
		static TypeTuple& _populate( TypeTuple& typet, std::initializer_list<name_t> names );
		static TypeTuple& _populate( TypeTuple& typet, std::initializer_list<TypeTuple> attributes );
		static TypeTuple& _populate( TypeTuple& typet, std::initializer_list<std::pair<name_t, TypeTuple>> attributes );

		friend class Tuple;
	};
}
