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
		static std::shared_ptr<Characters> Cats;
		if( !Cats )
			Cats = std::shared_ptr<Characters>( new Characters() );
		return *Cats;
	}

	bool Characters::isLetterUpperCase( eon_char c ) const noexcept
	{
		if( c <= 0xDE )
			return ( c >= 0x41 && c <= 0x5A )
			|| ( c >= 0xC0 && c <= 0xD6 )
			|| ( c >= 0xD8 && c <= 0xDE );
		else
			return std::binary_search(
				letter_upper_case->begin(),
				letter_upper_case->end(),
				c );
	}
	bool Characters::isLetterLowerCase( eon_char c ) const noexcept
	{
		if( c <= 0xFF )
			return ( c >= 0x61 && c <= 0x7A )
			|| ( c >= 0xDF && c <= 0xF6 )
			|| ( c >= 0xF8 && c <= 0xFF );
		else
			return std::binary_search(
				letter_lower_case->begin(),
				letter_lower_case->end(),
				c );
	}
	bool Characters::isLetterModifier( eon_char c ) const noexcept
	{
		return ( c >= 0x2B0 && c <= 0x2C1 ) || ( c >= 0x2C6 && c <= 0x2D1 )
			|| std::binary_search( letter_modifier->begin(),
				letter_modifier->end(), c );
	}



	bool Characters::is( eon_char codepoint, charcat category ) const noexcept
	{
		switch( category )
		{
			case charcat::Cc:
				return isOtherControl( codepoint );
			case charcat::Cf:
				return isOtherFormat( codepoint );
			case charcat::Co:
				return isOtherPrivateUse( codepoint );
			case charcat::Cs:
				return isOtherSurrogate( codepoint );

			case charcat::Ll:
				return isLetterLowerCase( codepoint );
			case charcat::Lm:
				return isLetterModifier( codepoint );
			case charcat::Lo:
				return isLetterOther( codepoint );
			case charcat::Lt:
				return isLetterTitleCase( codepoint );
			case charcat::Lu:
				return isLetterUpperCase( codepoint );

			case charcat::Mc:
				return isMarkSpacingCombining( codepoint );
			case charcat::Me:
				return isMarkEnclosing( codepoint );
			case charcat::Mn:
				return isMarkNonSpacing( codepoint );

			case charcat::number_ascii_digit:
				return isNumberAsciiDigit( codepoint );
			case charcat::Nd:
				return isNumberDecimalDigit( codepoint );
			case charcat::Nl:
				return isNumberLetter( codepoint );
			case charcat::No:
				return isNumberOther( codepoint );

			case charcat::Pc:
				return isPunctuationConnector( codepoint );
			case charcat::Pd:
				return isPunctuationDash( codepoint );
			case charcat::Pe:
				return isPunctuationClose( codepoint );
			case charcat::Pf:
				return isPunctuationFinalQuote( codepoint );
			case charcat::Pi:
				return isPunctuationInitialQuote( codepoint );
			case charcat::Po:
				return isPunctuationOther( codepoint );
			case charcat::Ps:
				return isPunctuationOpen( codepoint );

			case charcat::Sc:
				return isSymbolCurrency( codepoint );
			case charcat::Sk:
				return isSymbolModifier( codepoint );
			case charcat::Sm:
				return isSymbolMath( codepoint );
			case charcat::So:
				return isSymbolOther( codepoint );

			case charcat::Zl:
				return isSeparatorLine( codepoint );
			case charcat::Zp:
				return isSeparatorParagraph( codepoint );
			case charcat::Zs:
				return isSeparatorSpace( codepoint );

			default:
				return false;
		}
	}
}
