#pragma once

#include <eontest/Test.h>
#include <eontokenizer/Tokenizer.h>
#include <eontokenizer/TokenParser.h>
#include <eontokenizer/ReTokenizer.h>
#include <eonsource/String.h>
#include <chrono>


namespace eon
{
	class TokenizerTest : public eontest::EonTest
	{
	public:
		Tokenizer Tok;
		void prepare()
		{
			Tok.registerTokenChar( name_space, ' ', Tokenizer::Match::sequence );
			Tok.registerTokenChar( name::get( "tab" ), '\t', Tokenizer::Match::sequence );
			Tok.registerSequenceToken( name_operator, "+" );
			Tok.registerSequenceToken( name_operator, "-" );
			Tok.registerSequenceToken( name_operator, "=" );
			Tok.registerSequenceToken( name_operator, "+=" );
			Tok.registerSequenceToken( name_operator, "+=" );
			Tok.registerTokenChars( name_symbol, "<>{}!@#$%&/?*/^~,.:;", Tokenizer::Match::single );
			Tok.registerTokenChar( name_backslash, '\\', Tokenizer::Match::single );
			Tok.registerTokenChar( name_doublequote, '"', Tokenizer::Match::single );
			Tok.registerTokenChar( name_singlequote, '\'', Tokenizer::Match::single );
			Tok.registerTokenCharcat( name_digits, charcat::number_ascii_digit, Tokenizer::Match::sequence );
			Tok.registerTokenChar( name_underscore, '_', Tokenizer::Match::sequence );
			Tok.registerTokenCharcat( name_letters, charcat::letter_lowercase, Tokenizer::Match::sequence );
			Tok.registerTokenCharcat( name_letters, charcat::letter_uppercase, Tokenizer::Match::sequence );
			Tok.registerTokenCharcat( name_letters, charcat::letter_titlecase, Tokenizer::Match::sequence );
			Tok.registerTokenCharcat( name_letters, charcat::letter_modifier, Tokenizer::Match::sequence );
			Tok.registerTokenCharcat( name_letters, charcat::letter_other, Tokenizer::Match::sequence );
			Tok.registerTokenChar( name_open, '(', Tokenizer::Match::single );
			Tok.registerTokenChar( name_close, ')', Tokenizer::Match::single );
			Tok.registerTokenChar( name_open_square, '[', Tokenizer::Match::single );
			Tok.registerTokenChar( name_close_square, ']', Tokenizer::Match::single );
		}
	};
	class TokenParserTest : public TokenizerTest {};
	class ReTokenizerTest : public TokenizerTest {};
}
