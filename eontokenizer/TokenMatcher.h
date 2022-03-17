#pragma once
#include "TokenParser.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  Eon Token Matcher Class - eon::TokenMatcher

	  The token matcher works with [eon::TokenParser] and can identify
	  sequences of tokens based on a sequence pattern setup.

	  The pattern is defined as a string. Enclose each element to be matched in
	  parenthesis.
	  The following can be placed inside an element:

	  <name>   : Name of the token type to match. This should match a name
	             registered with the [eon::Tokenizer] used to generate the
				 tokens, and/or the [eon::ReTokenizer] that has modified the
				 tokens, prior to [eon::TokenParser] getting them.
			     Note that it might more efficient to match both <name> and
				 <string> rather than just <string> only.
	  <string> : A single-quoted string to be matched. If the string starts
	             with a star "*", the match the end of the string, and if it
				 ends with one, match the start of the string.

	  If an element should be treated as optional, prefix it with a question
	  mark.
	  If an element should be matched, but not advanced (matched again), prefix
	  it with an ampersand.
	  If an element should not match (to match), prefix it with an exclamation
	  point.
	  If two or more elements should be treated as options (where only one need
	  to match), put "opt" in front and enclose them in parenthesis.
	  If two or more elements should be matched in sequence (where one or more
	  may be optional or contain options), put "seq" in front and enclose them
	  in parenthesis.

	  Example: "(letters '*to') ?opt((space) (tab)) seq((digits '9*') ?seq((symbol '.')(digits))) (symbol '!')"
	    Match sequence of tokens where token number:
	      1. Consists of letters only and ends with "to"
	      2. May or may not be a space or a tab token
	      3. Is a sequence of tokens where the first consists of digits
		     starting with 9, optionally followed by point and more digits.
	      4. Is the symbol "!"
	**************************************************************************/
	class TokenMatcher
	{
		/**********************************************************************
		  Construction
		**********************************************************************/
	public:

		//* Default construction, void matcher
		TokenMatcher() = default;

		//* Construct matcher for a sequence pattern
		EONEXCEPT( InvalidPattern );
		inline TokenMatcher( string pattern ) { Pattern.Root = true; Pattern.parse( pattern.begin() ); }

		TokenMatcher( const TokenMatcher& other ) { Pattern = other.Pattern; }
		inline TokenMatcher( TokenMatcher&& other ) noexcept { Pattern = std::move( other.Pattern ); }

		virtual ~TokenMatcher() = default;


		inline TokenMatcher& operator=( const TokenMatcher& other ) { Pattern = other.Pattern; return *this; }
		inline TokenMatcher& operator=( TokenMatcher&& other ) noexcept {
			Pattern = std::move( other.Pattern ); return *this; }




		/**********************************************************************
		  Matching
		**********************************************************************/

		//* Check if this matcher is empty (no pattern)
		inline operator bool() const noexcept { return !Pattern.empty(); }

		//* Check if the specified [eon::TokenParser] is matching the pattern
		//* of this matcher at its current position.
		bool match( TokenParser& parser ) const noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Element classes
		//
	private:

		struct Element
		{
			Element() { Chars = &Characters::get(); }

			Element& operator=( const Element& other ) noexcept {
				Optional = other.Optional; Advance = other.Advance; Negate = other.Negate; return *this; }

			virtual string::iterator parse( string::iterator i ) = 0;
			virtual Element* clone() const = 0;
			virtual bool match( TokenParser& parser ) const noexcept = 0;

			virtual bool isData() const noexcept { return false; }
			virtual bool isOptions() const noexcept { return false; }
			virtual bool isSequence() const noexcept { return false; }

			string::iterator parseName( string::iterator i, name_t& name );

			inline bool isNameChar( char_t c ) const noexcept {
				return Chars->isLetter( c ) || Chars->isNumberAsciiDigit( c ) || c == '_'; }
			inline bool isSpaceChar( char_t c ) const noexcept { return c == SpaceChr || c == TabChr || c == NewlineChr; }

			bool Optional{ false };
			bool Advance{ true };
			bool Negate{ false };
			const Characters* Chars{ nullptr };
		};

		struct DataElement : public Element
		{
			DataElement() = default;
			inline DataElement( const DataElement& other ) { *this = other; }
			inline DataElement( DataElement&& other ) noexcept { *this = std::move( other ); }
			virtual ~DataElement() = default;

			inline DataElement& operator=( const DataElement& other ) {
				Type = other.Type; Str = other.Str; StrEnd = other.StrEnd; *static_cast<Element*>( this ) = other;
				return *this; }
			inline DataElement& operator=( DataElement&& other ) noexcept {
				Type = other.Type; other.Type = no_name; Str = std::move( other.Str ); StrEnd = other.StrEnd;
				other.StrEnd = 0; *static_cast<Element*>( this ) = other; return *this; }

			string::iterator parse( string::iterator i ) override;
			inline Element* clone() const override { return new DataElement( *this ); }
			bool match( TokenParser& parser ) const noexcept override;

			inline bool isData() const noexcept override { return true; }

			string::iterator _parseStr( string::iterator i );

			name_t Type{ no_name };
			string Str;
			int StrEnd{ 0 };	// -1=start, 1=end
		};

		struct ContainerElement : public Element
		{
			ContainerElement() = default;
			ContainerElement( const ContainerElement& other ) { *this = other; }
			ContainerElement( ContainerElement&& other ) noexcept { *this = std::move( other ); }
			virtual ~ContainerElement() { reset(); }
			inline void reset() noexcept { for( auto elm : Elements ) delete elm; Elements.clear(); }

			ContainerElement& operator=( const ContainerElement& other ) {
				reset(); for( auto elm : other.Elements ) Elements.push_back( elm->clone() );
				*static_cast<Element*>( this ) = other; return *this; }
			inline ContainerElement& operator=( ContainerElement&& other ) noexcept {
				Elements = std::move( other.Elements ); *static_cast<Element*>( this ) = other; return *this; }

			inline bool empty() const noexcept { return Elements.empty(); }

			string::iterator parse( string::iterator i ) override;
			
			std::list<Element*> Elements;
			bool Root{ false };
		};

		struct OptionsElement : public ContainerElement
		{
			OptionsElement() = default;
			OptionsElement( const OptionsElement& other ) { *this = other; }
			OptionsElement( OptionsElement&& other ) noexcept { *this = std::move( other ); }
			virtual ~OptionsElement() = default;

			inline OptionsElement& operator=( const OptionsElement& other ) {
				*static_cast<ContainerElement*>( this ) = other; return *this; }
			inline OptionsElement& operator=( OptionsElement&& other ) noexcept {
				*static_cast<ContainerElement*>( this ) = std::move( other ); return *this; }

			inline Element* clone() const override { return new OptionsElement( *this ); }
			bool match( TokenParser& parser ) const noexcept override;

			inline bool isOptions() const noexcept override { return true; }
		};

		struct SequenceElement : public ContainerElement
		{
			SequenceElement() = default;
			SequenceElement( const SequenceElement& other ) { *this = other; }
			SequenceElement( SequenceElement&& other ) noexcept { *this = std::move( other ); }

			inline SequenceElement& operator=( const SequenceElement& other ) {
				*static_cast<ContainerElement*>( this ) = other; return *this; }
			inline SequenceElement& operator=( SequenceElement&& other ) noexcept {
				*static_cast<ContainerElement*>( this ) = std::move( other ); return *this; }

			inline Element* clone() const override { return new SequenceElement( *this ); }
			bool match( TokenParser& parser ) const noexcept override;

			inline bool isSequence() const noexcept override { return true; }
		};




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:

		SequenceElement Pattern;
	};
};
