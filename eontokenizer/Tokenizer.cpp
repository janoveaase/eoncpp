#include "Tokenizer.h"


namespace eon
{
	std::vector<Token> Tokenizer::operator()( source::Ref src )
	{
		Source = src;
		CurMatch = source::Ref( Source.source(), Source.start(), Source.source().push( Source.start(), 1 ) );
		CurMatchName = no_name;
		auto& chars = Characters::get();
		source::Ref unmatched( Source.source(), Source.start(), Source.start() );
		while( !Source.atEnd( CurMatch.last() ) )
		{
			auto match_name = _match();
			if( match_name != no_name )
			{
				if( unmatched )
				{
					Tokens.push_back( Token( unmatched, name_undef ) );
					unmatched = source::Ref();
				}
				CurMatchName = match_name;
			}
			else if( CurMatchName != no_name )
			{
				CurMatch.pullEnd();
				if( MatchEonNames && !Tokens.empty()
					&& _isNameCandidate( Tokens.back().type() ) && _isNameCandidate( CurMatchName ) )
					Tokens.back().extendType( CurMatch.end(), name_name );
				else if( MatchEonNames && ( CurMatchName == name_letters || CurMatchName == name_underscore ) )
					Tokens.push_back( Token( CurMatch, name_name ) );
				else
					Tokens.push_back( Token( CurMatch, CurMatchName ) );
				CurMatch.startToEnd();
				CurMatchName = no_name;
			}
			else
			{
				if( !unmatched )
					unmatched = CurMatch;
				else
					unmatched.pushEnd();
				CurMatch.pushStart();
			}
			if( !CurMatch.pushEnd() )
			{
				if( match_name != no_name )
				{
					if( MatchEonNames && !Tokens.empty()
						&& _isNameCandidate( Tokens.back().type() ) && _isNameCandidate( CurMatchName ) )
						Tokens.back().extendType( CurMatch.end(), name_name );
					else if( MatchEonNames && ( CurMatchName == name_letters || CurMatchName == name_underscore ) )
						Tokens.push_back( Token( CurMatch, name_name ) );
					else
						Tokens.push_back( Token( CurMatch, match_name ) );
				}
				else if( unmatched )
					Tokens.push_back( Token( unmatched, name_undef ) );
				break;
			}
		}
		return std::move( Tokens );
	}




	name_t Tokenizer::_match()
	{
		auto chr = CurMatch.source().chr( CurMatch.last() );
		if( chr == NewlineChr )
			return CurMatch.numChars() == 1 ? name_newline : no_name;

		auto f1 = CharMap.find( chr );
		if( f1 != CharMap.end() && ( CurMatchName == no_name || CurMatchName == f1->second.first ) )
		{
			if( f1->second.second == Match::sequence || CurMatch.numChars() == 1 )
				return f1->second.first;
		}

		auto f2 = CatMap.find( Characters::get().category( chr ) );
		if( f2 != CatMap.end() && ( CurMatchName == no_name || CurMatchName == f2->second.first ) )
		{
			if( f2->second.second == Match::sequence || CurMatch.numChars() == 1 )
				return f2->second.first;
		}

		auto f3 = SeqMap.find( CurMatch.str() );
		if( f3 != SeqMap.end() )
			return f3->second;

		return no_name;
	}
}
