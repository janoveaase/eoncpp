#pragma once

#include <eonname/Name.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::tup' namespace is for specific Æon Tuple Format elements
	**************************************************************************/
	namespace tup
	{
		/**********************************************************************
		  Eon Tuple Path Class - eon::tup::path

		  A path is a list of [eon::name_t] elements identifying an attribute
		  within an [eon::Tuple].
		**********************************************************************/
		class path
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			path() = default;
			inline path( const path& other ) { Value = other.Value; }
			inline path( path&& other ) noexcept {
				Value = std::move( other.Value ); }

			inline path( std::initializer_list<name_t> value ) {
				for( auto name : value ) Value.push_back( name ); }
			inline path( std::vector<name_t>::const_iterator beg,
				std::vector<name_t>::const_iterator end ) {
				Value.assign( beg, end ); }

			virtual ~path() = default;



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			//* Check if path is empty
			inline bool empty() const noexcept { return Value.empty(); }

			//* Check if path contains something
			inline operator bool() const noexcept { return !Value.empty(); }

			//* Get number of elements (names) in path
			inline size_t size() const noexcept { return Value.size(); }

			//* Get an element in the path
			//* Returns [eon::no_name] if position is beyond the last element.
			inline name_t at( size_t pos ) const noexcept {
				return pos < Value.size() ? Value[ pos ] : no_name; }

			//* Get the last element
			//* Returns [eon::no_name] if empty.
			inline name_t last() const noexcept { return !Value.empty()
				? Value[ Value.size() - 1 ] : no_name; }


			//* Get path with first element removed
			inline path chopFirst() const noexcept { return *this ? path(
				Value.begin() + 1, Value.end() ) : path(); }

			//* Get path with last element removed
			inline path chopLast() const noexcept { return *this ? path(
				Value.begin(), Value.end() - 1 ) : path(); }
			

			//* Get path as a string
			inline string str() const noexcept {
				string s; for( auto name : Value ) { if( !s.empty() )
					s += "/"; s += *name; } return s; }


			//* Comparison
			int compare( const path& other ) const noexcept;

			inline bool operator<( const path& other ) const noexcept {
				return compare( other ) < 0; }
			inline bool operator<=( const path& other ) const noexcept {
				return compare( other ) <= 0; }
			inline bool operator>( const path& other ) const noexcept {
				return compare( other ) > 0; }
			inline bool operator>=( const path& other ) const noexcept {
				return compare( other ) >= 0; }
			inline bool operator==( const path& other ) const noexcept {
				return compare( other ) == 0; }
			inline bool operator!=( const path& other ) const noexcept {
				return compare( other ) != 0; }



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			//* Set the path
			inline path& operator=( const path& other ) {
				Value = other.Value; return *this; }

			//* Take ownership of another path's value
			inline path& operator=( path&& other ) noexcept {
				Value = std::move( other.Value ); return *this; }

			inline path& operator=( std::initializer_list<name_t> names ) {
				for( auto name : names ) Value.push_back( name );
				return *this; }

			//* Clear the path
			inline void clear() noexcept { Value.clear(); }

			//* Add a name to the end of the path
			//* Will not add [eon::no_name], but also not report it!
			inline path& add( name_t name ) {
				if( name != no_name ) Value.push_back( name ); return *this; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			std::vector<name_t> Value;
		};
	}
}
