#include "ReTokenizer.h"


namespace eon
{
	bool ReTokenizer::RemoveRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		auto& pos = parser.current().source().start();
		while( parser && Remove.find( parser.current().type() ) != Remove.end() )
			parser.forward();
		return parser.current().source().start() != pos;
	}
	bool ReTokenizer::EncloseRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		if( !parser || EncloseStart != parser.current().type() || !parser.exists() )
			return false;
		auto initial = parser.pos();
		parser.forward();
		if( parser )
		{
			auto matched = Token( parser.current().source(), Name );
			if( parser.current().type() != EncloseEnd )
				return _match( initial, matched, parser, output );
			else
			{
				matched.source().pullEnd();
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
		while( parser )
		{
			if( !esc )
			{
				if( Escape != no_name && parser.current().type() == Escape )
					esc = true;
				else if( Nested )
				{
					if( parser.current().type() == EncloseStart )
						++nesting;
					else if( parser.current().type() == EncloseEnd )
					{
						if( --nesting == 0 )
						{
							matched.extend( parser.prior().source().end() );
							output.push_back( std::move( matched ) );
							parser.forward();
							return true;
						}
					}
				}
				else if( parser.current().type() == EncloseEnd )
				{
					matched.extend( parser.current().source().start() );
					output.push_back( std::move( matched ) );
					parser.forward();
					return true;
				}
			}
			else
				esc = false;
			parser.forward();
		}
		parser.pos( initial );
		return false;
	}
	bool ReTokenizer::PrefixEncloseRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		if( !parser || parser.current().str() != Prefix || parser.ahead().type() != EncloseStart )
			return false;
		auto initial = parser.pos();
		parser.forward( 2 );
		if( parser )
		{
			auto matched = Token( parser.current().source(), Name );
			if( parser.current().type() != EncloseEnd )
				return _match( initial, matched, parser, output );
			else
			{
				matched.source().pullEnd();
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
		auto initial = parser.pos();
		source::Ref src{ parser.current().source() };
		src.end( src.start() );
		Token matched( src, Name );
		return _match( initial, matched, parser, output );
	}
	bool ReTokenizer::ComboRule::_match( size_t initial, Token matched, TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		while( parser && Combo.find( parser.current().type() ) != Combo.end() )
		{
			if( !matched.source() )
				matched = Token( parser.current().source(), Name );
			else
				matched.extend( parser.current().source().end() );
			parser.forward();
		}
		if( !matched.source() )
			return false;
		if( Exclude.empty() || !Exclude.match( matched.source().str() ) )
		{
			output.push_back( std::move( matched ) );
			return true;
		}
		parser.pos( initial );
		return false;
	}
	bool ReTokenizer::PrefixComboRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		if( !parser || parser.current().str() != Prefix )
			return false;
		auto initial = parser.pos();
		auto matched = Token( parser.current().source(), Name );
		parser.forward();
		return _match( initial, matched, parser, output );
	}
	bool ReTokenizer::AlternatingRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		auto initial = parser.pos();
		Token matched( parser.current().source(), Name );
		return _match( initial, matched, parser, output );
	}
	bool ReTokenizer::AlternatingRule::_match( size_t initial, Token matched, TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		if( parser.current().is( A ) )
		{
			parser.forward();
			while( true )
			{
				if( !parser )
				{
					if( !EndOnA )
						break;
					matched.extend( parser.last().source().end() );
					output.push_back( std::move( matched ) );
					return true;
				}
				if( !parser.current().is( B ) )
				{
					if( !EndOnA )
						break;
					matched.extend( parser.prior().source().end() );
					output.push_back( std::move( matched ) );
					return true;
				}
				parser.forward();
				if( !parser.current().is( A ) )
				{
					if( EndOnA )
						break;
					matched.extend( parser.prior().source().end() );
					output.push_back( std::move( matched ) );
					return true;
				}
				parser.forward();
			}
		}
		parser.pos( initial );
		return false;
	}
	bool ReTokenizer::PrefixAlternatingRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		if( !parser || parser.current().str() != Prefix )
			return false;
		auto initial = parser.pos();
		auto matched = Token( parser.current().source(), Name );
		parser.forward();
		return _match( initial, matched, parser, output );
	}
	bool ReTokenizer::SequenceRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		auto initial = parser.pos();
		Token matched( parser.current().source(), Name );
		for( auto name : Sequence )
		{
			if( parser && parser.current().type() == name )
			{
				if( !matched.source() )
					matched = Token( parser.current().source(), Name );
				else
					matched.extend( parser.current().source().end() );
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
		parser.pos( initial );
		return false;
	}
	bool ReTokenizer::LiteralNameRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		if( parser && parser.current().is( name_name ) && Names.find( parser.current().str() ) != Names.end() )
		{
			auto matched = Token( parser.current().source(), Name );
			parser.forward();
			output.push_back( std::move( matched ) );
			return true;
		}
		else
			return false;
	}
	bool ReTokenizer::RegexRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		if( parser && parser.current().is( name_name ) && Pattern.match( parser.current().str() ) )
		{
			auto source = parser.current().source();
			source.pullEnd();
			output.push_back( Token( source, Name ) );
			parser.forward();
			return true;
		}
		else
			return false;
	}
	bool ReTokenizer::LinestartRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		if( parser && ( parser.current().source().start().bytePos() == 0 || parser.prior().type() == name_newline ) )
		{
			if( parser.current().type() == Linestart )
			{
				output.push_back( Token( parser.current().source(), Name ) );
				parser.forward();
				return true;
			}
		}
		return false;
	}




	std::vector<Token> ReTokenizer::operator()( TokenParser& parser ) const noexcept
	{
		std::vector<Token> output;
		while( parser )
		{
			if( !_matchARule( parser, output ) )
			{
				output.push_back( parser.current() );
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
