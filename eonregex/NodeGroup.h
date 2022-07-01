#pragma once

#include "Node.h"


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// The 'eon::rx' namespace enclosed special elements for Eon regular
	// expressions
	//
	namespace rx
	{
		// Node group node
		// Matches a group of other nodes
		class NodeGroup : public Node
		{
		protected:
			NodeGroup() = default;
		public:
			inline NodeGroup( const substring& source ) : Node( source ) { Type = NodeType::node_group; };
			inline NodeGroup( const NodeGroup& other ) { *this = other; }
			inline NodeGroup( NodeGroup&& other ) noexcept { *this = std::move( other ); }
			inline NodeGroup( Node* head, const substring& source ) noexcept : Node( source ) {
				Type = NodeType::node_group; Head = head; }
			virtual ~NodeGroup() { if( Head != nullptr ) delete Head; }

			inline Node* copy() const override { return new NodeGroup( *this ); }

			inline NodeGroup& operator=( const NodeGroup& other ) { *static_cast<Node*>( this ) = other;
				Head = other.Head != nullptr ? other.Head->copy() : nullptr; _Cur = (Node*)1; return *this; }
			inline NodeGroup& operator=( NodeGroup&& other ) noexcept { *static_cast<Node*>( this ) = std::move( other );
				Head = other.Head; other.Head = nullptr; return *this; }

		protected:
			bool _match( RxData& data, index_t steps ) override;

			inline string _strStruct() const override { return Head ? "(" + Head->strStruct() + ")" : "()"; }

			inline bool _equal( const Node& other, cmpflag flags ) const noexcept override {
				auto& o = *dynamic_cast<const NodeGroup*>( &other ); if( Head && o.Head )
					return Head->equal( *o.Head, cmpflag::deep | cmpflag::quant ); else return Head == o.Head; }

			inline void _removeDuplicates() override { if( Head ) Head->removeDuplicates(); }
			inline void _combinedFixed() override { if( Head ) Head->combineFixed(); }
			inline index_t _countMinCharsRemaining() noexcept override {
				return MinCharsRemaining = ( Head ? Head->_countMinCharsRemaining() * Quant.minQ() : 0 )
					+ ( Next ? Next->_countMinCharsRemaining() : 0 ); }
			virtual Node* _removeSuperfluousGroups() noexcept override;
			void _failFastFixedEnd( Node& head ) override;
			void _unmatch() noexcept override { if( Head->_matched() ) Head->_unmatch(); Node::_unmatch(); }

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
