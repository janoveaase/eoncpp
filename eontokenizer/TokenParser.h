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

		//* Cannot copy- or move-construct a token parser
		tokenparser( const tokenparser& ) = delete;
		tokenparser( tokenparser&& ) = delete;

		//* Default destruction
		virtual ~tokenparser() = default;




		/**********************************************************************
		  Modifier Methods
		**********************************************************************/

		//* Reclaim the tokens (makes the parser void)
		inline std::vector<token>&& reclaim() noexcept {
			Cur = 0; return std::move( Tokens ); }


		//* Move current [eon::token] one step forward
		//* Returns true if still at a valid token, false if reached the end.
		inline bool forward() noexcept {
			if( Cur < Tokens.size() ) ++Cur; return Cur < Tokens.size(); }

		//* Move current [eon::token] on step backward
		//* Returns true if the move was successful, false if already at the
		//* beginning of the vector.
		inline bool backward() noexcept {
			if( Cur == 0 ) return false; --Cur; return true; }


		//* Skip to next [eon::token] if current is spaces
		inline void skipSpaces() noexcept {
			if( *this && token().space() ) ++Cur; }

		//* Skip to the end of the current [eon::token]'s line
		inline void skipLine() noexcept {
			for( ; *this && !token().newline(); ++Cur ); }

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

		//* Get position of first [eon::token] on current token's line
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
