#include "ReTokenizer.h"


namespace eon
{
	bool ReTokenizer::RemoveRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		auto& pos = parser.viewed().source().start();
		while( !parser.atEnd() && Remove.find( parser.viewed().type() ) != Remove.end() )
			parser.forward();
		return parser.viewed().source().start() != pos;
	}
	bool ReTokenizer::EncloseRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		if( parser.atEnd() || EncloseStart != parser.viewed().type() || !parser.exists() )
			return false;
		auto initial = parser.viewedPos();
		parser.forward();
		if( !parser.atEnd() )
		{
			auto matched = Token( parser.viewed().source(), Name );
			if( parser.viewed().type() != EncloseEnd )
				return _match( initial, matched, parser, output );
			else
			{
				matched.source().moveEnd( -1 );
				output.push_back( std::move( matched ) );
				parser.forward();
				return true;
			}
		}
		else
			return false;
	}
	bool ReTokenizer::EncloseRule::_match( size_t initial, Token matched, TokenParser& parser, std::vector<Token>& output )
		const noexcept
	{
		bool esc = false;
		int nesting = 1;
		while( !parser.atEnd() )
		{
			if( !esc )
			{
				if( Escape != no_name && parser.viewed().type() == Escape )
					esc = true;
				else if( Nested )
				{
					if( parser.viewed().type() == EncloseStart )
						++nesting;
					else if( parser.viewed().type() == EncloseEnd )
					{
						if( --nesting == 0 )
						{
							matched.extend( parser.peekBehind().source().end() );
							output.push_back( std::move( matched ) );
							parser.forward();
							return true;
						}
					}
				}
				else if( parser.viewed().type() == EncloseEnd )
				{
					matched.extend( parser.viewed().source().start() );
					output.push_back( std::move( matched ) );
					parser.forward();
					return true;
				}
			}
			else
				esc = false;
			parser.forward();
		}
		parser.setView( initial );
		return false;
	}
	bool ReTokenizer::PrefixEncloseRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		if( parser.atEnd() || parser.viewed().str() != Prefix || parser.peekAhead().type() != EncloseStart )
			return false;
		auto initial = parser.viewedPos();
		parser.forward( 2 );
		if( !parser.atEnd() )
		{
			auto matched = Token( parser.viewed().source(), Name );
			if( parser.viewed().type() != EncloseEnd )
				return _match( initial, matched, parser, output );
			else
			{
				matched.source().moveEnd( -1 );
				output.push_back( std::move( matched ) );
				parser.forward();
				return true;
			}
		}
		else
			return false;
	}
	bool ReTokenizer::ComboRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		auto initial = parser.viewedPos();
		source::Ref src{ parser.viewed().source() };
		src.end( src.start() );
		Token matched( src, Name );
		return _match( initial, matched, parser, output );
	}
	bool ReTokenizer::ComboRule::_match( size_t initial, Token matched, TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		while( !parser.atEnd() && Combo.find( parser.viewed().type() ) != Combo.end() )
		{
			if( !matched.source() )
				matched = Token( parser.viewed().source(), Name );
			else
				matched.extend( parser.viewed().source().end() );
			parser.forward();
		}
		if( !matched.source() )
			return false;
		if( Exclude.empty() || !Exclude.match( matched.source().str() ) )
		{
			output.push_back( std::move( matched ) );
			return true;
		}
		parser.setView( initial );
		return false;
	}
	bool ReTokenizer::PrefixComboRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		if( parser.atEnd() || parser.viewed().str() != Prefix )
			return false;
		auto initial = parser.viewedPos();
		auto matched = Token( parser.viewed().source(), Name );
		parser.forward();
		return _match( initial, matched, parser, output );
	}
	bool ReTokenizer::AlternatingRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		auto initial = parser.viewedPos();
		Token matched( parser.viewed().source(), Name );
		return _match( initial, matched, parser, output );
	}
	bool ReTokenizer::AlternatingRule::_match( size_t initial, Token matched, TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		if( parser.viewed().is( A ) )
		{
			parser.forward();
			while( true )
			{
				if( parser.atEnd() )
				{
					if( !EndOnA )
						break;
					matched.extend( parser.last().source().end() );
					output.push_back( std::move( matched ) );
					return true;
				}
				if( !parser.viewed().is( B ) )
				{
					if( !EndOnA )
						break;
					matched.extend( parser.peekBehind().source().end() );
					output.push_back( std::move( matched ) );
					return true;
				}
				parser.forward();
				if( !parser.viewed().is( A ) )
				{
					if( EndOnA )
						break;
					matched.extend( parser.peekBehind().source().end() );
					output.push_back( std::move( matched ) );
					return true;
				}
				parser.forward();
			}
		}
		parser.setView( initial );
		return false;
	}
	bool ReTokenizer::PrefixAlternatingRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		if( parser.atEnd() || parser.viewed().str() != Prefix )
			return false;
		auto initial = parser.viewedPos();
		auto matched = Token( parser.viewed().source(), Name );
		parser.forward();
		return _match( initial, matched, parser, output );
	}
	bool ReTokenizer::SequenceRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		auto initial = parser.viewedPos();
		Token matched( parser.viewed().source(), Name );
		for( auto name : Sequence )
		{
			if( !parser.atEnd() && parser.viewed().type() == name )
			{
				if( !matched.source() )
					matched = Token( parser.viewed().source(), Name );
				else
					matched.extend( parser.viewed().source().end() );
				parser.forward();
			}
			else
			{
				matched = Token();
				break;
			}
		}

		if( matched.source() )
		{
			if( Exclude.empty() || !Exclude.match( matched.source().str() ) )
			{
				output.push_back( std::move( matched ) );
				return true;
			}
		}
		parser.setView( initial );
		return false;
	}
	bool ReTokenizer::LiteralNameRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		if( !parser.atEnd() && parser.viewed().is( name_name ) && Names.find( parser.viewed().str() ) != Names.end() )
		{
			auto matched = Token( parser.viewed().source(), Name );
			parser.forward();
			output.push_back( std::move( matched ) );
			return true;
		}
		else
			return false;
	}
	bool ReTokenizer::RegexRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		if( !parser.atEnd() && parser.viewed().is( name_name ) && Pattern.match( parser.viewed().str() ) )
		{
			auto source = parser.viewed().source();
			source.moveEnd( -1 );
			output.push_back( Token( source, Name ) );
			parser.forward();
			return true;
		}
		else
			return false;
	}
	bool ReTokenizer::LinestartRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		if( !parser.atEnd() && (
			parser.viewed().source().start().BytePos == 0 || parser.peekBehind().type() == name_newline ) )
		{
			if( parser.viewed().type() == Linestart )
			{
				output.push_back( Token( parser.viewed().source(), Name ) );
				parser.forward();
				return true;
			}
		}
		return false;
	}




	std::vector<Token> ReTokenizer::operator()( TokenParser& parser ) const noexcept
	{
		std::vector<Token> output;
		while( !parser.atEnd() )
		{
			if( !_matchARule( parser, output ) )
			{
				output.push_back( parser.viewed() );
				parser.forward();
			}
		}
		return output;
	}




	bool ReTokenizer::_matchARule( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		for( auto rule : Rules )
		{
			if( rule->match( parser, output ) )
				return true;
		}
		return false;
	}
}
