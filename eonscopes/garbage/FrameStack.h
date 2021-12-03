#pragma once

#include "Frame.h"



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
		  A scope where objects exists (in one form or another)

		  Scopes form a hierarchy with 'global' on top, and all other scopes
		  can reach 'global'.
		  To reach a scope that is on a different path from 'global', one has
		  to go through 'global' to reach it.
		**********************************************************************/
		class FrameStack
		{
		public:

			// Construct stack
			inline FrameStack() = default;
			inline virtual ~FrameStack() { while( !Frames.empty() ) pop(); }




			/******************************************************************
			  Stack access
			******************************************************************/
		public:

			//* Get a variable, start search at the top of the stack and
			//* progress down until found
			//* Returns nullptr if not found!
			inline type::Object* get( name_t name ) noexcept { return !Frames.empty() ? top().get( name ) : nullptr; }




			/******************************************************************
			  Direct frame access
			******************************************************************/
		public:

			//* Check if empty
			inline bool empty() const noexcept { return Frames.empty(); }

			//* Push new unnamed frame
			//* Returns the new frame
			inline Frame& push( index_t size ) { Frames.push_back( Frame( size, empty() ? nullptr : &top() ) ); return top(); }

			//* Push new named frame
			//* Returns the new frame
			inline Frame& push( name_t name, index_t size ) {
				Frames.push_back( Frame( name, size, empty() ? nullptr : &top() ) ); return top(); }

			// Pop the top frame
			inline void pop() { Frames.erase( Frames.end() - 1 ); }

			// Access the top frame
			inline Frame& top() { return Frames.at( Frames.size() - 1 ); }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

			std::vector<Frame> Frames;
		};
	}
}
