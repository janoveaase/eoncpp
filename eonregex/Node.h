#pragma once

#include "RxDefs.h"
#include "RxData.h"
#include "Quantifier.h"
#include <eonstack/Stack.h>
//#include <stack>


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
		// Super-class for all operators, locations and values
		class Node
		{
		protected:
			Node() = default;
		public:
			inline Node( const substring& source ) { Source = source; }
			inline Node( const Node& other ) { *this = other; }
			inline Node( Node&& other ) noexcept { *this = std::move( other ); }
			virtual ~Node() { if( Next != nullptr ) delete Next; }

			Node& operator=( const Node& other );
			Node& operator=( Node&& other ) noexcept;

			virtual Node* copy() const = 0;

			inline const substring& source() const noexcept { return Source; }
			inline NodeType type() const noexcept { return Type; }
			inline bool open() const noexcept { return Open; }

			bool match( RxData& data, size_t steps = nsize );


			// Get node structure as a string
			string strStruct() const;


			inline void resetQuantifier() noexcept { Quant = Quantifier(); }


			// Node comparison flags
			enum class cmpflag
			{
				none = 0x00,	// No flags
				deep = 0x01,	// Go deep (compare chained nodes)
				quant = 0x02	// Compare quantifiers
			};
			friend inline bool operator&( cmpflag a, cmpflag b ) noexcept {
				return static_cast<int>( a ) & static_cast<int>( b ); }
			friend inline cmpflag operator|( cmpflag a, cmpflag b ) noexcept {
				return static_cast<cmpflag>( static_cast<int>( a ) | static_cast<int>( b ) ); }
			friend inline cmpflag& operator|=( cmpflag& a, cmpflag b ) noexcept {
				return a = static_cast<cmpflag>( static_cast<int>( a ) | static_cast<int>( b ) ); }

			// Comparison
			bool equal( const Node& other, cmpflag flags = cmpflag::none ) const noexcept;


			// Optimizations
			void removeDuplicates( std::set<Node*>& removed );

		protected:
			virtual bool _match( RxData& data, size_t steps ) = 0;

			virtual string _strStruct() const { return string(); }

			virtual bool _equal( const Node& other, cmpflag flags ) const noexcept { return true; }
			virtual void _removeDuplicates( std::set<Node*>& removed ) {}

			using Stack = stack<RxData>;
			inline Stack _stack() { Stack data; data.reserve( 53 ); return data; }

		public:
			virtual size_t _countMinCharsRemaining() noexcept = 0;
			virtual Node* _removeSuperfluousGroups() noexcept {
				if( Next ) Next = Next->_removeSuperfluousGroups(); return this; }
			virtual Node* _exposeLiterals() { if( Next ) Next = Next->_exposeLiterals(); return this; }
			virtual void _failFastFixedEnd( Node& head );


		private:
			bool matchSingle( RxData& data, size_t steps );
			bool matchOneOrZero( RxData& data, size_t steps );
			bool matchRangeGreedy( RxData& data, size_t steps );
			void matchMax( RxData data, Stack& matches, size_t steps );
			bool _matchSpecialCase( RxData& data, Stack& matches );
			void _matchAny( RxData& data, Stack& matches );
			bool noNext( RxData& data, Stack& matches );
			bool nextMatches( RxData& data, Stack& matches );
			bool matchRangeNongreedy( RxData& data, size_t steps );

			bool matchNext( RxData& data, size_t steps );

			bool _preAnchorMatch( RxData& data );

		protected:
			Node* Next{ nullptr };
			Node* FixedEnd{ nullptr };
			size_t MinCharsRemaining{ 0 };
			Quantifier Quant;
			bool Name{ false };
			bool Open{ true };
			substring Source;
			NodeType Type{ NodeType::undef };
			Anchor PreAnchoring{ Anchor::none };
			RecordedPos PrevPos;

			friend class Graph;
			friend class NodeGroup;
			friend class FixedValue;
		};
	}
}
