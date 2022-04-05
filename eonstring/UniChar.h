#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <map>
#include <locale>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	//* Defining eon::char_t as a 32 bit integer (that debuggers should also be
	//* able to show proper character for)
	using char_t = char32_t;

	//* Defining eon::byte_t to reduce confusion with UTF-8/Unicode characters
	using byte_t = unsigned char;

	// Definition of other types
	using int_t = long int;
	using short_t = int16_t;
	using long_t = int64_t;
	using index_t = size_t;
	static const index_t no_index{ SIZE_MAX };
	using flt_t = double;
	using low_t = float;
	using high_t = long double;


	//* While not strictly necessary, these character definitions can help
	//* increase clarity and avoid some confusions.
	static const char_t NullChr{ '\0' };
	static const char_t SpaceChr{ ' ' }, TabChr{ '\t' };
	static const char_t PointChr{ '.' }, CommaChr{ ',' };
	static const char_t ColonChr{ ':' }, SemiColonChr{ ';' };
	static const char_t DblQuoteChr{ '"' }, SglQuoteChr{ '\'' };
	static const char_t ZeroChr{ '0' };
	static const char_t NewlineChr{ '\n' }, CReturnChr{ '\r' };
	static const char_t StarChr{ '*' };
	static const char_t EqualChr{ '=' }, PlusChr{ '+' }, MinusChr{ '-' };
	static const char_t RoundBrcOpenChr{ '(' }, RoundBrcCloseChr{ ')' };
	static const char_t CurlyBrcOpenChr{ '{' }, CurlyBrcCloseChr{ '}' };
	static const char_t SquareBrcOpenChr{ '[' }, SquareBrcCloseChr{ ']' };
	static const char_t AngleBrcOpenChr{ '{' }, AngleBrcCloseChr{ '}' };
	static const char_t ForwSlashChr{ '/' }, BackSlashChr{ '\\' };
	static const char_t BarChr{ '|' };

	//* Special character that indicates a 'char_t' nothing value
	static const char_t nochar{ UINT32_MAX };

	//* Special character used as default value in parameter declarations to
	//* indicate that the 'char_t' parameter in question will have the same
	//* value as the previous 'char_t' parameter.
	//* It is the same as [eon::nochar], but with a name that is more
	//* appropriate for the context.
	static const char_t same_char = nochar;


	//* Check if the given 'value' is a valid codepoint
	static inline bool isValidCodepoint( char_t value ) noexcept {
		return value < 0xC0 || ( value > 0xC1 && value < 0x140000 ); }




	/**************************************************************************
	  Character Categories

	  Categories based on
		https://www.fileformat.info/info/unicode/category/index.htm
	**************************************************************************/
	enum class charcat
	{
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

		number_ascii_digit,						// Special to Eon
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
		Zs = separator_space,

		other_control,
		Cc = other_control,
		other_format,
		Cf = other_format,
		other_private_use,
		Co = other_private_use,
		other_surrogate,
		Cs = other_surrogate,

		undef
	};




	/**************************************************************************
	  The 'Characaters' class is an implicit global class that can be used to
	  check if a specific [eon::char_t] value belongs to a specific
	  [eon::charcat] character category.
	**************************************************************************/
	class Characters
	{
	private:
		Characters();			// No explicit construction!

	public:
		Characters( const Characters& ) = delete;
		Characters( Characters&& ) = delete;

		virtual ~Characters();



		/**********************************************************************
		  Access
		**********************************************************************/
	public:

		//* There can only be one 'Characters' object and you access it using
		//* this static method
		static const Characters& get();

		inline const std::locale& locale() const noexcept { return Locale; }

		//* Check if a character belongs to the "other" categotry
		inline bool isOther( char_t c ) const noexcept {
			return isOtherControl( c ) || isOtherFormat( c ) || isOtherPrivateUse( c ) || isOtherSurrogate( c ); }
		inline bool isOtherControl( char_t c ) const noexcept {
			return ( c >= 0x00 && c <= 0x1F ) || ( c >= 0x7F && c <= 0x9F ); }
		inline bool isOtherFormat( char_t c ) const noexcept {
			return std::binary_search( other_format->begin(), other_format->end(), c ); }
		inline bool isOtherPrivateUse( char_t c ) const noexcept {
			return c == 0xE000 || c == 0xF8FF || c == 0xF0000 || c == 0xFFFFD || c == 0x100000 || c == 0x10FFFD; }
		inline bool isOtherSurrogate( char_t c ) const noexcept {
			return c == 0xD800 || c == 0xDB7F || c == 0xDB80 || c == 0xDBFF || c == 0xDC00 || c == 0xDFFF; }

		//* Check if a character belongs to the "letter" categotry
		inline bool isLetter( char_t c ) const noexcept { return isLetterUpperCase( c ) || isLetterLowerCase( c )
			|| isLetterTitleCase( c ) || isLetterModifier( c ) || isLetterOther( c ); }
		bool isLetterUpperCase( char_t c ) const noexcept;
		bool isLetterLowerCase( char_t c ) const noexcept;
		inline bool isLetterTitleCase( char_t c ) const noexcept {
			return std::binary_search( letter_title_case.begin(), letter_title_case.end(), c ); }
		bool isLetterModifier( char_t c ) const noexcept;
		inline bool isLetterOther( char_t c ) const noexcept {
			return c >= 0xAA && std::binary_search( letter_other->begin(), letter_other->end(), c ); }
		// ASCII letters only
		inline bool isAsciiLetter( char_t c ) const noexcept { return ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ); }

		//* Check if a character belongs to the "mark" categotry
		inline bool isMarkSpacingCombining( char_t c ) const noexcept {
			return std::binary_search( mark_spacing_combining->begin(), mark_spacing_combining->end(), c ); }
		inline bool isMarkNonSpacing( char_t c ) const noexcept {
			return std::binary_search( mark_nonspacing->begin(), mark_nonspacing->end(), c ); }
		inline bool isMarkEnclosing( char_t c ) const noexcept {
			return std::binary_search( mark_enclosing.begin(), mark_enclosing.end(), c ); }

		//* Check if a character belongs to the "number" category
		inline bool isNumber( char_t c ) const noexcept {
			return isNumberDecimalDigit( c ) || isNumberLetter( c ) || isNumberOther( c ); }
		inline bool isNumberAsciiDigit( char_t c ) const noexcept { return c >= '0' && c <= '9'; }
		inline bool isNumberDecimalDigit( char_t c ) const noexcept { return c <= 0x39 ? c >= 0x30 && c <= 0x39
			: std::binary_search( number_decimal_digit->begin(), number_decimal_digit->end(), c ); }
		inline bool isNumberLetter( char_t c ) const noexcept { return
			( c >= 0x2160 && c <= 0x2182 ) || std::binary_search( number_letter->begin(), number_letter->end(), c ); }
		inline bool isNumberOther( char_t c ) const noexcept {
			return std::binary_search( number_other->begin(), number_other->end(), c ); }

		//* Check if a character belongs to the "punctuation" categotry
		inline bool isPunctuation( char_t c ) const noexcept {
			return isPunctuationConnector( c ) || isPunctuationDash( c ) || isPunctuationOpen( c ) || isPunctuationClose( c )
			|| isPunctuationInitialQuote( c ) || isPunctuationFinalQuote( c ) || isPunctuationOther( c ); }
		inline bool isPunctuationConnector( char_t c ) const noexcept {
			return std::binary_search( punctuation_connector.begin(), punctuation_connector.end(), c ); }
		inline bool isPunctuationDash( char_t c ) const noexcept {
			return std::binary_search( punctuation_dash.begin(), punctuation_dash.end(), c ); }
		inline bool isPunctuationOpen( char_t c ) const noexcept {
			return std::binary_search( punctuation_open->begin(), punctuation_open->end(), c ); }
		inline bool isPunctuationClose( char_t c ) const noexcept {
			return std::binary_search( punctuation_close->begin(), punctuation_close->end(), c ); }
		inline bool isPunctuationInitialQuote( char_t c ) const noexcept {
			return std::binary_search( punctuation_initial_quote.begin(), punctuation_initial_quote.end(), c ); }
		inline bool isPunctuationFinalQuote( char_t c ) const noexcept {
			return std::binary_search( punctuation_final_quote.begin(), punctuation_final_quote.end(), c ); }
		inline bool isPunctuationOther( char_t c ) const noexcept {
			return std::binary_search( punctuation_other->begin(), punctuation_other->end(), c ); }

		//* Check if a character belongs to the "symbol" categotry
		inline bool isSymbol( char_t c ) const noexcept {
			return isSymbolCurrency( c ) || isSymbolModifier( c ) || isSymbolMath( c ) || isSymbolOther( c ); }
		inline bool isSymbolCurrency( char_t c ) const noexcept {
			return std::binary_search( symbol_currency->begin(), symbol_currency->end(), c ); }
		inline bool isSymbolModifier( char_t c ) const noexcept {
			return std::binary_search( symbol_modifier->begin(), symbol_modifier->end(), c ); }
		inline bool isSymbolMath( char_t c ) const noexcept {
			return std::binary_search( symbol_math->begin(), symbol_math->end(), c ); }
		inline bool isSymbolOther( char_t c ) const noexcept {
			return std::binary_search( symbol_other->begin(), symbol_other->end(), c ); }

		//* Check if a character belongs to the "separator" categotry
		inline bool isSeparator( char_t c ) const noexcept {
			return isSeparatorSpace( c ) || isSeparatorLine( 0 ) || isSeparatorParagraph( c ); }
		inline bool isSeparatorLine( char_t c ) const noexcept { return c == 0x2028; }
		inline bool isSeparatorParagraph( char_t c ) const noexcept { return c == 0x2029; }
		inline bool isSeparatorSpace( char_t c ) const noexcept { return ( c >= 0x08 && c <= 0x0D ) || c == 0x20
			|| ( c >= 0x2000 && c <= 0x200A ) || c == 0xA0 || c == 0x1680 || c == 0x202F || c == 0x205F || c == 0x3000; }


		//* Check if a character belongs to a named [eon::charcat] character
		//* 'category'
		bool is( char_t codepoint, charcat category ) const noexcept;

		//* Get the [eon::charcat] character category for the specified
		//* character
		//* Returns charcat::undef if unable to categorize.
		charcat category( char_t codepoint ) const noexcept;



		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	private:
		std::locale Locale;
		const std::vector<char_t>* other_format{ nullptr };

		const std::vector<char_t>* letter_upper_case{ nullptr };
		const std::vector<char_t> letter_title_case{ 0x01C5, 0x01C8, 0x01CB, 0x01F2, 0x1F88, 0x1F89, 0x1F8A, 0x1F8B, 0x1F8C,
			0x1F8D, 0x1F8E, 0x1F8F, 0x1F98, 0x1F99, 0x1F9A, 0x1F9B, 0x1F9C, 0x1F9D, 0x1F9E, 0x1F9F, 0x1FA8, 0x1FA9, 0x1FAA,
			0x1FAB, 0x1FAC, 0x1FAD, 0x1FAE, 0x1FAF, 0x1FBC, 0x1FCC, 0x1FFC };
		const std::vector<char_t>* letter_lower_case{ nullptr };
		const std::vector<char_t>* letter_modifier{ nullptr };
		const std::vector<char_t>* letter_other{ nullptr };

		const std::vector<char_t>* mark_spacing_combining{ nullptr };
		const std::vector<char_t>* mark_nonspacing{ nullptr };
		const std::vector<char_t> mark_enclosing{
			0x0488, 0x0489, 0x1ABE, 0x20DD, 0x20DE, 0x20DF, 0x20E0, 0x20E2, 0x20E3, 0x20E4, 0xA670, 0xA671, 0xA672 };

		const std::vector<char_t>* number_decimal_digit{ nullptr };
		const std::vector<char_t>* number_letter{ nullptr };
		const std::vector<char_t>* number_other{ nullptr };

		const std::vector<char_t> punctuation_connector{
			0x005F, 0x203F, 0x2040, 0x2054, 0xFE33, 0xFE34, 0xFE4D, 0xFE4E, 0xFE4F, 0xFF3F };
		const std::vector<char_t> punctuation_dash{
			0x002D, 0x058A, 0x05BE, 0x1400, 0x1806, 0x2010, 0x2011, 0x2012, 0x2013, 0x2014, 0x2015, 0x2E17, 0x2E1A, 0x2E3A,
			0x2E3B, 0x2E40, 0x301C, 0x3030, 0x30A0, 0xFE31, 0xFE32, 0xFE58, 0xFE63, 0xFF0D, 0x10EAD };
		const std::vector<char_t>* punctuation_open{ nullptr };
		const std::vector<char_t>* punctuation_close{ nullptr };
		const std::vector<char_t> punctuation_initial_quote{
			0x00AB, 0x2018, 0x201B, 0x201C, 0x201F, 0x2039, 0x2E02, 0x2E04, 0x2E09, 0x2E0C, 0x2E1C, 0x2E20 };
		const std::vector<char_t> punctuation_final_quote{
			0x00BB, 0x2019, 0x201D, 0x203A, 0x2E03, 0x2E05, 0x2E0A, 0x2E0D, 0x2E1D, 0x2E21 };
		const std::vector<char_t>* punctuation_other{ nullptr };

		const std::vector<char_t>* symbol_currency{ nullptr };
		const std::vector<char_t>* symbol_modifier{ nullptr };
		const std::vector<char_t>* symbol_math{ nullptr };
		const std::vector<char_t>* symbol_other{ nullptr };
	};


	//* Check if the given codepoint is a digit (ASCII only!)
	static inline bool isDigit( char_t codepoint ) noexcept { return codepoint >= '0' && codepoint <= '9'; }

	//* Check if the given codepoint is a numeral (any number character)
	static inline bool isNumeral( char_t codepoint ) noexcept {
		return Characters::get().isNumberDecimalDigit( codepoint ); }

	//* Check if the given codepoint is a hex digit
	static inline bool isHexDigit( char_t codepoint ) noexcept {
		return ( codepoint >= '0' && codepoint <= '9' )
			|| ( codepoint >= 'A' && codepoint <= 'F' )
			|| ( codepoint >= 'a' && codepoint <= 'f' ); }

	//* Given a hex digit, return the numerical value
	static inline byte_t hexToNum( byte_t digit ) noexcept {
		return ( digit >= '0' && digit <= '9' ) ? ( digit - '0' )
			: ( digit >= 'A' && digit <= 'F' ) ? ( digit - 'A' + 10 )
			: ( digit >= 'a' && digit <= 'f' ) ? ( digit - 'a' + 10 ) : 0; }

	//* Given a byte, convert it into two hex digits
	static inline std::string byteToHex( byte_t byte ) noexcept {
		byte_t hi = byte / 16, lo = byte % 16; std::string str( "00" );
		str[ 0 ] = hi < 10 ? '0' + hi : 'A' + hi - 10; str[ 0 ] = lo < 10 ? '0' + lo : 'A' + lo - 10; return str; }

	//* Check if the given codepoint is an octal digit
	static inline bool isOctalDigit( char_t codepoint ) noexcept { return codepoint >= '0' && codepoint <= '7'; }
};
