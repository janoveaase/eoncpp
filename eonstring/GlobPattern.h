#pragma once
#include "String.h"


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	// Eon Glob Pattern Class - eon::globpattern.
	//
	// A glob pattern is a string in which the characters '*' and '?' means
	// zero or more arbitrary characters and exactly one arbitrary character
	// respectively. Glob patterns are typically used to match file name and
	// file types but can also be used in regular string matching. While not
	// as powerful as regular expressions, matching is less costly.
	//
	// While the simplest glob pattern will adhere stricly to the above
	// description, we can also specify that directory separators in paths
	// should not be matched by '*' or '?', that letter cases should be
	// ignored, and/or that '*' and '?' can be escaped using backspace (or a
	// custom character), thus allowing us to specifically match these
	// characters in the input string.
	//
	class globpattern
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		globpattern() = default;
		globpattern( const globpattern& ) = default;
		globpattern( globpattern&& ) noexcept = default;

		inline globpattern( string pattern ) : Value( std::move( pattern ) ) {}




		///////////////////////////////////////////////////////////////////////
		//
		// Pattern state
		//
	public:

		// Set the pattern using assignment operator
		globpattern& operator=( const globpattern& ) = default;
		globpattern& operator=( globpattern&& ) noexcept = default;
		inline globpattern& operator=( string pattern ) { Value = std::move( pattern ); return *this; }

		// Get the pattern as a string
		inline const string& pattern() const noexcept { return Value; }

		// Check if empty (no pattern).
		inline bool empty() const noexcept { return Value.empty(); }


		// Specify characters that should not be matched by '*' or '?'.
		// For example, '/' and '\\' in paths.
		// Default is to match all characters.
		inline void doNotMatch( std::initializer_list<char_t> characters ) {
			matchAll(); for( auto c : characters ) IgnoreChars.insert( c ); }

		// Specify that all characters should be matched by '*' and '?'.
		// This will effectively undo [doNotMatch] specifications.
		inline void matchAll() noexcept { IgnoreChars.clear(); }

		// Get set of characters that will not be matched by '*' and '?'.
		// The set can be empty if all are matched.
		inline const std::set<char_t>& charsNotMatched() const noexcept { return IgnoreChars; }


		// Specify that case differences should be ignored.
		// Default is to not ignore!
		inline void ignoreCase( bool ignore = true ) noexcept { IgnoreCase = ignore; }

		// Check if case is being ignored.
		inline bool ignoringCase() const noexcept { return IgnoreCase; }




		///////////////////////////////////////////////////////////////////////
		//
		// Pattern matching
		//
	public:

		// Check if the specified input string matches the pattern.
		inline bool match( const string& input ) const noexcept { return match( input.substr() ); }

		// Check if the specified input substring matches the pattern.
		// NOTE: If substring is in 'high-to-low' ordering, matching will be done in reverse!
		inline bool match( const substring& input ) const noexcept { return _match( input ); }


		// Search through the input string and return a substring for the first
		// section that matches the pattern.
		// NOTE: Will use greedy matching (trying to gobble up as much as possible for '*')!
		inline substring findFirst( const string& input ) const noexcept { return findFirst( input.substr() ); }

		// Search through the input string and return a substring for the last
		// section that matches the pattern.
		// NOTE: Will use greedy matching (trying to gobble up as much as possible for '*')!
		inline substring findLast( const string& input ) const noexcept {
			return findLast( input.substr().highToLow() ).lowToHigh(); }


		// Search through the input substring and return a substring for the
		// first section that matches the pattern.
		// NOTE: If substring is in 'high-to-low' ordering, this will be the same as [findLast]!
		// NOTE: Will use greedy matching (trying to gobble up as much as possible for '*')!
		inline substring findFirst( const substring& input ) const noexcept {
			return input.isHighToLow() ? _findLast( input ) : _findFirst( input ); }

		// Search through the input substring and return a substring for the
		// last section that matches the pattern.
		// NOTE: If substring is in 'high-to-low' ordering, this will be the same as [findLast]!
		// NOTE: Will use greedy matching (trying to gobble up as much as possible for '*')!
		inline substring findLast( const substring& input ) const noexcept {
			return input.isHighToLow() ? _findLast( input ) : _findFirst( input ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Helpers
		//
	private:

		inline bool _match( const substring& input ) const noexcept
		{
			return (
				input.isHighToLow() ? _matchHighToLow( Value.substr(), input ) : _matchLowToHigh( Value.substr(), input )
				) == input.end();
		}

		// Return iterator for end of matched section.
		string_iterator _matchLowToHigh(
			const substring& pattern, const substring& area, bool search = false ) const noexcept;

		// Return iterator for end of matched section.
		string_iterator _matchHighToLow(
			const substring& pattern, const substring& area, bool search = false ) const noexcept;

		substring _findFirst( const substring& area ) const noexcept;
		substring _findLast( const substring& area ) const noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		string Value;
		std::set<char_t> IgnoreChars;
		bool IgnoreCase{ false };
		char_t EscapeChar{ nochar };
	};
}
