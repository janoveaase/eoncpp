#include "../Node.h"


namespace eon
{
	namespace rx
	{
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
			// If there are fewer characters remaining than the minimum required by the
			// pattern, we can report failure right now!
			if( data.remaining() < MinCharsRemaining )
				return false;

			// Cases:
			// 1 single match
			// 2 zero or one
			// 3 range greedy
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

			if( Name && !eon::validName( substring( data.pos(), data_tmp.pos() ) ) )
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
					matches.push( matches.top() );
					matches.top().advance();
					if( MinCharsRemaining > 0 && matches.top().remaining() < MinCharsRemaining - 1 )
						return;
				}
				if( Next == nullptr )
					matches.pop();
			}
			else
			{
				while( matches.size() - 1 < Quant.max() )
				{
					if( !_match( matches.top(), steps ) )
					{
						if( Next == nullptr )
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
			while( matches.size() > Quant.min() )
			{
				size_t next_steps = data.speedOnly() ? 1 : data.accuracyOnly() ? SIZE_MAX : 6;
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

					if( matches.empty() )
					{
						// Check if we can get a way with zero matches
						if( Quant.min() == 0 && Next == nullptr )
							return true;
						else
							break;
					}
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
				if( matches >= Quant.min() && matches <= Quant.max() )
				{
					data = std::move( data_tmp );
					return true;
				}
				return false;
			}

			// Now make sure the rest matches, or try matching this once more
			while( matches <= Quant.max() )
			{
				size_t next_steps = data.speedOnly() ? 1 : data.accuracyOnly() ? SIZE_MAX : 6;
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


		string Node::strStruct() const
		{
			auto s = _strStruct() + Quant.str();
			if( Next )
				return s + Next->strStruct();
			else
				return s;
		}


		bool Node::equal( const Node& other, cmpflag flags ) const noexcept
		{
			// Different types are definitely not equal
			if( Type != other.Type )
				return false;

			// Cmpare quantifiers if so instructed
			if( flags & cmpflag::quant && Quant != other.Quant )
				return false;

			// Compare contents
			cmpflag cflags = cmpflag::quant;
			if( flags & cmpflag::deep )
				cflags |= cmpflag::deep;
			if( !_equal( other, cflags ) )
				return false;

			// Go deep?
			if( flags & cmpflag::deep )
			{
				if( Next != other.Next && ( !Next || !other.Next ) )
					return false;
				if( Next )
				{
					if( !Next->equal( *other.Next, cmpflag::deep | cmpflag::quant ) )
						return false;
				}
			}

			// Equal!
			return true;
		}


		void Node::removeDuplicates( std::set<Node*>& removed )
		{
			if( removed.find( this ) != removed.end() )
				return;
			_removeDuplicates( removed );
			while( Next != nullptr )
			{
				Next->removeDuplicates( removed );

				// Find out if the two are identical, quantification excluded
				if( equal( *Next, cmpflag::none ) )
				{
					// They are
					// Make a new qantification that includes both
					if( Quant.Min > 0 && Next->Quant.Min > 0 )
						Quant.Min += Next->Quant.Min;
					else
						Quant.Min = Quant.Min > Next->Quant.Min ? Quant.Min : Next->Quant.Min;
					if( Quant.Max < SIZE_MAX && Next->Quant.Max < SIZE_MAX )
						Quant.Max = SIZE_MAX - Quant.Max < Next->Quant.Max ? SIZE_MAX : Quant.Max + Next->Quant.Max;
					else
						Quant.Max = SIZE_MAX;
					if( !Quant.Greedy && Next->Quant.Greedy )
						Quant.Greedy = true;
					auto next = Next;
					Next = Next->Next;
					next->Next = nullptr;
					delete next;
				}
				else
					break;
			}
		}
	}
}
