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
		Token matched;
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
	bool ReTokenizer::SequenceRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
	{
		auto initial = parser.pos();
		Token matched;
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
			for( auto rule : Rules )
			{
				if( rule->match( parser, output ) )
					goto ok;
			}
			output.push_back( parser.current() );
			parser.forward();
		ok:
			;
		}
		return output;
	}
}
