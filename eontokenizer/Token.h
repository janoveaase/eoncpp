#pragma once
#include <eonsource/SourceRef.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Token Categories

	  These are similar to [charcat] character categories, but more condensed.
	**************************************************************************/
	enum class tokencat
	{
		//* The 'letter' token category includes 'letter_lowercase',
		//* 'letter_modifier', 'letter_other', 'letter_titlecase', and
		//* 'letter_uppercase' character categories.
		//* A token of this category can contain multiple different characters.
		letter,				

		//* The 'number' token category includes only the custom
		//* 'number_ascii_digit' characater category.
		//* A token of this category can contain multiple different characters.
		number,

		//* The 'extended_number' token category includes
		//* 'number_decimal_digit' excluding 'number_ascii_digit')
		//* A token of this category can contain multiple different characters.
		extended_number,

		//* The 'other_number' token category includes 'number_letter' and
		//* 'number_other' characater categories.
		//* A token of this category can contain multiple characters but only
		//* of the same value.
		other_number,

		//* The 'newline' token category includes the newline character and
		//* only one per token.
		newline,

		//* The 'space' token category includes the 'separator_space' character
		//* category (except newline).
		//* A token of this category can contain multiple characters but only
		//* of the same value.
		space,

		//* The 'separator' token category includes 'separator_line' and
		//* separator_paragraph' character categories.
		//* A token of this category can contain multiple characters but only
		//* of the same value.
		separator,

		//* The 'punctuation' token category includes 'punctuation_connector',
		//* 'punctuation_dash', 'punctuation_close', 'punctuation_final_quote',
		//* 'punctuation_initial_quote', 'punctuation_other', and
		//* 'punctuation_open' characater categories.
		//* A token of this category can contain multiple characters but only
		//* of the same value.
		punctuation,

		//* The 'symbol' token category includes 'symbol_currency',
		//* 'symbol_modifier', 'symbol_math', and 'symbol_other' character
		//* categories.
		//* A token of this category can contain multiple characters but only
		//* of the same value.
		symbol,

		//* The 'mark' token category includes 'mark_spacing_combining',
		//* 'mark_enclosing', and 'mark_nonspacing' character categories.
		//* A token of this category can contain multiple characters but only
		//* of the same value.
		mark,

		//* The 'other' token category includes 'other_control',
		//* 'other_format', 'other_private_use', and 'other_surrogate'
		//* character categories.
		//* A token of this category can contain multiple characters but only
		//* of the same value.
		other,

		//* Undefined token category
		undef
	};

	/**************************************************************************
	  Eon Token Class - eon::token

	  A token is a reference to a sequence of one or more characters in an
	  [eon::source] that blongs to a specific [eon::tokencat] token category.
	  All tokens have an [eon::sourceref] and a token category identifier.
	**************************************************************************/
	class token
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		//* Default construction, invalid token
		token() = default;

		//* Copy the 'other' token
		inline token( const token& other ) noexcept { *this = other; }

		//* Construct token for a source reference and character category
		inline token( const sourceref& source, tokencat category ) noexcept {
			Src = source; Category = category; }

		//* Construct token for a 'source', with 'line' number, and 'start'
		//* and 'end' iterator positions
		inline token( const source& source, size_t line,
			const string_iterator& start, const string_iterator& end,
			tokencat category ) noexcept { Src = sourceref( source, sourcepos(
				line, substring( start, end ) ) ); Category = category; }

		//* Default destructor
		~token() = default;




		/**********************************************************************
		  Modifier Methods
		**********************************************************************/

		//* Copy the 'other' token
		inline token& operator=( const token& other ) noexcept {
			Src = other.Src; Category = other.Category; return *this; }




		/**********************************************************************
		  Read-only Methods
		**********************************************************************/

		//* Access the full source reference of the token
		inline const sourceref& source() const noexcept { return Src; }

		//* Access the [eon::tokencat] token category of the token
		inline tokencat category() const noexcept { return Category; }

		//* Check for an explicit [eon::tokencat] category
		inline bool letter() const noexcept {
			return Category == tokencat::letter; }
		inline bool number() const noexcept {
			return Category == tokencat::number; }
		inline bool extendedNumber() const noexcept {
			return Category == tokencat::extended_number; }
		inline bool otherNumber() const noexcept {
			return Category == tokencat::other_number; }
		inline bool newline() const noexcept {
			return Category == tokencat::newline; }
		inline bool space() const noexcept {
			return Category == tokencat::space; }
		inline bool separator() const noexcept {
			return Category == tokencat::separator; }
		inline bool punctuation() const noexcept {
			return Category == tokencat::punctuation; }
		inline bool symbol() const noexcept {
			return Category == tokencat::symbol; }
		inline bool mark() const noexcept {
			return Category == tokencat::mark; }
		inline bool other() const noexcept {
			return Category == tokencat::other; }

		//* Check if this token's characters matches exactly those of the
		//* specified substring
		inline bool match( const substring& substr ) const noexcept {
			return substr.numChars() == Src.pos().numChars()
				&& Src.str().compare( substr ) == 0; }

		//* Check if this token's characters matches exactly those of the
		//* specified C-string
		inline bool match( const char* cstr ) const noexcept {
			return match( substring( cstr ) ); }

		//* Check if this token's character sequence starts with the same
		//* characters as the specified substring
		inline bool startsWith( const substring& substr ) const noexcept {
			return Src.str().startsWith( substr ); }

		//* Check if this token's character sequence ends with the same
		//* characters as the specified substring
		inline bool endsWith( const substring& substr ) const noexcept {
			return Src.str().endsWith( substr ); }

		//* Check if this token contains only this character
		inline bool containsOnly( char_t c ) const noexcept {
			auto str = Src.str(); return !str.empty() && !str.findFirstNotOf(
				string( c ).substr() ); }

		//* Get token substring
		inline substring substr() const noexcept { return Src.str(); }


	private:
		sourceref Src;
		tokencat Category{ tokencat::undef };
	};
};
