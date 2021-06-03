#include "Object.h"



Object& Object::operator=( const Object& other ) noexcept
{
	Chars = other.Chars;
	Min = other.Min;
	Max = other.Max;
	return *this;
}
Object& Object::operator=( Object &&other ) noexcept
{
	Chars = std::move( other.Chars );
	Min = other.Min; other.Min = _num_min;
	Max = other.Max; other.Max = _num_max;
	return *this;
}
