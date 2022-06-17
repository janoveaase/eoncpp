#pragma once

#include "../Node.h"


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
		// Operator node
		// Matches only if next node doesn't match
		class OpNot : public Node
		{
		public:
			inline OpNot( const substring& source ) : Node( source ) { Type = NodeType::op_not; }
			inline OpNot( const OpNot& other ) : Node( other ) { *this = other; }
			inline OpNot( OpNot&& other ) noexcept : Node( std::move( other ) ) { *this = std::move( other ); }
			inline OpNot( Node* value, const substring& source ) : Node( source ) {
				Type = NodeType::op_not; Value = value; }
			virtual ~OpNot() { if( Value != nullptr ) delete Value; }

			inline Node* copy() const override { return new OpNot( *this ); }

			inline OpNot& operator=( const OpNot& other ) { *static_cast<Node*>( this ) = other;
				Value = other.Value != nullptr ? other.Value->copy() : nullptr; return *this; }
			inline OpNot& operator=( OpNot&& other ) noexcept { *static_cast<Node*>( this ) = std::move( other );
			Value = other.Value; other.Value = nullptr; return *this; }

			inline bool _equal( const Node& other, cmpflag flags ) const noexcept override {
				auto& o = *dynamic_cast<const OpNot*>( &other ); if( Value && o.Value )
					return Value->equal( *o.Value, cmpflag::deep | cmpflag::quant ); else return Value == o.Value; }

		private:
			inline bool _match( RxData& data, size_t steps ) override { return !Value->match( data, steps ); }
			inline string _strStruct() const override { return Value ? "!" + Value->strStruct() : "!"; }
			inline size_t _countMinCharsRemaining() noexcept override {
				return MinCharsRemaining = Next ? Next->_countMinCharsRemaining() : 0; }

		private:
			Node* Value{ nullptr };

			friend class Graph;
		};
	}
}
