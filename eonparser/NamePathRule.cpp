#include "NamePathRule.h"


namespace eon
{
	namespace parser
	{
		bool NamePathRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
		{
			// Basic format: "@<name>[/<name>/.../<name>]"
			if( parser.current().is( name_at ) )
			{
				auto initial = parser.pos();
				parser.forward();
				if( parser.current().is( name_name ) )
				{
					Token matched( parser.current().source(), name_namepath );
					parser.forward();

					// Accept sequence of alternating slashes and names
					while( true )
					{
						if( !parser || !parser.current().is( name_operator ) || parser.current().str() != "/" )
						{
							output.push_back( std::move( matched ) );
							return true;
						}
						parser.forward();
						if( !parser.current().is( name_name ) )
							break;
						matched.extend( parser.current().source().end() );
						parser.forward();
					}
				}
				parser.pos( initial );
			}
			return false;
		}
	}
}
