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
		* Node group node
		* Matches a group of other nodes
		*/
		class NodeGroup : public Node
		{
		protected:
			NodeGroup() = default;
		public:
			NodeGroup( const substring& source ) : Node( source ) {
				Type = NodeType::node_group; };
			NodeGroup( const NodeGroup& other ) { *this = other; }
			NodeGroup( NodeGroup&& other ) noexcept {
				*this = std::move( other ); }
			inline NodeGroup( Node* head, const substring& source ) noexcept
				: Node( source ) { Type = NodeType::node_group; Head = head; }
			virtual ~NodeGroup() { if( Head != nullptr ) delete Head; }

			inline Node* copy() const override {
				return new NodeGroup( *this ); }

			inline NodeGroup& operator=( const NodeGroup& other ) {
				*static_cast<Node*>( this ) = other;
				Head = other.Head != nullptr ? other.Head->copy() : nullptr;
				_Cur = (Node*)1; return *this; }
			inline NodeGroup& operator=( NodeGroup&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other );
				Head = other.Head; other.Head = nullptr; return *this; }

		protected:
			bool _match( RxData& data, size_t steps ) override;

			void _append( Node* node ) noexcept;
			inline bool _locked() const noexcept { return _Cur == nullptr; }
			inline void _lock() noexcept { _Cur = nullptr; }

		private:
			Node* Head{ nullptr };
			Node* _Cur{ (Node*)1 };

			friend class Graph;
		};
	}
}