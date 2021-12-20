#include "Attribute.h"
#include "MetaData.h"


namespace eon
{
	namespace type
	{
		Qualifier mapQualifier( string qual )
		{
			if( qual == "read" )
				return Qualifier::read;
			else if( qual == "modify" )
				return Qualifier::modify;
			else if( qual == "consume" )
				return Qualifier::consume;
			else
				return Qualifier::undef;
		}
		string mapQualifier( Qualifier qual )
		{
			switch( qual )
			{
				case Qualifier::read:
					return "read";
				case Qualifier::modify:
					return "modify";
				case Qualifier::consume:
					return "consume";
				default:
					return "N/A";
			}
		}




		Attribute::Attribute( name_t name, MetaData&& meta, Object* value )
		{
			Name = name;
			Meta = new MetaData( std::move( meta ) );
			if( value )
				Type = value->type();
			assign( value );
		}
		Attribute::Attribute( name_t name, const TypeTuple& type, MetaData&& meta, Object* value )
		{
			Name = name;
			Type = type;
			Meta = new MetaData( std::move( meta ) );
			assign( value );
		}
		Attribute::Attribute( name_t name, name_t type, MetaData&& meta, Object* value )
		{
			Name = name;
			Type = TypeTuple( { type } );
			Meta = new MetaData( std::move( meta ) );
			assign( value );
		}
		Attribute::Attribute( name_t name, MetaData&& meta, Qualifier qual, Object* value )
		{
			Name = name;
			Meta = new MetaData( std::move( meta ) );
			Qual = qual;
			if( value )
				Type = value->type();
			assign( value );
		}
		Attribute::Attribute( name_t name, const TypeTuple& type, MetaData&& meta, Qualifier qual, Object* value )
		{
			Name = name;
			Type = type;
			Meta = new MetaData( std::move( meta ) );
			Qual = qual;
			assign( value );
		}
		Attribute::Attribute( name_t name, name_t type, MetaData&& meta, Qualifier qual, Object* value )
		{
			Name = name;
			Type = TypeTuple( { type } );
			Meta = new MetaData( std::move( meta ) );
			Qual = qual;
			assign( value );
		}
		Attribute::~Attribute()
		{
			if( Meta != nullptr )
				delete Meta;
		}
	}
}
