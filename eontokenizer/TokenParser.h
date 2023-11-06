#pragma once
#include "Token.h"


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// Eon Token Parser Class - eon::TokenParser
	//
	// The token parser takes posession of a raw 'std::vector<[eon::Token]>'
	// object (created by [eon::Tokenizer]), and allows scanning forward and
	// backward as while iterating the tokens, allowing for a more precise
	// identification of token types and sequences based on their context.
	//
	class TokenParser
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		// Default construction, void parser.
		TokenParser() = default;

		// Construct parser for a specific set of tokens.
		// NOTE: The ownership of the tokens is transferred to the parser.
		//       When the parser is done, they can be reclaimed if necessary.
		inline TokenParser( std::vector<Token>&& tokens ) noexcept { Tokens = std::move( tokens ); }

		// Cannot copy-construct a token parser.
		TokenParser( const TokenParser& ) = delete;

		// Take ownership of the data of another token parser.
		inline TokenParser( TokenParser&& other ) noexcept { *this = std::move( other ); }

		// Default destruction.
		virtual ~TokenParser() = default;




		///////////////////////////////////////////////////////////////////////
		//
		// Modifier Methods
		//

		// Discard current data and take ownership of those of another token parser.
		inline TokenParser& operator=( TokenParser&& other ) noexcept {
			Tokens = std::move( other.Tokens ); View = other.View; other.View = 0; return *this; }

		// Reclaim the tokens (makes the parser void).
		inline std::vector<Token>&& reclaim() noexcept { View = 0; return std::move( Tokens ); }


		// Move 'token view' one or more steps forward.
		// Returns true unless zero or too many steps.
		inline bool forward( size_t steps = 1 ) noexcept {
			if( Tokens.size() - View < steps ) return false; View += steps; return true; }

		// Move 'token view' one or more steps backward.
		// Returns true unless zero or too many steps.
		inline bool backward( size_t steps = 1 ) noexcept { if( View < steps ) return false; View -= steps; return true; }


		// Move 'token view' to the next token if the currently viewed token is of the named type.
		// Returns true if moved.
		inline bool forwardIf( name_t type ) noexcept { return !atEnd() && viewed().is( type ) && forward(); }

		// Move 'token view' to the first newline token (end of line in source).
		inline void moveToEol() noexcept { for( ; !atEnd() && !viewed().is( name_newline ); ++View ); }

		// Move 'token view' to the first token on the next line in the source.
		// NOTE: This can be another newline if the next line is empty!
		inline void movePastEol() noexcept {
			for( ; !atEnd(); ++View ) { if( viewed().is( name_newline ) ) { ++View; break; } } }

		// Move 'token view' to the token at the specified position within the tokens vector.
		// Returns true unless the new view position is out of range!
		// NOTE: Setting to one past the last element is legal!
		inline bool setView( size_t pos ) noexcept { if( pos > Tokens.size() ) return false; View = pos; return true; }




		///////////////////////////////////////////////////////////////////////
		//
		// Read-only Methods
		//

//		// Check if there 'token view' is for a token in the tokens vector.
//		inline operator bool() const noexcept { return View < Tokens.size(); }

		// Check if at the end of the token sequence.
		inline bool atEnd() const noexcept { return View >= Tokens.size(); }

		// Get currently viewed token.
		// WARNING: The 'token view' must be less than [size()]!
		inline const Token& viewed() const { return Tokens[ View ]; }

		// Check if there is a token at the specified number of steps forward
		// (positive argument) or backward (negative argument) of the current
		// 'token view'.
		inline bool exists( int steps = 1 ) const noexcept {
			return steps < 0 ? static_cast<size_t>( -steps ) <= View
				: View + static_cast<size_t>( steps ) < Tokens.size(); }

		// Peek at a token a number of steps ahead of 'token view'.
		// NOTE: Use [exists(int)] to make sure there is such a token!
		inline const Token& peekAhead( size_t steps = 1 ) const { return peek( View + steps ); }

		// Peek at a token a number of steps behind of 'token view'.
		// NOTE: Use [exists(int)] to make sure there is such a token!
		inline const Token& peekBehind( size_t steps = 1 ) const { return peek( View - steps ); }

		// Peek at a token in a specific position in the tokens vector.
		// WARNING: The position must be less than [size()]!
		inline const Token& peek( size_t pos ) const { return Tokens[ pos ]; }

		// Peek at the token at the very end of the tokens vector.
		inline const Token& last() const { return Tokens[ Tokens.size() - 1 ]; }

		// Get the position of the first token that is on the same line in the source as the currently viewed.
		size_t lineStart() const;

		// Get position of currently viewed token.
		inline size_t viewedPos() const noexcept { return View; }

		// Get total number of tokens in tokens vector.
		inline size_t size() const noexcept { return Tokens.size(); }




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	EON_PRIVATE:

		std::vector<Token> Tokens;
		size_t View{ 0 };
	};
};
