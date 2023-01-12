#pragma once
#include <eonexcept/Exception.h>
#include <unordered_map>


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// Eon ReflexiveMap Class - eon::reflexive_map
	//
	// This class is like std::unordered_map, but allows mapping both from key
	// to value and value to key. (Not that key and value have much meaning in
	// this sense - they are relative terms according to which way you want to
	// map.)
	//
	// NOTE: Should mainly be used on simple/small types as data is duplicated!
	// NOTE: Both keys and values are const! To change one, you must remove +
	//       insert again.
	//
	template<typename T1, typename T2>
	class reflexive_map
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		reflexive_map() = default;
		inline reflexive_map( const reflexive_map& other ) { One = other.One; Two = other.Two; }
		inline reflexive_map( reflexive_map&& other ) noexcept {
			One = std::move( other.One ); Two = std::move( other.Two ); }

		inline reflexive_map( std::initializer_list<std::pair<T1, T2>> elements ) {
			for( auto& elm : elements ) { One[ elm.first ] = elm.second;Two[ elm.second ] = elm.first; } }

		virtual ~reflexive_map() = default;




		///////////////////////////////////////////////////////////////////////
		//
		// Modifier Methods
		//
	public:

		// Replace elements by copying another reflexive_map
		inline reflexive_map& operator=( const reflexive_map& other ) { One = other.One; Two = other.Two; return *this; }

		// Replace elements by taking ownership of the elements of another reflexive_map
		inline reflexive_map& operator=( reflexive_map&& other ) noexcept {
			One = std::move( other.One ); Two = std::move( other.Two ); return *this; }

		// Clear all elements
		void clear() noexcept { One.clear(); Two.clear(); }




		///////////////////////////////////////////////////////////////////////
		//
		// Read-only Methods
		//
	public:

		// Check if empty
		inline bool empty() const noexcept { return One.empty(); }

		// Use as boolean
		// Returns true if not empty!
		inline operator bool() const noexcept { return !One.empty(); }

		// Get number of reflexive_map elements
		inline size_t size() const noexcept { return One.size(); }




		///////////////////////////////////////////////////////////////////////
		//		
		// Map Operations
		//
	public:

		// Insert an element
		inline void insert( const T1& one, const T2& two ) { One[ one ] = two; Two[ two ] = one; }

		// Access an element
		// WARNING: Will not check if the element exists!
		inline const T2& one( const T1& element ) const { return One.find( element )->second; }
		inline const T1& two( const T2& element ) const { return Two.find( element )->second; }

		// Remove an element
		inline void removeOne( const T1& element ) noexcept { auto one = One.find( element ); if( one != One.end() ) {
			Two.erase( Two.find( one->second ) ); One.erase( one ); } }
		inline void removeTwo( const T2& element ) noexcept { auto two = Two.find( element ); if( two != Two.end() ) {
			One.erase( One.find( two->second ) ); Two.erase( two ); } }

		// Check if map contains a specific element
		inline bool containsOne( const T1& element ) const noexcept { return One.find( element ) != One.end(); }
		inline bool containsTwo( const T2& element ) const noexcept { return Two.find( element ) != Two.end(); }




		///////////////////////////////////////////////////////////////////////
		//
		// Iteration
		//
	public:

		using iterator_one = typename std::unordered_map<T1, T2>::const_iterator;
		using iterator_two = typename std::unordered_map<T2, T1>::const_iterator;

		inline iterator_one begin() const noexcept { return One.begin(); }
		inline iterator_one end() const noexcept { return One.end(); }

		inline iterator_one beginOne() const noexcept { return One.begin(); }
		inline iterator_one endOne() const noexcept { return One.end(); }

		inline iterator_two beginTwo() const noexcept { return Two.begin(); }
		inline iterator_two endTwo() const noexcept { return Two.end(); }

		inline iterator_one findOne( const T1& element ) const noexcept { return One.find( element ); }
		inline iterator_two findTwo( const T2& element ) const noexcept { return Two.find( element ); }



		
		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		std::unordered_map<T1, T2> One;
		std::unordered_map<T2, T1> Two;
	};
}
