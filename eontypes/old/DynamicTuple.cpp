#include "DynamicTuple.h"
#include "Name.h"
#include "MetaData.h"
#include "Tuple.h"


namespace eon
{
	DynamicTuple::DynamicTuple( const DynamicTuple& other ) : type::BasicTuple( type::BasicTuple::Class::dynamic )
	{
		for( auto& attribute : other )
		{
			if( attribute.name() != no_name )
				NamedAttribs[ attribute.name() ] = Attributes.size();
			Attributes.push_back( attribute );
		}
	}



	void DynamicTuple::str( type::Stringifier& str ) const
	{
		str.spacingAlways();
		bool first = true;
		for( auto& element : Attributes )
		{
			if( first )
				first = false;
			else
			{
				str.addRaw( "," );
				str.spacingAlways();
			}

			if( element.name() )
			{
				str.addWord( element.name() );
				if( element.value()->generalType() == name_dynamic )
				{
					str.addRaw( ":" );
					str.addIndent();
					str.newLine();
					element.value()->str( str );
					str.reduceIndent();
				}
				else
				{
					str.addWord( "=" );
					element.value()->str( str );
				}
			}
			else
			{
				if( element.value()->generalType() == name_dynamic )
				{
					str.addRaw( "(" );
					str.addIndent();
					str.newLine();
					element.value()->str( str );
					str.reduceIndent();
				}
				else
					element.value()->str( str );
			}
		}
	}




	void DynamicTuple::operator+=( type::DynamicAttribute attribute )
	{
		if( generalType() != name_dynamic )
			throw type::AccessDenied();
		if( attribute.name() != no_name )
		{
			if( exists( attribute.name() ) )
				throw type::DuplicateName( *attribute.name() );
			NamedAttribs[ attribute.name() ] = Attributes.size();
		}
		Attributes.push_back( attribute );
	}
	void DynamicTuple::operator+=( const DynamicTuple& other )
	{
		if( generalType() != name_dynamic )
			throw type::AccessDenied();
		for( auto& attribute : other.Attributes )
		{
			if( attribute.name() != no_name )
			{
				if( exists( attribute.name() ) )
					throw type::DuplicateName( *attribute.name() );
				NamedAttribs[ attribute.name() ] = Attributes.size();
			}
			Attributes.push_back( attribute );
		}
	}

	void DynamicTuple::operator-=( const DynamicTuple& other )
	{
		if( generalType() != name_dynamic || !Type.compare( other.type() ) )
			throw type::AccessDenied();

		// Keep only unnamed attributes that have no positional match in
		// 'other', and named attributes that have a named counterpart in
		// 'other'.
		auto old = std::move( Attributes );
		auto old_names = std::move( NamedAttribs );
		for( size_t i = 0; i < other.Attributes.size(); ++i )
		{
			auto& other_elm = other.Attributes[ i ];
			if( other_elm.name() != no_name && old_names.find( other_elm.name() ) == old_names.end() )
			{
				if( old[ i ].name() != no_name )
					NamedAttribs[ old[ i ].name() ] = Attributes.size();
				Attributes.push_back( std::move( old[ i ] ) );
			}
		}
		for( size_t i = other.Attributes.size(); i < old.size(); ++i )
		{
			if( old[ i ].name() != no_name )
				NamedAttribs[ old[ i ].name() ] = Attributes.size();
			Attributes.push_back( std::move( old[ i ] ) );
		}
	}

	void DynamicTuple::remove( name_t name )
	{
		if( generalType() != name_dynamic )
			throw type::AccessDenied();
		auto found = NamedAttribs.find( name );
		if( found == NamedAttribs.end() )
			throw type::NotFound();
		Attributes.erase( Attributes.begin() + found->second );
		NamedAttribs.erase( found );
	}
	void DynamicTuple::remove( size_t pos )
	{
		if( generalType() != name_dynamic )
			throw type::AccessDenied();
		if( pos >= Attributes.size() )
			throw type::NotFound();
		for( auto named = NamedAttribs.begin(); named != NamedAttribs.end(); ++named )
		{
			if( named->second == pos )
			{
				NamedAttribs.erase( named );
				break;
			}
		}
		Attributes.erase( Attributes.begin() + pos );
	}


	int DynamicTuple::compare( const DynamicTuple& other ) const noexcept
	{
		index_t pos = 0;
		index_t size = Attributes.size() < other.Attributes.size() ? Attributes.size() : other.Attributes.size();
		int cmp{ 0 };
		for( ; pos < size; ++pos )
		{
			auto& lhs = Attributes[ pos ];
			auto& rhs = other.Attributes[ pos ];

			// Make sure the names are the same
			if( lhs.name() != rhs.name() )
				return lhs.name() < rhs.name() ? -1 : 1;

			// Make sure the types are the same
			cmp = _type( pos ).compare( other._type( pos ) );
			if( cmp != 0 )
				return cmp;

			// Make sure we have values
			if( !lhs.value() )
			{
				if( rhs.value() )
					continue;
				else
					return -1;
			}
			else if( !rhs.value() )
				return 1;

			// Make sure general types matches
			auto general_type = lhs.value()->generalType();
			if( general_type != rhs.value()->generalType() )
				return general_type < rhs.value()->generalType() ? -1 : 1;

			// If we have syntax attributes, compare on names
			if( lhs.qualifier() & type::Qualifier::syntax )
				cmp = ( (NameInstance*)lhs.value() )->compare( *(NameInstance*)rhs.value() );

			else
			{
				// If we have tuples, then compare as such
				if( general_type == name_dynamic )
					cmp = ( (DynamicTuple*)lhs.value() )->compare( *(DynamicTuple*)rhs.value() );
				else if( general_type == name_tuple )
					cmp = ( (Tuple*)lhs.value() )->compare( *(Tuple*)rhs.value() );
				else if( general_type == name_meta )
					cmp = ( (MetaData*)lhs.value() )->compare( *(MetaData*)rhs.value() );
				else if( general_type == name_instance )
					cmp = ( (type::Instance*)lhs.value() )->compare( *(type::Instance*)rhs.value() );

				// TODO: Should we include enums and definitions here?
			}

			if( cmp != 0 )
				return cmp;
		}
		return pos < other.Attributes.size() ? -1 : pos < Attributes.size() ? 1 : 0;
	}




	TypeTuple DynamicTuple::_makeTupleID( const std::initializer_list<type::Object*>& values )
	{
		std::vector<TypeElement*> elements;
		for( auto value : values )
		{
			if( !value )
				throw type::NullValue();
			auto& type = value->type();
			if( type.isName() )
				elements.push_back( new NameElement( type.asName() ) );
			else
				elements.push_back( new TypeTuple( type ) );
		}
		return TypeTuple( elements );
	}
	TypeTuple DynamicTuple::_makeTupleID( const std::initializer_list<type::Attribute>& attributes ) noexcept
	{
		std::vector<TypeElement*> elements;
		for( auto attribute : attributes )
		{
			if( attribute.type().isName() )
				elements.push_back( new NameElement( attribute.type().asName() ) );
			else
				elements.push_back( new TypeTuple( attribute.type() ) );
		}
		return  TypeTuple( elements );
	}
	TypeTuple DynamicTuple::_makeTupleID( const std::initializer_list<type::DynamicAttribute>& attributes ) noexcept
	{
		std::vector<TypeElement*> elements;
		for( auto attribute : attributes )
		{
			if( attribute.type().isName() )
				elements.push_back( new NameElement( attribute.type().asName() ) );
			else
				elements.push_back( new TypeTuple( attribute.type() ) );
		}
		return  TypeTuple( elements );
	}

	name_t DynamicTuple::_nameValue( index_t pos ) const noexcept
	{
		return ( (const NameInstance*)Attributes[ pos ].value() )->value();
	}
}
