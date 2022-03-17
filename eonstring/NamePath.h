#pragma once
#include "Name.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon Name Path Class - eon::namepath

	  A name path is a list of names that can be used to identify a
	  resource in a hierarchial structure by defining a path through the
	  structure to a target.

	  In string format, the names (elements) are separated using forward slash.
	**************************************************************************/
	class namepath
	{
		/******************************************************************
			Construction
		******************************************************************/
	public:

		namepath() = default;
		inline namepath( const namepath& other ) { Value = other.Value; }
		inline namepath( namepath&& other ) noexcept {
			Value = std::move( other.Value ); }

		inline namepath( const string& ref ) { *this = ref; }
		inline namepath( std::initializer_list<name_t> value ) { for( auto name : value ) Value.push_back( name ); }
		inline namepath( std::vector<name_t>::const_iterator beg, std::vector<name_t>::const_iterator end ) {
			Value.assign( beg, end ); }

		virtual ~namepath() = default;



		/******************************************************************
			Read-only Methods
		******************************************************************/
	public:

		//* Check if reference is empty
		inline bool empty() const noexcept { return Value.empty(); }

		//* Check if reference contains something
		inline operator bool() const noexcept { return !Value.empty(); }

		//* Get number of elements (names) in reference
		inline size_t numElms() const noexcept { return Value.size(); }

		//* Get an element in the reference
		//* Returns [eon::no_name] if position is beyond the last element.
		inline name_t at( size_t pos ) const noexcept { return pos < Value.size() ? Value[ pos ] : no_name; }

		//* Get the last element
		//* Returns [eon::no_name] if empty.
		inline name_t last() const noexcept { return !Value.empty() ? Value[ Value.size() - 1 ] : no_name; }


		//* Get reference with first element removed
		inline namepath chopFirst() const noexcept {
			return *this ? namepath( Value.begin() + 1, Value.end() ) : namepath(); }

		//* Get reference with last element removed
		inline namepath chopLast() const noexcept {
			return *this ? namepath( Value.begin(), Value.end() - 1 ) : namepath(); }
			

		//* Get reference as a string
		inline string str() const noexcept {
			string s{ "@" }; for( auto name : Value ) {
				if( s.numChars() > 2 ) s += "/"; s += eon::str( name ); } return s; }


		//* Get hash value
		inline size_t hash() const noexcept {
			size_t val{ 0 }; for( auto name : Value ) val = val * 37 + (size_t)name; return val; }


		//* Comparison
		int compare( const namepath& other ) const noexcept;

		inline bool operator<( const namepath& other ) const noexcept { return compare( other ) < 0; }
		inline bool operator<=( const namepath& other ) const noexcept { return compare( other ) <= 0; }
		inline bool operator>( const namepath& other ) const noexcept { return compare( other ) > 0; }
		inline bool operator>=( const namepath& other ) const noexcept { return compare( other ) >= 0; }
		inline bool operator==( const namepath& other ) const noexcept { return compare( other ) == 0; }
		inline bool operator!=( const namepath& other ) const noexcept { return compare( other ) != 0; }



		/******************************************************************
			Write Methods
		******************************************************************/
	public:

		//* Set the reference
		inline namepath& operator=( const namepath& other ) { Value = other.Value; return *this; }

		//* Take ownership of another reference's value
		inline namepath& operator=( namepath&& other ) noexcept { Value = std::move( other.Value ); return *this; }

		//* Parse from string
		//* NOTE: Will ignore illegal name characters and only use legal
		//*       ones. Empty elements will also be ignored!
		namepath& operator=( const string& ref );

		inline namepath& operator=( std::initializer_list<name_t> names ) {
			for( auto name : names ) Value.push_back( name ); return *this; }

		//* Clear the reference
		inline void clear() noexcept { Value.clear(); }

		//* Add a name to the end of the reference
		//* Will not add [eon::no_name], but also not report it!
		inline namepath& add( name_t name ) { if( name != no_name ) Value.push_back( name ); return *this; }




		///////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:
		std::vector<name_t> Value;
	};
}


namespace std
{
	//* Allow implicit use of [eon::namepath] as key/value when used in
	//* containers such as 'std::unordered_map' and 'std::unordered_set'.
	template<>
	struct hash<::eon::namepath> {
		inline size_t operator()( const ::eon::namepath& rhs ) const {
			return static_cast<size_t>( rhs.hash() ); } };
	template<>
	struct equal_to<::eon::namepath> {
		inline bool operator()( const ::eon::namepath& lhs,
			const ::eon::namepath& rhs ) const { return lhs == rhs; } };
};
