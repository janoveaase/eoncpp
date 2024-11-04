#include "TokenMatcher.h"
#include <eonsource/String.h>
#include "Tokenizer.h"


namespace eon
{
	EON_NAME( opt );
	EON_NAME( seq );

#ifdef EON_TEST_MODE
	static void createTokenizer( Tokenizer& tokenizer )
	{
		tokenizer.registerCharSequenceAsToken( charcat::letter, name_letters );
		tokenizer.registerCharSequenceAsToken( charcat::number_ascii_digit, name_digits );
		tokenizer.registerSingleCharAsToken( ' ', name_space );
	}

	struct TestMatcher
	{
		source::String Src;
		TokenParser Parser;
		TokenMatcher Obj;

		TestMatcher(
			const string& pattern = "(digits) (space)", string input_source = "one 23 for 6", size_t view = 2 )
			: Src( source::String( "test", std::move( input_source ) ) ), Obj( pattern )
		{

			Tokenizer tokenizer;
			createTokenizer( tokenizer );
			Parser = TokenParser( tokenizer( source::Ref( Src ) ) );
			Parser.setView( view );
		}
	};

	struct NameTestData
	{
		string Str{ "true 2!" };
		name_t Name{ no_name };
	};
#endif




	EON_TEST_2STEP( TokenMatcher, operator_bool, false,
		TokenMatcher obj,
		EON_FALSE( obj ) );
	EON_TEST_2STEP( TokenMatcher, operator_bool, true,
		TestMatcher obj,
		EON_TRUE( obj.Obj ) );

	bool TokenMatcher::match( TokenParser& parser ) const noexcept
	{
		auto pos = parser.viewedPos();
		auto result = Pattern.match( parser );
		parser.setView( pos );
		return result;
	}
	EON_TEST_2STEP( TokenMatcher, match, true,
		TestMatcher obj,
		EON_TRUE( obj.Obj.match( obj.Parser ) ) );
	EON_TEST_3STEP( TokenMatcher, match, false,
		TestMatcher obj,
		obj.Parser.setView( 3 ),
		EON_FALSE( obj.Obj.match( obj.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher, match, qmark_false,
		TestMatcher obj( "(digits) ?(letters) (letters)", "one 23 for 6" ),
		EON_FALSE( obj.Obj.match( obj.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher, match, qmark_true1,
		TestMatcher obj( "(digits) ?(letters) (space)", "one 23 for 6" ),
		EON_TRUE( obj.Obj.match( obj.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher, match, qmark_true2,
		TestMatcher obj( "(digits) ?(space) (letters)", "one 23 for 6" ),
		EON_TRUE( obj.Obj.match( obj.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher, match, and_false,
		TestMatcher obj( "(digits) &(space) (letters)", "one 23 for 6" ),
		EON_FALSE( obj.Obj.match( obj.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher, match, and_true,
		TestMatcher obj( "(digits) &(space) (space) (letters)", "one 23 for 6" ),
		EON_TRUE( obj.Obj.match( obj.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher, match, exclamation_false,
		TestMatcher obj( "(digits) !(space) (letters)", "one 23 for 6" ),
		EON_FALSE( obj.Obj.match( obj.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher, match, exclamation_true,
		TestMatcher obj( "(digits) !(letters) (space) (letters)", "one 23 for 6" ),
		EON_TRUE( obj.Obj.match( obj.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher, match, minus_false1,
		TestMatcher obj( "(digits) -(space) (letters)", "one 23 for 6" ),
		EON_FALSE( obj.Obj.match( obj.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher, match, minus_false2,
		TestMatcher obj( "(digits) -(letters) (space) (letters)", "one 23 for 6" ),
		EON_FALSE( obj.Obj.match( obj.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher, match, minus_true,
		TestMatcher obj( "(digits) -(letters) (letters)", "one 23 for 6" ),
		EON_TRUE( obj.Obj.match( obj.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher, match, options_false,
		TestMatcher obj( "(digits) opt((letters) (digits)) (letters)", "one 23 for 6" ),
		EON_FALSE( obj.Obj.match( obj.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher, match, options_true,
		TestMatcher obj( "(digits) opt((letters) (space)) (letters)", "one 23 for 6" ),
		EON_TRUE( obj.Obj.match( obj.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher, match, sequence_false,
		TestMatcher obj( "(digits) seq((space) (digits)) (space)", "one 23 for 6" ),
		EON_FALSE( obj.Obj.match( obj.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher, match, sequence_true,
		TestMatcher obj( "(digits) seq((space) (letters)) (space)", "one 23 for 6" ),
		EON_TRUE( obj.Obj.match( obj.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher, match, seq_within_opt,
		TestMatcher obj( "seq((digits) opt((letters)(space))) (letters)", "one 23 for 6" ),
		EON_TRUE( obj.Obj.match( obj.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher, match, opt_within_seq,
		TestMatcher obj( "opt(seq((digits) (letters)) opt((letters)(space))) (letters)", "one 23 for 6" ),
		EON_TRUE( obj.Obj.match( obj.Parser ) ) );




	string::iterator TokenMatcher::parseName( string::iterator i, name_t& name )
	{
		string::iterator pos = i;
		string str{ *i };
		for( ++i; i; ++i )
		{
			if( Characters::get().isLetter( *i ) || Characters::get().isNumberAsciiDigit( *i ) || *i == '_' )
				str += *i;
			else
				break;
		}
		name = eon::name( str );
		if( name == no_name )
			throw InvalidPattern( "Not a valid name at position " + string( pos.numChar() ) + ": \"" + str + "\"!" );
		return i;
	}
	EON_TEST_2STEP( TokenMatcher, parseName, false,
		NameTestData data,
		EON_RAISE( TokenMatcher::parseName( data.Str.begin() + 5, data.Name ),
			TokenMatcher::InvalidPattern ) );
	EON_TEST_3STEP( TokenMatcher, parseName, true,
		NameTestData data,
		TokenMatcher::parseName( data.Str.begin(), data.Name ),
		EON_EQ( name_true, data.Name ) );




#ifdef EON_TEST_MODE
	struct DataElementTestData
	{
		DataElementTestData( string pattern, size_t pos = 0 ) {
			Pattern = std::move( pattern ); Pos = Pattern.begin() + pos; }
		string Pattern;
		string::iterator Pos;
		TokenMatcher::DataElement Obj;
	};
	struct DataElementParsedData : public DataElementTestData
	{
		enum class Advance
		{
			advance,
			stay
		};
		DataElementParsedData(
			string source,
			const string& pattern,
			size_t pos = 0,
			Advance advance = Advance::advance ) : DataElementTestData( pattern, pos )
		{
			Src = eon::source::String( "test", std::move( source ) );
			Tokenizer tokenizer;
			createTokenizer( tokenizer );
			Parser = TokenParser( tokenizer( eon::source::Ref( Src ) ) );
			Obj.parse( Pos );
			if( advance == Advance::stay )
				Obj.Flags = TokenMatcher::flags::none;
		}
		source::String Src;
		TokenParser Parser;
	};
#endif

	string::iterator TokenMatcher::DataElement::parse( string::iterator i )
	{
		while( i )
		{
			if( !_parse( i ) )
				return i;
		}
		throw InvalidPattern( "Missing ')' at the end!" );
	}
	EON_TEST_2STEP( TokenMatcher_DataElement, parse, empty,
		DataElementTestData data( "" ),
		EON_RAISE( TokenMatcher::DataElement().parse( data.Pos ), TokenMatcher::InvalidPattern ) );
	EON_TEST_2STEP( TokenMatcher_DataElement, parse, missing_closure,
		DataElementTestData data( "(digits" ),
		EON_RAISE( data.Obj.parse( data.Pos ), TokenMatcher::InvalidPattern ) );
	EON_TEST_3STEP( TokenMatcher_DataElement, parse, type,
		DataElementTestData data( "(digits)" ),
		data.Obj.parse( data.Pos + 1 ),
		EON_EQ( name_digits, data.Obj.Type ) );
	EON_TEST_3STEP( TokenMatcher_DataElement, parse, str,
		DataElementTestData data( "('digits')" ),
		data.Obj.parse( data.Pos + 1 ),
		EON_EQ( "digits", data.Obj.Str ) );
	EON_TEST_2STEP( TokenMatcher_DataElement, parse, str_missing_closure,
		DataElementTestData data( "('digits)" ),
		EON_RAISE( data.Obj.parse( data.Pos + 1 ), TokenMatcher::InvalidPattern ) );


	bool TokenMatcher::DataElement::_parse( string::iterator& i )
	{
		if( isSpaceChar( *i ) )
			++i;
		else if( *i == ')' )
		{
			++i;
			return false;
		}
		else if( *i == '\'' )
			i = _parseStr( i );
		else if( Characters::get().isLetter( *i ) || Characters::get().isNumberAsciiDigit( *i ) || *i == '_' )
			i = parseName( i, Type );
		else
			throw InvalidPattern( "Unexepected character at position " + string( i.numChar() ) + "!" );
		return true;
	}
	EON_TEST_3STEP( TokenMatcher_DataElement, _parse, closure,
		string source = ")",
		auto pos = source.begin(),
		EON_NO_X( TokenMatcher::DataElement()._parse( pos ) ) );
	EON_TEST_3STEP( TokenMatcher_DataElement, _parse, type,
		DataElementTestData data( "space)" ),
		data.Obj._parse( data.Pos ),
		EON_EQ( name_space, data.Obj.Type ) );
	EON_TEST_3STEP( TokenMatcher_DataElement, _parse, str,
		DataElementTestData data( "'space')" ),
		data.Obj._parse( data.Pos ),
		EON_EQ( "space", data.Obj.Str ) );

	string::iterator TokenMatcher::DataElement::_parseStr( string::iterator i )
	{
		if( ++i )
			return _parseStartedStr( i );
		throw InvalidPattern(
			"String value is missing terminating single quote mark at position " + string( i.numChar() ) + "!" );
	}
	EON_TEST_2STEP( TokenMatcher_DataElement, _parseStr, missing_data,
		DataElementTestData data( "'" ),
		EON_RAISE( data.Obj._parse( data.Pos ), TokenMatcher::InvalidPattern ) );
	EON_TEST_3STEP( TokenMatcher_DataElement, _parseStr, basic,
		DataElementTestData data( "'data'" ),
		data.Obj._parse( data.Pos ),
		EON_EQ( "data", data.Obj.Str ) );

	string::iterator TokenMatcher::DataElement::_parseStartedStr( string::iterator i )
	{
		if( _openEndedStart( i ) )
		{
			Open = OpenEnded::start;
			++i;
		}
		return _parseStrContents( i );
	}
	EON_TEST_3STEP( TokenMatcher_DataElement, _parseStartedStr, open_ended_start1,
		DataElementTestData data( "*data'" ),
		data.Obj._parseStartedStr( data.Pos ),
		EON_EQ( "data", data.Obj.Str ) );
	EON_TEST_3STEP( TokenMatcher_DataElement, _parseStartedStr, open_ended_start2,
		DataElementTestData data( "*data'" ),
		data.Obj._parseStartedStr( data.Pos ),
		EON_EQ( TokenMatcher::DataElement::OpenEnded::start, data.Obj.Open ) );

	string::iterator TokenMatcher::DataElement::_parseStrContents( string::iterator i )
	{
		for( ; i; ++i )
		{
			if( _openEndedEnd( i ) )
			{
				if( Open != OpenEnded::none )
					throw InvalidPattern( "String value cannot end with '*' at position " + string( i.numChar() )
						+ " when also started with '*'!" );
				Open = OpenEnded::end;
				++i;
			}
			if( *i == SglQuoteChr )
				return ++i;
			else if( *i == BackSlashChr )
				i = _parseStrEscaped( i );
			else
				Str += *i;
		}
		throw InvalidPattern(
			"String value is missing terminating single quote mark at position " + string( i.numChar() ) + "!" );
	}
	EON_TEST_3STEP( TokenMatcher_DataElement, _parseStrContents, proper_closure,
		DataElementTestData data( "data'" ),
		data.Obj._parseStrContents( data.Pos ),
		EON_EQ( "data", data.Obj.Str ) );
	EON_TEST_2STEP( TokenMatcher_DataElement, _parseStrContents, end_of_string,
		DataElementTestData data( "data" ),
		EON_RAISE( data.Obj._parseStrContents( data.Pos ), TokenMatcher::InvalidPattern ) );
	EON_TEST_3STEP( TokenMatcher_DataElement, _parseStrContents, escaped_single_quote,
		DataElementTestData data( "dat\\'a'" ),
		data.Obj._parseStrContents( data.Pos ),
		EON_EQ( "dat'a", data.Obj.Str ) );
	EON_TEST_3STEP( TokenMatcher_DataElement, _parseStrContents, open_start_and_end,
		DataElementTestData data( "data*'" ),
		data.Obj.Open = TokenMatcher::DataElement::OpenEnded::start,
		EON_RAISE( data.Obj._parseStrContents( data.Pos ), TokenMatcher::InvalidPattern ) );

	string::iterator TokenMatcher::DataElement::_parseStrEscaped( string::iterator i )
	{
		++i;
		Str += *i;
		return i;
	}
	EON_TEST_3STEP( TokenMatcher_DataElement, _parseStrEscaped, basic1,
		DataElementTestData data( "\\' " ),
		data.Pos = data.Obj._parseStrEscaped( data.Pos ),
		EON_EQ( "1:1", data.Pos.encode() ) );
	EON_TEST_3STEP( TokenMatcher_DataElement, _parseStrEscaped, basic2,
		DataElementTestData data( "\\' " ),
		data.Pos = data.Obj._parseStrEscaped( data.Pos ),
		EON_EQ( "'", data.Obj.Str ) );


	bool TokenMatcher::DataElement::match( TokenParser& parser ) const noexcept
	{
		bool result = true;

		if( Type != no_name && !parser.viewed().is( Type ) )
			result = false;
		if( !Str.empty() )
			result = _matchStr( parser );
		if( ( result || negate() ) && advance() )
			parser.forward();
		if( negate() )
			result = !result;
		return result ? result : optional();
	}
	EON_TEST_2STEP( TokenMatcher_DataElement, match, type,
		DataElementParsedData data( "one ", "letters)" ),
		EON_TRUE( data.Obj.match( data.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher_DataElement, match, str,
		DataElementParsedData data( "one ", "'one')" ),
		EON_TRUE( data.Obj.match( data.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher_DataElement, match, type_and_str,
		DataElementParsedData data( "one ", "letters 'one')" ),
		EON_TRUE( data.Obj.match( data.Parser ) ) );
	EON_TEST_3STEP( TokenMatcher_DataElement, match, negate_false,
		DataElementParsedData data( "one ", "letters 'one')" ),
		data.Obj.Flags = TokenMatcher::flags::negate,
		EON_FALSE( data.Obj.match( data.Parser ) ) );
	EON_TEST_3STEP( TokenMatcher_DataElement, match, negate_true,
		DataElementParsedData data( "non ", "letters 'one')" ),
		data.Obj.Flags = TokenMatcher::flags::negate,
		EON_TRUE( data.Obj.match( data.Parser ) ) );
	EON_TEST_3STEP( TokenMatcher_DataElement, match, advance,
		DataElementParsedData data( "one ", "letters 'one')" ),
		data.Obj.match( data.Parser ),
		EON_EQ( 1, data.Parser.viewedPos() ) );
	EON_TEST_3STEP( TokenMatcher_DataElement, match, no_advance,
		DataElementParsedData data( "one ", "letters 'one')", 0, DataElementParsedData::Advance::stay ),
		data.Obj.match( data.Parser ),
		EON_EQ( 0, data.Parser.viewedPos() ) );

	bool TokenMatcher::DataElement::_matchStr( const TokenParser& parser ) const noexcept
	{
		switch( Open )
		{
			case OpenEnded::start:
				return parser.viewed().str().endsWith( Str );
			case OpenEnded::end:
				return parser.viewed().str().startsWith( Str );
			default:
				return parser.viewed().str() == Str;
		}
	}
	EON_TEST_2STEP( TokenMatcher_DataElement, _matchStr, true,
		DataElementParsedData data( "one ", "letters 'one')" ),
		EON_TRUE( data.Obj._matchStr( data.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher_DataElement, _matchStr, false,
		DataElementParsedData data( "non ", "letters 'one')" ),
		EON_FALSE( data.Obj._matchStr( data.Parser ) ) );




	string::iterator TokenMatcher::ContainerElement::parse( string::iterator i )
	{
		Parser parser( i, Elements, Root );
		return parser.parse();
	}
	EON_NO_TEST( TokenMatcher_ContainerElement, parse );

#ifdef EON_TEST_MODE
	struct ContainerParser
	{
		string Str;
		string::iterator I;
		std::list<std::shared_ptr<TokenMatcher::Element>> Elements;
		bool Root{ false };
		TokenMatcher::ContainerElement::Parser Obj;


		ContainerParser( string str, size_t pos = 0, name_t elm_type = no_name )
			: Str( std::move( str ) ), I( Str.begin() + pos ), Obj( I, Elements, Root )
		{
			Obj.ElmType = elm_type;
		}
	};
#endif

	string::iterator TokenMatcher::ContainerElement::Parser::parse()
	{
		while( I )
		{
			if( _endOfElement() )
				return I;
			if( !_skipSpace() && !_parseFlag() && !_parseName() && !_parseNewElement() )
				throw InvalidPattern( "Unexpected character at position " + string( I.numChar() ) + "!" );
		}
		if( !*Root )
			throw InvalidPattern( "Missing ')' at the end!" );
		return I;
	}
	EON_TEST_3STEP( TokenMatcher_ContainerElement_Parser, parse, type,
		ContainerParser parser( "letters)" ),
		parser.Obj.parse(),
		EON_EQ( name_letters, parser.Obj.ElmType ) );
	EON_TEST_3STEP( TokenMatcher_ContainerElement_Parser, parse, opt,
		ContainerParser parser( "opt((letters)(digits)))" ),
		parser.Obj.parse(),
		EON_EQ( 1, parser.Elements.size() ) );
	EON_TEST_3STEP( TokenMatcher_ContainerElement_Parser, parse, seq,
		ContainerParser parser( "seq((letters)(digits)))" ),
		parser.Obj.parse(),
		EON_EQ( 1, parser.Elements.size() ) );


	bool TokenMatcher::ContainerElement::Parser::_parseFlag() noexcept
	{
		switch( *I )
		{
			case '?':
				ParseFlags = flags::optional | flags::advance;
				break;
			case '&':
				ParseFlags = flags::none;
				break;
			case '!':
				ParseFlags = flags::negate;
				break;
			case '-':
				ParseFlags = flags::negate | flags::advance;
				break;
			default:
				return false;
		}
		++I;
		return true;
	}
	EON_TEST_2STEP( TokenMatcher_ContainerElement_Parser, _parseFlag, no_flag,
		ContainerParser parser( "x" ),
		EON_FALSE( parser.Obj._parseFlag() ) );
	EON_TEST_2STEP( TokenMatcher_ContainerElement_Parser, _parseFlag, qmark,
		ContainerParser parser( "?" ),
		EON_TRUE( parser.Obj._parseFlag() ) );
	EON_TEST_2STEP( TokenMatcher_ContainerElement_Parser, _parseFlag, and,
		ContainerParser parser( "&" ),
		EON_TRUE( parser.Obj._parseFlag() ) );
	EON_TEST_2STEP( TokenMatcher_ContainerElement_Parser, _parseFlag, exclamation,
		ContainerParser parser( "!" ),
		EON_TRUE( parser.Obj._parseFlag() ) );
	EON_TEST_2STEP( TokenMatcher_ContainerElement_Parser, _parseFlag, minus,
		ContainerParser parser( "-" ),
		EON_TRUE( parser.Obj._parseFlag() ) );

	bool TokenMatcher::ContainerElement::Parser::_parseNewElement()
	{
		if( *I != RoundBrcOpenChr )
			return false;
		_createNewElement();
		Elements->back()->Flags = ParseFlags;
		ParseFlags = flags::advance;
		ElmType = no_name;
		I = Elements->back()->parse( ++I );
		return true;
	}
	EON_TEST_2STEP( TokenMatcher_ContainerElement_Parser, _parseNewElement, false,
		ContainerParser parser( "letters)" ),
		EON_FALSE( parser.Obj._parseNewElement() ) );
	EON_TEST_2STEP( TokenMatcher_ContainerElement_Parser, _parseNewElement, true,
		ContainerParser parser( "(letters))" ),
		EON_TRUE( parser.Obj._parseNewElement() ) );

	void TokenMatcher::ContainerElement::Parser::_createNewElement()
	{
		if( ElmType == name_opt )
			Elements->push_back( std::make_shared<OptionsElement>() );
		else if( ElmType == name_seq )
			Elements->push_back( std::make_shared<SequenceElement>() );
		else if( ElmType == no_name )
			Elements->push_back( std::make_shared<DataElement>() );
		else
			throw InvalidPattern(
				R"(Expected "opt" or "seq" at position )" + string( Pos.numChar() ) + ", not \""
				+ eon::str( ElmType ) + "\"!" );
	}
	EON_TEST_3STEP( TokenMatcher_ContainerElement_Parser, _createNewElement, data,
		ContainerParser parser( "", 0, no_name ),
		parser.Obj._createNewElement(),
		EON_TRUE( (*parser.Elements.begin())->isData() ) );
	EON_TEST_3STEP( TokenMatcher_ContainerElement_Parser, _createNewElement, opt,
		ContainerParser parser( "", 0, name_opt ),
		parser.Obj._createNewElement(),
		EON_TRUE( ( *parser.Elements.begin() )->isOptions() ) );
	EON_TEST_3STEP( TokenMatcher_ContainerElement_Parser, _createNewElement, seq,
		ContainerParser parser( "", 0, name_seq ),
		parser.Obj._createNewElement(),
		EON_TRUE( ( *parser.Elements.begin() )->isSequence() ) );
	EON_TEST_2STEP( TokenMatcher_ContainerElement_Parser, _createNewElement, exception,
		ContainerParser parser( "", 0, name_action ),	// <- An invalid name
		EON_RAISE( parser.Obj._createNewElement(), TokenMatcher::InvalidPattern ) );


#ifdef EON_TEST_MODE
	struct OptionsElementTestData
	{
		source::String Src;
		TokenParser Parser;
		TokenMatcher::OptionsElement Obj;


		OptionsElementTestData( string source )
		{
			auto elm = std::make_shared<TokenMatcher::DataElement>();
			elm->Type = name_letters;
			Obj.Elements.push_back( elm );
			elm =  std::make_shared<TokenMatcher::DataElement>();
			elm->Type = name_digits;
			Obj.Elements.push_back( elm );
			Src = eon::source::String( "test", std::move( source ) );
			Tokenizer tokenizer;
			createTokenizer( tokenizer );
			Parser = TokenParser( tokenizer( eon::source::Ref( Src ) ) );
		}
	};
#endif

	bool TokenMatcher::OptionsElement::match( TokenParser& parser ) const noexcept
	{
		auto pos = parser.viewedPos();
		bool result = false;
		for( auto& elm : Elements )
		{
			result = elm->match( parser );
			if( result )
				break;
		}
		if( negate() )
			result = !result;
		if( !advance() )
			parser.setView( pos );
		return result || optional();
	}
	EON_TEST_2STEP( TokenMatcher_OptionsElement, match, false,
		OptionsElementTestData data( " " ),
		EON_FALSE( data.Obj.match( data.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher_OptionsElement, match, true1,
		OptionsElementTestData data( "one" ),
		EON_TRUE( data.Obj.match( data.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher_OptionsElement, match, true2,
		OptionsElementTestData data( "234" ),
		EON_TRUE( data.Obj.match( data.Parser ) ) );


#ifdef EON_TEST_MODE
	struct SequenceElementTestData
	{
		source::String Src;
		TokenParser Parser;
		TokenMatcher::SequenceElement Obj;


		SequenceElementTestData( string source )
		{
			auto elm = std::make_shared<TokenMatcher::DataElement>();
			elm->Type = name_letters;
			Obj.Elements.push_back( elm );
			elm = std::make_shared<TokenMatcher::DataElement>();
			elm->Type = name_digits;
			Obj.Elements.push_back( elm );
			Src = eon::source::String( "test", std::move( source ) );
			Tokenizer tokenizer;
			createTokenizer( tokenizer );
			Parser = TokenParser( tokenizer( eon::source::Ref( Src ) ) );
		}
	};
#endif

	bool TokenMatcher::SequenceElement::match( TokenParser& parser ) const noexcept
	{
		auto pos = parser.viewedPos();
		bool result = _matchSequence( parser );
		if( negate() )
			result = !result;
		if( !advance() )
			parser.setView( pos );
		return result || optional();
	}
	EON_TEST_2STEP( TokenMatcher_SequenceElement, match, false,
		SequenceElementTestData data( "one " ),
		EON_FALSE( data.Obj.match( data.Parser ) ) );
	EON_TEST_2STEP( TokenMatcher_SequenceElement, match, true,
		SequenceElementTestData data( "one234" ),
		EON_TRUE( data.Obj.match( data.Parser ) ) );

	bool TokenMatcher::SequenceElement::_matchSequence( TokenParser& parser ) const noexcept
	{
		for( auto elm : Elements )
		{
			if( !elm->match( parser ) )
				return false;
		}
		return true;
	}
	EON_NO_TEST( TokenMatcher_SequenceElement, _matchSequence );
}
