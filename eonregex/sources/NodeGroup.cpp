#include "../NodeGroup.h"


namespace eon
{
	namespace rx
	{
		bool NodeGroup::_match( RxData& data, index_t steps )
		{
			if( Head != nullptr )
			{
				RxData param_b{ data };
				Head->_setGroup( this );
				if( Head->match( param_b, steps ) )
				{
					data = std::move( param_b );
					return true;
				}
				else
					return false;
			}
			else		// Empty group (should be optimized away)
				return true;
		}

		Node* NodeGroup::_removeSuperfluousGroups() noexcept
		{
			// Recurse first
			if( Head )
				Head = Head->_removeSuperfluousGroups();
			if( Next )
				Next = Next->_removeSuperfluousGroups();

			// If we only have one element, then grouping is not necessary!
			if( Head && !Head->Next )
			{
				auto node = Head; Head = nullptr;
				node->Next = Next; Next = nullptr;
				node->Quant.Min *= Quant.Min;
				node->Quant.Max = Quant.Max == INDEX_MAX || node->Quant.Max == INDEX_MAX ? INDEX_MAX
					: Quant.Max * node->Quant.Max;
				node->Name = Name;
				node->Open = Open;
				node->PreAnchoring |= PreAnchoring;
				delete this;
				return node;
			}
			return this;
		}
		void NodeGroup::_failFastFixedEnd( Node& head )
		{
			if( Head )
				Head->_failFastFixedEnd( *Head );
			if( Next )
				Next->_failFastFixedEnd( head );
		}

		void NodeGroup::_append( Node* node ) noexcept
		{
			if( !_locked() )
			{
				if( Head != nullptr )
				{
					Head = node;
					_Cur = Head;
				}
				else
				{
					_Cur->Next = node;
					_Cur = _Cur->Next;
				}
			}
		}
	}
}
