#include "RawParser.h"
#include "OperatorRules.h"
#include "BoolLiteralRule.h"
#include "DefineRules.h"
#include "NamePathRule.h"
#include "NumericalRules.h"
#include <eontypes/Bool.h>
#include <eontypes/Byte.h>
#include <eontypes/Char.h>
#include <eontypes/Integer.h>
#include <eontypes/Floatingpt.h>
#include <eontypes/Name.h>
#include <eontypes/Handle.h>
#include <eontypes/String.h>
#include <eontypes/Bytes.h>
#include <eontypes/Path.h>
#include <eontypes/Regex.h>
#include <eontypes/NamePath.h>
#include <eontypes/OperatorAction.h>
#include <eontypes/DataTuple.h>
#include <eontypes/EonType.h>
#include <eontypes/DynamicTuple.h>
#include <stack>


namespace eon
{
	namespace parser
	{
		std::vector<Element> Raw::operator()()
		{
			Elements.clear();
			while( Parser )
			{
				if( !_parse() )
				{
					Elements.clear();
					break;
				}
			}
			if( !_processOperators() )
				Elements.clear();
			return std::move( Elements );
		}




		bool Raw::_parse()
		{
			auto& token = Parser.current();

			if( token.is( name_name ) )
				Elements.push_back( Element( name_name, eon::name( token.str() ), token.source() ) );
//			else if( token.is( name_literal ) )
//				Elements.push_back( Element( name_literal, eon::name( token.str() ), token.source() ) );
			else if( token.is( name_string ) )
				Elements.push_back( Element( name_string, new StringInstance( token.str(), token.source() ) ) );
			else if( token.is( name_bytes ) )
				Elements.push_back( Element( name_bytes, new BytesInstance( token.str().stdstr(), token.source() ) ) );
			else if( token.is( name_path ) )
				Elements.push_back( Element( name_path, new PathInstance( path( token.str() ), token.source() ) ) );
			else if( token.is( name_char ) )
				Elements.push_back( Element( name_char, new CharInstance( *token.str().begin(), token.source() ) ) );
			else if( token.is( name_byte ) )
				Elements.push_back( Element( name_byte, new ByteInstance( static_cast<byte_t>( *token.str().begin() ),
					token.source() ) ) );
			else if( token.is( name_regex ) )
				Elements.push_back( Element( name_regex, new RegexInstance( token.str(), token.source() ) ) );
			else if( token.is( name_bool ) )
				Elements.push_back( Element( name_bool, new BoolInstance( token.str() == "true", token.source() ) ) );
			else if( token.is( name_float ) )
				Elements.push_back( Element( name_float, new FloatingptInstance<high_t>( token.str().toDouble(),
					token.source() ) ) );
			else if( token.is( name_namepath ) )
				Elements.push_back( Element( name_namepath, new NamePathInstance( token.str(), token.source() ) ) );
			else if( token.is( name_digits ) )
				Elements.push_back( Element( name_int, new IntegerInstance<int_t>(
					static_cast<int_t>( token.str().toInt64() ), token.source() ) ) );
			else if( token.is( name_short ) )
				Elements.push_back( Element( name_short, new IntegerInstance<short_t>(
					static_cast<short_t>( token.str().toInt64() ), token.source() ) ) );
			else if( token.is( name_long ) )
				Elements.push_back( Element( name_long, new IntegerInstance<long_t>( token.str().toInt64(),
					token.source() ) ) );
			else if( token.is( name_low ) )
				Elements.push_back( Element( name_low, new IntegerInstance<low_t>(
					static_cast<low_t>( token.str().toDouble() ), token.source() ) ) );
			else if( token.is( name_high ) )
				Elements.push_back( Element( name_high, new IntegerInstance<high_t>( token.str().toLongDouble(),
					token.source() ) ) );

			else if( token.is( name_symbol ) )
				Elements.push_back( Element( name_symbol, *token.str().begin(), token.source() ) );
			else if( token.is( name_point ) && token.str().numChars() == 3 )
				Elements.push_back( Element( name_ellipsis, token.source() ) );

			else if( token.is( name_defvar ) )
				Elements.push_back( Element( name_defvar, token.source() ) );

			else if( token.is( name_indentation ) )
				Elements.push_back( Element( name_indentation, token.str().numChars() / 2, token.source() ) );

			else if( token.is( name_operator ) )
				Elements.push_back( Element( name_operator, type::operators::mapCode( token.str() ), token.source() ) );
			else if( token.is( name_open ) )
				Elements.push_back( Element( name_open, token.source() ) );
			else if( token.is( name_close ) )
				Elements.push_back( Element( name_close, token.source() ) );
			else if( token.is( name_open_square ) )
				Elements.push_back( Element( name_brace, name_open_square, token.source() ) );
			else if( token.is( name_close_square ) )
				Elements.push_back( Element( name_brace, name_close_square, token.source() ) );
			else if( token.is( name_open_curly ) )
				Elements.push_back( Element( name_brace, name_open_curly, token.source() ) );
			else if( token.is( name_close_curly ) )
				Elements.push_back( Element( name_brace, name_close_curly, token.source() ) );

			else if( token.is( name_newline ) )
				_processNewline();

			else
			{
				Reporter->error( "This does not belong her!", token.source() );
				return false;
			}

			Parser.forward();
			return true;
		}

		void Raw::_processNewline()
		{
			Elements.push_back( Element( name_newline, Parser.current().source() ) );

			if( Parser && Parser.current().is( name_indentation ) )
			{
				Parser.forward();
				Elements.push_back( Element( name_indentation, Parser.current().str().numChars() / 2,
					Parser.current().source() ) );
			}
		}


		bool Raw::_processOperators()
		{
			// Go through the elements and properly identify operators
			std::stack<type::operators::code> ops;
			for( auto elm = Elements.begin(); elm != Elements.end(); ++elm )
			{
				if( elm->is( name_open_square ) )
					if( !_processSquareBraceOperator( elm ) )
						return false;
			}
			return true;
		}
		std::vector<Element>::iterator Raw::_skipEnclosed( std::vector<Element>::iterator first )
		{
			// We will skip any elements inside 
			std::stack<std::vector<Element>::iterator> brackets;
			brackets.push( first );
			std::vector<Element>::iterator elm = first;

			for( ++elm; elm != Elements.end() && !brackets.empty(); ++elm )
			{
				if( elm->is( name_close ) )
				{
					if( brackets.top()->is( name_open ) )
						brackets.pop();
					else
					{
						Reporter->error( "Unbalanced parenthesis!", brackets.top()->source() );
						return Elements.end();
					}
				}
				else if( elm->is( name_close_curly ) )
				{
					if( brackets.top()->is( name_open_curly ) )
						brackets.pop();
					else
					{
						Reporter->error( "Unbalanced curly brackets!", brackets.top()->source() );
						return Elements.end();
					}
				}
				else if( elm->is( name_close_square ) )
				{
					if( brackets.top()->is( name_open_square ) )
						brackets.pop();
					else
					{
						Reporter->error( "Unbalanced square brackets!", brackets.top()->source() );
						return Elements.end();
					}
				}
				else if( elm->is( name_open ) || elm->is( name_open_curly ) || elm->is( name_open_square ) )
					brackets.push( elm );
			}
			if( !brackets.empty() )
			{
				if( brackets.top()->is( name_open ) )
					Reporter->error( "Unablacked parenthesis!", brackets.top()->source() );
				else if( brackets.top()->is( name_open_curly ) )
					Reporter->error( "Unablacked curly brackets!", brackets.top()->source() );
				else
					Reporter->error( "Unablacked square brackets!", brackets.top()->source() );
				return Elements.end();
			}
			return elm;
		}
		bool Raw::_processSquareBraceOperator( std::vector<Element>::iterator first )
		{
			// Locate the matching closing ']'. If there is a ':' inbetween, we
			// have a slice operator, otherwise element.
			// Note that we will identify all element and slice operators
			// nested within!

			// The 'bool' element is true if slice and false if just element
			std::stack<std::pair<std::vector<Element>::iterator, bool>> nesting;
			nesting.push( std::make_pair( first, false ) );	// The opening '['

			for( auto elm = first + 1; elm != Elements.end(); ++elm )
			{
				if( elm->is( name_symbol ) && elm->symbValue() == ':' )
				{
					*elm = Element( name_operator, type::operators::code::slice2, elm->source() );
					nesting.top().second = true;
				}

				else if( elm->is( name_open_square ) )
					nesting.push( std::make_pair( first, false ) );

				else if( elm->is( name_close_square ) )
				{
					if( nesting.top().second )
					{
						*nesting.top().first = Element( name_operator, type::operators::code::slice,
							nesting.top().first->source() );
						*elm = Element( name_operator, type::operators::code::slice3, elm->source() );
					}
					else
					{
						*nesting.top().first = Element( name_operator, type::operators::code::element,
							nesting.top().first->source() );
						*elm = Element( name_operator, type::operators::code::element2, elm->source() );
					}
					nesting.pop();
					if( nesting.empty() )
						break;
				}

				else if( elm->is( name_open ) || elm->is( name_open_curly ) )
				{
					elm = _skipEnclosed( elm );
					if( elm == Elements.end() )
						return false;
				}
			}
			if( !nesting.empty() )
			{
				Reporter->error( "Unbalanced square brackets!", nesting.top().first->source() );
				return false;
			}
			return true;
		}


		void Raw::_prepare()
		{
			// Get basic tokens
			Reporter->info( "Running initial tokenizing", Source );
			auto tokens = tokenizer()( Source );

			// Make the tokens more substantial
			Reporter->info( "Re-tokenizing for optimal parsing", Source );
			TokenParser parser( std::move( tokens ) );
			Parser = TokenParser( _retokenize()( parser ) );
		}


		Tokenizer& Raw::tokenizer()
		{
			static Tokenizer tokenizer;
			if( !tokenizer )
			{
				tokenizer.registerTokenChar( name_space, ' ', Tokenizer::Match::sequence );
				tokenizer.registerTokenChar( name_tab, '\t', Tokenizer::Match::sequence );
				tokenizer.registerTokenCharcat( name_letters, charcat::letter_lowercase, Tokenizer::Match::sequence );
				tokenizer.registerTokenCharcat( name_letters, charcat::letter_uppercase, Tokenizer::Match::sequence );
				tokenizer.registerTokenCharcat( name_letters, charcat::letter_titlecase, Tokenizer::Match::sequence );
				tokenizer.registerTokenCharcat( name_letters, charcat::letter_modifier, Tokenizer::Match::sequence );
				tokenizer.registerTokenCharcat( name_letters, charcat::letter_other, Tokenizer::Match::sequence );
				tokenizer.registerTokenCharcat( name_digits, charcat::number_ascii_digit, Tokenizer::Match::sequence );
				tokenizer.registerTokenChar( name_underscore, '_', Tokenizer::Match::sequence );
				tokenizer.registerTokenChar( name_point, '.', Tokenizer::Match::sequence );
				tokenizer.registerTokenChars( name_operator, "+-*/=~&|^<>,", Tokenizer::Match::single );
				tokenizer.registerSequenceToken( name_operator, "==" );
				tokenizer.registerSequenceToken( name_operator, "!=" );
				tokenizer.registerSequenceToken( name_operator, ">=" );
				tokenizer.registerSequenceToken( name_operator, "<=" );
				tokenizer.registerSequenceToken( name_operator, "+=" );
				tokenizer.registerSequenceToken( name_operator, "-=" );
				tokenizer.registerSequenceToken( name_operator, "*=" );
				tokenizer.registerSequenceToken( name_operator, "/=" );
				tokenizer.registerSequenceToken( name_member, "'s" );
				tokenizer.registerSequenceToken( name_operator, "<<" );
				tokenizer.registerSequenceToken( name_operator, ">>" );
				tokenizer.registerSequenceToken( name_operator, "<=>" );
				tokenizer.registerTokenChars( name_symbol, ":;!#%?", Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_at, '@', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_doublequote, '"', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_singlequote, '\'', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_open, '(', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_close, ')', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_open_square, '[', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_close_square, ']', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_open_curly, '{', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_close_curly, '}', Tokenizer::Match::single );
				tokenizer.registerTokenChar( name_backslash, '\\', Tokenizer::Match::single );
				tokenizer.registerEonNameTokens();
			}
			return tokenizer;
		}
		ReTokenizer& Raw::_retokenize()
		{
			static ReTokenizer retok;
			if( !retok )
			{
				// Create literal string, bytes, path, char and byte
				retok.addRule( new ReTokenizer::EncloseRule( name_string, name_doublequote, name_backslash ) );
				retok.addRule( new ReTokenizer::PrefixEncloseRule( name_bytes, "b", name_doublequote, name_backslash ) );
				retok.addRule( new ReTokenizer::PrefixEncloseRule( name_path, "p", name_doublequote, name_backslash ) );
				retok.addRule( new ReTokenizer::EncloseRule( name_char, name_singlequote, name_backslash ) );
				retok.addRule( new ReTokenizer::PrefixEncloseRule( name_byte, "b", name_singlequote, name_backslash ) );
				retok.addRule( new ReTokenizer::PrefixEncloseRule( name_regex, "r", name_doublequote, name_backslash ) );

				// Percentage operators
				retok.addRule( new PercentOfRule( name_operator ) );
				retok.addRule( new AsPercentOfRule( name_operator ) );

				// Create indentation tokens
				retok.addRule( new ReTokenizer::LinestartRule( name_indentation, name_space ) );

				// Create boolean literals
				retok.addRule( new BoolLiteralRule( name_bool ) );

				// Create define rules
				retok.addRule( new DefineVarRule( name_defvar ) );

				// Create numerical rules
				retok.addRule( new ReTokenizer::SequenceRule( name_float, { name_digits, name_point, name_digits } ) );
				retok.addRule( new ShortRule( name_short ) );
				retok.addRule( new LongRule( name_long ) );
				retok.addRule( new LowRule( name_low ) );
				retok.addRule( new HighRule( name_high ) );

				// Create literal namepath tokens
				retok.addRule( new NamePathRule( name_namepath ) );

//				// Create names by combining letters, digits and underscore
//				// Use a regex to prevent all digits
//				retok.addRule( new ReTokenizer::ComboRule( name_name, { name_letters, name_digits, name_underscore },
//					regex{ R"(^\d+$)" } ) );

				// Get rid of spaces otherwise
				retok.addRule( new ReTokenizer::RemoveRule( { name_space } ) );
			}
			return retok;
		}
	}
}
