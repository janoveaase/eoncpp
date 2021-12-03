#include "Tuple.h"
#include "Instance.h"


namespace eon
{
	tuple::attribute::attribute( name_t name, eon::tuple* meta, instance* value )
	{
		Name = name;
		NameMeta = meta;
		Value = value;
		Value->addRef();
	}
	tuple::attribute::attribute( name_t name, instance* value )
	{
		Name = name;
		Value = value;
		Value->addRef();
	}
	tuple::attribute::attribute( instance* value )
	{
		Value = value;
		Value->addRef();
	}




	tuple::tuple( tup::varscope& scope ) {
		Type = scope.tuple( tup::tuplespec() ); }
	tuple::tuple( tup::varscope& scope, std::initializer_list<attribute> attributes )
	{
		tup::tuplespec type;
		for( auto& attrib : attributes )
		{
			if( attrib.Name != no_name )
			{
				type.AttributeNames[ attrib.Name ] = Attributes.size();
				if( attrib.NameMeta != nullptr )
					Meta[ attrib.Name ] = attrib.NameMeta;
			}
			type.Attributes.push_back( attrib.Value->type() );
			Attributes.push_back( attrib.Value );
		}
		Type = scope.tuple( std::move( type ) );
	}

	tuple::~tuple()
	{
		for( auto attrib : Attributes )
			attrib->delRef();
		for( auto& meta : Meta )
			delete meta.second;
	}



	tuple* tuple::copy() const
	{
		auto t = new tuple();
		t->Type = Type;
		for( auto& attribute : Attributes )
			t->Attributes.push_back( attribute->copy() );
		for( auto& meta : Meta )
			t->Meta[ meta.first ] = meta.second->copy();
		return t;
	}

	string tuple::str() const
	{
		std::unordered_map<size_t, name_t> named;
		for( auto& elm : Type->attributeNames() )
			named[ elm.second ] = elm.first;

		string s = "(";
		for( size_t i = 0; i < Attributes.size(); ++i )
		{
			if( i > 0 )
				s += ", ";
			auto name = named.find( i );
			if( name != named.end() )
			{
				s += *name->second;
				auto meta = metadata( name->second );
				if( meta )
					s += "<" + meta->str() + ">";
				s += "=";
			}
			s += Attributes[ i ]->str();
		}
		return s;
	}




	int tuple::compare( const tuple& other ) const noexcept
	{
		auto i = Attributes.begin(), j = other.Attributes.begin();
		for( ; i != Attributes.end() && j != other.Attributes.end(); ++i, ++j )
		{
			auto cmp = ( *i )->compare( **j );
			if( cmp != 0 )
				return cmp;
		}
		return 0;
	}
}
