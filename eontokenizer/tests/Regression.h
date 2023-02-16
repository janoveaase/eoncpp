#pragma once

#include <eontest/Test.h>
#include <eontokenizer/Tokenizer.h>
#include <eontokenizer/TokenParser.h>
#include <eontokenizer/ReTokenizer.h>
#include <eontokenizer/TokenMatcher.h>
#include <eonsource/String.h>


namespace eon
{
	class TokenizerTest : public eontest::EonTest
	{
	public:
		Tokenizer Tok;
		void prepare()
		{
			Tok.registerSingleCharSequenceAsToken( ' ', name_space );
			Tok.registerSingleCharSequenceAsToken( '\t', name( "tab" ) );
			Tok.registerSingleCharAsToken( '+', name_operator );
			Tok.registerSingleCharAsToken( '-', name_operator );
			Tok.registerSingleCharAsToken( '=', name_operator );
			Tok.registerCharSequenceAsToken( "+=", name_operator );
			Tok.registerCharSequenceAsToken( "-=", name_operator );
			Tok.registerAnySingleCharAsToken( "<>{}!@#$%&/?*/^~,:;", name_symbol );
			Tok.registerSingleCharSequenceAsToken( '.', name_point );
			Tok.registerSingleCharAsToken( '\\', name_backslash );
			Tok.registerSingleCharAsToken( '"', name_doublequote );
			Tok.registerSingleCharAsToken( '\'', name_singlequote );
			Tok.registerCharSequenceAsToken( charcat::number_ascii_digit, name_digits );
			Tok.registerSingleCharSequenceAsToken( '_', name_underscore );
			Tok.registerCharSequenceAsToken( charcat::letter_lowercase, name_letters );
			Tok.registerCharSequenceAsToken( charcat::letter_uppercase, name_letters );
			Tok.registerCharSequenceAsToken( charcat::letter_titlecase, name_letters );
			Tok.registerCharSequenceAsToken( charcat::letter_modifier, name_letters );
			Tok.registerCharSequenceAsToken( charcat::letter_other, name_letters );
			Tok.registerSingleCharAsToken( '(', name_open );
			Tok.registerSingleCharAsToken( ')', name_close );
			Tok.registerSingleCharAsToken( '[', name_open_square );
			Tok.registerSingleCharAsToken( ']', name_close_square );
		}
		string simpleJoin( const std::vector<Token>& tokens ) const { return _join( tokens, true ); }
		string fullJoin( const std::vector<Token>& tokens ) const { return _join( tokens, false ); }
		string _join( const std::vector<Token>& tokens, bool simple ) const
		{
			string result;
			for( auto& token : tokens )
			{
				if( !result.empty() )
					result += "|";
				if( !simple )
					result << eon::str( token.type() ) << "=";
				result << token.str();
			}
			return result;
		}
	};
	class TokenParserTest : public TokenizerTest {};
	class ReTokenizerTest : public TokenizerTest {};
}
