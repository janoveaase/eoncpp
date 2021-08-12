#pragma once

#include "Node.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::rx' namespace enclosed special elements for Eon regular
	  expressions
	**************************************************************************/
	namespace rx
	{
		/*
		* Location node
		* Matches only if at end of string or end of line if line mode
		*/
		class LocEnd : public Node
		{
		public:
			inline LocEnd( const substring& source ) : Node( source ) { Type = NodeType::loc_end; }
			inline LocEnd( const LocEnd& other ) : Node( other ) { Type = NodeType::loc_end; }
			inline LocEnd( LocEnd&& other ) noexcept : Node( std::move( other ) ) { Type = NodeType::loc_end; }
			virtual ~LocEnd() = default;

			inline Node* copy() const override { return new LocEnd( *this ); }

			inline LocEnd& operator=( const LocEnd& other ) { *static_cast<Node*>( this ) = other; return *this; }
			inline LocEnd& operator=( LocEnd&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other ); return *this; }

		private:
			inline bool _match( RxData& param, size_t steps ) override {
				return !param ? true : ( param.lines() && param() == '\n' ) ? param.advance() : false; }
			inline string _strStruct() const override { return "$"; }
		};
	}
}
