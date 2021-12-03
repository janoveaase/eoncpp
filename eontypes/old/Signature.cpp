#include "TypeSpec.h"


namespace eon
{
	namespace tup
	{
		string tuplespec::str() const
		{
			string s = "(";
			bool first = true;
			for( auto attribute : Attributes )
			{
				if( first )
					first = false;
				else
					s += ",";
				s += attribute->str();
			}
			s += ")";
			return s;
		}




		int tuplespec::compare( const typespec& other ) const noexcept
		{
			auto cmp = _compare( other );
			if( cmp != 0 )
				return cmp;
			auto& o = *dynamic_cast<const tuplespec*>( &other );
			size_t i = 0, j = 0;
			for( ; i < Attributes.size() && j < o.Attributes.size(); ++i, ++j )
			{
				auto cmp = Attributes[ i ]->compare( *o.Attributes[ j ] );
				if( cmp != 0 )
					return cmp;
			}
			return i < AttributeNames.size() ? -1 : j < o.AttributeNames.size() ? 1 : 0;
		}

		bool tuplespec::compatible( const tuplespec& other ) const noexcept
		{
			for( auto& name : other.AttributeNames )
			{
				auto found = AttributeNames.find( name.first );
				if( found != AttributeNames.end() )
					return false;
				if( *Attributes[ found->second ] != *other.Attributes[ name.second ] )
					return false;
			}
			return true;
		}




		int templatetypes::compare( const templatetypes& other ) const noexcept
		{
			size_t i = 0;
			for( ; i < Types.size() && i < other.Types.size(); ++i )
			{
				auto cmp = name::compare( Types[ i ], other.Types[ i ] );
				if( cmp != 0 )
					return cmp;
			}
			return i < Types.size() ? -1 : i < other.Types.size() ? 1 : 0;
		}

		string templatetypes::str() const
		{
			string s = " of ";
			bool first = true;
			for( auto name : Types )
			{
				if( first )
					first = false;
				else
					s += ", ";
				s += *name + "?";
			}
			return s;
		}




		int standardspec::compare( const typespec& other ) const noexcept
		{
			auto cmp = _compare( other );
			if( cmp != 0 )
				return cmp;
			auto o = dynamic_cast<const standardspec*>( &other );
			cmp = name::compare( Name, o->Name );
			if( cmp != 0 )
				return cmp;
			return Templates.compare( o->Templates );
		}




		int customspec::compare( const typespec& other ) const noexcept
		{
			auto cmp = _compare( other );
			if( cmp != 0 )
				return cmp;
			auto o = dynamic_cast<const customspec*>( &other );
			cmp = name::compare( Name, o->Name );
			if( cmp != 0 )
				return cmp;
			return Templates.compare( o->Templates );
		}




		string actionspec::str() const
		{
			string s = *Name;
			if( Returns )
				s += ">rt:" + Returns->str();
			if( Arguments )
			{
				for( auto& arg : Arguments->attributes() )
				{
					switch( arg->qualifier() )
					{
						case Qualifier::read:
							s += ">rd:" + arg->str();
							break;
						case Qualifier::modify:
							s += ">md:" + arg->str();
							break;
						case Qualifier::consume:
							s += ">cs:" + arg->str();
							break;
						case Qualifier::syntax:
							s += ">sn:" + arg->str();
							break;
						default:		// Not possible!
							throw InternalError( "actionspec::str: No qualifier on argument!" );
					}
				}
			}
			return s;
		}

		int actionspec::compare( const typespec& other ) const noexcept
		{
			auto cmp = _compare( other ); if( cmp != 0 ) return cmp;
			auto o = dynamic_cast<const actionspec*>( &other );
			if( Name < o->Name ) return -1; else if( o->Name < Name ) return 1;
			if( !Returns )
				return o->Returns ? -1 : 0;
			else if( !o->Returns )
				return 1;
			cmp = Returns->compare( *o->Returns ); if( cmp != 0 ) return cmp;
			if( !Arguments )
				return o->Arguments ? -1 : 0;
			else if( !o->Arguments )
				return 1;
			return Arguments->compare( *o->Arguments );
		}
	}
}
