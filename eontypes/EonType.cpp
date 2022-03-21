#include "EonType.h"


namespace eon
{
	EonType::EonType( std::initializer_list<name_t> names, source::Ref source )
	{
		switch( names.size() )
		{
			case 0:
				break;
			case 1:
				Name = *names.begin();
				Source = source;
				break;
			default:
				Name = name_plain;
				for( auto& name : names )
					Attributes.push_back( EonType( name, source ) );
				Source = source;
				break;
		}
	}
	EonType::EonType( std::initializer_list<EonType> types, source::Ref source )
	{
		switch( types.size() )
		{
			case 0:
				break;
			case 1:
				Name = types.begin()->Name;
				AttributeName = types.begin()->AttributeName;
				Attributes = types.begin()->Attributes;
				NamedAttributes = types.begin()->NamedAttributes;
				Source = source;
				break;
			default:
				Name = name_plain;
				Attributes = types;
				for( size_t i = 0; i < Attributes.size(); ++i )
				{
					if( Attributes[ i ].AttributeName != no_name )
						NamedAttributes[ Attributes[ i ].AttributeName ] = i;
				}
				Source = source;
				break;
		}
	}




	void EonType::str( type::Stringifier& str ) const
	{
		str.addWord( "T(" );
		str.noSpacing();
		if( Name != name_plain )
		{
			if( AttributeName != no_name )
			{
				str.addWord( AttributeName );
				str.noSpacing();
				str.addWord( "=" );
				str.noSpacing();
			}
			str.addWord( Name );
		}
		else
		{
			bool first = true;
			for( auto& attribute : Attributes )
			{
				if( first )
					first = false;
				else
				{
					str.addRaw( "," );
					str.spacingAlways();
				}
				attribute._toStr( str );
			}
		}
		str.addRaw( ")" );
	}




	bool EonType::compatibleWith( const EonType& other ) const noexcept
	{
		if( other.Name == name_plain )
		{
			if( Name == name_plain )
				return _compatibleAttributes( other );
			else if( !other.Attributes.empty() )
				return compatibleWith( other.Attributes[ 0 ] );
		}
		else if( other.Name == name_data )
		{
			if( Name == name_plain || Name == name_data )
				return _compatibleAttributes( other );
			else if( !other.Attributes.empty() )
				return compatibleWith( other.Attributes[ 0 ] );
		}
		else if( other.Name == name_dynamic )
		{
			if( Name == name_plain || Name == name_data || Name == name_dynamic )
				return _compatibleAttributes( other );
			else if( !other.Attributes.empty() )
				return compatibleWith( other.Attributes[ 0 ] );
		}
		return Name == other.Name;
	}




	void EonType::_toStr( type::Stringifier& str ) const
	{
		if( AttributeName != no_name )
		{
			str.addWord( AttributeName );
			str.noSpacing();
			str.addWord( "=" );
			str.noSpacing();
		}
		if( Name != name_plain )
			str.addWord( Name );
		else
		{
			str.addWord( "(" );
			str.noSpacing();
			bool first = true;
			for( auto& attribute : Attributes )
			{
				if( first )
					first = false;
				else
				{
					str.addRaw( "," );
					str.spacingAlways();
				}
				attribute._toStr( str );
			}
			str.addRaw( ")" );
		}
	}

	int EonType::_compareAttributes( const EonType& other ) const noexcept
	{
		size_t i = 0, j = 0;
		for( ; i < Attributes.size() && j < other.Attributes.size(); ++i, ++j )
		{
			auto cmp = Attributes[ i ].compare( other.Attributes[ j ] );
			if( cmp != 0 )
				return cmp;
		}
		return j < other.Attributes.size() ? -1 : i < Attributes.size() ? 1 : 0;
	}
	bool EonType::_compatibleAttributes( const EonType& other ) const noexcept
	{
		// All named attributes in 'other' must exist and have the same type in 'this'
		// All unnamed attributes must match with 'this' on position and type
		for( size_t i = 0; i < Attributes.size(); ++i )
		{
			if( Attributes[ i ].AttributeName != no_name )
			{
				auto p = other.pos( Attributes[ i ].AttributeName );
				if( p == no_pos || !Attributes[ i ].compatibleWith( other.Attributes[ p ] ) )
					return false;
			}
			else
			{
				if( i >= other.Attributes.size() || !Attributes[ i ].compatibleWith( other.Attributes[ i ] ) )
					return false;
			}
		}
		return true;
	}
}
