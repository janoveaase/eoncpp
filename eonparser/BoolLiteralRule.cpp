#include "BoolLiteralRule.h"


namespace eon
{
	namespace parser
	{
		bool BoolLiteralRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
		{
			// Just match names 'true' and 'false'
			static std::set<string> literals{ "true", "false" };
			if( parser.current().is( name_letters ) && literals.find( parser.current().str() ) != literals.end() )
			{
				auto matched = Token( parser.current().source(), Name );
				parser.forward();
				output.push_back( std::move( matched ) );
				return true;
			}
			else
				return false;
		}
	}
}
