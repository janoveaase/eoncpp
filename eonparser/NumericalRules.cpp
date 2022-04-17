#include "NumericalRules.h"


namespace eon
{
	namespace parser
	{
		bool ShortRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
		{
			static regex pattern{ R"(^\d+S$)" };
			if( parser && parser.current().is( name_name ) && pattern.match( parser.current().str() ) )
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
		bool LongRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
		{
			static regex pattern{ R"(^\d+L$)" };
			if( parser && parser.current().is( name_name ) && pattern.match( parser.current().str() ) )
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

		bool LowRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
		{
			static regex pattern{ R"(^\d+L$)" };

			// Match digits followed by '.' followed by name where all are digits except the last which is 'L'
			if( parser.current().is( name_digits )
				&& parser.exists() && parser.ahead().is( name_point )
				&& parser.exists( 2 ) && parser.ahead( 2 ).is( name_name ) && pattern.match( parser.ahead( 2 ).str() ) )
			{
				auto matched = Token( parser.current().source(), Name );
				parser.forward( 2 );
				matched.extend( parser.current().source().end() );
				matched.source().pullEnd();
				parser.forward();
				output.push_back( std::move( matched ) );
				return true;
			}
			else
				return false;
		}
		bool HighRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
		{
			static regex pattern{ R"(^\d+H$)" };

			// Match digits followed by '.' followed by name where all are digits except the last which is 'H'
			if( parser.current().is( name_digits )
				&& parser.exists() && parser.ahead().is( name_point )
				&& parser.exists( 2 ) && parser.ahead( 2 ).is( name_name ) && pattern.match( parser.ahead( 2 ).str() ) )
			{
				auto matched = Token( parser.current().source(), Name );
				parser.forward( 2 );
				matched.extend( parser.current().source().end() );
				matched.source().pullEnd();
				parser.forward();
				output.push_back( std::move( matched ) );
				return true;
			}
			else
				return false;
		}
	}
}
