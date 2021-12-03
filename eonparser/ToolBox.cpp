#include "ToolBox.h"
#include "RegexRule.h"
#include "BoolLiteralRule.h"
#include "NamePathRule.h"


namespace eon
{
	namespace parser
	{
		Tokenizer& ToolBox::tokenizer()
		{
			static Tokenizer tokenizer;
			if( !tokenizer )
			{
				tokenizer.registerTokenChar( name_space, ' ', Tokenizer::Match::sequence );
				tokenizer.registerTokenChar( name_tab, '\t', Tokenizer::Match::sequence );
				tokenizer.registerTokenCharcat( name_int, charcat::number_ascii_digit, Tokenizer::Match::sequence );
				tokenizer.registerTokenCharcat( name_letters, charcat::letter_lowercase, Tokenizer::Match::sequence );
				tokenizer.registerTokenCharcat( name_letters, charcat::letter_uppercase, Tokenizer::Match::sequence );
				tokenizer.registerTokenCharcat( name_letters, charcat::letter_titlecase, Tokenizer::Match::sequence );
				tokenizer.registerTokenCharcat( name_letters, charcat::letter_modifier, Tokenizer::Match::sequence );
				tokenizer.registerTokenCharcat( name_letters, charcat::letter_other, Tokenizer::Match::sequence );
				tokenizer.registerTokenCharcat( name_digits, charcat::number_ascii_digit, Tokenizer::Match::sequence );
				tokenizer.registerTokenChar( name_underscore, '_', Tokenizer::Match::sequence );
				tokenizer.registerTokenChar( name_point, '.', Tokenizer::Match::sequence );
				tokenizer.registerTokenChar( name_operator, '+', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_operator, '-', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_operator, '*', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_operator, '/', Tokenizer::Match::single );
				tokenizer.registerSequenceToken( name_operator, "==" );
				tokenizer.registerSequenceToken( name_operator, "!=" );
				tokenizer.registerSequenceToken( name_operator, ">=" );
				tokenizer.registerSequenceToken( name_operator, "<=" );
				tokenizer.registerSequenceToken( name_operator, "+=" );
				tokenizer.registerSequenceToken( name_operator, "-=" );
				tokenizer.registerSequenceToken( name_operator, "*=" );
				tokenizer.registerSequenceToken( name_operator, "/=" );
				tokenizer.registerSequenceToken( name_member, "'s" );
				tokenizer.registerTokenChar( name_operator, '=', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_operator, '~', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_operator, '&', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_operator, '|', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_operator, '^', Tokenizer::Match::single );
				tokenizer.registerSequenceToken( name_operator, "<<" );
				tokenizer.registerSequenceToken( name_operator, ">>" );
				tokenizer.registerTokenChars( name_symbol, ":;%!@#?,*", Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_doublequote, '"', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_singlequote, '\'', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_open, '(', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_close, ')', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_open_square, '[', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_close_square, ']', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_open_curly, '{', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_close_curly, '}', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_open_angle, '<', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_close_angle, '>', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_backslash, '\\', Tokenizer::Match::single );
//			tokenizer.registerTokenChar( name_questionmark, '?', Tokenizer::Match::single );
//			tokenizer.registerTokenChar( name_hash, '#', Tokenizer::Match::single );
//			tokenizer.registerTokenChar( name_at, '@', Tokenizer::Match::single );
			}
			return tokenizer;
		}
		ReTokenizer& ToolBox::retok()
		{
			static ReTokenizer retok;
			if( !retok )
			{
				// Create names by combining letters, digits and underscore
				// Use a regex to prevent all digits
				retok.addRule( new ReTokenizer::ComboRule( name_name, { name_letters, name_digits, name_underscore },
					regex{ R"("^\d+$")" } ) );
				retok.addRule( new ReTokenizer::PrefixComboRule( name::get( "literal_name" ), "#",
					{ name_letters, name_digits, name_underscore }, regex{ R"("^#\d+$")" } ) );

				// Create literal string, bytes, path, char and byte
				retok.addRule( new ReTokenizer::EncloseRule( name_string, name_doublequote, name_backslash ) );
				retok.addRule( new ReTokenizer::PrefixEncloseRule( name_bytes, "b", name_doublequote, name_backslash ) );
				retok.addRule( new ReTokenizer::PrefixEncloseRule( name_path, "p", name_doublequote, name_backslash ) );
				retok.addRule( new ReTokenizer::EncloseRule( name_char, name_singlequote, name_backslash ) );
				retok.addRule( new ReTokenizer::PrefixEncloseRule( name_byte, "b", name_singlequote, name_backslash ) );

				// Create indentation tokens
				retok.addRule( new ReTokenizer::LinestartRule( name_indentation, name_space ) );

				// Create regex tokens
				retok.addRule( new RegexRule( name_regex ) );

				// Create boolean literals
				retok.addRule( new BoolLiteralRule( name_bool ) );

				// Create floating-point number tokens
				retok.addRule( new ReTokenizer::SequenceRule( name_float, { name_int, name_point, name_int } ) );

				// Create literal namepath tokens
				retok.addRule( new NamePathRule( name_namepath ) );

				// Get rid of spaces otherwise
				retok.addRule( new ReTokenizer::RemoveRule( { name_space } ) );
			}
			return retok;
		}

		void ToolBox::reset() noexcept
		{
			while( !OpStack.empty() )
				OpStack.pop();
			while( TreeStack.empty() )
				TreeStack.pop();
		}

		void ToolBox::_prepare()
		{
			// Get basic tokens
			Reporter->info( "Running initial tokenizing", Source );
			auto tokens = tokenizer()( Source );

			// Make the tokens more substantial
			Reporter->info( "Re-tokenizing for optimal parsing", Source );
			TokenParser parser( std::move( tokens ) );
			Parser = TokenParser( retok()( parser ) );

			// Start out with a zero-level indentation
			pushIndent( 0 );
		}
	}
}
