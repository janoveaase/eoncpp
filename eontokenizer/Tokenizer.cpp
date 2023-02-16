#include "Tokenizer.h"
#include <eonsource/String.h>


namespace eon
{
#ifdef EON_TEST_MODE
	struct TestTokenizer
	{
		TestTokenizer() { Src = source::String( "test", "abc def  ghi 123.456 cba fed" ); }
		TestTokenizer( string to_register )
		{
			if( to_register == "registerSingleCharAsToken" )
				Obj.registerSingleCharAsToken( ' ', name_space );
			else if( to_register == "registerSingleCharSequenceAsToken" )
				Obj.registerSingleCharSequenceAsToken( ' ', name_space );
			else if( to_register == "registerAnySingleCharAsToken:char" )
				Obj.registerAnySingleCharAsToken( "abc", name_letters );
			else if( to_register == "registryAnySingleCharSequenceAsToken" )
				Obj.registryAnySingleCharSequenceAsToken( "def", name_letters );
			else if( to_register == "registerAnySingleCharAsToken:cat" )
				Obj.registerAnySingleCharAsToken( charcat::number_ascii_digit, name_int );
			else if( to_register == "registerCharSequenceAsToken" )
				Obj.registerCharSequenceAsToken( charcat::number_ascii_digit, name_int );
			else if( to_register == "registerCharSequenceAsToken" )
				Obj.registerCharSequenceAsToken( "ghi", name_array );
			else if( to_register == "registerCharSequenceAsToken:cat" )
				Obj.registerCharSequenceAsToken( charcat::number_ascii_digit, name_int );
			else if( to_register == "registerCharSequenceAsToken:string" )
				Obj.registerCharSequenceAsToken( "456", name_int );
		}
		string next( const string& str ) { return !str.empty() ? ";" : ""; }
		string grp( Tokenizer::CharacterGrouping group ) {
			return ( group == Tokenizer::CharacterGrouping::single ) ? "single" : "sequence"; }
		string charMap()
		{
			string result;
			for( auto& elm : Obj.Conf.CharMap )
				result << next( result ) << elm.first << "=" << eon::str( elm.second.first ) << ":" << grp(
					elm.second.second );
			return result;
		}
		string charCat( charcat cat ) { return std::to_string( static_cast<int>( cat ) ); }
		string catMap()
		{
			string result;
			for( auto& elm : Obj.Conf.CatMap )
				result << next( result ) << charCat( elm.first ) << "=" << eon::str( elm.second.first ) << ":" << grp(
					elm.second.second );
			return result;
		}
		string seqMap()
		{
			string result;
			for( auto& elm : Obj.Conf.SeqMap )
				result << next( result ) << elm.first << "=" << eon::str( elm.second );
			return result;
		}
		void basicPrep()
		{
			Obj.registerSingleCharAsToken( ' ', name_space );
			Obj.registerCharSequenceAsToken( charcat::number_ascii_digit, name_int );
			Obj.registerAnySingleCharAsToken( "abc", name( "abc" ) );
			Obj.registryAnySingleCharSequenceAsToken( "def", name( "def" ) );
			Obj.registerCharSequenceAsToken( "ghi", name( "ghi" ) );
			Obj.registerSingleCharAsToken( '.', name_point );
		}
		string tokenize()
		{
			auto tokens = Obj( Src );
			string str;
			for( auto& token : tokens )
			{
				if( !str.empty() )
					str += "|";
				str << eon::str( token.type() ) << "=" << token.str();
			}
			return str;
		}
		source::String Src;
		Tokenizer Obj;
	};
#endif

	EON_TEST_2STEP( Tokenizer, hasConfiguration, false,
		Tokenizer obj,
		EON_FALSE( obj.hasConfiguration() ) );

	EON_TEST_3STEP( Tokenizer, hasConfiguration, true,
		Tokenizer obj,
		obj.registerSingleCharAsToken( ' ', name_space ),
		EON_TRUE( obj.hasConfiguration() ) );

	EON_TEST_2STEP( Tokenizer, registerSingleCharAsToken, basic,
		TestTokenizer obj( "registerSingleCharAsToken" ),
		EON_EQ( " =space:single", obj.charMap() ) );

	EON_TEST_2STEP( Tokenizer, registerSingleCharSequenceAsToken, basic,
		TestTokenizer obj( "registerSingleCharSequenceAsToken" ),
		EON_EQ( " =space:sequence", obj.charMap() ) );

	EON_TEST_2STEP( Tokenizer, registerAnySingleCharAsToken, char,
		TestTokenizer obj( "registerAnySingleCharAsToken:char" ),
		EON_EQ( "a=letters:single;b=letters:single;c=letters:single", obj.charMap() ) );

	EON_TEST_2STEP( Tokenizer, registryAnySingleCharSequenceAsToken, basic,
		TestTokenizer obj( "registryAnySingleCharSequenceAsToken" ),
		EON_EQ( "d=letters:sequence;e=letters:sequence;f=letters:sequence", obj.charMap() ) );

	EON_TEST_2STEP( Tokenizer, registerAnySingleCharAsToken, cat,
		TestTokenizer obj( "registerAnySingleCharAsToken:cat" ),
		EON_EQ( obj.charCat( charcat::number_ascii_digit ) + "=int:single", obj.catMap() ) );

	EON_TEST_2STEP( Tokenizer, registerCharSequenceAsToken, cat,
		TestTokenizer obj( "registerCharSequenceAsToken:cat" ),
		EON_EQ( obj.charCat( charcat::number_ascii_digit ) + "=int:sequence", obj.catMap() ) );

	EON_TEST_2STEP( Tokenizer, registerCharSequenceAsToken, string,
		TestTokenizer obj( "registerCharSequenceAsToken:string" ),
		EON_EQ( "456=int", obj.seqMap() ) );

	EON_TEST_2STEP( Tokenizer, registerEonNamesAsTokens, false,
		TestTokenizer obj,
		EON_FALSE( obj.Obj.Conf.MatchEonNames ) );
	EON_TEST_3STEP( Tokenizer, registerEonNamesAsTokens, true,
		TestTokenizer obj,
		obj.Obj.registerEonNamesAsTokens(),
		EON_TRUE( obj.Obj.Conf.MatchEonNames ) );




	std::vector<Token> Tokenizer::operator()( source::Ref src )
	{
		Scanner scanner( Conf, src );
		return scanner.scan();
	}
	EON_TEST_3STEP( Tokenizer, operator_call, basic,
		TestTokenizer obj,
		obj.basicPrep(),
		EON_EQ(
			"abc=a|abc=b|abc=c|space= |def=def|space= |space= |ghi=ghi|space= |int=123|point=.|int=456|space= |abc=c|abc=b"
			"|abc=a|space= |def=fed", obj.tokenize() ) );



	Tokenizer::Scanner::Scanner( const Configuration& conf, source::Ref source )
	{
		Conf = &conf;
		Source = source;
		CurMatch = source::Ref( Source.source(), Source.start(), Source.source().getPosAtOffset( Source.start(), 1 ) );
		Chars = &Characters::get();
		Unmatched = source::Ref( Source.source(), Source.start(), Source.start() );
	}

	std::vector<Token> Tokenizer::Scanner::scan()
	{
		while( !Source.atEnd( CurMatch.last() ) && _scan() )
			;
		return std::move( Tokens );
	}

	bool Tokenizer::Scanner::_scan()
	{
		auto type_name = _identifyType();
		if( type_name != no_name )
			_processNewType( type_name );
		else if( CurMatchName != no_name )
			_processOldType();
		else
			_processUnmatched();
		if( !CurMatch.moveEnd( 1 ) )
		{
			_processEndOfSource( type_name );
			return false;
		}
		return true;
	}

	name_t Tokenizer::Scanner::_identifyType()
	{
		auto chr = CurMatch.source().chr( CurMatch.last() );
		if( chr == NewlineChr )
			return CurMatch.numChars() == 1 ? name_newline : no_name;

		auto type_name = _extendToMaximum();
		if( type_name == no_name )
		{
			type_name = _matchCharMap( chr );
			if( type_name == no_name )
				return _matchCategoryMap( chr );
		}
		return type_name;
	}

	name_t Tokenizer::Scanner::_extendToMaximum()
	{
		if( Conf->LongestSeq == 0 )
			return no_name;
		auto source = _maximizeSource();
		return _shrinkToMaxPossible( source );
	}

	name_t Tokenizer::Scanner::_shrinkToMaxPossible( source::Ref& source )
	{
		for( ; source.numChars() > 0; source.moveEnd( -1 ) )
		{
			auto found = Conf->SeqMap.find( source.str() );
			if( found != Conf->SeqMap.end() )
			{
				if( CurMatchName != no_name )
					break;
				CurMatch.moveEnd( static_cast<int>( source.numChars() ) - 1 );
				return found->second;
			}
		}
		return no_name;
	}

	source::Ref Tokenizer::Scanner::_maximizeSource()
	{
		source::Ref source = CurMatch;
		source.start( CurMatch.last() );
		for( index_t i = 1; i < Conf->LongestSeq && source.moveEnd( 1 ); ++i )
			;
		return source;
	}

	name_t Tokenizer::Scanner::_matchCharMap( char_t chr )
	{
		auto found = Conf->CharMap.find( chr );
		if( found != Conf->CharMap.end() && ( CurMatchName == no_name || CurMatchName == found->second.first ) )
		{
			if( found->second.second == CharacterGrouping::sequence || CurMatch.numChars() == 1 )
				return found->second.first;
		}
		return no_name;
	}

	name_t Tokenizer::Scanner::_matchCategoryMap( char_t chr )
	{
		auto category = Chars->category( chr );
		for( auto& pos_cat : Conf->CatMap )
		{
			if( pos_cat.first && category && ( CurMatchName == no_name || CurMatchName == pos_cat.second.first ) )
			{
				if( pos_cat.second.second == CharacterGrouping::sequence || CurMatch.numChars() == 1 )
					return pos_cat.second.first;
			}
		}
		return no_name;
	}

	void Tokenizer::Scanner::_processNewType( name_t type_name )
	{
		if( Unmatched )
		{
			Tokens.push_back( Token( Unmatched, name_undef ) );
			Unmatched = source::Ref();
		}
		CurMatchName = type_name;
	}

	void Tokenizer::Scanner::_processOldType()
	{
		CurMatch.moveEnd( -1 );
		if( !_extendWithNewType() && !_recordNameToken() )
			Tokens.push_back( Token( CurMatch, CurMatchName ) );
		CurMatch.moveStartToEnd();
		CurMatchName = no_name;
	}

	bool Tokenizer::Scanner::_extendWithNewType()
	{
		if( Conf->MatchEonNames && !Tokens.empty()
			&& _isNameCandidate( Tokens.back().type() ) && _isNameCandidate( CurMatchName ) )
		{
			Tokens.back().extendWithNewType( CurMatch.end(), name_name );
			return true;
		}
		return false;
	}

	bool Tokenizer::Scanner::_recordNameToken()
	{
		if( Conf->MatchEonNames && ( CurMatchName == name_letters || CurMatchName == name_underscore ) )
		{
			Tokens.push_back( Token( CurMatch, name_name ) );
			return true;
		}
		return false;
	}

	void Tokenizer::Scanner::_processUnmatched()
	{
		if( !Unmatched )
			Unmatched = CurMatch;
		else
			Unmatched.moveEnd( 1 );
		CurMatch.moveStart( 1 );
	}

	void Tokenizer::Scanner::_processEndOfSource( name_t type_name )
	{
		if( type_name != no_name )
		{
			if( !_extendWithNewType() && !_recordNameToken() )
				Tokens.push_back( Token( CurMatch, type_name ) );
		}
		else if( Unmatched )
			Tokens.push_back( Token( Unmatched, name_undef ) );
	}
}
