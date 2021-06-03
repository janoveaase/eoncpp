#include "NodeGroup.h"


namespace eon
{
	namespace rx
	{
		bool NodeGroup::_match( RxData& data, size_t steps )
		{
			if( Head != nullptr )
			{
				RxData param_b{ data };
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
