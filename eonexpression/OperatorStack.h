#pragma once
#include <eonvariables/Operators.h>
#include <vector>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::expr' namespace encloses all hidden expression functionality
	**************************************************************************/
	namespace expr
	{
		/**********************************************************************
		  Eon Operator Stack - eon::expr::opstack

		  Operators are temporarily stored on a stack while parsing.
		  Since std::stack does not allow for direct access to lower elements,
		  this class will take its place.
		**********************************************************************/
		class opstack
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline opstack() { Stack.reserve( 13 ); }
			virtual ~opstack() = default;




			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline operator bool() const noexcept { return !Stack.empty(); }
			inline bool empty() const noexcept { return Stack.empty(); }
			inline size_t size() const noexcept { return Stack.size(); }

			// Returns vars::operators::code::undef if empty!
			inline vars::operators::code top() const noexcept {
				return Stack.empty() ? vars::operators::code::undef : Stack[ Stack.size() - 1 ]; }

			// Returns vars::operators::code::undef if invalid position
			inline vars::operators::code at( size_t pos ) const noexcept {
				return pos < Stack.size() ? Stack[ pos ] : vars::operators::code::undef; }




			/******************************************************************
			  Modifier Methods
			******************************************************************/
		public:

			inline void push( vars::operators::code op_code ) { Stack.push_back( op_code ); }
			inline void pop() noexcept {
				if( !Stack.empty() ) try { Stack.resize( Stack.size() - 1 ); } catch( ... ) {} }

			// Replace element at given position
			inline void replace( size_t pos, vars::operators::code new_op_code ) {
				if( pos < Stack.size() ) Stack[ pos ] = new_op_code; }




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		protected:
			std::vector<vars::operators::code> Stack;
		};
	}
}
