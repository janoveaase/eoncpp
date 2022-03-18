#include "NumericalRules.h"


namespace eon
{
	namespace parser
	{
		bool ShortRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
		{
			// Match digits followed by 'S'
			if( parser.current().is( name_digits )
				&& parser.exists() && parser.ahead().is( name_name ) && parser.ahead().str() == "S" )
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
		bool LongRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
		{
			// Match digits followed by 'L'
			if( parser.current().is( name_digits )
				&& parser.exists() && parser.ahead().is( name_name ) && parser.ahead().str() == "L" )
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

		bool LowRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
		{
			// Match digits followed by '.' followed by digits followed by 'L'
			if( parser.current().is( name_digits )
				&& parser.exists() && parser.ahead().is( name_point )
				&& parser.exists( 2 ) && parser.ahead( 2 ).is( name_digits )
				&& parser.exists( 3 ) && parser.ahead( 3 ).is( name_name ) && parser.ahead( 3 ).str() == "L" )
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
		bool HighRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
		{
			// Match digits followed by '.' followed by digits followed by 'H'
			if( parser.current().is( name_digits )
				&& parser.exists() && parser.ahead().is( name_point )
				&& parser.exists( 2 ) && parser.ahead( 2 ).is( name_digits )
				&& parser.exists( 3 ) && parser.ahead( 3 ).is( name_name ) && parser.ahead( 3 ).str() == "H" )
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
	}
}
