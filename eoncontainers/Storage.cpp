#include "Storage.h"


namespace eon
{
	Storage::Storage( size_t num_elms, size_t elm_size )
	{
		Memory = new char[ num_elms * elm_size ];
		NumElms = num_elms;
		ElmSize = elm_size;
	}

	//Storage::Storage( Storage&& other, size_t additional_elements )
	//{
	//	if( SIZE_MAX - other.NumElms < additional_elements )
	//		throw SizeOverflow();
	//	Memory = new char[ ( other.NumElms + additional_elements ) * other.ElmSize ];
	//	NumElms = other.NumElms + additional_elements;
	//	ElmSize = other.ElmSize;
	//	memcpy( Memory, other.Memory, other.NumElms * other.ElmSize );
	//	other.clear();
	//}




	Storage& Storage::operator=( Storage&& other ) noexcept
	{
		Memory = other.Memory; other.Memory = 0;
		NumElms = other.NumElms; other.NumElms = 0;
		ElmSize = other.ElmSize; other.ElmSize = 0;
		return *this;
	}

	void Storage::moveRight( size_t start, size_t slots_to_move )
	{
		char* from = Memory + ( start * ElmSize );
		char* to = from + ( slots_to_move * ElmSize );
		size_t size = ( NumElms - start - slots_to_move ) * ElmSize;
		memmove( to, from, size );
	}
	void Storage::moveLeft( size_t start, size_t slots_to_move )
	{
		char* from = Memory + ( start * ElmSize );
		char* to = from - ( slots_to_move * ElmSize );
		size_t size = ( NumElms - start ) * ElmSize;
		memmove( to, from, size );
	}
}
