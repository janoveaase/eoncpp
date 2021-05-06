#pragma once
#include "sources/Graph.h"
#include "sources/Match.h"
#include "sources/RxData.h"
#include <eonexcept/Exception.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon Regular Expression Class - eon::regex

	  Regular expressions on [eon::string]s.
	**************************************************************************/
	class regex
	{
	public:
		/**********************************************************************
		  Construction
		**********************************************************************/

		//* Default constructor for empty expression
		regex() = default;

		//* Copy the 'other' expression
		inline regex( const regex& other ) { *this = other; }

		//* Take ownership of the 'other' expression
		inline regex( regex&& other ) noexcept { *this = std::move( other ); }

		//* Construct for an [eon::string] 'expression'
		//* WARNING: Will throw InvalidExpression if invalid formatting.
		//*          The exception message will contain details about what's
		//*          wrong with the 'expression' string.
		regex( const substring& expression ) {
			Raw = expression; Graph.parse( Raw ); }

		//* Construct for an std::string 'expression'
		//* WARNING: Will throw InvalidExpression if invalid formatting.
		//*          The exception message will contain details about what's
		//*          wrong with the 'expression' string.
		regex( const std::string& expression ) {
			Raw = substring( expression ); Graph.parse( Raw ); }

		//* Construct for a 'const char*' (C-string)
		//* WARNING: Will throw InvalidExpression if invalid formatting.
		//*          The exception message will contain details about what's
		//*          wrong with the 'expression' string.
		regex( const char* expression ) {
			Raw = substring( expression ); Graph.parse( Raw ); }


		//* Default destruction
		virtual ~regex() = default;




		/**********************************************************************
		  Modifier Methods
		**********************************************************************/

		//* Copy the 'other' expression
		inline regex& operator=( const regex& other ) {
			Graph = other.Graph; Raw = other.Raw; return *this; }

		//* Take ownership of the details of the 'other' expression
		inline regex& operator=( regex&& other ) noexcept { Graph = std::move(
			other.Graph ); Raw = std::move( other.Raw ); return *this; }


		//* Clear this expression
		inline void clear() noexcept { Graph.clear(); Raw.clear(); }
		



		/**********************************************************************
		  Read-only Methods
		**********************************************************************/

		//* Get the complete pattern (boundary markers and flags included)
		inline const substring& raw() const noexcept { return Raw; }




		/**********************************************************************
		  Matching and Searching
		**********************************************************************/

		//* Match against a specified [eon::substring] 'str'
		//* Returns an [eon::rx::match] object that is either 'true' if there
		//* were a match or 'false' if not. If 'true', then use
		//* [eon::rx::match::all()] to access the entire match (as an
		//* [eon::substring]) or [eon::rx::match::group(name)] to get a
		//* specific capture.
		rx::match match( const substring& str ) const;
		inline rx::match match( const string& str ) const {
			return match( str.substr() ); }
		inline rx::match match( const std::string& str ) const {
			return match( substring( str ) ); }
		inline rx::match match( const char* str ) const {
			return match( substring( str ) ); }

		//* Find the first section of the [eon::substring] that matches
		//* Returns an [eon::rx::match] object that is either 'true' if there
		//* were a match or 'false' if not. If 'true', then use
		//* [eon::rx::match::all()] to access the entire match (as an
		//* [eon::substring]) or [eon::rx::match::group(name)] to get a
		//* specific capture.
		rx::match findFirst( const substring& str ) const;
		inline rx::match findFirst( const string& str ) const {
			return match( str.substr() ); }
		inline rx::match findFirst( const std::string& str ) const {
			return match( substring( str ) ); }
		inline rx::match findFirst( const char* str ) const {
			return match( substring( str ) ); }

		//* Find the last section of the [eon::substring that matches]
		//* Returns an [eon::rx::match] object that is either 'true' if there
		//* were a match or 'false' if not. If 'true', then use
		//* [eon::rx::match::all()] to access the entire match (as an
		//* [eon::substring]) or [eon::rx::match::group(name)] to get a
		//* specific capture.
		rx::match findLast( const substring& str ) const;
		inline rx::match findLast( const string& str ) const {
			return match( str.substr() ); }
		inline rx::match findLast( const std::string& str ) const {
			return match( substring( str ) ); }
		inline rx::match findLast( const char* str ) const {
			return match( substring( str ) ); }



	private:
		rx::Graph Graph;
		substring Raw;
	};
}