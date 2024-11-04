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

		TokenMatcher( const TokenMatcher& ) = default;
		TokenMatcher( TokenMatcher&& ) noexcept = default;

		virtual ~TokenMatcher() = default;


		TokenMatcher& operator=( const TokenMatcher& ) = default;
		TokenMatcher& operator=( TokenMatcher&& ) noexcept = default;




		///////////////////////////////////////////////////////////////////////
		//
		// Matching
		//

		// Check if this matcher is empty (no pattern)
		inline explicit operator bool() const noexcept { return !Pattern.empty(); }

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
			flags Flags{ flags::advance };

			Element() = default;
			Element( const Element& ) noexcept = default;
			Element( Element&& ) noexcept = default;
			virtual ~Element() = default;

			Element& operator=( const Element& ) noexcept = default;
			Element& operator=( Element&& ) noexcept = default;

			virtual string::iterator parse( string::iterator i ) = 0;
			virtual std::shared_ptr<Element> clone() const = 0;
			virtual bool match( TokenParser& parser ) const noexcept = 0;

			inline bool optional() const noexcept { return Flags && flags::optional; }
			inline bool negate() const noexcept { return Flags && flags::negate; }
			inline bool advance() const noexcept { return Flags && flags::advance; }

			virtual bool isData() const noexcept { return false; }
			virtual bool isOptions() const noexcept { return false; }
			virtual bool isSequence() const noexcept { return false; }
		};

		struct DataElement : public Element
		{
			enum class OpenEnded
			{
				none,
				start,
				end
			};

			name_t Type{ no_name };
			string Str;
			OpenEnded Open{ OpenEnded::none };


			DataElement() = default;
			inline DataElement( const DataElement& ) = default;
			inline DataElement( DataElement&& ) noexcept = default;
			~DataElement() final = default;

			DataElement& operator=( const DataElement& ) = default;
			DataElement& operator=( DataElement&& ) noexcept = default;

			string::iterator parse( string::iterator i ) override;
			bool _parse( string::iterator& i );
			inline std::shared_ptr<Element> clone() const override { return std::make_shared<DataElement>( *this ); }
			bool match( TokenParser& parser ) const noexcept override;
			bool _matchStr( const TokenParser& parser ) const noexcept;

			inline bool isData() const noexcept override { return true; }

			string::iterator _parseStr( string::iterator i );
			string::iterator _parseStartedStr( string::iterator i );
			inline bool _openEndedStart( const string::iterator& i ) const noexcept { return *i == '*'; }
			inline bool _openEndedEnd( const string::iterator& i ) const noexcept {
				if( *i == '*' ) { auto next = i + 1; return next && *next == SglQuoteChr; } return false; }
			string::iterator _parseStrContents( string::iterator i );
			string::iterator _parseStrEscaped( string::iterator i );
		};

		struct ContainerElement : public Element
		{
			std::list<std::shared_ptr<Element>> Elements;
			bool Root{ false };


			ContainerElement() = default;
			ContainerElement( const ContainerElement& other ) { *this = other; }
			ContainerElement( ContainerElement&& other ) noexcept { *this = std::move( other ); }
			~ContainerElement() override { Elements.clear(); }
			inline void reset() noexcept { Elements.clear(); }

			ContainerElement& operator=( const ContainerElement& other )
			{
				reset();
				for( auto& elm : other.Elements )
					Elements.push_back( elm->clone() );
				*static_cast<Element*>( this ) = *static_cast<const Element*>( &other );
				return *this;
			}
			ContainerElement& operator=( ContainerElement&& other ) noexcept
			{
				Elements = std::move( other.Elements );
				*static_cast<Element*>( this ) = std::move( *static_cast<Element*>( &other ) );
				return *this;
			}

			inline bool empty() const noexcept { return Elements.empty(); }

			string::iterator parse( string::iterator i ) override;

			struct Parser
			{
				flags ParseFlags{ flags::advance };
				name_t ElmType{ no_name };
				string::iterator Pos;
				string::iterator I;
				std::list<std::shared_ptr<Element>>* Elements{ nullptr };
				bool* Root{ nullptr };


				Parser() = default;
				inline Parser( const string::iterator& pos, std::list<std::shared_ptr<Element>>& elements, bool& root )
					: Pos( pos ), I( pos ), Elements( &elements ), Root( &root ) {}
				string::iterator parse();
				inline bool _skipSpace() noexcept { if( isSpaceChar( *I ) ) { ++I; return true; } return false; }
				bool _parseFlag() noexcept;
				inline bool _endOfElement() noexcept { if( *I == ')' ) { ++I; return true; } return false; }
				inline bool _parseName() {
					if( isNameChar( *I ) ) { Pos = I; I = parseName( I, ElmType ); return true; } return false; }
				bool _parseNewElement();
				void _createNewElement();
			};
		};

		struct OptionsElement : public ContainerElement
		{
			OptionsElement() = default;
			OptionsElement( const OptionsElement& ) = default;
			OptionsElement( OptionsElement&& ) noexcept = default;

			OptionsElement& operator=( const OptionsElement& ) = default;
			OptionsElement& operator=( OptionsElement&& ) noexcept = default;

			inline std::shared_ptr<Element> clone() const override { return std::make_shared<OptionsElement>( *this ); }
			bool match( TokenParser& parser ) const noexcept override;

			inline bool isOptions() const noexcept override { return true; }
		};

		struct SequenceElement : public ContainerElement
		{
			SequenceElement() = default;
			SequenceElement( const SequenceElement& ) = default;
			SequenceElement( SequenceElement&& ) noexcept = default;

			SequenceElement& operator=( const SequenceElement& ) = default;
			SequenceElement& operator=( SequenceElement&& ) noexcept = default;

			inline std::shared_ptr<Element> clone() const override { return std::make_shared<SequenceElement>( *this ); }
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
