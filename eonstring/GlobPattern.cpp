#include "GlobPattern.h"
#include "Locale.h"
#include <eoninlinetest/InlineTest.h>


namespace eon
{
	EON_TEST( globpattern, globpattern, default,
		EON_EQ( "", globpattern().pattern() ) );

	EON_TEST_3STEP( globpattern, globpattern, copy_1,
		globpattern source( "abc" ),
		globpattern obj( source ),
		EON_EQ( "abc", source.pattern() ) );
	EON_TEST_3STEP( globpattern, globpattern, copy_2,
		globpattern source( "abc" ),
		globpattern obj( source ),
		EON_EQ( "abc", obj.pattern() ) );

	EON_TEST_3STEP( globpattern, globpattern, move_1,
		globpattern source( "abc" ),
		globpattern obj( std::move( source ) ),
		EON_EQ( "", source.pattern() ) );
	EON_TEST_3STEP( globpattern, globpattern, move_2,
		globpattern source( "abc" ),
		globpattern obj( std::move( source ) ),
		EON_EQ( "abc", obj.pattern() ) );

	EON_TEST( globpattern, globpattern, string,
		EON_EQ( "abc", globpattern( string( "abc" ) ).pattern() ) );
	EON_TEST( globpattern, globpattern, cstring,
		EON_EQ( "abc", globpattern( "abc" ).pattern() ) );




	EON_TEST_3STEP( globpattern, operator_asgn, copy_1,
		globpattern source( "abc" ),
		globpattern obj = source,
		EON_EQ( "abc", source.pattern() ) );
	EON_TEST_3STEP( globpattern, operator_asgn, copy_2,
		globpattern source( "abc" ),
		globpattern obj = source,
		EON_EQ( "abc", obj.pattern() ) );

	EON_TEST_3STEP( globpattern, operator_asgn, move_1,
		globpattern source( "abc" ),
		globpattern obj = std::move( source ),
		EON_EQ( "", source.pattern() ) );
	EON_TEST_3STEP( globpattern, operator_asgn, move_2,
		globpattern source( "abc" ),
		globpattern obj = std::move( source ),
		EON_EQ( "abc", obj.pattern() ) );

	EON_TEST( globpattern, operator_asgn, string,
		EON_EQ( "abc", globpattern( string( "abc" ) ).pattern() ) );
	EON_TEST( globpattern, operator_asgn, cstring,
		EON_EQ( "abc", globpattern( "abc" ).pattern() ) );

	EON_TEST( globpattern, pattern, empty,
		EON_EQ( "", globpattern().pattern() ) );
	EON_TEST( globpattern, pattern, non_empty,
		EON_EQ( "abc", globpattern( "abc" ).pattern() ) );

	EON_TEST( globpattern, empty, true,
		EON_TRUE( globpattern().empty() ) );
	EON_TEST( globpattern, empty, false,
		EON_FALSE( globpattern( " " ).empty() ) );


	EON_TEST_3STEP( globpattern, doNotMatch, basic,
		globpattern obj( "abc" ),
		obj.doNotMatch( { 'a', 'b', 'c' } ),
		EON_EQ( 3, obj.charsNotMatched().size() ) );

	EON_NO_TEST( globpattern, matchAll );
	EON_NO_TEST( globpattern, charsNotMatched );


	EON_TEST_2STEP( globpattern, ignoreCase, default,
		globpattern obj,
		EON_FALSE( obj.ignoringCase() ) );
	EON_TEST_3STEP( globpattern, ignoreCase, yes,
		globpattern obj,
		obj.ignoreCase(),
		EON_TRUE( obj.ignoringCase() ) );
	EON_TEST_3STEP( globpattern, ignoreCase, no,
		globpattern obj,
		obj.ignoreCase( false ),
		EON_FALSE( obj.ignoringCase() ) );




	EON_TEST( globpattern, match, empty,
		EON_TRUE( globpattern().match( "" ) ) );
	EON_TEST( globpattern, match, star_only_empty,
		EON_TRUE( globpattern( "*" ).match( "" ) ) );
	EON_TEST( globpattern, match, star_only_nonempty,
		EON_TRUE( globpattern( "*" ).match( "abc" ) ) );
	EON_TEST( globpattern, match, qmark_only_empty,
		EON_FALSE( globpattern( "?" ).match( "" ) ) );
	EON_TEST( globpattern, match, qmark_only_singleton,
		EON_TRUE( globpattern( "?" ).match( "a" ) ) );
	EON_TEST( globpattern, match, qmark_only_multiple,
		EON_FALSE( globpattern( "?" ).match( "abc" ) ) );
	EON_TEST( globpattern, match, starting_star,
		EON_TRUE( globpattern( "*cdef" ).match( "abcdef" ) ) );
	EON_TEST( globpattern, match, middle_star,
		EON_TRUE( globpattern( "ab*ef" ).match( "abcdef" ) ) );
	EON_TEST( globpattern, match, ending_star,
		EON_TRUE( globpattern( "abcd*" ).match( "abcdef" ) ) );
	EON_TEST( globpattern, match, starting_qmark,
		EON_TRUE( globpattern( "?bcdef" ).match( "abcdef" ) ) );
	EON_TEST( globpattern, match, middle_qmark,
		EON_TRUE( globpattern( "ab?def" ).match( "abcdef" ) ) );
	EON_TEST( globpattern, match, ending_qmark,
		EON_TRUE( globpattern( "abcde?" ).match( "abcdef" ) ) );
	EON_TEST( globpattern, match, mix_true,
		EON_TRUE( globpattern( "a*d?f*j??m" ).match( "abcdefghijklm" ) ) );
	EON_TEST( globpattern, match, mix_false,
		EON_FALSE( globpattern( "a*d?f*j??m" ).match( "abcdefghijkl" ) ) );
	EON_TEST( globpattern, match, no_globs,
		EON_TRUE( globpattern( "abcdef" ).match( "abcdef" ) ) );
	EON_TEST( globpattern, match, short_pattern,
		EON_FALSE( globpattern( "abc" ).match( "abcdef" ) ) );
	EON_TEST( globpattern, match, no_globs_casediff,
		EON_FALSE( globpattern( "abcdef" ).match( "aBcdef" ) ) );
	EON_TEST_3STEP( globpattern, match, no_globs_casediff_icase,
		globpattern obj( "abcdef" ),
		obj.ignoreCase(),
		EON_TRUE( obj.match( "aBcdef" ) ) );
	EON_TEST( globpattern, match, horror_pattern_true,
		EON_TRUE( globpattern( "a*a*a*a*a*a*a*a*a*a" ).match( string( index_t( 100 ), char_t( 'a' ) ) ) ) );
	EON_TEST( globpattern, match, horror_pattern_false,
		EON_FALSE( globpattern( "a*a*a*a*a*a*a*a*a*a*b" ).match( string( index_t( 100 ), char_t( 'a' ) ) ) ) );

	EON_TEST( globpattern, match, highToLow_star_only_nonempty,
		EON_TRUE( globpattern( "*" ).match( substring( "abc" ).highToLow() ) ) );
	EON_TEST( globpattern, match, highToLow_qmark_only_singleton,
		EON_TRUE( globpattern( "?" ).match( substring( "a" ).highToLow() ) ) );
	EON_TEST( globpattern, match, highToLow_qmark_only_multiple,
		EON_FALSE( globpattern( "?" ).match( substring( "abc" ).highToLow() ) ) );
	EON_TEST( globpattern, match, highToLow_starting_star,
		EON_TRUE( globpattern( "*dcba" ).match( substring( "abcdef" ).highToLow() ) ) );
	EON_TEST( globpattern, match, highToLow_middle_star,
		EON_TRUE( globpattern( "fe*ba" ).match( substring( "abcdef" ).highToLow() ) ) );
	EON_TEST( globpattern, match, highToLow_ending_star,
		EON_TRUE( globpattern( "fedc*" ).match( substring( "abcdef" ).highToLow() ) ) );
	EON_TEST( globpattern, match, highToLow_starting_qmark,
		EON_TRUE( globpattern( "?edcba" ).match( substring( "abcdef" ).highToLow() ) ) );
	EON_TEST( globpattern, match, highToLow_middle_qmark,
		EON_TRUE( globpattern( "fe?cba" ).match( substring( "abcdef" ).highToLow() ) ) );
	EON_TEST( globpattern, match, highToLow_ending_qmark,
		EON_TRUE( globpattern( "fedcb?" ).match( substring( "abcdef" ).highToLow() ) ) );
	EON_TEST( globpattern, match, highToLow_mix_true,
		EON_TRUE( globpattern( "m*j?h*d??a" ).match( substring( "abcdefghijklm" ).highToLow() ) ) );
	EON_TEST( globpattern, match, highToLow_mix_false,
		EON_FALSE( globpattern( "m*j?h*d??a" ).match( substring( "bcdefghijklm" ).highToLow() ) ) );
	EON_TEST( globpattern, match, highToLow_no_globs,
		EON_TRUE( globpattern( "fedcba" ).match( substring( "abcdef" ).highToLow() ) ) );
	EON_TEST( globpattern, match, highToLow_no_globs_casediff,
		EON_FALSE( globpattern( "fedcba" ).match( substring( "aBcdef" ).highToLow() ) ) );
	EON_TEST_3STEP( globpattern, match, highToLow_no_globs_casediff_icase,
		globpattern obj( "fedcba" ),
		obj.ignoreCase(),
		EON_TRUE( obj.match( substring( "aBcdef" ).highToLow() ) ) );


	EON_TEST_2STEP( globpattern, findFirst, no_globs_true,
		string obj = "axybxycxyd",
		EON_EQ( obj.substr( 1, 2 ), globpattern( "xy" ).findFirst( obj ) ) );
	EON_TEST_2STEP( globpattern, findFirst, no_globs_false,
		string obj = "axybxycxyd",
		EON_FALSE( globpattern( "xyz" ).findFirst( obj ) ) );
	EON_TEST_2STEP( globpattern, findFirst, starting_star,
		string obj( "alpha beta gamma delta epsilon" ),
		EON_EQ( obj.substr( obj.begin(), obj.begin() + 10 ), globpattern( "*ta" ).findFirst( obj ) ) );
	EON_TEST_2STEP( globpattern, findFirst, middle_star,
		string obj( "alpha beta gamma delta epsilon" ),
		EON_EQ( obj.substr( 11, 2 ), globpattern( "g*a" ).findFirst( obj ) ) );
	EON_TEST_2STEP( globpattern, findFirst, ending_star,
		string obj( "alpha beta gamma delta epsilon" ),
		EON_EQ( obj.substr( obj.begin() + 11 ), globpattern( "g*" ).findFirst( obj ) ) );
	EON_TEST_2STEP( globpattern, findFirst, starting_qmark,
		string obj( "alpha beta gamma delta epsilon" ),
		EON_EQ( obj.substr( 7, 3 ), globpattern( "?ta" ).findFirst( obj ) ) );
	EON_TEST_2STEP( globpattern, findFirst, middle_qmark,
		string obj( "alpha beta gamma delta epsilon" ),
		EON_EQ( obj.substr( 11, 3 ), globpattern( "g?m" ).findFirst( obj ) ) );
	EON_TEST_2STEP( globpattern, findFirst, ending_qmark,
		string obj( "alpha beta gamma delta epsilon" ),
		EON_EQ( obj.substr( 11, 2 ), globpattern( "g?" ).findFirst( obj ) ) );


	EON_TEST_2STEP( globpattern, findLast, no_globs_true,
		string obj = "axybxycxyd",
		EON_EQ( obj.substr( 7, 2 ), globpattern( "xy" ).findLast( obj ) ) );
	EON_TEST_2STEP( globpattern, findLast, no_globs_false,
		string obj = "axybxycxyd",
		EON_FALSE( globpattern( "xyz" ).findLast( obj ) ) );
	EON_TEST_2STEP( globpattern, findLast, starting_star,
		string obj( "alpha beta gamma delta epsilon" ),
		EON_EQ( obj.substr( obj.begin(), obj.end() - 8 ), globpattern( "*ta" ).findLast( obj ) ) );
	EON_TEST_2STEP( globpattern, findLast, middle_star,
		string obj( "alpha beta gamma delta epsilon" ),
		EON_EQ( obj.substr( 11, 11 ), globpattern( "g*a" ).findLast( obj ) ) );
	EON_TEST_2STEP( globpattern, findLast, ending_star,
		string obj( "alpha beta gamma delta epsilon" ),
		EON_EQ( obj.substr( obj.begin() + 11 ), globpattern( "g*" ).findLast( obj ) ) );
	EON_TEST_2STEP( globpattern, findLast, starting_qmark,
		string obj( "alpha beta gamma delta epsilon" ),
		EON_EQ( obj.substr( 19, 3 ), globpattern( "?ta" ).findLast( obj ) ) );
	EON_TEST_2STEP( globpattern, findLast, middle_qmark,
		string obj( "alpha beta gamma delta epsilon" ),
		EON_EQ( obj.substr( 11, 3 ), globpattern( "g?m" ).findLast( obj ) ) );
	EON_TEST_2STEP( globpattern, findLast, ending_qmark,
		string obj( "alpha beta gamma delta epsilon" ),
		EON_EQ( obj.substr( 11, 2 ), globpattern( "g?" ).findLast( obj ) ) );




	struct _MatchArgs
	{
		inline _MatchArgs( string_iterator input, string_iterator pattern, bool reverse ) {
			Input = input; Pattern = pattern; Reverse = reverse; }

		inline void newPatternLoop() noexcept {
			InputBak = Input; PatternBak = Reverse ? --Pattern : ++Pattern; Star = true; }

		inline bool matchNonStar( const std::set<char_t>& ignore_chars, bool icase ) noexcept
		{
			const locale& loc = locale::get();
			bool match = *Pattern == '?' && ignore_chars.find( *Input ) == ignore_chars.end();
			if( !match )
				match = icase ? (
					loc.toLower( static_cast<wchar_t>( *Pattern ) ) == loc.toLower( static_cast<wchar_t>( *Input ) ) ) : (
						*Pattern == *Input );
			if( match )
				Star = false;
			return match;
		}

		inline void recordMatchedChar() noexcept { ++Input; ++Pattern; }
		inline void recordRMatchedChar() noexcept { --Input; if( Reverse ) --Pattern; else ++Pattern; }

		inline bool deadEnd( const std::set<char_t>& ignore_chars ) const noexcept {
			return !PatternBak.hasSource() || ignore_chars.find( *InputBak ) != ignore_chars.end(); }

		inline void backTrack() noexcept { Input = ++InputBak; Pattern = PatternBak; }
		inline void rBackTrack() noexcept { Input = --InputBak; Pattern = PatternBak; }

		string_iterator Input, Pattern, InputBak, PatternBak;
		bool Reverse{ false };
		bool Star{ false };
	};

	string_iterator globpattern::_matchLowToHigh(
		const substring& pattern, const substring& area, bool search ) const noexcept
	{
		_MatchArgs args( area.begin(), pattern.begin(), false );
		while( args.Input < area.end() )
		{
			if( search && !args.Star && args.Pattern == pattern.end() )
				return args.Input;
			else if( *args.Pattern == '*' )
				args.newPatternLoop();
			else if( args.matchNonStar( IgnoreChars, IgnoreCase ) )
				args.recordMatchedChar();
			else
			{
				if( args.deadEnd( IgnoreChars ) )
					return string_iterator();
				args.backTrack();
			}
		}
		while( *args.Pattern == '*' )
			++args.Pattern;
		if( args.Pattern < pattern.end() || args.Input < area.end() )
			return string_iterator();
		return args.Input;
	}

	string_iterator globpattern::_matchHighToLow(
		const substring& pattern, const substring& area, bool search ) const noexcept
	{
		_MatchArgs args( area.begin(), pattern.begin(), pattern.isHighToLow() );
		while( args.Input > area.end() )
		{
			if( search && !args.Star && args.Pattern == pattern.end() )
				return args.Input;
			else if( *args.Pattern == '*' )
				args.newPatternLoop();
			else if( args.matchNonStar( IgnoreChars, IgnoreCase ) )
				args.recordRMatchedChar();
			else
			{
				if( args.deadEnd( IgnoreChars ) )
					return string_iterator();
				args.rBackTrack();
			}
		}
		while( *args.Pattern == '*' )
		{
			if( pattern.isHighToLow() )
				--args.Pattern;
			else
				++args.Pattern;
		}
		if( args.Input > area.end() )
			return string_iterator();
		if( pattern.isLowToHigh() && args.Pattern < pattern.end() )
			return string_iterator();
		if( pattern.isHighToLow() && args.Pattern > pattern.end() )
			return string_iterator();
		return args.Input;
	}


	substring globpattern::_findFirst( const substring& area ) const noexcept
	{
		auto min_length = Value.numChars() - Value.count( '*' );
		auto short_end = area.end() - min_length;
		for( string_iterator i = area.begin(); i < short_end; ++i )
		{
			auto end = _matchLowToHigh( Value.substr(), substring( i, area.end() ), true );
			if( end.hasSource() )
				return substring( i, end );
		}
		return substring();
	}

	substring globpattern::_findLast( const substring& area ) const noexcept
	{
		auto min_length = Value.numChars() - Value.count( '*' );
		auto short_end = area.end() + min_length;
		for( string_iterator i = area.begin(); i > short_end; --i )
		{
			auto end = _matchHighToLow( Value.substr().highToLow(), substring( i, area.end() ), true );
			if( end.hasSource() )
				return substring( i, end );
		}
		return substring();
	}
}
