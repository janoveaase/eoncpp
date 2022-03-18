#include "DefineRules.h"


namespace eon
{
	namespace parser
	{
		bool DefineVarRule::match( TokenParser& parser, std::vector<Token>& output ) const noexcept
		{
			// We must match the name "define" + variable name for there to be any matches here!
			if( !parser.current().is( name_name ) || parser.current().str() != "define"
				|| !parser.exists( 2 ) || !parser.ahead( 2 ).is( name_name ) || !parser.exists( 3 ) )
				return false;

			auto pos = parser.pos();
			parser.forward( 3 );
			if( parser.current().is( name_space ) )
				parser.forward();

			// If we next have a symbol '=', we have a match
			if( parser && parser.current().is( name_operator ) && parser.current().str() == "=" )
			{
				parser.pos( pos );
				auto matched = Token( parser.current().source(), Name );
				parser.forward();
				output.push_back( std::move( matched ) );
				return true;
			}

			// For there to be a match, the next element must be the name "as"
			if( !parser || !parser.current().is( name_name ) || parser.current().str() != "as" )
			{
				parser.pos( pos );
				return false;
			}
			parser.forward();
			if( parser && parser.current().is( name_space ) )
				parser.forward();

			// Now for the complicated stuff ...

			// The following will positively identify a variable definition
			//   (define <name> as) [thread] shared <name> ...
			//   (define <name> as) <name> of ...
			//   (define <name> as) <name != type, trait, enum, thread, console, gui, service> ...
			static TokenMatcher opt1( "?(name 'thread') (name 'shared') (name)" );
			static TokenMatcher opt2( "(name) (name 'of')" );
			static TokenMatcher opt3( "&(name) !opt((type) (trait) (enum) (thread) (console) (gui) (service))" );

			auto a = opt1.match( parser );
			auto b = opt2.match( parser );
			auto c = opt3.match( parser );
			if( opt1.match( parser ) || opt2.match( parser ) || opt3.match( parser ) )
			{
				parser.pos( pos );
				auto matched = Token( parser.current().source(), Name );
				parser.forward();
				output.push_back( std::move( matched ) );
				return true;
			}

			parser.pos( pos );
			return false;
		}
	}
}
