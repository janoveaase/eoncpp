#pragma once
#include "TokenParser.h"


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// Eon Token Matcher Class - eon::TokenMatcher
	//
	// The token matcher works with [eon::TokenParser] and can identify
	// sequences of tokens based on a sequence pattern.
	//
	// The pattern is defined as a string, each element to be matched is
	// enclosed by parenthesis.
	// The following can be placed inside an element:
	//
	// <name>   : Name of the token type to match. This should match a name
	//            registered with the [eon::Tokenizer] used to generate the
	//            tokens, and/or the [eon::ReTokenizer] that has modified the
	//            tokens, prior to [eon::TokenParser] getting them.
	//            Note that it might be more efficient to match both <name> and
	//            <string> rather than just <string> only.
	// <string> : A single-quoted string to be matched. If the string starts
	//            with a star "*", the match the end of the string, and if it
	//            ends with one, match the start of the string.
	//
	// Speceial prefixes:
	//  ? : Element is optional.
	//  & : Match element but do not advance to the next.
	//  ! : Do not match element and do not advance to the next.
	//  - : Match anything except this element and advance to the next.
	//
	// Functions:
	//  opt(<elements>) : Out of two or more elements, match one of them.
	//  seq(<elements>) : Two or more elements to be matched in sequence.
	// Elements within functions can also be functions.
	//
	// Example: "(letters '*to') ?opt((space) (tab)) seq((digits '9*') ?seq((symbol '.')(digits))) (symbol '!')"
	//   Match sequence of tokens where token number ... :
	//     1. Consists of letters only and ends with "to".
	//     2. May or may not be a space or a tab token.
	//     3. Is a sequence of tokens where the first consists of digits
	//        starting with 9, optionally followed by point and more digits.
	//     4. Is the symbol "!".
	//
	class TokenMatcher
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		// Default construction, void matcher
		TokenMatcher() = default;

		// Construct matcher for a sequence pattern
		EONEXCEPT( InvalidPattern );
		inline TokenMatcher( string pattern ) { Pattern.Root = true; Pattern.parse( pattern.begin() ); }

		TokenMatcher( const TokenMatcher& other ) { Pattern = other.Pattern; }
		inline TokenMatcher( TokenMatcher&& other ) noexcept { Pattern = std::move( other.Pattern ); }

		virtual ~TokenMatcher() = default;


		inline TokenMatcher& operator=( const TokenMatcher& other ) { Pattern = other.Pattern; return *this; }
		inline TokenMatcher& operator=( TokenMatcher&& other ) noexcept {
			Pattern = std::move( other.Pattern ); return *this; }




		///////////////////////////////////////////////////////////////////////
		//
		// Matching
		//

		// Check if this matcher is empty (no pattern)
		inline operator bool() const noexcept { return !Pattern.empty(); }

		// Check if the specified [eon::TokenParser] is matching the pattern
		// of this matcher at its current position.
		bool match( TokenParser& parser ) const noexcept;




		///////////////////////////////////////////////////////////////////////
		//
		// Element classes
		//
	EON_PRIVATE:

		enum class flags
		{
			none = 0x00,
			optional = 0x01,
			negate = 0x02,
			advance = 0x04
		};
		inline friend flags operator|( flags a, flags b ) noexcept {
			return static_cast<flags>( static_cast<int>( a ) | static_cast<int>( b ) ); }
		inline friend bool operator&&( flags a, flags b ) noexcept {
			return static_cast<bool>( static_cast<int>( a ) & static_cast<int>( b ) ); }


		static inline bool isNameChar( char_t c ) noexcept {
			return Characters::get().isLetter( c ) || Characters::get().isNumberAsciiDigit( c ) || c == '_'; }
		static inline bool isSpaceChar( char_t c ) noexcept { return c == SpaceChr || c == TabChr || c == NewlineChr; }
		static string::iterator parseName( string::iterator i, name_t& name );

		struct Element
		{
			Element& operator=( const Element& other ) noexcept { Flags = other.Flags; return *this; }

			virtual string::iterator parse( string::iterator i ) = 0;
			virtual Element* clone() const = 0;
			virtual bool match( TokenParser& parser ) const noexcept = 0;

			inline bool optional() const noexcept { return Flags && flags::optional; }
			inline bool negate() const noexcept { return Flags && flags::negate; }
			inline bool advance() const noexcept { return Flags && flags::advance; }

			virtual bool isData() const noexcept { return false; }
			virtual bool isOptions() const noexcept { return false; }
			virtual bool isSequence() const noexcept { return false; }

			flags Flags{ flags::advance };
		};

		struct DataElement : public Element
		{
			DataElement() = default;
			inline DataElement( const DataElement& other ) { *this = other; }
			inline DataElement( DataElement&& other ) noexcept { *this = std::move( other ); }
			virtual ~DataElement() = default;

			enum class OpenEnded
			{
				none,
				start,
				end
			};

			inline DataElement& operator=( const DataElement& other ) {
				Type = other.Type; Str = other.Str; Open = other.Open; *static_cast<Element*>( this ) = other;
				return *this; }
			inline DataElement& operator=( DataElement&& other ) noexcept {
				Type = other.Type; other.Type = no_name; Str = std::move( other.Str ); Open = other.Open;
				other.Open = OpenEnded::none; *static_cast<Element*>( this ) = other; return *this; }

			string::iterator parse( string::iterator i ) override;
			bool _parse( string::iterator& i );
			inline Element* clone() const override { return new DataElement( *this ); }
			bool match( TokenParser& parser ) const noexcept override;
			bool _matchStr( TokenParser& parser ) const noexcept;

			inline bool isData() const noexcept override { return true; }

			string::iterator _parseStr( string::iterator i );
			string::iterator _parseStartedStr( string::iterator i );
			inline bool _openEndedStart( string::iterator i ) const noexcept { return *i == '*'; }
			inline bool _openEndedEnd( string::iterator i ) const noexcept {
				if( *i == '*' ) { auto next = i + 1; return next && *next == SglQuoteChr; } return false; }
			string::iterator _parseStrContents( string::iterator i );
			string::iterator _parseStrEscaped( string::iterator i );

			name_t Type{ no_name };
			string Str;
			OpenEnded Open{ OpenEnded::none };
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

			struct Parser
			{
				Parser() = default;
				inline Parser( string::iterator pos, std::list<Element*>& elements, bool& root ) {
					I = pos; Pos = pos; Elements = &elements; Root = &root; }
				string::iterator parse();
				inline bool _skipSpace() noexcept { if( isSpaceChar( *I ) ) { ++I; return true; } return false; }
				bool _parseFlag() noexcept;
				inline bool _endOfElement() noexcept { if( *I == ')' ) { ++I; return true; } return false; }
				inline bool _parseName() {
					if( isNameChar( *I ) ) { Pos = I; I = parseName( I, ElmType ); return true; } return false; }
				bool _parseNewElement();
				void _createNewElement();

				flags ParseFlags{ flags::advance };
				name_t ElmType{ no_name };
				string::iterator Pos, I;
				std::list<Element*>* Elements{ nullptr };
				bool* Root{ nullptr };
			};

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
			bool _matchSequence( TokenParser& parser ) const noexcept;

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
