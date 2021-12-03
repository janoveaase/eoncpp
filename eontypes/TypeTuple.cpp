#include "TypeTuple.h"


namespace eon
{
	void TypeTuple::str( type::Stringifier& str ) const
	{
		str.addRaw( "T(" );
		str.noSpacing();
		bool first = true;
		for( auto element : Elements )
		{
			if( first )
				first = false;
			else
			{
				str.addRaw( "," );
				str.spacingAlways();
			}
			if( element->isNameElement() )
				str.addWord( ( (const NameElement*)element )->Value );
			else
				( (const TypeTuple*)element )->str( str );
		}
		str.addRaw( ")" );
	}




	bool TypeTuple::compatibleWith( const TypeTuple& other ) const noexcept
	{
		// 'other' is compatible with 'this' if:
		//   1. They compare equal.
		//   2. 'this' has <= attributes than 'other' and they all have the same types in the same order.
		//   3. 'this' has named attributes that match the named attributes of 'other',
		//      while all unnamed (if any) matches as in pt. 2.
		
		if( *this == other )
			return true;
		if( other.Elements.size() < Elements.size() )
			return false;
		for( size_t i = 0; i < Elements.size(); ++i )
		{
			auto& my_elm = Elements[ i ];
			if( my_elm->Name != no_name )
			{
				auto found = other.Named.find( my_elm->Name );
				if( found == other.Named.end() )
					return false;
				if( !my_elm->compatibleWith( *other.Elements[ found->second ] ) )
					return false;
			}
			else
			{
				auto other_elm = other.Elements[ i ];
				if( !my_elm->compatibleWith( *other_elm ) )
					return false;
			}
		}
		return true;
	}




	void TypeTuple::_makeName()
	{
		auto id = _makeNameStr();
		AsName = name::compilerGet( std::move( id ) );
	}
	string TypeTuple::_makeNameStr() const
	{
		string id;
		for( auto elm : Elements )
		{
			if( !id.empty() )
				id += ",";
			if( elm->isNameElement() )
				id += *( (const NameElement*)elm )->Value;
			else if( elm->isEllipsisElement() )
			{
				auto& e = *(const EllipsisElement*)elm;
				id += "(";
				if( e.Type != no_name )
					id += *e.Type + " ";
				id += "...)";
			}
			else
			{
				id += "(";
				id += ( (const TypeTuple*)elm )->_makeNameStr();
				id += ")";
			}
		}
		return id;
	}

	void TypeTuple::_add( TypeElement* element )
	{
		if( element->Name != no_name )
		{
			if( Named.find( element->Name ) != Named.end() )
				throw type::DuplicateName( *element->Name );
			Named[ element->Name ] = Elements.size();
		}
		Elements.push_back( element );
	}
}
