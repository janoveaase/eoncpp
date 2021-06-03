#pragma once

#include "Defs.h"


class Object
{
public:
	Object() noexcept {}
	Object( Object& other ) { *this = other; }
	Object( Object&& other ) noexcept { *this = std::move( other ); }
	virtual ~Object() noexcept {}

	Object& operator=( const Object& other ) noexcept;
	Object& operator=( Object&& other ) noexcept;

private:
	_text Chars;
	_num Min{ _num_min };
	_num Max{ _num_max };
};
