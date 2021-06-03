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
		* Value node
		* Matches any character
		*/
		class Any : public Node
		{
		public:
			Any( const substring& source ) : Node( source ) {
				Type = NodeType::val_any; }
			Any( const Any& other ) : Node( other ) {
				Type = NodeType::val_any; }
			Any( Any&& other ) noexcept : Node( std::move( other ) ) {
				Type = NodeType::val_any; }
			virtual ~Any() = default;

			inline Node* copy() const override { return new Any( *this ); }

			inline Any& operator=( const Any& other ) {
				*static_cast<Node*>( this ) = other; return *this; }
			inline Any& operator=( Any&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other );
				return *this; }

		private:
			inline bool _match( RxData& data, size_t step ) override {
				return data ? data.advance() : false; }
		};
	}
}
