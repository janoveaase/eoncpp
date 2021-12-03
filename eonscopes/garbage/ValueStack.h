#pragma once

#include <eontypes/TypeSystem.h>



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::scope' namespace encloses all hidden scopes functionality
	**************************************************************************/
	namespace scope
	{
		/**********************************************************************
		  The Value Stack is used by expressions and actions to hold operands/
		  arguments and sub-expressions.
		  Return values are also pushed onto the stack!
		**********************************************************************/
		class ValueStack
		{
		public:
			ValueStack() = default;
			virtual ~ValueStack() { while( !empty() ) { top()->selfDelete(); pop(); } }

			//* Check if empty
			inline bool empty() const noexcept { return Values.empty(); }

			//* Push new value
			inline void push( type::Object* value ) { Values.push_back( value ); }

			//* Access the top value
			inline type::Object* top() { return Values.at( Values.size() - 1 ); }

			//* Pop the top value
			inline void pop() { Values.erase( Values.end() - 1 ); }

			//* Pop the top operand
			//* If it is an operator, then it and all it's arguments will be
			//* popped!
			void popOperand();


		private:
			std::vector<type::Object*> Values;
		};
	}
}
