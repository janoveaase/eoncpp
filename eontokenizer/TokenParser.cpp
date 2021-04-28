#include "TokenParser.h"


namespace eon
{
	size_t tokenparser::lineStart() const
	{
		for( size_t i = Cur; i > 0; --i )
		{
			if( at( i - 1 ).newline() )
				return i;
		}
		return 0;
	}

	bool tokenparser::match(
		const std::initializer_list<eon::string>& elements ) const noexcept
	{
		if( !*this )
			return false;
		auto pos = Cur;
		for( auto& element : elements )
		{
			if( pos == Tokens.size() )
				return false;
			if( element == "\n" && Tokens[ pos ].newline() )
				continue;
			if( !element.empty() )
			{
				if( element == "*" )
					;
				else if( element.startsWith( '*' ) )
				{
					if( !Tokens[ pos ].endsWith(
						element.substr( element.begin() + 1 ) ) )
						return false;
				}
				else if( element.endsWith( '*' ) )
				{
					if( !Tokens[ pos ].startsWith( element.substr(
						element.begin(),
						element.begin() + ( element.numChars() - 1 ) ) ) )
						return false;
				}
				else if( !Tokens[ pos ].match( element.substr() ) )
					return false;
			}
			++pos;
		}
		return true;
	}
}
