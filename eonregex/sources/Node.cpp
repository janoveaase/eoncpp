#include "../Node.h"
#include "../NodeGroup.h"
#include "FixedValue.h"


namespace eon
{
	namespace rx
	{
		Node& Node::operator=( const Node& other )
		{
			Matched = other.Matched;
			Next = other.Next != nullptr ? other.Next->copy() : nullptr;
			Group = nullptr;
			FixedEnd = other.FixedEnd;
			MinCharsRemaining = other.MinCharsRemaining;
			Quant = other.Quant;
			Name = other.Name;
			Open = other.Open;
			Source = other.Source;
			Type = other.Type;
			PreAnchoring = other.PreAnchoring;
			PrevPos = other.PrevPos;
			return *this;
		}
		Node& Node::operator=( Node&& other ) noexcept
		{
			Matched = std::move( other.Matched );
			Next = other.Next; other.Next = nullptr;
			Group= nullptr;
			FixedEnd = other.FixedEnd; other.FixedEnd = nullptr;
			MinCharsRemaining = other.MinCharsRemaining; other.MinCharsRemaining = 0;
			Quant = other.Quant; other.Quant = Quantifier();
			Name = other.Name; other.Name = false;
			Open = other.Open; other.Open = true;
			Source = other.Source; other.Source.clear();
			Type = other.Type;
			PreAnchoring = other.PreAnchoring; other.PreAnchoring = Anchor::none;
			PrevPos = other.PrevPos;
			return *this;
		}




		bool Node::match( RxData& data, index_t steps )
		{
			// If this node has already been successfully matched, don't try again!
			if( _matched() )
			{
				Matched.addCaptures( data.captures() );
				data = Matched;
				return true;
			}

			// If there are fewer characters remaining than the minimum required by the
			// pattern, we can report failure right now!
			if( data.remaining() < MinCharsRemaining )
			{
				_unmatch();
				return false;
			}

			// Check anchors
			if( PreAnchoring != Anchor::none && !_preAnchorMatch( data ) )
			{
				_unmatch();
				return false;
			}

			// If we have a fixed end, then we can fail fast by checking if it's there or not
			if( FixedEnd )
			{
				FixedValue* endvalue = (FixedValue*)FixedEnd;
				if( endvalue->value().substr() != substring(
					data.source().end() - endvalue->value().numChars(), data.source().end() ) )
				{
					_unmatch();
					return false;
				}
			}

			// Cases:
			// 1 single match
			// 2 zero or one
			// 3 range greedy
			// 4 range non-greedy

			bool success{ false };
			if( Quant.minQ() == 1 && Quant.maxQ() == 1 )
				success = _matchSingle( data, steps );
			else if( Quant.minQ() == 0 && Quant.maxQ() == 1 )
				success = _matchOneOrZero( data, steps );
			else if( Quant.greedy() )
				success = _matchRangeGreedy( data, steps );
			else
				success = _matchRangeNongreedy( data, steps );

			if( success )
				Matched = data;
			else
				_unmatch();
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



		bool Node::_matchSingle( RxData& data, index_t steps )
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
		bool Node::_matchOneOrZero( RxData& data, index_t steps )
		{
			// Try to match one first
			if( _matchSingle( data, steps ) )
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
		bool Node::_matchRangeGreedy( RxData& data, index_t steps )
		{
			if( PrevPos.same( data.pos(), data.marker() ) )
				return false;
			PrevPos.mark( data.pos(), data.marker() );

			// Match as many as possible from the start
			auto matches = _stack();
			matches.push( data );
			_matchMax( matches, steps );
			if( matches.size() <= Quant.Min )
				return false;

			if( Name && ( matches.empty() || !eon::validName( substring( data.pos(), matches.top().pos() ) ) ) )
				return false;

			// No next means we have nothing more to do
			if( !_next() )
				return _noNext( data, matches );

			// Now make sure the rest matches, or move down the stack
			// (backgrack) until they do
			return _matchNext( data, matches );
		}
		void Node::_matchMax( Stack& matches, index_t steps )
		{
			// Some special cases can be processed faster
			if( _matchSpecialCase( matches ) )
				return;

			matches.push( matches.top() );
			while( true )
			{
				_unmatch();
				if( !_match( matches.top(), steps ) )
				{
					matches.pop();
					break;
				}
				if( matches.size() > Quant.maxQ() )
					break;
				if( matches.size() > 1 && matches.top().pos() == matches.at( 1 ).pos() )
					break;
				matches.push( matches.top() );
			}
		}
		bool Node::_matchSpecialCase( Stack& matches )
		{
			switch( Type )
			{
				case NodeType::val_any:
					_matchAny( matches );
					return true;
				default:
					return false;
			}
		}
		void Node::_matchAny( Stack& matches )
		{
			// Goble as much as we can
			int gobbled{ 0 };
			while( matches.size() <= Quant.maxQ() && matches.top()() )
			{
				matches.push( matches.top() );
				matches.top().advance();
				if( gobbled < MinCharsRemaining )
					++gobbled;
				if( MinCharsRemaining > 0
					&& ( matches.top().remaining() == 0 || matches.top().remaining() < MinCharsRemaining - gobbled ) )
					return;
			}
		}
		bool Node::_noNext( RxData& data, Stack& matches )
		{
			if( matches.size() >= Quant.minQ() )
			{
				if( !matches.empty() )
					data = std::move( matches.top() );
				return true;
			}
			return false;
		}
		bool Node::_matchNext( RxData& data, Stack& matches )
		{
			// 'matches' is a stack of RxData objects where the bottom element
			// is the start of the current potential greedy match and the top
			// is the end.
			index_t next_steps = data.speedOnly() ? 1 : data.accuracyOnly() ? INDEX_MAX : 6;
			bool capturing = Next == nullptr && Group->type() == NodeType::capt_group;
			while( matches.size() > Quant.minQ() )
			{
				if( capturing )
					Group->_capture( matches.top() );
				if( _next()->match( matches.top(), next_steps ) )
				{
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
						if( Quant.minQ() == 0 && _next() == nullptr && ( !Name || eon::validName( substring(
							matches.size() < 2 ? data.pos() : matches.at( 1 ).pos(), matches.top().pos() ) ) ) )
							return true;
						else
							break;
					}
				}
			}
			if( _next() && _next()->match( data, next_steps ) )
				return true;
			return false;
		}
		bool Node::_matchRangeNongreedy( RxData& data, index_t steps )
		{
			// Match as few as possible from the start
			RxData data_tmp{ data };
			index_t matches = 0;
			while( matches < Quant.minQ() )
			{
				if( !_match( data_tmp, steps ) )
					break;
				++matches;
			}
			if( Name && !eon::validName( substring( data.pos(), data_tmp.pos() ) ) )
				return false;

			// No next means we have nothing more to do
			if( !_next() )
			{
				if( matches >= Quant.minQ() && matches <= Quant.maxQ() )
				{
					data = std::move( data_tmp );
					return true;
				}
				return false;
			}

			// Now make sure the rest matches, or try matching this once more
			bool capturing = Next == nullptr && Group->type() == NodeType::capt_group;
			while( matches <= Quant.maxQ() )
			{
				if( capturing )
					Group->_capture( data_tmp );
				index_t next_steps = data.speedOnly() ? 1 : data.accuracyOnly() ? INDEX_MAX : 6;
				if( _next()->match( data_tmp, next_steps ) )
				{
					// Got a match
					data = std::move( data_tmp );
					return true;
				}
				else
				{
					// Didn't get a match, try matching this again
					_unmatch();
					if( !_match( data_tmp, steps ) )
						return false;
					++matches;
				}
			}
			return false;
		}
		bool Node::_matchNext( RxData& data, index_t steps )
		{
			if( Next == nullptr )
				return true;
			return Next->match( data, steps );
		}

		bool Node::_preAnchorMatch( RxData& data )
		{
			if( !data() )			// End of input cannot be start of anything
				return false;
			if( PreAnchoring & Anchor::spaces )
			{
				if( string::isSeparatorSpace( data() )
					|| ( data.pos().numChar() > 0 && data.prev() != NullChr && !string::isSeparatorSpace( data.prev() ) ) )
					return false;
			}
			if( PreAnchoring & Anchor::word )
			{
				if( !string::isWordChar( data() )
					|| ( data.pos().numChar() > 0 && data.prev() != NullChr && !string::isSeparatorSpace( data.prev() )
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

			// Different pre-anchoring are not equal
			if( PreAnchoring != other.PreAnchoring )
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


		void Node::removeDuplicates()
		{
			_removeDuplicates();
			while( Next != nullptr )
			{
				Next->removeDuplicates();

				// Find out if the two are identical, quantification excluded, pre-anchor included!
				if( equal( *Next, cmpflag::none ) )
				{
					// They are
					// Make a new qantification that includes both
					if( Quant.Min > 0 && Next->Quant.Min > 0 )
						Quant.Min += Next->Quant.Min;
					else
						Quant.Min = Quant.Min > Next->Quant.Min ? Quant.Min : Next->Quant.Min;
					if( Quant.Max < INDEX_MAX && Next->Quant.Max < INDEX_MAX )
						Quant.Max = INDEX_MAX - Quant.Max < Next->Quant.Max ? INDEX_MAX : Quant.Max + Next->Quant.Max;
					else
						Quant.Max = INDEX_MAX;
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
		void Node::combineFixed()
		{
			_combinedFixed();
			while( Type == NodeType::val_fixed && Quant.Min == 1 && Quant.Max == 1 && Next
				&& Next->Type == NodeType::val_fixed && Next->Quant.Min == 1 && Next->Quant.Max == 1 )
			{
				( (FixedValue*)this )->append( ( (FixedValue*)Next )->value() );
				auto next = Next;
				Next = Next->Next;
				next->Next = nullptr;
				delete next;
			}
			if( Next )
				Next->combineFixed();
		}
	}
}
