#include "TypeTupleFactory.h"


namespace eon
{
	TypeTuple typetuple::newExpression( TypeTuple&& return_type )
	{
		TypeTuple typet;
		typet.add( name_expression );
		typet.set( name_return, std::move( return_type ) );
		return typet;
	}


	TypeTuple typetuple::newLambda( TypeTuple&& return_type )
	{
		TypeTuple typet;
		typet.add( name_lambda );
		typet.set( name_return, std::move( return_type ) );
		return typet;
	}


	TypeTuple typetuple::newAction( name_t type_name, name_t action_name, name_t category )
	{
		TypeTuple typet;
		typet.add( name_action );
		typet.set( name_name, action_name );
		typet.set( name_type, type_name );
		typet.set( name_category, category );
		return typet;
	}


	TypeTuple typetuple::convert( std::initializer_list<TypeTuple> attributes )
	{
		TypeTuple typet;
		for( auto& elm : attributes )
			typet.add( elm );
		return typet;
	}

	TypeTuple typetuple::convert( std::initializer_list<std::pair<name_t, TypeTuple>> attributes )
	{
		TypeTuple typet;
		for( auto& elm : attributes )
		{
			if( elm.first == no_name )
				typet.add( elm.second );
			else
				typet.set( elm.first, elm.second );
		}
		return typet;
	}




	TypeTuple typetuple::_new( name_t tuple_type )
	{
		TypeTuple typet;
		typet.add( name_tuple );
		typet.set( name_type, tuple_type );
		return typet;
	}

	TypeTuple& typetuple::_populate( TypeTuple& typet, std::initializer_list<name_t> names )
	{
		TypeTuple attribs;
		for( auto& name : names )
			attribs.add( name );
		typet.set( name_value, std::move( attribs ) );
		return typet;
	}

	TypeTuple& typetuple::_populate( TypeTuple& typet, std::initializer_list<TypeTuple> attributes )
	{
		TypeTuple attribs;
		for( auto& attribute : attributes )
			attribs.add( attribute );
		typet.set( name_value, std::move( attribs ) );
		return typet;
	}

	TypeTuple& typetuple::_populate( TypeTuple& typet, std::initializer_list<std::pair<name_t, TypeTuple>> attributes )
	{
		TypeTuple attribs;
		for( auto& attribute : attributes )
		{
			if( attribute.first == no_name )
				attribs.add( attribute.second );
			else
				attribs.set( attribute.first, attribute.second );
		}
		typet.set( name_value, std::move( attribs ) );
		return typet;
	}
}
