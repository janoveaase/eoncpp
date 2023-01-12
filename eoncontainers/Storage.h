#pragma once

#include <stdint.h>
#include <eonexcept/Exception.h>



///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	EONEXCEPT( SizeOverflow );

	///////////////////////////////////////////////////////////////////////////
	//
	// Eon Storage class - eon::Storage
	//
	// This is a utility class for other containers!
	//
	// Fixed size contiguous memory for storing a fixed number of elements of
	// fixed size. The class will handle allocation and decallocation, but not
	// calling destructors! The owning container must deal with that.
	//
	class Storage
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		// Construct a void storage - a place-holder
		Storage() = default;

		// Construct for a specified number of elements of specified size.
		Storage( size_t num_elms, size_t elm_size );

		// Construct as a larger version of the specified storage
		// The other storage will be left empty after this!
		// Throws:
		//   - eon::SizeOverflow : The total size is more than maximum!
//		Storage( Storage&& other, size_t additional_elements );

		// Cannot copy!
		Storage( const Storage& ) = delete;

		// Take ownership of another storage's elements
		inline Storage( Storage&& other ) noexcept { *this = std::move( other ); }

		~Storage() { clear(); }



		///////////////////////////////////////////////////////////////////////
		//
		// Modifiers
		//
	public:

		// Cannot copy!
		Storage& operator=( const Storage& ) = delete;

		// Take ownership of another storage's elements
		Storage& operator=( Storage&& other ) noexcept;

		// Set element at position
		// WARNING: Will not check if position is valid!
		inline void set( size_t pos, const char* elm ) { memcpy( Memory + ( pos * ElmSize ), elm, ElmSize ); }

		// Move elements to the right a number of steps
		// WARNING: Will not check if arguments are valid! Also, the owning container is
		// responsible for calling destructor for elements that will be lost!
		void moveRight( size_t start, size_t slots_to_move );

		// Move elements to the left a number of steps
		// WARNING: Will not check if arguments are valid! Also, the owning container is
		// responsible for calling destructor for elements that will be lost!
		void moveLeft( size_t start, size_t slots_to_move );

		// Get mutable reference to element
		// WARNING: Will not check if position is valid!
		template<typename T>
		inline T& mut( size_t pos ) { return *(T*)( Memory + ( pos * ElmSize ) ); }

		// Clear the storage
		// WARNING: The owning container is responsible for calling constructors first!
		inline void clear() { delete[] Memory; Memory = nullptr; NumElms = 0; ElmSize = 0; }




		///////////////////////////////////////////////////////////////////////
		//
		// Read-only
		//
	public:

		// Check if storage is empty
		inline operator bool() const noexcept { return NumElms > 0; }
		inline bool empty() const noexcept { return NumElms == 0; }

		// Get number of elements
		inline size_t numElements() const noexcept { return NumElms; }

		// Access element at position
		// WARNING: Will not check if position is valid!
		inline const char* get( size_t pos ) const { return Memory + ( pos * ElmSize ); }

		// Get immutable reference to element
		// WARNING: Will not check if position is valid!
		template<typename T>
		inline const T& immut( size_t pos ) const { return *(const T*)get( pos ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		char* Memory{ nullptr };
		size_t ElmSize{ 0 };
		size_t NumElms{ 0 };
	};
}
