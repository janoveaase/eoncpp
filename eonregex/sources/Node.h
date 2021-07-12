#pragma once

#include "RxDefs.h"
#include "RxData.h"
#include "Quantifier.h"
#include <stack>


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
		* Super-class for all operators, locations and values
		*/
		class Node
		{
		protected:
			Node() = default;
		public:
			Node( const substring& source ) { Source = source; }
			Node( const Node& other ) { *this = other; }
			Node( Node&& other ) noexcept { *this = std::move( other ); }
			virtual ~Node();

			Node& operator=( const Node& other );
			Node& operator=( Node&& other ) noexcept;

			virtual Node* copy() const = 0;

			inline const substring& source() const noexcept { return Source; }
			inline NodeType type() const noexcept { return Type; }
			inline bool open() const noexcept { return Open; }

			bool match( RxData& data, size_t steps = nsize );

		protected:
			virtual bool _match( RxData& data, size_t steps ) = 0;

			using stack = std::stack<RxData, std::vector<RxData>>;
			inline stack _stack() {
				std::vector<RxData> data; data.reserve( 75 );
				return stack( std::move( data ) ); }


		private:
			bool matchSingle( RxData& data, size_t steps );
			bool matchOneOrZero( RxData& data, size_t steps );
			bool matchRangeGreedy( RxData& data, size_t steps );
			void matchMax( stack& matches, size_t steps );
			bool noNext( RxData& data, stack& matches );
			bool nextMatches( RxData& data, stack& matches );
			bool matchRangeNongreedy( RxData& data, size_t steps );

			bool matchNext( RxData& data, size_t steps );

		private:
			Node* Next{ nullptr };
			Quantifier Quant;
			bool Name{ false };
			bool Open{ true };
		protected:
			substring Source;
			NodeType Type{ NodeType::undef };

			friend class Graph;
			friend class NodeGroup;
		};
	}
}
