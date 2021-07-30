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
		* Matches only if at start of string or start of line if line mode
		*/
		class LocStart : public Node
		{
		public:
			LocStart( const substring& source ) : Node( source ) {
				Type = NodeType::loc_start; };
			LocStart( const LocStart& other ) : Node( other ) {
				Type = NodeType::loc_start; }
			LocStart( LocStart&& other ) noexcept
				: Node( std::move( other ) ) { Type = NodeType::loc_start;
			}
			virtual ~LocStart() = default;

			inline Node* copy() const override {
				return new LocStart( *this ); }

			inline LocStart& operator=( const LocStart& other ) {
				*static_cast<Node*>( this ) = other; return *this;}
			inline LocStart& operator=( LocStart&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other ); return *this;
			}

		private:
			inline bool _match( RxData& param, size_t steps ) override {
				return param && ( param.pos().numChar() == 0
					|| ( param.lines() && param.prev( '\n' ) ) ); }
		};
	}
}