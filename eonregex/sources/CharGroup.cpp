#include "CharGroup.h"


namespace eon
{
	namespace rx
	{
		string CharGroup::CharGrp::str() const
		{
			string s{ "[" };
			if( Negate )
				s += "!";
			for( auto c : Chars )
				s += c;
			for( auto& r : Ranges )
				s += r.first + string( "-" ) + r.second;
			for( auto c : Special )
				s += string( "\\" ) + c;
			return s + "]";
		}


		bool CharGroup::_match( RxData& data, size_t steps )
		{
			if( data )
			{
				if( _match( data() ) )
					return Value.Negate ? false : data.advance();
				if( data.flags() & Flag::icase )
				{
					auto& chars = Characters::get();
#ifdef EON_APPLE
					auto l = std::tolower<wchar_t>( static_cast<wchar_t>( data() ), chars.locale() );
					auto u = std::toupper<wchar_t>( static_cast<wchar_t>( data() ), chars.locale() );
#else
					auto l = std::tolower<char_t>( data(), chars.locale() );
					auto u = std::toupper<char_t>( data(), chars.locale() );
#endif
					if( l != u )
					{
						if( data() == l )
						{
							if( _match( u ) )
								return Value.Negate ? false : data.advance();
						}
						else
						{
							if( _match( l ) )
								return Value.Negate ? false : data.advance();
						}
					}
				}
			}
			return Value.Negate;
		}
		bool CharGroup::_match( char_t chr )
		{
			auto found = Value.Chars.find( chr );
			if( found != Value.Chars.end() )
				return true;
			for( auto& range : Value.Ranges )
			{
				if( chr >= range.first && chr <= range.second )
					return true;
			}
			return false;
		}
	}
}
