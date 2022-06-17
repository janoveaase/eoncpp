#include "../Node.h"
#include "../NodeGroup.h"
#include "FixedValue.h"


namespace eon
{
	namespace rx
	{
		Node& Node::operator=( const Node& other )
		{
			Type = other.Type;
			Next = other.Next != nullptr ? other.Next->copy() : nullptr;
			FixedEnd = other.FixedEnd;
			Quant = other.Quant;
			Source = other.Source;
			Name = other.Name;
			Open = other.Open;
			PreAnchoring = other.PreAnchoring;
			return *this;
		}
		Node& Node::operator=( Node&& other ) noexcept
		{
			Next = other.Next; other.Next = nullptr;
			FixedEnd = other.FixedEnd; other.FixedEnd = nullptr;
			Quant = other.Quant; other.Quant = Quantifier();
			Name = other.Name; other.Name = false;
			Open = other.Open; other.Open = true;
			Source = other.Source; other.Source.clear();
			Type = other.Type;
			PreAnchoring = other.PreAnchoring; other.PreAnchoring = Anchor::none;
			return *this;
		}




		bool Node::match( RxData& data, size_t steps )
		{
			// If there are fewer characters remaining than the minimum required by the
			// pattern, we can report failure right now!
			if( data.remaining() < MinCharsRemaining )
				return false;

			// Check anchors
			if( PreAnchoring != Anchor::none && !_preAnchorMatch( data ) )
				return false;

			// If we have a fixed end, then we can fail fast by checking if it's there or not
			if( FixedEnd )
			{
				FixedValue* endvalue = (FixedValue*)FixedEnd;
				if( endvalue->value().substr() != substring(
					data.source().end() - endvalue->value().numChars(), data.source().end() ) )
					return false;
			}

			// Cases:
			// 1 single match
			// 2 zero or one
			// 3 range greedy
			// 4 range non-greedy

			bool success{ false };
			if( Quant.minQ() == 1 && Quant.maxQ() == 1 )
				success = matchSingle( data, steps );
			else if( Quant.minQ() == 0 && Quant.maxQ() == 1 )
				success = matchOneOrZero( data, steps );
			else if( Quant.greedy() )
				success = matchRangeGreedy( data, steps );
			else
				success = matchRangeNongreedy( data, steps );

			return success;
		}


		void Node::_failFastFixedEnd( Node& head )
		{
			if( Next )
			{
				if( Type == NodeType::val_fixed && Next->type() == NodeType::loc_end )
					head.FixedEnd = this;
				Next->_failFastFixedEnd( head );
			}
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
			if( PrevPos.same( data.pos(), data.marker() ) )
				return false;
			PrevPos.mark( data.pos(), data.marker() );

			// Match as many as possible from the start
			auto matches = _stack();
			matchMax( data, matches, steps );
			if( matches.size() < Quant.Min )
				return false;

			if( Name && ( matches.empty() || !eon::validName( substring( data.pos(), matches.top().pos() ) ) ) )
				return false;

			// No next means we have nothing more to do
			if( Next == nullptr )
				return noNext( data, matches );

			// Now make sure the rest matches, or move down the stack
			// (backgrack) until they do
			return nextMatches( data, matches );
		}
		void Node::matchMax( RxData data, Stack& matches, size_t steps )
		{
			// Some special cases can be processed faster
			if( _matchSpecialCase( data, matches ) )
				return;

			while( matches.size() < Quant.maxQ() )
			{
				if( !_match( matches.empty() ? data : matches.top(), steps ) )
					break;
				matches.push( matches.empty() ? data : matches.top() );
			}
		}
		bool Node::_matchSpecialCase( RxData& data, Stack& matches )
		{
			switch( Type )
			{
				case NodeType::val_any:
					_matchAny( data, matches );
					return true;
				default:
					return false;
			}
		}
		void Node::_matchAny( RxData& data, Stack& matches )
		{
			// Goble as much as we can
			int gobbled{ 0 };
			while( matches.size() < Quant.maxQ() && (matches.empty() ? data() : matches.top()() ) )
			{
				matches.push( matches.empty() ? data : matches.top() );
				matches.top().advance();
				if( gobbled < MinCharsRemaining )
					++gobbled;
				if( MinCharsRemaining > 0
					&& ( matches.top().remaining() == 0 || matches.top().remaining() < MinCharsRemaining - gobbled ) )
					return;
			}
		}
		bool Node::noNext( RxData& data, Stack& matches )
		{
			if( matches.size() >= Quant.minQ() )
			{
				if( !matches.empty() )
					data = std::move( matches.top() );
				return true;
			}
			return false;
		}
		bool Node::nextMatches( RxData& data, Stack& matches )
		{
			size_t next_steps = data.speedOnly() ? 1 : data.accuracyOnly() ? SIZE_MAX : 6;
			while( matches.size() >= Quant.minQ() )
			{
				if( Next->match( matches.empty() ? data : matches.top(), next_steps ) )
				{
					// Got a match?
					if( !matches.empty() )
						data = std::move( matches.top() );
					return true;
				}
				else
				{
					// Didn't get match, pop the stack
					matches.pop();

					if( matches.empty() )
					{
						// Check if we can get away with zero matches
						if( Quant.minQ() == 0 && Next == nullptr && ( !Name || eon::validName( substring(
							matches.size() < 2 ? data.pos() : matches.at( 1 ).pos(), matches.top().pos() ) ) ) )
							return true;
						else
							break;
					}
				}
			}
			if( Next && Next->match( data, next_steps ) )
				return true;
			return false;
		}
		bool Node::matchRangeNongreedy( RxData& data, size_t steps )
		{
			// Match as few as possible from the start
			RxData data_tmp{ data };
			size_t matches = 0;
			while( matches < Quant.minQ() )
			{
				if( !_match( data_tmp, steps ) )
					break;
				++matches;
			}
			if( Name && !eon::validName( substring( data.pos(), data_tmp.pos() ) ) )
				return false;

			// No next means we have nothing more to do
			if( Next == nullptr )
			{
				if( matches >= Quant.minQ() && matches <= Quant.maxQ() )
				{
					data = std::move( data_tmp );
					return true;
				}
				return false;
			}

			// Now make sure the rest matches, or try matching this once more
			while( matches <= Quant.maxQ() )
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

		bool Node::_preAnchorMatch( RxData& data )
		{
			if( !data() )			// End of input cannot be start of anything
				return false;
			if( PreAnchoring & Anchor::word )
			{
				if( !string::isWordChar( data() )
					|| ( data.pos().numChar() > 0 && !string::isSpaceChar( data.prev() )
						&& !string::isPunctuation( data.prev() ) ) )
					return false;
			}
			if( PreAnchoring & Anchor::line )
			{
				if( data.pos().numChar() > 0 && data.prev() != NewlineChr )
					return false;
			}
			if( PreAnchoring & Anchor::input )
			{
				if( data.pos().numChar() > 0 )
					return false;
			}
			return true;
		}


		string Node::strStruct() const
		{
			string s;
			if( PreAnchoring & Anchor::input )
				s += "^";
			else if( PreAnchoring & Anchor::word )
				s += "\\b";
			s += _strStruct() + Quant.str();
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
