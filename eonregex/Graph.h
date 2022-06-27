#pragma once

#include "RxDefs.h"
#include "RxData.h"
#include "Node.h"
#include "CharGroup.h"
#include "OpOr.h"


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
		class Graph
		{
		public:
			Graph() = default;
			inline Graph( const Graph& other ) { *this = other; }
			inline Graph( Graph&& other ) noexcept { *this = std::move( other ); }
			virtual ~Graph() { clear(); };

			Graph& operator=( const Graph& other );
			Graph& operator=( Graph&& other ) noexcept;

			inline void clear() noexcept { if( Head != nullptr ) { delete Head; Head = nullptr; } }

			void parse( substring source, substring flags );

			inline Flag flags() const noexcept { return MyFlags; }

			inline bool empty() const noexcept { return Head == nullptr; }

			inline bool match( RxData& param ) const {
				if( Head ) { Head->_unmatch(); return Head->match( param ); } else return false; }

			inline const substring& source() const noexcept { return Source; }
			
			inline string strStruct() const { return Head ? Head->strStruct() : string(); }

		private:
			void parse();

			void parseFlags( substring flags );

			class ParseParam
			{
			public:
				ParseParam() = delete;
				inline ParseParam( const ParseParam& other ) {
					Copy = true; GroupCount = other.GroupCount; Source = other.Source; CurPos = other.CurPos; }
				ParseParam( ParseParam&& ) = delete;
				inline ParseParam( const substring& source ) { Source = source; CurPos = Source.begin();}
				~ParseParam() { if( !Copy && HeadNode != nullptr ) delete HeadNode; }

				inline operator bool() const noexcept { return static_cast<bool>( CurPos && CurPos != Source.end() ); }

				inline char_t operator()() noexcept { return *CurPos; }
				inline bool advance() noexcept { return ++CurPos; }
				inline string_iterator pos() const noexcept { return CurPos; }
				inline void pos( const string_iterator& pos ) noexcept { CurPos = pos; }

				inline void startGroup() noexcept { ++GroupCount; }
				inline bool inGroup() const noexcept { return GroupCount > 0; }
				inline void endGroup() noexcept { --GroupCount; }

				inline const substring& source() const noexcept { return Source; }

				inline index_t strSize() const noexcept { return Source.numChars(); }

				void set( Node* node ) noexcept;
				inline Node* head() const noexcept { return HeadNode; }
				inline Node* cur() const noexcept { return CurNode; }
				void replaceCur( Node* node ) noexcept;
				inline Node* extract() noexcept { auto node = HeadNode; HeadNode = nullptr; CurNode = nullptr; return node; }

				inline void resetPreAnchor() noexcept { PreAnchoring = Anchor::none; }
				inline void preAnchor( Anchor anchor ) noexcept { PreAnchoring |= anchor; }
				inline Anchor preAnchor() const noexcept { return PreAnchoring; }

			private:
				substring Source;
				string_iterator CurPos;
				int GroupCount{ 0 };
				Node* HeadNode{ nullptr };
				Node* PrevNode{ nullptr };
				Node* CurNode{ nullptr };
				Anchor PreAnchoring{ Anchor::none };
				bool Copy{ false };
			};

			// Returns true if end character was seen (and advanced past)
			// Returns false if no end character (set or seen)
			bool parse( ParseParam& param, int64_t end = -1 );

			Node* parseNode( ParseParam& param );

			Node* parseNot( ParseParam& param, const string_iterator& start );
			Node* parseOr( OpOr* node_or, ParseParam& param, const string_iterator& start );

			Node* parseCharGroup( ParseParam& param, const string_iterator& start );
			void parseCharGroupRange( ParseParam& param, CharGroup::CharGrp& value, char_t start );

			Node* parseCaptureRelated( ParseParam& param, const string_iterator& start );
			Node* parseCapture( ParseParam& param, const string_iterator& start );
			Node* parseBackref( ParseParam& param, const string_iterator& start );

			Node* parseEscaped( ParseParam& param, const string_iterator& start );

			bool parseQuantifier( ParseParam& param, index_t& min, index_t& max, const string_iterator& start );

			Node* parseNodeGroup( ParseParam& param, const string_iterator& start );

			Node* parseFixed( ParseParam& param, const string_iterator& start );

			Node* setQuantifier( ParseParam& param, index_t min, index_t max );

			void _removeDuplicates();
			void _countMinCharsRemaining();
			void _removeSuperfluousGroups() noexcept;
			void _exposeLiterals();
			void _failFastFixedEnd();




		private:
			substring Source;
			Node* Head{ nullptr };
			Flag MyFlags{ Flag::none };
		};
	}
}
