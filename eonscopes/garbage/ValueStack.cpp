#include "ValueStack.h"
#include <eontypes/Action.h>


namespace eon
{
	namespace scope
	{
		void ValueStack::popOperand()
		{
			auto operand = top(); pop();
			if( operand->generalType() == name_action )
			{
				// TODO: Account for missing arguments, with and without default values!
				for( index_t i = 0; i < ( (type::Action*)top() )->arguments().numAttributes(); ++i )
					popOperand();
			}
		}
	}
}
