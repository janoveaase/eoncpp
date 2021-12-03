#pragma once
#include "Token.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon Token Parser Class - eon::TokenParser

	  The token parser takes possession of a 'std::vector<[eon::Token]>' object
	  and allows you to scan forwards and backwards as you move through the
	  tokens, determining what is what and enable you to interpret them into a
	  specific context.
	**************************************************************************/
	class TokenParser
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		//* Default construction, void parser
		TokenParser() = default;

		//* Construct parser for a specific set of tokens
		//* NOTE: The ownership of the tokens is transferred to the parser.
		//*       When the parser is done, they can be reclaimed.
		inline TokenParser( std::vector<Token>&& tokens ) noexcept { Tokens = std::move( tokens ); }

		//* Cannot copy-construct a token parser
		TokenParser( const TokenParser& ) = delete;

		//* Take ownership of the data of another token parser
		inline TokenParser( TokenParser&& other ) noexcept { *this = std::move( other ); }

		//* Default destruction
		virtual ~TokenParser() = default;




		/**********************************************************************
		  Modifier Methods
		**********************************************************************/

		//* Take ownership of the data of another token parser
		inline TokenParser& operator=( TokenParser&& other ) noexcept {
			Tokens = std::move( other.Tokens ); Cur = other.Cur; other.Cur = 0; return *this; }

		//* Reclaim the tokens (makes the parser void)
		inline std::vector<Token>&& reclaim() noexcept { Cur = 0; return std::move( Tokens ); }


		//* Move current [eon::Token] one step forward
		//* Returns true if still at a valid token, false if reached the end.
		inline bool forward( size_t steps = 1 ) noexcept {
			if( Tokens.size() - Cur >= steps ) Cur += steps; else Cur = Tokens.size(); return Cur < Tokens.size(); }

		//* Move current [eon::Token] on step backward
		//* Returns true if the move was successful, false if already at the
		//* beginning of the vector.
		inline bool backward( size_t steps = 1 ) noexcept {
			if( Cur == 0 ) return false; else if( Cur >= steps ) Cur -= steps; else Cur = 0; return true; }


		//* Skip to next [eon::Token] if current is of the named type
		//* Returns true if skipped
		inline bool skipIf( name_t type ) noexcept { if( *this && current().is( type ) ) {
			++Cur; return true; } return false; }

		//* Skip to the end of the current [eon::Token]'s line
		inline void skipToEol() noexcept { for( ; *this && !current().is( name_newline ); ++Cur ); }

		//* Skip to the start of the next line
		inline void skipPastEol() noexcept {
			for( ; *this; ++Cur ) { if( current().is( name_newline ) ) { ++Cur; break; } } }

		//* Set position of (new) current [eon::Token]
		inline void pos( size_t pos ) noexcept { Cur = pos; }




		/**********************************************************************
		  Read-only Methods
		**********************************************************************/

		//* Check if there is a valid 'current [eon::Token]'
		inline operator bool() const noexcept { return Cur < Tokens.size(); }

		//* Get current [eon::Token]
		inline const Token& current() const { return Tokens[ Cur ]; }

		//* Check if an [eon::Token] exists up to several steps ahead from the
		//* current
		inline bool exists(size_t steps_ahead = 1) const noexcept { return Cur + steps_ahead < Tokens.size(); }

		//* Get a [eon::Token] ahead of current
		inline const Token& ahead( size_t steps_ahead = 1 ) const { return at( Cur + steps_ahead ); }

		//* Get [eon::Token] at specified 'pos'ition in the vector
		inline const Token& at( size_t pos ) const { return Tokens[ pos ]; }

		//* Get total number of tokens
		inline size_t size() const noexcept { return Tokens.size(); }

		//* Get [eon::Token] immediately prior to current
		inline const Token& prior() const { return Tokens[ Cur - 1 ]; }

		//* Get the last [eon::Token] (of all) in the token sequence
		inline const Token& last() const { return Tokens[ Tokens.size() - 1 ]; }

		//* Get position of first [eon::Token] on current token's line - use
		//* last line if reached the end
		size_t lineStart() const;


		//* Match a sequence of elements
		//* Use "*" to match anything
		//* Use "*<something>" to match element ending with "<something>"
		//* Use "<something>*" to match element beginning with "<something>"
		bool match( const std::initializer_list<eon::string>& elements );


		//* Get position of current [eon::Token]
		inline size_t pos() const noexcept { return Cur; }




	private:
		std::vector<Token> Tokens;
		size_t Cur{ 0 };
	};
};
