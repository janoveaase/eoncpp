#pragma once

#include "Storage.h"


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// Eon Vector Class - eon::vector
	//
	// A dynamic, direct-access container storing elements in sequential memory.
	//
	template<typename T>
	class vector
	{
	public:
		using key_type = T;
		using mapped_type = T;
		using value_type = T;
		using reference = T&;
		using const_reference = const T&;

		class const_iterator
		{
		public:
			const_iterator() = default;
			const_iterator( const const_iterator& other ) noexcept { Owner = other.Owner; Pos = other.Pos; }

			inline const_iterator& operator++() noexcept {
				if( Owner ) Pos = Pos < Owner->numElements() ? Pos + 1 : SIZE_MAX; return *this; }
			inline const_iterator operator++( int ) noexcept {
				const_iterator tmp( *this ); ++*this; return tmp; }

			inline const T& operator*() const { return Owner->at( Pos ); }
			inline const T* operator->() const { return &Owner->at( Pos ); }

			const_iterator& operator+=( int elements ) noexcept
			{
				if( elements < 0 )
					return *this -= -elements;
				else if( Owner && elements > 0 && Pos != SIZE_MAX )
				{
					if( Owner->NumElements - Pos < elements )
						Pos += elements;
					else
						Pos = SIZE_MAX;
				}
				return *this;
			}
			const_iterator& operator-=( int elements ) noexcept
			{
				if( elements < 0 )
					return *this += -elements;
				if( Owner && elements > 0 )
				{
					if( Pos == SIZE_MAX )
						Pos = Owner->NumElements;
					if( Pos >= elements )
						Pos -= elements;
					else
						Pos = 0;
				}
				return *this;
			}
			inline friend const_iterator operator+( const const_iterator& itr, int elements ) noexcept {
				return const_iterator( itr ) += elements; }
			inline friend const_iterator operator-( const const_iterator& itr, int elements ) noexcept {
				return const_iterator( itr ) -= elements; }
			inline operator bool() const noexcept { return Pos != SIZE_MAX; }

			int compare( const const_iterator& other ) const noexcept
			{
				return Owner != other.Owner ? ( Owner < other.Owner ? -1 : 1 ) : (
					Pos >= Owner->numElements() && other.Pos >= Owner->numElements() ? 0 : (
						Pos < other.Pos ? -1 : other.Pos < Pos ? 1 : 0 ) );
			}
			inline bool operator<( const const_iterator& other ) const noexcept { return compare( other ) < 0; }
			inline bool operator<=( const const_iterator& other ) const noexcept { return compare( other ) <= 0; }
			inline bool operator>( const const_iterator& other ) const noexcept { return compare( other ) > 0; }
			inline bool operator>=( const const_iterator& other ) const noexcept { return compare( other ) >= 0; }
			inline bool operator==( const const_iterator& other ) const noexcept { return compare( other ) == 0; }
			inline bool operator!=( const const_iterator& other ) const noexcept { return compare( other ) != 0; }

		private:
			inline const_iterator( const vector<T>* owner, size_t pos ) noexcept {
				Owner = owner; Pos = ( Owner && pos < Owner->NumElements ) ? pos : SIZE_MAX; }
			friend class vector<T>;

		protected:
			const vector<T>* Owner{ nullptr };
			size_t Pos{ SIZE_MAX };
		};

		class iterator : public const_iterator
		{
		public:
			iterator() = default;
			iterator( const iterator& other ) noexcept : const_iterator( other ) {}

			inline T& operator*() { return ((vector<T>*)this->Owner)->at( this->Pos ); }
			inline T* operator->() { return &((vector<T>*)this->Owner)->at( this->Pos ); }

			inline iterator& operator+=( int elements ) noexcept {
				*(const_iterator*)this += elements; return *this; }
			inline iterator& operator-=( int elements ) noexcept {
				*(const_iterator*)this -= elements; return *this; }

			inline friend iterator operator+( const iterator& itr, int elements ) noexcept {
				return iterator( itr ) += elements; }
			inline friend iterator operator-( const iterator& itr, int elements ) noexcept {
				return iterator( itr ) -= elements; }

		private:
			inline iterator( vector<T>* owner, size_t pos ) noexcept {
				this->Owner = owner; this->Pos = ( this->Owner && pos < this->Owner->NumElements ) ? pos : SIZE_MAX; }
			friend class vector<T>;
		};




		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		// Construct empty vector
		vector() = default;

		// Copy another vector
		inline vector( const vector& other ) { *this = other; }

		// Take ownership of another vector's elements
		inline vector( vector&& other ) noexcept { *this = std::move( other ); }

		// Construct from a literal list of elements
		inline vector( std::initializer_list<T> elements ) { for( auto& elm : elements ) append( std::move( elm ) ); }

		// Construct with an initial size and an initial element state to use for all elements
		vector( size_t num_elements, const_reference init_elm )
		{
			Elements = Storage( num_elements, sizeof( T ) );
			for( size_t i = 0; i < num_elements; ++i )
				append( init_elm );
		}

		// Construct for a sequence of elements
		template<class InputIt>
		inline vector( InputIt beg, InputIt end ) { for( auto i = beg; i != end; ++i ) append( *i ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Modifiers
		//
	public:

		// Copy another vector's elements
		vector& operator=( const vector& other )
		{
			clear();
			Elements = Storage( other.numElements(), sizeof( T ) );
			for( size_t i = 0; i < other.numElements(); ++i )
				append( T( other[ i ] ) );
			return *this;
		}

		// Take ownership of another vector's elements
		inline vector& operator=( vector&& other ) noexcept {
			Elements = std::move( other.Elements ); NumElements = other.NumElements; other.NumElements = 0; return *this; }

		// Assign a literal set of elements
		vector& operator=( std::initializer_list<T> elements )
		{
			clear();
			Elements = Storage( elements.size(), sizeof( T ) );
			for( auto& elm : elements )
				append( std::move( elm ) );
			return *this;
		}

		// Reserve memory for a number of elements
		// Note that if the vector is already at this capacity or larger, nothing will happen!
		void reserve( size_t num_elements )
		{
			if( capacity() < num_elements )
			{
				if( empty() )
					Elements = Storage( num_elements, sizeof( T ) );
				else
					Elements = Storage( Elements, num_elements - Elements.numElements() );
			}
		}

		// Clear all elements
		void clear() noexcept
		{
			for( size_t i = 0; i < Elements.numElements(); ++i )
				_destroy( i );
			Elements.clear();
			NumElements = 0;
		}

		// Append an element to the end of the vector
		inline void append( T&& element )
		{
			if( full() )
				_grow();
			new ( (void*)Elements.get( NumElements++ ) ) T( std::move( element ) );
		}
		inline void append( const_reference element )
		{
			if( full() )
				_grow();
			new ( (void*)Elements.get( NumElements++ ) ) T( element );
		}

		// Insert an element anywhere inside the vector
		// If position is >= current number of elements, this is the same as calling append.
		// For any other position, elements within the vector will be moved to make space for the new.
		void insert( size_t pos, T&& element )
		{
			if( pos >= NumElements )
				append( std::move( element ) );
			else
			{
				if( full() )
					_grow();
				Elements.moveRight( pos, 1 );
				new ( (void*)Elements.get( pos ) ) T( std::move( element ) );
				++NumElements;
			}
		}
		void insert( size_t pos, const_reference element )
		{
			if( pos == NumElements )
				append( element );
			else
			{
				if( full() )
					_grow();
				Elements.moveRight( pos, 1 );
				new ( (void*)Elements.get( pos ) ) T( element );
				++NumElements;
			}
		}

		// Remove the last element from the vector
		// Has no effect if the vector is empty!
		inline void pop() {
			if( NumElements > 0 )
				Elements.mut<T>( --NumElements ).~T(); }

		// Remove an element anywhere inside the vector
		// Has no effect if position is not within the current number of elements.
		// If position of last element, this is the same as calling pop.
		// For any other position, elements within the vector will be moved to fill the space of the removed element.
		void remove( size_t pos )
		{
			if( NumElements > 0 )
			{
				if( pos == NumElements - 1 )
					pop();
				else if( pos < NumElements - 1 )
				{
					Elements.mut<T>( pos ).~T();
					Elements.moveLeft( pos + 1, 1 );
					--NumElements;
				}
			}
		}

		// Access element at position as mutable object
		// Throws:
		//   - std::out_of_range : Invalid position
		inline T& at( size_t pos )
		{
			if( pos < NumElements )
				return Elements.mut<T>( pos );
			else
				throw std::out_of_range( "eon::vector<T>::at" );
		}

		// Access element at position as mutable object
		// WARNING: Will not check if position is legal!
		inline T& operator[]( size_t pos ) { return Elements.mut<T>( pos ); }





		///////////////////////////////////////////////////////////////////////
		//
		// Read-only
		//
	public:

		// Check if vector is empty
		inline operator bool() const noexcept { return static_cast<bool>( Elements ); }
		inline bool empty() const noexcept { return NumElements == 0; }

		// Check if vector is full (adding new element will cause it to grow)
		inline bool full() const noexcept { return NumElements == Elements.numElements(); }

		// Get number of elements
		inline size_t numElements() const noexcept { return NumElements; }

		// Get position of last element
		// NOTE: Will return zero if vector is empty!
		inline size_t lastPos() const noexcept { return NumElements > 0 ? NumElements - 1 : 0; }

		// Get total capacity of the vector
		inline size_t capacity() const noexcept { return Elements.numElements(); }

		// Get element at position
		// Throws:
		//   - std::out_of_range : Invalid position
		inline const_reference at( size_t pos ) const {
			if( pos < NumElements ) return Elements.immut<T>( pos ); else throw std::out_of_range( "eon::vector<T>::at" ); }

		// Get elemetn at position
		// WARNING: No checking that position is valid!
		inline const_reference operator[]( size_t pos ) const { return Elements.immut<T>( pos ); }

		// Iteration
		inline iterator begin() noexcept { return iterator( this, 0 ); }
		inline const_iterator begin() const noexcept { return const_iterator( this, 0 ); }
		inline const_iterator cbegin() const noexcept { return const_iterator( this, 0 ); }
		inline iterator end() noexcept { return iterator( this, SIZE_MAX ); }
		inline const_iterator end() const noexcept { return const_iterator( this, SIZE_MAX ); }
		inline const_iterator cend() const noexcept { return const_iterator( this, SIZE_MAX ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
	private:

		inline void _destroy( size_t elm_no ) {
			Elements.mut<T>( elm_no ).~T(); }

		void _grow()
		{
			if( empty() )
				Elements = Storage( 13, sizeof( T ) );
			else
			{
				size_t new_size = Elements.numElements() < SIZE_MAX / 2 ? Elements.numElements() * 2 : SIZE_MAX;
				if( new_size == Elements.numElements() )
					throw std::length_error( "Cannot grow eon::vector - max size reached!" );
				Storage new_elements( new_size, sizeof( T ) );
				for( size_t i = 0; i < NumElements; ++i )
					new ( (void*)new_elements.get( i ) ) T( std::move( at( i ) ) );
				Elements = std::move( new_elements );
			}
		}

		inline size_t _pos( int64_t pos ) const noexcept {
			return pos >= 0 ? static_cast<size_t>( pos ) : static_cast<size_t>( -pos ) > Elements.numElements() ? 0
				: Elements.numElements() - static_cast<size_t>( -pos ); }



	private:
		Storage Elements;
		size_t NumElements{ 0 };
	};
}
