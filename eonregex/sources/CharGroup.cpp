#include "CharGroup.h"


namespace eon
{
	namespace rx
	{
		bool CharGroup::_match( RxData& data, size_t steps )
		{
			if( data )
			{
				if( _match( data() ) )
					return Value.Negate ? false : data.advance();
				if( data.flags() & Flag::icase )
				{
					auto l = std::tolower( static_cast<int>( data() ) );
					auto u = std::toupper( static_cast<int>( data() ) );
					if( l != u )
					{
						if( data() == l )
						{
							if( _match( u ) )
								return Value.Negate ? false
								: data.advance();
						}
						else
						{
							if( _match( l ) )
								return Value.Negate ? false
								: data.advance();
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
