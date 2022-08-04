#include "Edt.h"
#include <eonparser/BoolLiteralRule.h>
#include <eonparser/DefineRules.h>
#include <eonparser/NumericalRules.h>
#include <eonparser/NamePathRule.h>
#include <eontypes/Primitives.h>
#include <eontypes/StandardTypes.h>
#include <eontypes/TypeTupleObj.h>


namespace eon
{
	DataTuple edt::load( source::Reporter& issues )
	{
		DataTuple tuple;
		if( ToLoad )
			_load( *ToLoad, tuple, issues );
		return tuple;
	}

	bool edt::validate( const DataTuple& target, const DataTuple& source, DataTuple* issues )
	{
		return false;
	}




	void edt::_toStr( const DataTuple& tuple, Stringifier& str, bool comma_sep )
	{
		for( index_t i = 0; i < tuple.numAttributes(); ++i )
		{
			auto& attribute = tuple[ i ];
			if( attribute.type().name() == name_data )
			{
				if( attribute.name() == no_name )
				{
					str.start_grp2( "(" );
					_toStr( *(DataTuple*)attribute.value(), str, true );
					str.end_grp2( ")" );
				}
				else
				{
					str.word( eon::str( attribute.name() ) );
					str.append( ":" );
					str.start_block();
					_toStr( *(DataTuple*)attribute.value(), str, false );
					str.end_block();
				}
			}
			else
			{
				if( attribute.name() != no_name )
				{
					str.word( eon::str( attribute.name() ) );
					str.op1( "=" );
				}
				attribute.value()->str( str );
			}
			if( comma_sep )
			{
				if( i < tuple.numAttributes() - 1 )
				{
					str.punct( "," );
					str.sec_split_block();
				}
			}
			else
				str.hard_lf();
		}
	}

	void edt::_load( source::Ref source, DataTuple& tuple, source::Reporter& reporter )
	{
		Reporter = &reporter;
		TokenParser temp( _tokenizer()( source ) );
		TokenParser parser( _retokenize()( temp ) );
		_loadTuple( tuple, parser, 0, false );
	}
	void edt::_loadTuple( DataTuple& tuple, TokenParser& parser, index_t indentation, bool end_by_close )
	{
		while( parser )
		{
			_skipComments( parser );
			if( indentation > 0 && _indentation( parser ) < indentation )
				return;
			if( end_by_close && parser.current().is( name_close ) )
				return;
			if( parser.current().is( name_name ) )
			{
				// We have either a 'key' or a name value
				if( parser.exists() && parser.ahead().is( name_assign ) )
					_loadAssignedChild( tuple, name( parser.current().str() ), parser );
				else if( parser.exists() && parser.ahead().is( name_colon ) )
					_loadIndentedChild( tuple, name( parser.current().str() ), parser );
				else
				{
					tuple.add( new NameInstance( name( parser.current().str() ), parser.current().source() ) );
					parser.forward();
				}
			}
			else if( parser.current().is( name_newline ) || parser.current().is( name_indentation ) )
				parser.forward();
			else if( end_by_close && parser.current().is( name_comma ) )
				parser.forward();
			else
				_loadFreeChild( tuple, parser );
		}
	}
	void edt::_loadAssignedChild( DataTuple& tuple, name_t child_name, TokenParser& parser )
	{
		parser.forward( 2 );	// Skip name and '='
		_skipComments( parser );
		auto value = _loadValue( parser );
		if( value )
			tuple.add( child_name, value );
		if( parser && value && value->type() != name_typetuple )
			parser.forward();
	}
	void edt::_loadIndentedChild( DataTuple& tuple, name_t child_name, TokenParser& parser )
	{
		auto indentation = _indentation( parser ) + 2;
		parser.forward( 2 );	// Skip name and ':'
		_skipComments( parser );
		_skipNewlines( parser );
		if( !parser )
		{
			Reporter->error( "Unexpected end of source!", parser.last().source() );
			return;
		}
		if( _indentation( parser ) < indentation )
		{
			Reporter->error( "Expected indented tuple attributes here!", parser.current().source() );
			return;
		}
		parser.forward();	// Skip indentation
		DataTuple child;
		_loadTuple( child, parser, indentation, false );
		tuple.add( child_name, new DataTuple( std::move( child ) ) );
	}
	void edt::_loadFreeChild( DataTuple& tuple, TokenParser& parser )
	{
		auto value = _loadValue( parser );
		if( value )
			tuple.add( value );
		if( parser && value && value->type() != name_typetuple )
			parser.forward();
	}
	type::Object* edt::_loadValue( TokenParser& parser )
	{
		if( parser.current().is( name_bool ) )
			return new BoolInstance( parser.current().str() == "true", parser.current().source() );
		else if( parser.current().is( name_byte ) )
			return new ByteInstance(
				static_cast<byte_t>( *parser.current().str().unescape().begin() ), parser.current().source() );
		else if( parser.current().is( name_char ) )
			return new CharInstance( *parser.current().str().begin(), parser.current().source() );
		else if( parser.current().is( name_minus ) )
			return _loadNegativeNumber( parser );
		else if( parser.current().is( name_short ) )
			return new IntegerInstance<short_t>( parser.current().str().toShort(), parser.current().source() );
		else if( parser.current().is( name_long ) )
			return new IntegerInstance<long_t>( parser.current().str().toLong(), parser.current().source() );
		else if( parser.current().is( name_float ) )
			return new FloatingptInstance<flt_t>( parser.current().str().toFloat(), parser.current().source() );
		else if( parser.current().is( name_low ) )
			return new FloatingptInstance<low_t>( parser.current().str().toLow(), parser.current().source() );
		else if( parser.current().is( name_high ) )
			return new FloatingptInstance<high_t>( parser.current().str().toHigh(), parser.current().source() );
		else if( parser.current().is( name_digits ) )
			return new IntegerInstance<int_t>( parser.current().str().toInt(), parser.current().source() );
		else if( parser.current().is( name_name ) )
			return new NameInstance( name( parser.current().str() ), parser.current().source() );
		else if( parser.current().is( name_string ) )
			return new StringInstance( parser.current().source().str(), parser.current().source() );
		else if( parser.current().is( name_bytes ) )
			return new BytesInstance( parser.current().source().str().stdstr(), parser.current().source() );
		else if( parser.current().is( name_regex ) )
			return new RegexInstance( parser.current().source().str(), parser.current().source() );
		else if( parser.current().is( name_namepath ) )
			return new NamePathInstance( parser.current().source().str(), parser.current().source() );
		else if( parser.current().is( name_path ) )
			return new PathInstance( parser.current().source().str(), parser.current().source() );
		else if( parser.current().is( name_type ) )
			return _loadTypeTuple( parser );
		else if( parser.current().is( name_open ) )
			return _loadTupleValue( parser );
		else
		{
			Reporter->error( "Don't know what this is!", parser.current().source() );
			return nullptr;
		}
	}
	type::Object* edt::_loadNegativeNumber( TokenParser& parser )
	{
		parser.forward();	// Skip minus sign
		if( !parser )
		{
			Reporter->error( "Unexpected end of EDT data!", parser.last().source() );
			return nullptr;
		}

		if( parser.current().is( name_short ) )
			return new IntegerInstance<short_t>( -parser.current().str().toShort(), parser.current().source() );
		else if( parser.current().is( name_long ) )
			return new IntegerInstance<long_t>( -parser.current().str().toLong(), parser.current().source() );
		else if( parser.current().is( name_float ) )
			return new FloatingptInstance<flt_t>( -parser.current().str().toFloat(), parser.current().source() );
		else if( parser.current().is( name_low ) )
			return new FloatingptInstance<low_t>( -parser.current().str().toLow(), parser.current().source() );
		else if( parser.current().is( name_high ) )
			return new FloatingptInstance<high_t>( -parser.current().str().toHigh(), parser.current().source() );
		else if( parser.current().is( name_digits ) )
			return new IntegerInstance<int_t>( -parser.current().str().toInt(), parser.current().source() );
		else
		{
			parser.backward();
			Reporter->error( "Unexpected minus sign here!", parser.current().source() );
			return nullptr;
		}
	}
	type::Object* edt::_loadTypeTuple( TokenParser& parser )
	{
		parser.forward();	// Skip 'T('
		EonType typetuple;
		_loadTypeTuple( typetuple, parser );
		return new TypeTupleObj( typetuple );
	}
	type::Object* edt::_loadTupleValue( TokenParser& parser )
	{
		parser.forward();	// Skip '('
		DataTuple value;
		_loadTuple( value, parser, _indentation( parser ), true );
		return new DataTuple( std::move( value ) );
	}

	void edt::_loadTypeTuple( EonType& tuple, TokenParser& parser )
	{
		_skipComments( parser );

		// Empty type?
		if( parser.current().is( name_close ) )
			return;

		// The type tuple consists of named and unnamed attributes where the value is
		// either name or type-tuple
		while( parser )
		{
			// Get attribute
			if( parser.current().is( name_name ) )
			{
				// We either have a named attribute or an unnamed attribute with name value
				if( parser.exists() && parser.ahead().is( name_assign ) )
					_loadTypeTupleChild( tuple, name( parser.current().str() ), parser );
				else
				{
					tuple << EonType( name( parser.current().str() ) );
					parser.forward();
				}
			}
			else if( parser.current().is( name_open ) )
			{
				parser.forward();
				EonType child;
				_loadTypeTuple( child, parser );
				tuple << std::move( child );
			}
			else
			{
				Reporter->error( "Expected a name or a type-tuple here!", parser.current().source() );
				return;
			}

			// Get ')' or comma
			_skipComments( parser );
			if( parser.current().is( name_close ) )
			{
				parser.forward();
				return;
			}
			if( !parser.current().is( name_comma ) )
			{
				Reporter->error( "Expected comma or ')' here!", parser.current().source() );
				return;
			}
			parser.forward();
		}

		Reporter->error( "Unexected end of source!", parser.last().source() );
	}
	void edt::_loadTypeTupleChild( EonType& tuple, name_t child_name, TokenParser& parser )
	{
		auto source = parser.current().source();
		parser.forward( 2 );	// Skip name and '='
		_skipComments( parser );
		if( !parser )
		{
			Reporter->error( "Unexected end of source!", parser.last().source() );
			return;
		}
		if( parser.current().is( name_name ) )
		{
			source.end( parser.current().source().end() );
			tuple << EonType( name( parser.current().str() ), child_name, source );
			parser.forward();
		}
		else if( parser.current().is( name_open ) )
		{
			EonType child;
			_loadTypeTuple( child, parser );
			tuple << std::move( child );
		}
		else
			Reporter->error( "Expected a name or a type-tuple here!", parser.current().source() );
	}

	index_t edt::_indentation( TokenParser& parser )
	{
		auto& first_token_on_line = parser.at( parser.lineStart() );
		if( first_token_on_line.is( name_indentation ) )
			return first_token_on_line.str().numChars();
		else
			return 0;

	}
	void edt::_skipComments( TokenParser& parser )
	{
		while( parser && parser.current().is( name_comment ) )
			parser.forward();
	}
	void edt::_skipNewlines( TokenParser& parser )
	{
		while( parser && parser.current().is( name_newline ) )
			parser.forward();
	}

	Tokenizer& edt::_tokenizer()
	{
		static Tokenizer tok;
		if( !tok )
		{
			tok.registerEonNameTokens( true );
			tok.registerTokenChar( name_space, ' ', Tokenizer::Match::sequence );
//			tok.registerTokenChar( name( "tab" ), '\t', Tokenizer::Match::sequence );
			tok.registerTokenChar( name_doublequote, '"', Tokenizer::Match::single );
			tok.registerTokenChar( name_singlequote, '\'', Tokenizer::Match::single );
			tok.registerTokenCharcat( name_digits, charcat::number_ascii_digit, Tokenizer::Match::sequence );
			tok.registerTokenChar( name_underscore, '_', Tokenizer::Match::sequence );
			tok.registerTokenCharcat( name_letters, charcat::letter_lowercase, Tokenizer::Match::sequence );
			tok.registerTokenCharcat( name_letters, charcat::letter_uppercase, Tokenizer::Match::sequence );
			tok.registerTokenCharcat( name_letters, charcat::letter_titlecase, Tokenizer::Match::sequence );
			tok.registerTokenCharcat( name_letters, charcat::letter_modifier, Tokenizer::Match::sequence );
			tok.registerTokenCharcat( name_letters, charcat::letter_other, Tokenizer::Match::sequence );
			tok.registerTokenChar( name_open, '(', Tokenizer::Match::single );
			tok.registerTokenChar( name_close, ')', Tokenizer::Match::single );
			tok.registerTokenChar( name_at, '@', Tokenizer::Match::single );
			tok.registerTokenChar( name_backslash, '\\', Tokenizer::Match::single );
			tok.registerTokenChar( name_assign, '=', Tokenizer::Match::single );
			tok.registerSequenceToken( name_minus, "-" );
			tok.registerTokenChar( name_point, '.', Tokenizer::Match::sequence );
			tok.registerTokenChar( name_colon, ':', Tokenizer::Match::sequence );
			tok.registerTokenChar( name_comma, ',', Tokenizer::Match::single );
			tok.registerTokenChar( name_slash, '/', Tokenizer::Match::single );
			tok.registerSequenceToken( name( "line_comment" ), "//" );
			tok.registerSequenceToken( name( "comment_start" ), "/-" );
			tok.registerSequenceToken( name( "comment_end" ), "-/" );
			tok.registerSequenceToken( name_type, "T(" );

//			tok.registerSequenceToken( name_operator, "+" );
//			tok.registerSequenceToken( name_operator, "-" );
//			tok.registerSequenceToken( name_operator, "=" );
//			tok.registerSequenceToken( name_operator, "+=" );
//			tok.registerSequenceToken( name_operator, "+=" );
//			tok.registerTokenChars( name_symbol, "<>{}!@#$%&/?*/^~,:;", Tokenizer::Match::single );
//			tok.registerTokenChar( name_open_square, '[', Tokenizer::Match::single );
//			tok.registerTokenChar( name_close_square, ']', Tokenizer::Match::single );
		}
		return tok;
	}
	ReTokenizer& edt::_retokenize()
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
//			retok.addRule( new PercentOfRule( name_operator ) );
//			retok.addRule( new AsPercentOfRule( name_operator ) );

			// Create indentation tokens
			retok.addRule( new ReTokenizer::LinestartRule( name_indentation, name_space ) );

			// Create boolean literals
			retok.addRule( new parser::BoolLiteralRule( name_bool ) );

			// Create define rules
			retok.addRule( new parser::DefineVarRule( name_defvar ) );

			// Create numerical rules
			retok.addRule( new ReTokenizer::SequenceRule( name_float, { name_digits, name_point, name_digits } ) );
			retok.addRule( new parser::ShortRule( name_short ) );
			retok.addRule( new parser::LongRule( name_long ) );
			retok.addRule( new parser::LowRule( name_low ) );
			retok.addRule( new parser::HighRule( name_high ) );

//			// Name rule
//			retok.addRule( new ReTokenizer::ComboRule( name_name, { name_letters, name_digits, name_underscore },
//				regex{ R"(^\d+$)" } ) );

			// Create literal namepath tokens
			retok.addRule( new parser::NamePathRule( name_namepath ) );

			// Create comment tokens
			retok.addRule( new ReTokenizer::EncloseRule( name_comment, name( "linecomment" ), name_newline ) );
			retok.addRule(
				new ReTokenizer::EncloseRule( name_comment, name( "commentstart" ), name( "commentend" ) ) );

			// Get rid of spaces otherwise
			retok.addRule( new ReTokenizer::RemoveRule( { name_space } ) );
		}
		return retok;
	}
}
