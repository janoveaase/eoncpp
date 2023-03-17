#include "TupleFactory.h"


namespace eon
{
	Tuple& scope::global()
	{
		static Tuple scope;
		if( !scope.type() )
			scope = Tuple( name_scope, TuplePerm::all_normal | TuplePerm::types | TuplePerm::actions );
		return scope;
	}

	Tuple& scope::package( name_t package_name )
	{
		auto scope_name = compilerName( "package#" + str( package_name ) );
		if( global().exists( scope_name ) )
			return global().get<Tuple>( scope_name );
		else
		{
			auto scope = Tuple( scope_name, TuplePerm::all_normal, &global() );
			return global().addTuple( scope_name, std::move( scope ) );
		}
	}

	Tuple scope::normal( Tuple& parent_scope, std::initializer_list<AttributePair> attributes )
	{
		Tuple scope( name_scope, attributes, TuplePerm::modify | TuplePerm::actions, &parent_scope );
		return scope;
	}




	Tuple& cache::create( name_t cache_name, TuplePerm permissions )
	{
		auto c_name = compilerName( "cache#" + str( cache_name ) );
		if( scope::global().exists( c_name ) )
			throw type::DuplicateName( "Cannot create cache \"" + str( cache_name ) + "\"" );
		else
			return scope::global().addTuple( c_name, Tuple( name_cache, permissions ) );
	}

	Tuple& cache::create( name_t cache_name, std::initializer_list<AttributePair> attributes, TuplePerm permissions )
	{
		auto c_name = compilerName( "cache#" + str( cache_name ) );
		if( scope::global().exists( c_name ) )
			throw type::DuplicateName( "Cannot create cache \"" + str( cache_name ) + "\"" );
		else
			return scope::global().addTuple( c_name, Tuple( name_cache, attributes, permissions ) );
	}

	Tuple& cache::get( name_t cache_name )
	{
		auto c_name = compilerName( "cache#" + str( cache_name ) );
		if( !scope::global().exists( c_name ) )
			throw type::NotFound( "Cannot access cache \"" + str( cache_name ) + "\"" );
		else
			return scope::global().get<Tuple>( c_name );
	}




	Tuple& unit::create( name_t unit_name, std::initializer_list<AttributePair> attributes )
	{
		auto u_name = compilerName( "unit#" + str( unit_name ) );
		if( scope::global().exists( u_name ) )
			throw type::DuplicateName( "Cannot create unit \"" + str( unit_name ) + "\"" );
		else
			return scope::global().addTuple( u_name,
				Tuple( name_unit, attributes, TuplePerm::all_normal | TuplePerm::types ) );
	}

	Tuple& unit::get( name_t unit_name )
	{
		auto u_name = compilerName( "unit#" + str( unit_name ) );
		if( !scope::global().exists( u_name ) )
			throw type::NotFound( "Cannot access unit \"" + str( unit_name ) + "\"" );
		else
			return scope::global().get<Tuple>( u_name );
	}




	Tuple tuple::newCustom( name_t tupletype, std::initializer_list<AttributePair> attributes )
	{
		if( tupletype == name_static )
			return newStatic( std::move( attributes ) );
		else if( tupletype == name_dynamic )
			return newDynamic( std::move( attributes ) );
		else if( tupletype == name_data )
			return newData( std::move( attributes ) );
		else
			throw InvalidTupleType(
				"Cannot construct new tuple of type \"" + str( tupletype )
				+ "\" - only \"static\", \"dynamic\", and \"data\"!" );
	}

	Tuple tuple::newCustom( name_t tupletype, std::vector<AttributePair> attributes )
	{
		if( tupletype == name_static )
			return Tuple( name_static, std::move( attributes ) );
		else if( tupletype == name_dynamic )
			return Tuple( name_dynamic, std::move( attributes ) );
		else if( tupletype == name_data )
			return Tuple( name_data, std::move( attributes ) );
		else
			throw InvalidTupleType(
				"Cannot construct new tuple of type \"" + str( tupletype )
				+ "\" - only \"static\", \"dynamic\", and \"data\"!" );
	}
}
