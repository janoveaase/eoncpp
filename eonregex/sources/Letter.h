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
		// Value node
		// Matches all letters
		class Letter : public Node
		{
		public:
			inline Letter( const substring& source ) : Node( source ) { Type = NodeType::val_lower; }
			inline Letter( const Letter& other ) : Node( other ) { Type = NodeType::val_lower; }
			inline Letter( Letter&& other ) noexcept : Node( std::move( other ) ) {
				Type = NodeType::val_lower; }
			virtual ~Letter() = default;

			inline Node* copy() const override { return new Letter( *this ); }

			inline Letter& operator=( const Letter& other ) { *static_cast<Node*>( this ) = other; return *this; }
			inline Letter& operator=( Letter&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other ); return *this; }

		private:
			inline bool _match( RxData& data, index_t step ) override {
				return string::isLetter( data() ) ? data.advance() : false; }
			inline string _strStruct() const override { return "\\u"; }
			inline index_t _countMinCharsRemaining() noexcept override {
				return MinCharsRemaining = Quant.minQ() + ( Next ? Next->_countMinCharsRemaining() : 0 ); }
		};

		// Value node
		// Matches any non-letter
		class NotLetter : public Node
		{
		public:
			inline NotLetter( const substring& source ) : Node( source ) { Type = NodeType::val_upper; };
			inline NotLetter( const NotLetter& other ) : Node( other ) { Type = NodeType::val_upper; }
			inline NotLetter( NotLetter&& other ) noexcept : Node( std::move( other ) ) { Type = NodeType::val_upper; }
			virtual ~NotLetter() = default;

			inline Node* copy() const override { return new NotLetter( *this ); }

			inline NotLetter& operator=( const NotLetter& other ) { *static_cast<Node*>( this ) = other; return *this; }
			inline NotLetter& operator=( NotLetter&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other ); return *this; }

		private:
			inline bool _match( RxData& data, index_t step ) override {
				return !string::isLetter( data() ) ? data.advance() : false; }
			inline string _strStruct() const override { return "\\U"; }
			inline index_t _countMinCharsRemaining() noexcept override {
				return MinCharsRemaining = Quant.minQ() + ( Next ? Next->_countMinCharsRemaining() : 0 ); }
		};
	}
}
