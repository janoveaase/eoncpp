#pragma once
#include "RxDefs.h"


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// The 'eon::rx' namespace enclosed special elements for Eon regular
	// expressions
	//
	namespace rx
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Eon Regular Expression Match Class - eon::rx::match
		//
		// The result of a regular expressions match or search.
		//
		class match
		{
		public:
			///////////////////////////////////////////////////////////////////
			//
			// Construction
			//

			// Default constructor for empty/failed/false match
			match() = default;

			// Copy the 'other' match
			inline match( const match& other ) { *this = other; }

			// Take ownership of the 'other' match
			inline match( match&& other ) noexcept { *this = std::move( other ); }

			// Construct from a map of [eon::name_t] to [eon::substring]
			inline match( captures_t* captures ) noexcept { Captures = captures; }

			inline virtual ~match() { clear(); }




			///////////////////////////////////////////////////////////////////
			//
			// Modifier Methods
			//

			// Copy the 'other' match
			inline match& operator=( const match& other ) {
				clear(); if( other.Captures ) Captures = new captures_t( *other.Captures ); return *this; }

			// Take ownership of the details of the 'other' match
			inline match& operator=( match&& other ) noexcept {
				clear(); if( other.Captures ) { Captures = other.Captures; other.Captures = nullptr; } return *this; }


			// Clear this match
			inline void clear() noexcept { if( Captures ) { delete Captures; Captures = nullptr; } }




			///////////////////////////////////////////////////////////////////
			//
			// Read-only Methods
			//

			// Check if there was a match
			inline operator bool() const noexcept { return Captures; }

			// Get number of captures (including the entire match)
			inline size_t size() const noexcept { return Captures ? Captures->size() : 0; }

			// Get the entire match
			inline substring all() const noexcept { return group( name_complete ); }

			// Get a capture
			inline substring group( const name_t name ) const noexcept { if( !Captures ) return substring();
				auto found = Captures->find( name ); return found != Captures->end() ? found->second : substring(); }

			using iterator = captures_t::const_iterator;
			inline iterator begin() const noexcept { return Captures ? Captures->begin() : iterator(); }
			inline iterator end() const noexcept { return Captures ? Captures->end() : iterator(); }




		private:
			captures_t* Captures{ nullptr };

			friend class RegEx;
		};
	}
}
