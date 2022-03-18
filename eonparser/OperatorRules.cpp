#include "OperatorRules.h"


namespace eon
{
	namespace parser
	{
		bool PercentOfRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
		{
			// Match symbol '%' followed by name 'as'
			if( parser.current().is( name_symbol ) && parser.current().str() == "%"
				&& parser.exists() && parser.ahead().is( name_name ) && parser.ahead().str() == "of" )
			{
				auto matched = Token( parser.current().source(), Name );
				parser.forward();
				matched.extend( parser.current().source().end() );
				parser.forward();
				output.push_back( std::move( matched ) );
				return true;
			}
			else
				return false;
		}
		bool AsPercentOfRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
		{
			// Match name 'as' followed by symbol '%' followed by name 'as'
			if( parser.current().is( name_name ) && parser.current().str() == "as"
				&& parser.exists() && parser.ahead().is( name_symbol ) && parser.ahead().str() == "%"
				&& parser.exists( 2 ) && parser.ahead( 2 ).is( name_name ) && parser.ahead( 2 ).str() == "of" )
			{
				auto matched = Token( parser.current().source(), Name );
				parser.forward();
				matched.extend( parser.current().source().end() );
				parser.forward();
				matched.extend( parser.current().source().end() );
				parser.forward();
				output.push_back( std::move( matched ) );
				return true;
			}
			else
				return false;
		}
	}
}
