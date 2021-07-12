#pragma once
#include "Token.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon Token Parser Class - eon::tokenparser

	  The token parser takes possession of a 'std::vector<[eon::token]>' object
	  and allows you to scan forwards and backwards as you move through the
	  tokens, determining what is what and enable you to interpret them into a
	  specific context.
	**************************************************************************/
	class tokenparser
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		//* Default construction, void parser
		tokenparser() = default;

		//* Construct parser for a specific set of tokens
		//* NOTE: The ownership of the tokens is transferred to the parser.
		//*       When the parser is done, they can be reclaimed.
		inline tokenparser( std::vector<token>&& tokens ) noexcept {
			Tokens = std::move( tokens ); }

		//* Cannot copy-construct a token parser
		tokenparser( const tokenparser& ) = delete;

		//* Take ownership of the data of another token parser
		inline tokenparser( tokenparser&& other ) noexcept {
			*this = std::move( other ); }

		//* Default destruction
		virtual ~tokenparser() = default;




		/**********************************************************************
		  Modifier Methods
		**********************************************************************/

		//* Take ownership of the data of another token parser
		inline tokenparser& operator=( tokenparser&& other ) noexcept {
			Tokens = std::move( other.Tokens ); Cur = other.Cur; other.Cur = 0;
			return *this; }

		//* Reclaim the tokens (makes the parser void)
		inline std::vector<token>&& reclaim() noexcept {
			Cur = 0; return std::move( Tokens ); }


		//* Move current [eon::token] one step forward
		//* Returns true if still at a valid token, false if reached the end.
		inline bool forward( size_t steps = 1 ) noexcept { if( Tokens.size()
			- Cur >= steps ) Cur += steps; else Cur = Tokens.size();
			return Cur < Tokens.size(); }

		//* Move current [eon::token] on step backward
		//* Returns true if the move was successful, false if already at the
		//* beginning of the vector.
		inline bool backward( size_t steps = 1 ) noexcept { if( Cur == 0 )
			return false; else if( Cur >= steps ) Cur -= steps; else Cur = 0;
			return true; }


		//* Skip to next [eon::token] if current is spaces
		//* Returns true if skipped
		inline bool skipSpaces() noexcept { if( *this && current().space() ) {
			++Cur; return true; } return false; }

		//* Skip to the end of the current [eon::token]'s line
		inline void skipToEol() noexcept {
			for( ; *this && !current().newline(); ++Cur ); }

		//* Skip to the start of the next line
		inline void skipPastEol() noexcept { for( ; *this; ++Cur ) { if(
			current().newline() ) { ++Cur; break; } } }

		//* Set position of (new) current [eon::token]
		inline void pos( size_t pos ) noexcept { Cur = pos; }




		/**********************************************************************
		  Read-only Methods
		**********************************************************************/

		//* Check if there is a valid 'current [eon::token]'
		inline operator bool() const noexcept { return Cur < Tokens.size(); }

		//* Get current [eon::token]
		inline const token& current() const { return Tokens[ Cur ]; }

		//* Check if an [eon::token] exists up to several steps ahead from the
		//* current
		inline bool exists(size_t steps_ahead = 1) const noexcept {
			return Cur + steps_ahead < Tokens.size(); }

		//* Get a [eon::token] ahead of current
		inline const token& ahead( size_t steps_ahead = 1 ) const {
			return at( Cur + steps_ahead ); }

		//* Get [eon::token] at specified 'pos'ition in the vector
		inline const token& at( size_t pos ) const { return Tokens[ pos ]; }

		//* Get total number of tokens
		inline size_t size() const noexcept { return Tokens.size(); }

		//* Get [eon::token] immediately prior to current
		inline const token& prior() const { return Tokens[ Cur - 1 ]; }

		//* Get last [eon::token]
		inline const token& last() const {
			return Tokens[ Tokens.size() - 1 ]; }

		//* Get position of first [eon::token] on current token's line - use
		//* last line if reached the end
		size_t lineStart() const;


		//* Match a sequence of elements
		//* Use "*" to match anything
		//* Use "*<something>" to match element ending with "<something>"
		//* Use "<something>*" to match element beginning with "<something>"
		bool match( const std::initializer_list<eon::string>& elements )
			const noexcept;


		//* Get position of current [eon::token]
		inline size_t pos() const noexcept { return Cur; }




	private:
		std::vector<token> Tokens;
		size_t Cur{ 0 };
	};
};
