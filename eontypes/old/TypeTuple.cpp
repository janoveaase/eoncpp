#include "TypeTuple.h"


namespace eon
{
	void TypeTuple::str( type::Stringifier& str ) const
	{
		if( isName() )
			str.addWord( asName() );
		else
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
	}




	bool TypeTuple::compatibleWith( const TypeTuple& other ) const noexcept
	{
		// 'other' is compatible with 'this' if:
		//   1. They compare equal.
		//   2. 'this' has <= #attributes than 'other' and they all have the same types in the same order.
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

				// Special case: 'this' is an empty tuple, 'other' is a non-empty tuple
				if( my_elm->isTupleElement() && other_elm->isTupleElement() )
				{
					auto& my_tup = *(TypeTuple*)my_elm;
					auto& other_tup = *(TypeTuple*)other_elm;
					if( my_tup.TupleType != other_tup.TupleType )
						return false;
					if( other_tup.Elements.empty() )
						return true;
				}

				if( !my_elm->compatibleWith( *other_elm ) )
					return false;
			}
		}
		return true;
	}




	void TypeTuple::_makeName()
	{
		auto id = _makeNameStr();
		if( !id.empty() )
			AsName = compilerName( std::move( id ) );
	}
	string TypeTuple::_makeNameStr() const
	{
		if( isName() && ( !isTuple() || TupleType == name_plain ) )
			return eon::str( ( (NameElement*)Elements[ 0 ] )->Value );
		string id;
		if( TupleType )
			id = eon::str( TupleType ) + "(";
		for( auto elm : Elements )
		{
			if( !id.empty() && *id.last() != '(' )
				id += ",";
			if( elm->Name )
				id += eon::str( elm->Name ) + "=";
			if( elm->isNameElement() )
				id += eon::str( ( (const NameElement*)elm )->Value );
			else if( elm->isEllipsisElement() )
			{
				auto& e = *(const EllipsisElement*)elm;
				id += "(";
				if( e.Type != no_name )
					id += eon::str( e.Type ) + " ";
				id += "...)";
			}
			else
			{
				id += "(";
				id += ( (const TypeTuple*)elm )->_makeNameStr();
				id += ")";
			}
		}
		if( TupleType )
			id += ")";
		return id;
	}

	void TypeTuple::_add( TypeElement* element )
	{
		if( element->Name != no_name )
		{
			if( Named.find( element->Name ) != Named.end() )
				throw type::DuplicateName( eon::str( element->Name ) );
			Named[ element->Name ] = Elements.size();
		}
		Elements.push_back( element );
	}
}
