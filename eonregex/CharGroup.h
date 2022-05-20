#pragma once

#include "Node.h"
#include <unordered_set>


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
		* Matches a character group
		*/
		class CharGroup : public Node
		{
		public:
			struct CharGrp
			{
				CharGrp() = default;
				inline CharGrp( const CharGrp& other ) { *this = other; }
				inline CharGrp( CharGrp&& other ) noexcept { *this = std::move( other ); }
				~CharGrp() = default;

				inline CharGrp& operator=( const CharGrp& other ) { Chars = other.Chars; Ranges = other.Ranges;
					Special = other.Special; Negate = other.Negate; return *this; }
				inline CharGrp& operator=( CharGrp&& other ) noexcept {
					Chars = std::move( other.Chars ); Negate = other.Negate; Special = std::move( other.Special );
					Ranges = std::move( other.Ranges ); return *this; }

				string str() const;

				inline bool operator==( const CharGrp& other ) const noexcept { return Chars == other.Chars
					&& Ranges == other.Ranges && Special == other.Special && Negate == other.Negate; }

				std::unordered_set<char_t> Chars;
				std::vector<std::pair<char_t, char_t>> Ranges;
				std::unordered_set<char_t> Special;
				bool Negate{ false };
			};

		public:
			inline CharGroup( const substring& source ) : Node( source ) { Type = NodeType::val_chargroup; }
			inline CharGroup( const CharGroup& other ) { *this = other; }
			inline CharGroup( CharGroup&& other ) noexcept { *this = std::move( other ); }
			inline CharGroup( CharGrp&& value, const substring& source ) : Node( source ) {
				Type = NodeType::val_chargroup; Value = std::move( value ); }
			virtual ~CharGroup() = default;

			inline Node* copy() const override { return new CharGroup( *this ); }

			inline CharGroup& operator=( const CharGroup& other ) {
				*static_cast<Node*>( this ) = other; Value = other.Value; return *this; }
			inline CharGroup& operator=( CharGroup&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other ); Value = std::move( other.Value ); return *this; }

		private:
			bool _match( RxData& data, size_t steps ) override;
			bool _match( char_t chr );

			inline string _strStruct() const override { return Value.str(); }

			inline bool _equal( const Node& other, cmpflag flags ) const noexcept override {
				return Value == dynamic_cast<const CharGroup*>( &other )->Value; }
			inline size_t _countMinCharsRemaining() noexcept override {
				return MinCharsRemaining = Quant.minQ() + ( Next ? Next->_countMinCharsRemaining() : 0 ); }

		private:
			CharGrp Value;
		};
	}
}
