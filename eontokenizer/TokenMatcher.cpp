#include "TokenMatcher.h"


namespace eon
{
	bool TokenMatcher::match( TokenParser& parser ) const noexcept
	{
		auto pos = parser.pos();
		auto result = Pattern.match( parser );
		parser.pos( pos );
		return result;
	}




	string::iterator TokenMatcher::Element::parseName( string::iterator i, name_t& name )
	{
		auto pos = i;
		string str{ *i };
		for( ++i; i; ++i )
		{
			if( Chars->isLetter( *i ) || Chars->isNumberAsciiDigit( *i ) || *i == '_' )
				str += *i;
			else
				break;
		}
		name = eon::name( str );
		if( name == no_name )
			throw InvalidPattern( "Not a valid name at position " + string( pos.numChar() ) + ": \"" + str + "\"!" );
		return i;
	}


	string::iterator TokenMatcher::DataElement::parse( string::iterator i )
	{
		while( i )
		{
			if( isSpaceChar( *i ) )
			{
				++i;
				continue;
			}
			else if( *i == ')' )
				return ++i;
			else if( *i == '\'' )
				i = _parseStr( i );
			else if( Chars->isLetter( *i ) || Chars->isNumberAsciiDigit( *i ) || *i == '_' )
				i = parseName( i, Type );
			else
				throw InvalidPattern( "Unexepected character at position " + string( i.numChar() ) + "!" );
		}
		throw InvalidPattern( "Missing ')' at the end!" );
	}
	string::iterator TokenMatcher::DataElement::_parseStr( string::iterator i )
	{
		bool escaped = false;
		if( ++i )
		{
			if( i && *i == '*' )
			{
				StrEnd = 1;
				++i;
			}
			for( ; i; ++i )
			{
				if( !escaped )
				{
					if( *i == '*' )
					{
						auto next = i + 1;
						if( next && *next == SglQuoteChr )
						{
							StrEnd = -1;
							i = next;
						}
					}
					if( *i == SglQuoteChr )
						return ++i;
					else if( *i == BackSlashChr )
						escaped = true;
					else
						Str += *i;
				}
				else
				{
					Str += *i;
					escaped = false;
				}
			}
		}
		throw InvalidPattern( "String value is missing terminating single quote mark at position "
			+ string( i.numChar() ) + "!" );
	}
	bool TokenMatcher::DataElement::match( TokenParser& parser ) const noexcept
	{
		bool result = true;

		if( Type != no_name && !parser.current().is( Type ) )
			result = false;
		if( !Str.empty() )
		{
			switch( StrEnd )
			{
				case -1:
					if( !parser.current().str().startsWith( Str ) )
						result = false;
					break;
				case 1:
					if( !parser.current().str().endsWith( Str ) )
						result = false;
					break;
				default:
					if( parser.current().str() != Str )
						result = false;
					break;
			}
		}
		if( ( result || Negate ) && Advance )
			parser.forward();
		if( Negate )
			result = !result;
		return result ? result : Optional;
	}


	string::iterator TokenMatcher::ContainerElement::parse( string::iterator i )
	{
		auto name_opt = eon::name( "opt" );
		auto name_seq = eon::name( "seq" );
		bool optional = false;
		bool advance = true;
		bool negate = false;
		name_t elm_type{ no_name };
		string::iterator pos = i;
		while( i )
		{
			if( isSpaceChar( *i ) )
			{
				++i;
				continue;
			}
			else if( *i == '?' )
			{
				optional = true;
				++i;
			}
			else if( *i == '&' )
			{
				advance = false;
				++i;
			}
			else if( *i == '!' )
			{
				negate = true;
				++i;
			}
			else if( *i == ')' )
				return ++i;
			else if( isNameChar( *i ) )
			{
				pos = i;
				i = parseName( i, elm_type );
			}
			else if( *i == RoundBrcOpenChr )
			{
				if( elm_type == name_opt )
					Elements.push_back( new OptionsElement() );
				else if( elm_type == name_seq )
					Elements.push_back( new SequenceElement() );
				else if( elm_type == no_name )
					Elements.push_back( new DataElement() );
				else
					throw InvalidPattern( "Expected \"opt\" or \"seq\" at position " + string( pos.numChar() ) + ", not \""
						+ eon::str( elm_type ) + "\"!" );
				Elements.back()->Optional = optional;
				optional = false;
				Elements.back()->Advance = advance;
				advance = true;
				Elements.back()->Negate = negate;
				negate = false;
				elm_type = no_name;
				i = Elements.back()->parse( ++i );
			}
			else
				throw InvalidPattern( "Unexpected character at position " + string( i.numChar() ) + "!" );
		}
		if( !Root )
			throw InvalidPattern( "Missing ')' at the end!" );
		return i;
	}


	bool TokenMatcher::OptionsElement::match( TokenParser& parser ) const noexcept
	{
		auto pos = parser.pos();
		bool result = false;
		for( auto elm : Elements )
		{
			result = elm->match( parser );
			if( result )
				break;
		}
		if( Negate )
			result = !result;
		if( result )
		{
			if( !Advance )
				parser.pos( pos );
			return true;
		}
		return Optional;
	}


	bool TokenMatcher::SequenceElement::match( TokenParser& parser ) const noexcept
	{
		auto pos = parser.pos();
		bool result = true;
		for( auto elm : Elements )
		{
			result = elm->match( parser );
			if( !result )
				break;
		}
		if( Negate )
			result = !result;
		if( result )
		{
			if( !Advance )
				parser.pos( pos );
			return true;
		}
		return Optional;
	}
}
