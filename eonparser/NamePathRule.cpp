#include "NamePathRule.h"


namespace eon
{
	namespace parser
	{
		bool NamePathRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
		{
			// Start with '@'
			if( parser.current().is( name_at ) && parser.ahead().is( name_name ) )
			{
				auto initial = parser.pos();
				parser.forward();
				auto matched = Token( parser.current().source(), Name );
				parser.forward();

				// Accept sequence of alternating slashes and names
				while( true )
				{
					if( !parser || parser.current().str() != "/" )
					{
						matched.extend( parser ? parser.prior().source().end() : parser.last().source().end() );
						output.push_back( std::move( matched ) );
						return true;
					}
					else if( parser.current().is( name_name ) )
						parser.forward();
					else
						break;
				}
				parser.pos( initial );
			}
			return false;
		}
	}
}
