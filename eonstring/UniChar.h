#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <map>

/*
	Unicode Characters

	Categories based on https://www.fileformat.info/info/unicode/category/index.htm.
*/



namespace eon
{
	using eon_char = char32_t;


	// Common characters
	static const eon_char NullChr{ '\0' };
	static const eon_char SpaceChr{ ' ' }, TabChr{ '\t' };
	static const eon_char PointChr{ '.' }, CommaChr{ ',' };
	static const eon_char ColonChr{ ':' }, SemiColonChr{ ';' };
	static const eon_char DblQuoteChr{ '"' }, SglQuoteChr{ '\'' };
	static const eon_char ZeroChr{ '0' };
	static const eon_char NewlineChr{ '\n' }, CReturnChr{ '\r' };
	static const eon_char StarChr{ '*' };
	static const eon_char EqualChr{ '=' }, PlusChr{ '+' }, MinusChr{ '-' };
	static const eon_char RoundBrcOpenChr{ '(' }, RoundBrcCloseChr{ ')' };
	static const eon_char CurlyBrcOpenChr{ '{' }, CurlyBrcCloseChr{ '}' };
	static const eon_char SquareBrcOpenChr{ '[' }, SquareBrcCloseChr{ ']' };
	static const eon_char AngleBrcOpenChr{ '{' }, AngleBrcCloseChr{ '}' };
	static const eon_char ForwSlashChr{ '/' }, BackSlashChr{ '\\' };
	static const eon_char BarChr{ '|' };

	// Special characters
	static const eon_char eon_nochar{ UINT32_MAX };
	static const eon_char same_char = eon_nochar;

	// Check if the given value is a valid codepoint
	static inline bool isValidCodepoint( eon_char value ) noexcept {
		return value < 0xC0 || ( value > 0xC1 && value < 0x140000 ); }


	namespace unichar
	{
		struct LessThan
		{
			virtual bool operator()( eon_char lhs, eon_char rhs )
				const noexcept = 0;
		};

		struct DefaultLT : public LessThan
		{
			inline bool operator()( eon_char lhs, eon_char rhs )
				const noexcept override { return lhs < rhs; }
		};

		// A compare "less than" map is used to re-code eon_char objects so
		// that e.g. upper and lower case may compare equal, or accented
		// letters, etc.
		class CompareLtMap : public LessThan
		{
		public:
			CompareLtMap() = default;
			inline CompareLtMap( const CompareLtMap& other ) { *this = other; }
			inline CompareLtMap( CompareLtMap&& other ) noexcept {
				*this = std::move( other ); }
			~CompareLtMap() = default;

			inline CompareLtMap& operator=( const CompareLtMap& other ) {
				Start = other.Start; Map = other.Map; return *this; }
			inline CompareLtMap& operator=( CompareLtMap&& other ) noexcept {
				Start = other.Start; other.Start = 0; Map = std::move(
					other.Map ); return *this; }

			// The 'call' operator will return true if lhs < rhs!
			inline bool operator()( eon_char lhs, eon_char rhs ) const
				noexcept override {
				return lhs < Start ? rhs < Start ? lhs < rhs : true : rhs
					< Start ? false : map( lhs ) < map( rhs ); }

			// Map a eon_char
			// WARNING: The returned value IS NOT a valid code point, its only 
			//          purpose is to be compared!
			// If no mapping exists, the input value is returned
			inline eon_char map( eon_char c ) const noexcept {
				auto f = Map.find( c );
				return f != Map.end() ? f->second : c; }

		private:
			eon_char Start{ 0 };
			std::map<eon_char, eon_char> Map;
		};
	};


	// Category codes
	enum class charcat
	{
		undef,

		other_control,
		Cc = other_control,
		other_format,
		Cf = other_format,
		other_private_use,
		Co = other_private_use,
		other_surrogate,
		Cs = other_surrogate,

		letter_lowercase,
		Ll = letter_lowercase,
		letter_modifier,
		Lm = letter_modifier,
		letter_other,
		Lo = letter_other,
		letter_titlecase,
		Lt = letter_titlecase,
		letter_uppercase,
		Lu = letter_uppercase,

		mark_spacing_combining,
		Mc = mark_spacing_combining,
		mark_enclosing,
		Me = mark_enclosing,
		mark_nonspacing,
		Mn = mark_nonspacing,

		number_ascii_digit,						// Special
		number_decimal_digit,
		Nd = number_decimal_digit,
		number_letter,
		Nl = number_letter,
		number_other,
		No = number_other,

		punctuation_connector,
		Pc = punctuation_connector,
		punctuation_dash,
		Pd = punctuation_dash,
		punctuation_close,
		Pe = punctuation_close,
		punctuation_final_quote,
		Pf = punctuation_final_quote,
		punctuation_initial_quote,
		Pi = punctuation_initial_quote,
		punctuation_other,
		Po = punctuation_other,
		punctuation_open,
		Ps = punctuation_open,

		symbol_currency,
		Sc = symbol_currency,
		symbol_modifier,
		Sk = symbol_modifier,
		symbol_math,
		Sm = symbol_math,
		symbol_other,
		So = symbol_other,

		separator_line,
		Zl = separator_line,
		separator_paragraph,
		Zp = separator_paragraph,
		separator_space,
		Zs = separator_space
	};


	class Characters
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	private:
		Characters();			// No explicit construction!

	public:
		Characters( const Characters& ) = delete;
		Characters( Characters&& ) = delete;

		virtual ~Characters();



		///////////////////////////////////////////////////////////////////////
		//
		// Access
		//

	public:
		static const Characters& get();

		// The "other" cateotry
		inline bool isOther( eon_char c ) const noexcept { return
			isOtherControl( c ) || isOtherFormat( c )
			|| isOtherPrivateUse( c ) || isOtherSurrogate( c ); }
		inline bool isOtherControl( eon_char c ) const noexcept {
			return ( c >= 0x00 && c <= 0x1F ) || ( c >= 0x7F && c <= 0x9F ); }
		inline bool isOtherFormat( eon_char c ) const noexcept {
			return std::binary_search( other_format->begin(),
				other_format->end(), c ); }
		inline bool isOtherPrivateUse( eon_char c ) const noexcept {
			return c == 0xE000 || c == 0xF8FF || c == 0xF0000 || c == 0xFFFFD
				|| c == 0x100000 || c == 0x10FFFD; }
		inline bool isOtherSurrogate( eon_char c ) const noexcept {
			return c == 0xD800 || c == 0xDB7F || c == 0xDB80 || c == 0xDBFF
				|| c == 0xDC00 || c == 0xDFFF; }

		// The "letter" category
		inline bool isLetter( eon_char c ) const noexcept { return
			isLetterUpperCase( c ) || isLetterLowerCase( c )
			|| isLetterTitleCase( c ) || isLetterModifier( c )
			|| isLetterOther( c ); }
		bool isLetterUpperCase( eon_char c ) const noexcept;
		bool isLetterLowerCase( eon_char c ) const noexcept;
		inline bool isLetterTitleCase( eon_char c ) const noexcept {
			return std::binary_search( letter_title_case.begin(),
				letter_title_case.end(), c ); }
		bool isLetterModifier( eon_char c ) const noexcept;
		inline bool isLetterOther( eon_char c ) const noexcept {
			return c >= 0xAA && std::binary_search( letter_other->begin(),
				letter_other->end(), c ); }

		// The "mark" category
		inline bool isMarkSpacingCombining( eon_char c ) const noexcept {
			return std::binary_search( mark_spacing_combining->begin(),
				mark_spacing_combining->end(), c ); }
		inline bool isMarkNonSpacing( eon_char c ) const noexcept {
			return std::binary_search( mark_nonspacing->begin(),
				mark_nonspacing->end(), c ); }
		inline bool isMarkEnclosing( eon_char c ) const noexcept {
			return std::binary_search( mark_enclosing.begin(),
				mark_enclosing.end(), c ); }

		// The "number" category
		inline bool isNumberAsciiDigit( eon_char c ) const noexcept {
			return c >= '0' && c <= '9'; }
		inline bool isNumberDecimalDigit( eon_char c ) const noexcept {
			return c <= 0x39 ? c >= 0x30 && c <= 0x39
				: std::binary_search( number_decimal_digit->begin(),
					number_decimal_digit->end(), c ); }
		inline bool isNumberLetter( eon_char c ) const noexcept {
			return ( c >= 0x2160 && c <= 0x2182 )
				|| std::binary_search( number_letter->begin(),
					number_letter->end(), c ); }
		inline bool isNumberOther( eon_char c ) const noexcept {
			return std::binary_search( number_other->begin(),
				number_other->end(), c ); }

		// The punctuation category
		inline bool isPunctuation( eon_char c ) const noexcept { return
			isPunctuationConnector( c ) || isPunctuationDash( c )
			|| isPunctuationOpen( c ) || isPunctuationClose( c )
			|| isPunctuationInitialQuote( c ) || isPunctuationFinalQuote( c )
			|| isPunctuationOther( c ); }
		inline bool isPunctuationConnector( eon_char c ) const noexcept {
			return std::binary_search( punctuation_connector.begin(),
				punctuation_connector.end(), c ); }
		inline bool isPunctuationDash( eon_char c ) const noexcept {
			return std::binary_search( punctuation_dash.begin(),
				punctuation_dash.end(), c ); }
		inline bool isPunctuationOpen( eon_char c ) const noexcept {
			return std::binary_search( punctuation_open->begin(),
				punctuation_open->end(), c ); }
		inline bool isPunctuationClose( eon_char c ) const noexcept {
			return std::binary_search( punctuation_close->begin(),
				punctuation_close->end(), c ); }
		inline bool isPunctuationInitialQuote( eon_char c ) const noexcept {
			return std::binary_search( punctuation_initial_quote.begin(),
				punctuation_initial_quote.end(), c ); }
		inline bool isPunctuationFinalQuote( eon_char c ) const noexcept {
			return std::binary_search( punctuation_final_quote.begin(),
				punctuation_final_quote.end(), c ); }
		inline bool isPunctuationOther( eon_char c ) const noexcept {
			return std::binary_search( punctuation_other->begin(),
				punctuation_other->end(), c ); }

		// The symbol category
		inline bool isSymbol( eon_char c ) const noexcept {
			return isSymbolCurrency( c ) || isSymbolModifier( c )
				|| isSymbolMath( c ) || isSymbolOther( c ); }
		inline bool isSymbolCurrency( eon_char c ) const noexcept {
			return std::binary_search( symbol_currency->begin(),
				symbol_currency->end(), c ); }
		inline bool isSymbolModifier( eon_char c ) const noexcept {
			return std::binary_search( symbol_modifier->begin(),
				symbol_modifier->end(), c ); }
		inline bool isSymbolMath( eon_char c ) const noexcept {
			return std::binary_search( symbol_math->begin(),
				symbol_math->end(), c ); }
		inline bool isSymbolOther( eon_char c ) const noexcept {
			return std::binary_search( symbol_other->begin(),
				symbol_other->end(), c ); }

		// The separator category
		inline bool isSeparator( eon_char c ) const noexcept {
			return isSeparatorSpace( c ) || isSeparatorLine( 0 )
				|| isSeparatorParagraph( c ); }
		inline bool isSeparatorLine( eon_char c ) const noexcept {
			return c == 0x2028; }
		inline bool isSeparatorParagraph( eon_char c ) const noexcept {
			return c == 0x2029; }
		inline bool isSeparatorSpace( eon_char c ) const noexcept {
			return ( c >= 0x08 && c <= 0x0D ) || c == 0x20
				|| ( c >= 0x2000 && c <= 0x200A )
				|| c == 0xA0 || c == 0x1680 || c == 0x202F || c == 0x205F
				|| c == 0x3000; }


		// By raw category
		bool is( eon_char codepoint, charcat category ) const noexcept;



		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//

	private:

		const std::vector<eon_char>* other_format{ nullptr };

		const std::vector<eon_char>* letter_upper_case{ nullptr };
		const std::vector<eon_char> letter_title_case{
			0x01C5, 0x01C8, 0x01CB, 0x01F2, 0x1F88, 0x1F89, 0x1F8A, 0x1F8B,
			0x1F8C, 0x1F8D, 0x1F8E, 0x1F8F, 0x1F98, 0x1F99, 0x1F9A, 0x1F9B,
			0x1F9C, 0x1F9D, 0x1F9E, 0x1F9F, 0x1FA8, 0x1FA9, 0x1FAA, 0x1FAB,
			0x1FAC, 0x1FAD, 0x1FAE, 0x1FAF, 0x1FBC, 0x1FCC, 0x1FFC };
		const std::vector<eon_char>* letter_lower_case{ nullptr };
		const std::vector<eon_char>* letter_modifier{ nullptr };
		const std::vector<eon_char>* letter_other{ nullptr };

		const std::vector<eon_char>* mark_spacing_combining{ nullptr };
		const std::vector<eon_char>* mark_nonspacing{ nullptr };
		const std::vector<eon_char> mark_enclosing{
			0x0488, 0x0489, 0x1ABE, 0x20DD, 0x20DE, 0x20DF, 0x20E0, 0x20E2,
			0x20E3, 0x20E4, 0xA670, 0xA671, 0xA672 };

		const std::vector<eon_char>* number_decimal_digit{ nullptr };
		const std::vector<eon_char>* number_letter{ nullptr };
		const std::vector<eon_char>* number_other{ nullptr };

		const std::vector<eon_char> punctuation_connector{
			0x005F, 0x203F, 0x2040, 0x2054, 0xFE33, 0xFE34, 0xFE4D, 0xFE4E,
			0xFE4F, 0xFF3F };
		const std::vector<eon_char> punctuation_dash{ 0x002D, 0x058A,
			0x05BE, 0x1400, 0x1806, 0x2010, 0x2011, 0x2012, 0x2013, 0x2014,
			0x2015, 0x2E17, 0x2E1A, 0x2E3A, 0x2E3B, 0x2E40, 0x301C, 0x3030,
			0x30A0, 0xFE31, 0xFE32, 0xFE58, 0xFE63, 0xFF0D, 0x10EAD };
		const std::vector<eon_char>* punctuation_open{ nullptr };
		const std::vector<eon_char>* punctuation_close{ nullptr };
		const std::vector<eon_char> punctuation_initial_quote{
			0x00AB, 0x2018, 0x201B, 0x201C, 0x201F, 0x2039, 0x2E02, 0x2E04,
			0x2E09, 0x2E0C, 0x2E1C, 0x2E20 };
		const std::vector<eon_char> punctuation_final_quote{ 0x00BB, 0x2019,
			0x201D, 0x203A, 0x2E03, 0x2E05, 0x2E0A, 0x2E0D, 0x2E1D, 0x2E21 };
		const std::vector<eon_char>* punctuation_other{ nullptr };

		const std::vector<eon_char>* symbol_currency{ nullptr };
		const std::vector<eon_char>* symbol_modifier{ nullptr };
		const std::vector<eon_char>* symbol_math{ nullptr };
		const std::vector<eon_char>* symbol_other{ nullptr };
	};


	// Check if the given codepoint is a digit (ASCII only!)
	static inline bool isDigit( eon_char codepoint ) noexcept {
		return codepoint >= '0' && codepoint <= '9'; }

	// Check if the given codepoint is a numeral
	static inline bool isNumeral( eon_char codepoint ) noexcept {
		return Characters::get().isNumberDecimalDigit( codepoint ); }
};
