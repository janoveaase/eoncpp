#include "../OpOr.h"


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




		bool OpOr::_match( RxData& data, index_t steps )
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

		index_t OpOr::_countMinCharsRemaining() noexcept
		{
			MinCharsRemaining = 0;
			for( auto node : Optionals )
			{
				auto min = node->_countMinCharsRemaining() * Quant.minQ();
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
		Node* OpOr::_removeSuperfluousGroups() noexcept
		{
			if( Next )
				Next = Next->_removeSuperfluousGroups();
			for( auto opt = Optionals.begin(); opt != Optionals.end(); ++opt )
				*opt = (*opt)->_removeSuperfluousGroups();
			
			return this;
		}
		void OpOr::_failFastFixedEnd( Node& head )
		{
			for( auto& opt : Optionals )
				opt->_failFastFixedEnd( *opt );
			if( Next )
				Next->_failFastFixedEnd( head );
		}
	}
}
