#include "Node.h"


namespace eon
{
	namespace rx
	{
		Node::~Node()
		{
			if( Next != nullptr )
				delete Next;
		}




		Node& Node::operator=( const Node& other )
		{
			Type = other.Type;
			Next = other.Next != nullptr ? other.Next->copy() : nullptr;
			Quant = other.Quant;
			Source = other.Source;
			Name = other.Name;
			Open = other.Open;
			return *this;
		}
		Node& Node::operator=( Node&& other ) noexcept
		{
			Next = other.Next; other.Next = nullptr;
			Quant = other.Quant; other.Quant = Quantifier();
			Name = other.Name; other.Name = false;
			Open = other.Open; other.Open = true;
			Source = other.Source; other.Source.clear();
			Type = other.Type;
			return *this;
		}





		bool Node::match( RxData& data, size_t steps )
		{
			// Cases:
			// 1 single match
			// 2 zero or one
			// 3 range gredy
			// 4 range non-greedy

			if( Quant.min() == 1 && Quant.max() == 1 )
				return matchSingle( data, steps );
			else if( Quant.min() == 0 && Quant.max() == 1 )
				return matchOneOrZero( data, steps );
			else if( Quant.greedy() )
				return matchRangeGreedy( data, steps );
			else
				return matchRangeNongreedy( data, steps );
		}

		bool Node::matchSingle( RxData& data, size_t steps )
		{
			RxData data_tmp{ data };
			if( !_match( data_tmp, steps ) )
				return false;

			if( Name && !name::valid( substring(
				data.pos(), data_tmp.pos() ) ) )
				return false;

			if( Next != nullptr )
			{
				if( Next->match( data_tmp, steps ) )
				{
					data = std::move( data_tmp );
					return true;
				}
				else
					return false;
			}
			data = std::move( data_tmp );
			return true;
		}
		bool Node::matchOneOrZero( RxData& data, size_t steps )
		{
			// Try to match one first
			if( matchSingle( data, steps ) )
				return true;

			// That's OK, now we try to match zero
			if( Next != nullptr )
			{
				RxData data_tmp{ data };
				if( Next->match( data_tmp, steps ) )
				{
					data = std::move( data_tmp );
					return true;
				}
				return false;
			}
			else
				return true;
		}
		bool Node::matchRangeGreedy( RxData& data, size_t steps )
		{
			// Match as many as possible from the start
			auto matches = _stack();
			matches.push( data );
			matchMax( matches, steps );

			// No next means we have nothing more to do
			if( Next == nullptr )
				return noNext( data, matches );

			// Make sure that zero match is included
			if( matches.empty() )
				matches.push( data );

			// Now make sure the rest matches, or move down the stack until
			// they do
			return nextMatches( data, matches );
		}
		void Node::matchMax( stack& matches, size_t steps )
		{
			// Special case: the '.' (any) character
			if( Type == NodeType::val_any )
			{
				// Goble as much as we can
				while( matches.size() - 1 < Quant.max() && matches.top() )
				{
					matches.top().advance();
					matches.push( matches.top() );
				}
				matches.pop();
			}
			else
			{
				while( matches.size() - 1 < Quant.max() )
				{
					if( !_match( matches.top(), steps ) )
					{
						matches.pop();
						break;
					}
					matches.push( matches.top() );
				}
			}
		}
		bool Node::noNext( RxData& data, stack& matches )
		{
			if( matches.size() >= Quant.min() )
			{
				if( !matches.empty() )
					data = std::move( matches.top() );
				return true;
			}
			return false;
		}
		bool Node::nextMatches( RxData& data, stack& matches )
		{
			while( matches.size() >= Quant.min() )
			{
				size_t next_steps = data.speedOnly() ? 1
					: data.accuracyOnly() ? SIZE_MAX : 6;
				if( Next->match( matches.top(), next_steps ) )
				{
					// Got a match
					data = std::move( matches.top() );
					return true;
				}
				else
				{
					// Didn't get match, pop the stack
					matches.pop();

					// Check if we can get a way with zero matches
					if( matches.empty() && Quant.min() == 0 )
						return true;
				}
			}
			return false;
		}
		bool Node::matchRangeNongreedy( RxData& data, size_t steps )
		{
			// Match as few as possible from the start
			RxData data_tmp{ data };
			size_t matches = 0;
			while( matches < Quant.min() )
			{
				if( !_match( data_tmp, steps ) )
					break;
				++matches;
			}

			// No next means we have nothing more to do
			if( Next == nullptr )
			{
				if( matches <= Quant.max() )
				{
					data = std::move( data_tmp );
					return true;
				}
				return false;
			}

			// Now make sure the rest matches, or try matching this once more
			while( matches <= Quant.max() )
			{
				size_t next_steps = data.speedOnly() ? 1
					: data.accuracyOnly() ? SIZE_MAX : 6;
				if( Next->match( data_tmp, next_steps ) )
				{
					// Got a match
					data = std::move( data_tmp );
					return true;
				}
				else
				{
					// Didn't get a match, try matching this again
					if( !_match( data_tmp, steps ) )
						return false;
					++matches;
				}
			}
			return false;
		}
		bool Node::matchNext( RxData& data, size_t steps )
		{
			if( Next == nullptr )
				return true;
			return Next->match( data, steps );
		}
	}
}
