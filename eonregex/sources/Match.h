#pragma once
#include "RxDefs.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::rx' namespace enclosed special elements for Eon regular
	  expressions
	**************************************************************************/
	namespace rx
	{
		/**********************************************************************
		  Eon Regular Expression Match Class - eon::rx::match

		  The result of a regular expressions match or search.
		**********************************************************************/
		class match
		{
		public:
			/******************************************************************
			  Construction
			******************************************************************/

			//* Default constructor for empty/failed/false match
			match() = default;

			//* Copy the 'other' match
			inline match( const match& other ) { *this = other; }

			//* Take ownership of the 'other' match
			inline match( match&& other ) noexcept {
				*this = std::move( other ); }

			//* Construct from a map of [eon::name_t] to [eon::substring]
			inline match( std::map<name_t, substring>& captures ) noexcept {
				Captures = std::move( captures ); }




			/******************************************************************
			  Modifier Methods
			******************************************************************/

			//* Copy the 'other' match
			inline match& operator=( const match& other ) {
				Captures = other.Captures; return *this; }

			//* Take ownership of the details of the 'other' match
			inline match& operator=( match&& other ) noexcept {
				Captures = std::move( other.Captures ); return *this; }


			//* Clear this match
			inline void clear() noexcept { Captures.clear(); }




			/******************************************************************
			  Read-only Methods
			******************************************************************/

			//* Check if there was a match
			inline operator bool() const noexcept { return !Captures.empty(); }

			//* Get number of captures (including the entire match)
			inline size_t size() const noexcept { return Captures.size(); }

			//* Get the entire match
			inline substring all() const noexcept { return group( no_name ); }

			//* Get a capture
			inline substring group( const name_t name ) const noexcept {
				auto found = Captures.find( name ); return found
					!= Captures.end() ? found->second : substring(); }




		private:
			std::map<name_t, substring> Captures;

			friend class RegEx;
		};
	}
}