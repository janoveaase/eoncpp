#pragma once
#include <eonsource/SourceRef.h>
#include <eonstring/Name.h>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon Token Class - eon::Token

	  A token is a reference to a sequence of one or more characters in an
	  [eon::source] that belongs to a specific named [eon::name_t] token
	  category - name_undef if not identified.
	  NOTE: Newline is always a single token of type 'newline' (name_newline)!
	  All tokens have an [eon::sourceref] and a token type name.
	**************************************************************************/
	class Token
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		//* Default construction, invalid token
		Token() = default;

		//* Copy the 'other' token
		inline Token( const Token& other ) noexcept { *this = other; }

		//* Construct token for a source reference and character category
		inline Token( source::Ref source, name_t type ) noexcept { Src = source; Type = type; }

		//* Default destructor
		~Token() = default;




		/**********************************************************************
		  Modifier Methods
		**********************************************************************/

		//* Copy the 'other' token
		inline Token& operator=( const Token& other ) noexcept { Src = other.Src; Type = other.Type; return *this; }

		//* Extend token's source to the given end position
		inline bool extend( source::Pos pos ) noexcept {
			return pos > Src.end() && !Src.source().beyondEnd( pos ) ? Src.end( pos ) : false; }




		/**********************************************************************
		  Read-only Methods
		**********************************************************************/

		//* Access the full source reference of the token
		inline const source::Ref& source() const noexcept { return Src; }
		inline source::Ref& source() noexcept { return Src; }

		//* Access the token type [eon::name] of the token
		inline name_t type() const noexcept { return Type; }

		//* Check for an explicit type [eon::name]
		inline bool is( name_t type ) const noexcept { return Type == type; }

		//* Check if this token's characters matches exactly those of the
		//* specified string
		inline bool match( const string& str ) const { return str.numChars() == Src.numChars() && str == Src.str(); }

		//* Check if this token's characters matches exactly those of the
		//* specified C-string
		inline bool match( const char* cstr ) const { return match( string( cstr ) ); }

		//* Check if this token's character sequence starts with the same
		//* characters as the specified substring
		inline bool startsWith( const string& str ) { return Src.str().startsWith( str ); }

		//* Check if this token's character sequence ends with the same
		//* characters as the specified substring
		inline bool endsWith( const string& str ) { return Src.str().endsWith( str ); }

		//* Check if this token contains only this character
		inline bool containsOnly( char_t c ) const noexcept {
			auto str = Src.str(); return !str.empty() && !str.findFirstNotOf( string( c ).substr() ); }

		//* Get token string
		inline string str() const noexcept { return Src.str(); }


	private:
		source::Ref Src;
		name_t Type{ no_name };
	};
};
