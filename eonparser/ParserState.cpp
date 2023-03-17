#include "ParserState.h"


namespace eon
{
	namespace parser
	{
		void State::_initialize( source::Reporter& reporter )
		{
			Tokenizer tokenizer;
			_prepTokenizer( tokenizer );
			ReTokenizer retokenizer;
			_prepReTokenizer( retokenizer );

			TokenParser temp( tokenizer( *Source ) );
			Tokens = TokenParser( retokenizer( temp ) );
			Report = &reporter;
		}

		void State::_prepTokenizer( Tokenizer& tokenizer )
		{
			tokenizer.registerEonNamesAsTokens();
			tokenizer.registerSingleCharSequenceAsToken( ' ', name_space );

			tokenizer.registerSingleCharAsToken( '"', name_doublequote );
			tokenizer.registerSingleCharAsToken( '\'', name_singlequote );
			tokenizer.registerCharSequenceAsToken( charcat::number_ascii_digit, name_digits );
			tokenizer.registerSingleCharAsToken( '_', name_underscore );
			tokenizer.registerCharSequenceAsToken( charcat::letter_lowercase, name_letters );
			tokenizer.registerCharSequenceAsToken( charcat::letter_uppercase, name_letters );
			tokenizer.registerCharSequenceAsToken( charcat::letter_titlecase, name_letters );
			tokenizer.registerCharSequenceAsToken( charcat::letter_modifier, name_letters );
			tokenizer.registerCharSequenceAsToken( charcat::letter_other, name_letters );
			tokenizer.registerSingleCharAsToken( '(', symbol_open_round );
			tokenizer.registerSingleCharAsToken( ')', symbol_close_round );
			tokenizer.registerSingleCharAsToken( '@', name_at );
			tokenizer.registerSingleCharAsToken( '\\', name_backslash );
			tokenizer.registerSingleCharAsToken( '=', symbol_assign );
			tokenizer.registerSingleCharAsToken( '-', symbol_minus );
			tokenizer.registerSingleCharSequenceAsToken( '.', symbol_point );
			tokenizer.registerSingleCharSequenceAsToken( ':', symbol_colon );
			tokenizer.registerSingleCharAsToken( ',', symbol_comma );
			tokenizer.registerCharSequenceAsToken( "//", name( "line_comment" ) );
			tokenizer.registerCharSequenceAsToken( "/-", name( "comment_start" ) );
			tokenizer.registerCharSequenceAsToken( "-/", name( "comment_end" ) );
			tokenizer.registerSingleCharAsToken( '/', symbol_divide );
			tokenizer.registerSingleCharAsToken( ';', symbol_semicolon );
			tokenizer.registerAnySingleCharAsToken( "+*~&|^<>[]", name_operator );
			tokenizer.registerCharSequenceAsToken( "==", symbol_eq );
			tokenizer.registerCharSequenceAsToken( "!=", symbol_ne );
			tokenizer.registerCharSequenceAsToken( ">=", symbol_ge );
			tokenizer.registerCharSequenceAsToken( "<=", symbol_le );
			tokenizer.registerCharSequenceAsToken( "+=", symbol_plus_assign );
			tokenizer.registerCharSequenceAsToken( "-=", symbol_minus_assign );
			tokenizer.registerCharSequenceAsToken( "*=", symbol_multiply_assign );
			tokenizer.registerCharSequenceAsToken( "/=", symbol_divide_assign );
			tokenizer.registerCharSequenceAsToken( "'s", symbol_member );
			tokenizer.registerCharSequenceAsToken( "<<", symbol_push );
			tokenizer.registerCharSequenceAsToken( ">>", symbol_pull );
			tokenizer.registerCharSequenceAsToken( "<=>", symbol_cmp );
			tokenizer.registerCharSequenceAsToken( "T(", name_typetuple );
			tokenizer.registerCharSequenceAsToken( "static(", name_static );
			tokenizer.registerCharSequenceAsToken( "optional(", name_optional );
			tokenizer.registerCharSequenceAsToken( "dynamic(", name_dynamic );
			tokenizer.registerCharSequenceAsToken( "data(", name_data );
			tokenizer.registerCharSequenceAsToken( "ex(", name_expression );
			tokenizer.registerSingleCharAsToken( '#', name_hash );
		}
		void State::_prepReTokenizer( ReTokenizer& retokenizer )
		{
			retokenizer.addRule( new ReTokenizer::EncloseRule( name_string, name_doublequote, name_backslash ) );
			retokenizer.addRule( new ReTokenizer::PrefixEncloseRule( name_bytes, "B", name_doublequote, name_backslash ) );
			retokenizer.addRule( new ReTokenizer::PrefixEncloseRule( name_path, "p", name_doublequote, name_backslash ) );
			retokenizer.addRule( new ReTokenizer::EncloseRule( name_char, name_singlequote, name_backslash ) );
			retokenizer.addRule( new ReTokenizer::PrefixEncloseRule( name_byte, "B", name_singlequote, name_backslash ) );
			retokenizer.addRule( new ReTokenizer::PrefixEncloseRule( name_regex, "r", name_doublequote, name_backslash ) );
			retokenizer.addRule( new ReTokenizer::LinestartRule( name_indentation, name_space ) );
			retokenizer.addRule( new ReTokenizer::LiteralNameRule( name_bool, { "true", "false" } ) );
			retokenizer.addRule( new ReTokenizer::SequenceRule( name_float, { name_digits, symbol_point, name_digits } ) );
			retokenizer.addRule( new ReTokenizer::SequenceRule( name_literal, { name_hash, name_name } ) );
			retokenizer.addRule( new ReTokenizer::PrefixAlternatingRule( name_namepath, "@", name_name, symbol_divide ) );
			retokenizer.addRule( new ReTokenizer::EncloseRule( name_comment, name( "linecomment" ), name_newline ) );
			retokenizer.addRule(
				new ReTokenizer::EncloseRule( name_comment, name( "commentstart" ), name( "commentend" ) ) );
			retokenizer.addRule( new ReTokenizer::RemoveRule( { name_space } ) );
		}
	}
}
