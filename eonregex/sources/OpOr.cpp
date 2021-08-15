#include "OpOr.h"


namespace eon
{
	namespace rx
	{
		OpOr& OpOr::operator=( const OpOr& other )
		{
			*static_cast<Node*>( this ) = other;
			clear();
			for( auto opt : other.Optionals )
				Optionals.push_back( opt->copy() );
			return *this;
		}




		bool OpOr::_match( RxData& data, size_t steps )
		{
			RxData data_b{ data };
			for( auto& opt : Optionals )
			{
				if( opt->match( data_b, steps ) )
				{
					data = std::move( data_b );
					return true;
				}
			}
			return false;
		}

		size_t OpOr::_countMinCharsRemaining() noexcept
		{
			MinCharsRemaining = 0;
			for( auto node : Optionals )
			{
				auto min = node->_countMinCharsRemaining() * Quant.min();
				if( min < MinCharsRemaining )
				{
					MinCharsRemaining = min;
					if( min == 0 )
						break;
				}
			}
			MinCharsRemaining += Next ? Next->_countMinCharsRemaining() : 0;
			return MinCharsRemaining;
		}
	}
}
