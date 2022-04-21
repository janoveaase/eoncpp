#pragma once

#include "../Node.h"


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
		* Matches any digit character, [0-9]
		*/
		class Digit : public Node
		{
		public:
			inline Digit( const substring& source ) : Node( source ) { Type = NodeType::val_digit; }
			inline Digit( const Digit& other ) : Node( other ) { Type = NodeType::val_digit; }
			inline Digit( Digit&& other ) noexcept : Node( std::move( other ) ) { Type = NodeType::val_digit; }
			virtual ~Digit() = default;

			inline Node* copy() const override { return new Digit( *this ); }

			inline Digit& operator=( const Digit& other ) { *static_cast<Node*>( this ) = other; return *this; }
			inline Digit& operator=( Digit&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other ); return *this; }

		private:
			inline bool _match( RxData& data, size_t steps ) override {
				return string::isNumberAsciiDigit( data() ) ? data.advance() : false; }
			inline string _strStruct() const override { return "\\d"; }
			inline size_t _countMinCharsRemaining() noexcept override {
				return MinCharsRemaining = Quant.min() + ( Next ? Next->_countMinCharsRemaining() : 0 ); }
		};


		class NotDigit : public Node
		{
		public:
			inline NotDigit( const substring& source ) : Node( source ) { Type = NodeType::val_digit; }
			inline NotDigit( const NotDigit& other ) : Node( other ) { Type = NodeType::val_digit;}
			inline NotDigit( NotDigit&& other ) noexcept : Node( std::move( other ) ) { Type = NodeType::val_digit; }
			virtual ~NotDigit() = default;

			inline Node* copy() const override { return new NotDigit( *this ); }

			inline NotDigit& operator=( const NotDigit& other ) { *static_cast<Node*>( this ) = other; return *this; }
			inline NotDigit& operator=( NotDigit&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other ); return *this; }

		private:
			inline bool _match( RxData& data, size_t steps ) override {
				return data && !string::isNumberDecimalDigit( data() ) ? data.advance() : false; }
			inline string _strStruct() const override { return "\\D"; }
			inline size_t _countMinCharsRemaining() noexcept override {
				return MinCharsRemaining = Quant.min() + ( Next ? Next->_countMinCharsRemaining() : 0 );
			}
		};
	}
}
