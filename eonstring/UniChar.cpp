#include "UniChar.h"


namespace eon
{
	Characters::Characters()
	{
#include "other_format.cpp"
#include "letter_upper_case.cpp"
#include "letter_lower_case.cpp"
#include "letter_modifier.cpp"
#include "letter_other.cpp"
#include "mark_space_combining.cpp"
#include "mark_nonspacing.cpp"
#include "number_decimal_digit.cpp"
#include "number_letter.cpp"
#include "number_other.cpp"
#include "punctuation_open.cpp"
#include "punctuation_close.cpp"
#include "punctuation_other.cpp"
#include "symbol_currency.cpp"
#include "symbol_modifier.cpp"
#include "symbol_math.cpp"
#include "symbol_other.cpp"
	}

	Characters::~Characters()
	{
		delete other_format;
		delete letter_upper_case;
		delete letter_lower_case;
		delete letter_modifier;
		delete letter_other;
		delete mark_spacing_combining;
		delete mark_nonspacing;
		delete number_decimal_digit;
		delete number_letter;
		delete number_other;
		delete punctuation_open;
		delete punctuation_close;
		delete punctuation_other;
		delete symbol_currency;
		delete symbol_modifier;
		delete symbol_math;
		delete symbol_other;
	}




	const Characters& Characters::get()
	{
		static std::unique_ptr<Characters> Cats;
		if( !Cats )
			Cats = std::unique_ptr<Characters>( new Characters() );
		return *Cats;
	}

	bool Characters::isLetterUpperCase( char_t c ) const noexcept
	{
		if( c <= 0xDE )
			return ( c >= 0x41 && c <= 0x5A ) || ( c >= 0xC0 && c <= 0xD6 ) || ( c >= 0xD8 && c <= 0xDE );
		else
			return std::binary_search( letter_upper_case->begin(), letter_upper_case->end(), c );
	}
	bool Characters::isLetterLowerCase( char_t c ) const noexcept
	{
		if( c <= 0xFF )
			return ( c >= 0x61 && c <= 0x7A ) || ( c >= 0xDF && c <= 0xF6 ) || ( c >= 0xF8 && c <= 0xFF );
		else
			return std::binary_search( letter_lower_case->begin(), letter_lower_case->end(), c );
	}
	bool Characters::isLetterModifier( char_t c ) const noexcept
	{
		return ( c >= 0x2B0 && c <= 0x2C1 )
			|| ( c >= 0x2C6 && c <= 0x2D1 )
			|| std::binary_search( letter_modifier->begin(), letter_modifier->end(), c );
	}




	charcat Characters::category( char_t codepoint ) const noexcept
	{
		if( isLetterLowerCase( codepoint ) )
			return charcat::letter_lowercase;
		else if( isLetterUpperCase( codepoint ) )
			return charcat::letter_uppercase;
		else if( isLetterTitleCase( codepoint ) )
			return charcat::letter_titlecase;
		else if( isLetterModifier( codepoint ) )
			return charcat::letter_modifier;
		else if( isLetterOther( codepoint ) )
			return charcat::letter_other;

		else if( isMarkSpacingCombining( codepoint ) )
			return charcat::mark_spacing_combining;
		else if( isMarkEnclosing( codepoint ) )
			return charcat::mark_enclosing;
		else if( isMarkNonspacing( codepoint ) )
			return charcat::mark_nonspacing;

		else if( isNumberAsciiDigit( codepoint ) )
			return charcat::number_ascii_digit | charcat::number_decimal_digit;
		else if( isNumberDecimalDigit( codepoint ) )
			return charcat::number_decimal_digit;
		else if( isNumberLetter( codepoint ) )
			return charcat::number_letter;
		else if( isNumberOther( codepoint ) )
			return charcat::number_other;

		else if( isPunctuationConnector( codepoint ) )
			return charcat::punctuation_connector;
		else if( isPunctuationDash( codepoint ) )
			return charcat::punctuation_dash;
		else if( isPunctuationClose( codepoint ) )
			return charcat::punctuation_close;
		else if( isPunctuationFinalQuote( codepoint ) )
			return charcat::punctuation_final_quote;
		else if( isPunctuationInitialQuote( codepoint ) )
			return charcat::punctuation_initial_quote;
		else if( isPunctuationOther( codepoint ) )
			return charcat::punctuation_other;
		else if( isPunctuationOpen( codepoint ) )
			return charcat::punctuation_open;

		else if( isSymbolCurrency( codepoint ) )
			return charcat::symbol_currency;
		else if( isSymbolModifier( codepoint ) )
			return charcat::symbol_modifier;
		else if( isSymbolMath( codepoint ) )
			return charcat::symbol_math;
		else if( isSymbolOther( codepoint ) )
			return charcat::symbol_other;

		else if( isSeparatorLine( codepoint ) )
			return charcat::separator_line;
		else if( isSeparatorParagraph( codepoint ) )
			return charcat::separator_paragraph;
		else if( isSeparatorSpace( codepoint ) )
			return charcat::separator_space;

		else if( isOtherControl( codepoint ) )
			return charcat::other_control;
		else if( isOtherFormat( codepoint ) )
			return charcat::other_format;
		else if( isOtherPrivateUse( codepoint ) )
			return charcat::other_private_use;
		else if( isOtherSurrogate( codepoint ) )
			return charcat::other_surrogate;

		else
			return charcat::undef;
	}




	char_t zeroChar( char_t codepoint ) noexcept
	{
		static std::vector<char_t> zero{
			0x30,
			0x660, 0x6F0, 0x7C0, 0x966, 0x9E6, 0xA66, 0xAE6, 0xB66, 0xBE6, 0xC66, 0xCE6, 0xD66, 0xDE6, 0xE50, 0xED0, 0xF20,
			0x1040, 0x1090, 0x17E0, 0x1810, 0x1946, 0x19D0, 0x1A80, 0x1A90, 0x1B50, 0x1BB0, 0x1C50, 0x1C50, 0xA620, 0xA8D0,
			0xA900, 0xA9D0, 0xA9F0, 0xAA50, 0xABF0, 0xFF10,
			0x104A0, 0x10D30, 0x11066, 0x110F0, 0x11136, 0x111D0, 0x112F0, 0x11450, 0x114D0, 0x11650, 0x116C0, 0x11730,
			0x118E0, 0x11950, 0x11C50, 0x11D50, 0x11DA0, 0x11F50, 0x16A60, 0x16AC0, 0x16B50, 0x1D7CE, 0x1D7D8,
			0x1D7E2, 0x01D7EC, 0x1D7F6, 0x1E140, 0x1E2F0, 0x1E4F0, 0x1E950, 0x1FBF0
		};
		for( auto i : zero )
		{
			if( i <= codepoint && codepoint - i < 10 )
				return i;
		}
		return 0;
	}
}
