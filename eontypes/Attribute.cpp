#include "Attribute.h"


namespace eon
{
	namespace type
	{
		Attribute& Attribute::operator=( const Attribute& other )
		{
			Name = other.Name;
			Type = other.Type;
			Value = other.Value;
			return *this;
		}
		Attribute& Attribute::operator=( Attribute&& other ) noexcept
		{
			Name = other.Name; other.Name = no_name;
			Type = std::move( other.Type );
			Value = other.Value; other.Value = nullptr;
			return *this;
		}
	}
}
