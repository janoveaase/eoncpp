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
		// Location end - Matches end of word
		class LocWordEnd : public Node
		{
		public:
			inline LocWordEnd( const substring& source ) : Node( source ) { Type = NodeType::loc_wordend; }
			inline LocWordEnd( const LocWordEnd& other ) : Node( other ) { Type = NodeType::loc_wordend; }
			inline LocWordEnd( LocWordEnd&& other ) noexcept : Node( std::move( other ) ) { Type = NodeType::loc_wordend; }
			virtual ~LocWordEnd() = default;

			inline Node* copy() const override { return new LocWordEnd( *this ); }

			inline LocWordEnd& operator=( const LocWordEnd& other ) { *static_cast<Node*>( this ) = other; return *this; }
			inline LocWordEnd& operator=( LocWordEnd&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other ); return *this; }

		private:
			bool _match( RxData& data, size_t steps ) override;
			inline string _strStruct() const override { return "\\B"; }
			
			inline size_t _countMinCharsRemaining() noexcept override {
				return MinCharsRemaining = Next ? Next->_countMinCharsRemaining() : 0; }
		};
	}
}
