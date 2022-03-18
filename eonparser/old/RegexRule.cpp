#include "RegexRule.h"


namespace eon
{
	namespace parser
	{
		bool RegexRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
		{
			// Start with '?', then get string, with optional flags at the end
			if( parser.current().type() != name_questionmark
				|| !parser.exists() || parser.ahead().type() != name_doublequote )
				return false;

			auto initial = parser.pos();
			parser.forward();
			auto matched = Token( parser.current().source(), Name );
			parser.forward();

			// Now we must allow for escaped double quotes in whatever remains
			// NOTE: Newlines are not permitted!
			bool esc = false;
			while( parser )
			{
				if( parser.current().type() == name_newline )
				{
					parser.pos( initial );
					return false;
				}
				if( !esc )
				{
					if( parser.current().type() == name_backslash )
						esc = true;
					else if( parser.current().type() == name_doublequote )
						break;
				}
				else
					esc = false;
				parser.forward();
			}
			if( !parser )
			{
				parser.pos( initial );
				return false;
			}

			// Get flags - if any
			parser.forward();
			if( !parser )
				matched.extend( parser.last().source().end() );
			else if( parser.current().type() == name_letters )
			{
				matched.extend( parser.current().source().end() );
				parser.forward();
			}
			output.push_back( std::move( matched ) );
			return true;
		}
	}
}
