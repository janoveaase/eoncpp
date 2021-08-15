#pragma once
#include "Vector.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon Stack Class - eon::stack

	  Differs from std::stack in that you can read and modify (but not delete)
	  elements not at the top of the stack. (Simplification of std::stack 
	  with custom container.)
	**************************************************************************/
	template<typename T>
	class stack
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		stack() = default;
		inline stack( const stack& other ) { Elements = other.Elements; }
		inline stack( stack&& other ) noexcept { Elements = std::move( other.Elements ); }
		template<class InputIt>
		inline stack( InputIt beg, InputIt end ) { Elements.assign( beg, end ); }
		inline stack( std::initializer_list<T> elements ) { Elements.assign( elements ); }




		/**********************************************************************
		  Assignment operators
		**********************************************************************/
	public:

		inline stack& operator=( const stack& other ) { Elements = other.Elements; return *this; }
		inline stack& operator=( stack&& other ) noexcept { Elements = std::move( other.Elements ); return *this; }
		inline stack& operator=( std::initializer_list<T> elements ) { Elements = elements; }




		/**********************************************************************
		  Element access
		**********************************************************************/
	public:

		inline T& top() { return Elements.back(); }
		inline const T& top() const { return Elements.back(); }

		// Positive positions count from the top of the stack (meaning 0=top),
		// while negative count from the bottom (meaning -1=bottom).
		inline T& at( int64_t pos ) { return Elements.at( -( ++pos ) ); }
		inline const T& at( int64_t pos ) const { return Elements.at( -( ++pos ) ); }




		/**********************************************************************
		  Capacity
		**********************************************************************/
	public:

		inline bool empty() const noexcept { return Elements.empty(); }
		inline size_t size() const noexcept { return Elements.size(); }

		inline size_t max_size() const noexcept { return Elements.max_size(); }

		inline void reserve( size_t new_capacity ) { Elements.reserve( new_capacity ); }
		inline size_t capacity() const noexcept { return Elements.capacity(); }

		inline void shrink_to_fit() { Elements.shrink_to_fit(); }




		/**********************************************************************
		  Modifiers
		**********************************************************************/
	public:

		inline void clear() noexcept { Elements.clear(); }

		inline void push( const T& element ) { Elements.push_back( element ); }
		inline void pop() { Elements.pop_back(); }

		inline void resize( size_t new_size ) { Elements.resize( new_size ); }
		inline void resize( size_t new_size, const T& value ) { Elements.resize( new_size, value ); }

		inline void swap( stack<T>& other ) noexcept { Elements.swap( other.Elements ); }




		/**********************************************************************
		  Comparison
		**********************************************************************/
	public:

		inline bool operator==( const stack<T>& other ) const noexcept { return Elements == other.Elements; }
		inline bool operator!=( const stack<T>& other ) const noexcept { return Elements != other.Elements; }
		inline bool operator<( const stack<T>& other ) const noexcept { return Elements < other.Elements; }
		inline bool operator<=( const stack<T>& other ) const noexcept { return Elements <= other.Elements; }
		inline bool operator>( const stack<T>& other ) const noexcept { return Elements > other.Elements; }
		inline bool operator>=( const stack<T>& other ) const noexcept { return Elements >= other.Elements; }





	private:
		vector<T> Elements;
	};
}
