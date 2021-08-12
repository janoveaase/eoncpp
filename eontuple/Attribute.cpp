#include "Attribute.h"
#include "Tuple.h"
#include <eonvariables/Value.h>


namespace eon
{
	attribute::attribute( name_t name, size_t pos, vars::valueptr value )
	{
		Name = name;
		Pos = pos;
		MetaData = tupleptr( new tuple() );
		Value = value;
	}
	attribute::attribute( name_t name, size_t pos, tupleptr metadata, vars::valueptr value )
	{
		Name = name;
		Pos = pos;
		MetaData = metadata;
		Value = value;
	}


	attribute& attribute::operator=( const attribute& other )
	{
		Name = other.Name;
		Pos = other.Pos;
		MetaData = other.MetaData;
		Value = other.Value;
		return *this;
	}
	attribute& attribute::operator=( attribute&& other ) noexcept
	{
		Name = other.Name; other.Name = no_name;
		Pos = other.Pos; other.Pos = 0;
		MetaData = std::move( other.MetaData );
		Value = std::move( other.Value );
		return *this;
	}
}
