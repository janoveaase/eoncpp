#pragma once
#include <eonsource/SourceRef.h>
#include <eonstring/Name.h>
#include <eoninlinetest/InlineTest.h>


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// Eon Token Class - eon::Token
	//
	// A token is a reference to a sequence of one or more characters in an
	// [eon::source] that belongs to a specific named [eon::name_t] token
	// category - name_undef if not identified.
	// NOTE: Newline is always a single token of type 'newline' (name_newline)!
	// All tokens have an [eon::sourceref] and a token type name.
	//
	class Token
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		// Default construction, invalid token.
		Token() = default;

		// Copy the 'other' token.
		inline Token( const Token& other ) noexcept { *this = other; }

		// Construct token for a source reference and character category.
		inline Token( source::Ref source, name_t type ) noexcept { Src = source; Type = type; }

		// Default destructor.
		~Token() = default;




		///////////////////////////////////////////////////////////////////////
		//
		// Modifier Methods
		//
	public:

		// Copy the 'other' token.
		inline Token& operator=( const Token& other ) noexcept { Src = other.Src; Type = other.Type; return *this; }

		// Extend token's source to the given end position.
		inline bool extend( source::Pos pos ) noexcept {
			return pos > Src.end() && Src.source().isValid( pos ) ? Src.end( pos ) : false; }

		// Extend token's source to the given end position and specify a new type.
		inline bool extendWithNewType( source::Pos pos, name_t type_name ) noexcept {
			if( extend( pos ) ) { Type = type_name; return true; } return false; }




		///////////////////////////////////////////////////////////////////////
		//
		// Read-only Methods
		//
	public:

		// Check if the token contains something.
		inline operator bool() const noexcept { return static_cast<bool>( Src ); }

		// Check if the token contains nothing.
		inline bool empty() const noexcept { return Src.empty(); }

		// Get immutable reference to the source of the token.
		inline const source::Ref& source() const noexcept { return Src; }

		// Get mutable reference to the source of the token.
		inline source::Ref& source() noexcept { return Src; }

		// Access the token type [eon::name] of the token.
		inline name_t type() const noexcept { return Type; }

		// Check for an explicit type [eon::name].
		inline bool is( name_t type ) const noexcept { return Type == type; }

		// Check for one of several implicit types.
		inline bool isOneOf( std::set<name_t> types )const noexcept { return types.find( Type ) != types.end(); }

		// Check if this token's characters matches exactly those of the specified string.
		inline bool match( const string& str ) const { return str.numChars() == Src.numChars() && str == Src.str(); }

		// Check if this token's characters matches exactly those of the specified C-string.
		inline bool match( const char* cstr ) const { return match( string( cstr ) ); }

		// Check if this token's character sequence starts with the same characters as the specified substring.
		inline bool startsWith( const string& str ) { return Src.str().startsWith( str ); }

		// Check if this token's character sequence ends with the same characters as the specified substring.
		inline bool endsWith( const string& str ) { return Src.str().endsWith( str ); }

		// Check if this token contains only this character.
		inline bool containsOnly( char_t c ) const noexcept {
			auto str = Src.str(); return !str.empty() && !str.findFirstNotOf( string( c ).substr() ); }

		// Get token string.
		inline string str() const noexcept { return Src.str(); }

		// Get token as bytes.
		inline std::string bytes() const noexcept { return Src.bytes(); }




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		source::Ref Src;
		name_t Type{ no_name };
	};
};
