#pragma once
#include <vector>
#include <algorithm>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon Vector Class - eon::vector

	  Wrapper for std::vector with some additional functionality
	**************************************************************************/
	template<typename T>
	class vector
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		vector() = default;
		inline vector( const vector& other ) { Elements = other.Elements; }
		inline vector( vector&& other ) noexcept { Elements = std::move( other.Elements ); }
		inline vector( std::initializer_list<T> elements ) { Elements.assign( elements ); }
		inline vector( size_t count, const T& init_elm ) { Elements.assign( size, init_elm ); }
		template<class InputIt>
		inline vector( InputIt beg, InputIt end ) { Elements.assign( beg, end ); }




		/**********************************************************************
		  Assignment operators
		**********************************************************************/
	public:

		inline vector& operator=( const vector& other ) { Elements = other.Elements; return *this; }
		inline vector& operator=( vector&& other ) noexcept { Elements = std::move( other.Elements ); return *this; }
		inline vector& operator=( std::initializer_list<T> elements ) { Elements = elements; return *this; }




		/**********************************************************************
		  Element access
		**********************************************************************/
	public:

		// Access elements where negative positions count from the end of the
		// vector. I.e., -1 = last element.
		inline const T& at( int64_t pos ) const { return Elements.at( _pos( pos ) ); }
		inline T& at( int64_t pos ) { return Elements.at( _pos( pos ) ); }
		inline const T& operator[]( int64_t pos ) const { return Elements[ _pos( pos ) ]; }
		inline T& operator[]( int64_t pos ) { return Elements[ _pos( pos ) ]; }

		inline void assign( size_t count, const T& value ) { Elements.assign( count, value ); }
		template<class InputIt>
		inline void assign( InputIt beg, InputIt end ) { Elements.assign( beg, end ); }
		inline void assign( std::initializer_list<T> elements ) { Elements.assign( elements ); }

		inline const T& front() const { return Elements.front(); }
		inline T& front() { return Elements.front(); }
		inline const T& back() const { return Elements.back(); }
		inline T& back() { return Elements.back(); }

		// Get a slice of the vector
		// Negative positions count from the end of the vector.
		// If first < last, then the slice will be reversed.
		inline vector<T> slice( int64_t first, int64_t last ) const { auto f = _pos( first ), l = _pos( last );
			return f <= l ? vector<T>( begin() + f, begin() + l + 1 ) : vector<T>( rbegin() + l, rbegin() + ( f + 1 ) ); }

		// Check if elements is contained within this vector
		// Uses linear searching!
		inline bool contains( const T& element ) const noexcept { return std::find( begin(), end(), element ) != end(); }




		/**********************************************************************
		  Iteration
		**********************************************************************/
	public:

		using iterator = typename std::vector<T>::iterator;
		using const_iterator = typename std::vector<T>::const_iterator;
		using reverse_iterator = typename std::vector<T>::reverse_iterator;
		using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator;

		inline iterator begin() noexcept { return Elements.begin(); }
		inline iterator end() noexcept { return Elements.end(); }

		inline const_iterator begin() const noexcept { return Elements.begin(); }
		inline const_iterator end() const noexcept { return Elements.end(); }
		inline const_iterator cbegin() const noexcept { return Elements.cbegin(); }
		inline const_iterator cend() const noexcept { return Elements.cend(); }

		inline reverse_iterator rbegin() noexcept { return Elements.rbegin(); }
		inline reverse_iterator rend() noexcept { return Elements.rend(); }

		inline const_reverse_iterator rbegin() const noexcept { return Elements.rbegin(); }
		inline const_reverse_iterator rend() const noexcept { return Elements.rend(); }
		inline const_reverse_iterator crbegin() const noexcept { return Elements.crbegin(); }
		inline const_reverse_iterator crend() const noexcept { return Elements.crend(); }




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

		inline iterator insert( const_iterator pos, const T& element ) { return Elements.insert( pos, element ); }
		inline iterator insert( const_iterator pos, T&& element ) { return Elements.insert( pos, std::move( element ) ); }
		inline iterator insert( const_iterator pos, size_t count, const T& element ) {
			return Elements.insert( pos, count, element ); }
		template<class InputIt>
		inline iterator insert( const_iterator pos, InputIt beg, InputIt end ) { return Elements.insert( pos, beg, end ); }
		inline iterator insert( const_iterator pos, std::initializer_list<T> elements) {
			return Elements.insert( pos, elements ); }

		inline iterator erase( const_iterator pos ) { return Elements.erase( pos ); }
		inline iterator erase( const_iterator beg, const_iterator end ) { return Elements.erase( beg, end ); }

		inline void push_back( const T& value ) { Elements.push_back( value ); }
		inline void push_back( T&& value ) { Elements.push_back( std::move( value ) ); }

		inline void pop_back() { Elements.pop_back(); }

		inline void resize( size_t new_size ) { Elements.resize( new_size ); }
		inline void resize( size_t new_size, const T& value ) { Elements.resize( new_size, value ); }

		inline void swap( vector<T>& other ) noexcept { Elements.swap( other.Elements ); }




		/**********************************************************************
		  Comparison
		**********************************************************************/
	public:

		inline bool operator==( const vector<T>& other ) const noexcept { return Elements == other.Elements; }
		inline bool operator!=( const vector<T>& other ) const noexcept { return Elements != other.Elements; }
		inline bool operator<( const vector<T>& other ) const noexcept { return Elements < other.Elements; }
		inline bool operator<=( const vector<T>& other ) const noexcept { return Elements <= other.Elements; }
		inline bool operator>( const vector<T>& other ) const noexcept { return Elements > other.Elements; }
		inline bool operator>=( const vector<T>& other ) const noexcept { return Elements >= other.Elements; }




		/**********************************************************************
		  Helpers
		**********************************************************************/
	public:

		inline size_t _pos( int64_t pos ) const noexcept { return pos >= 0 ? static_cast<size_t>( pos )
			: static_cast<size_t>( -pos ) > Elements.size() ? 0 : Elements.size() - static_cast<size_t>( -pos ); }



	private:
		std::vector<T> Elements;
	};
}
