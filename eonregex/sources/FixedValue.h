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
		// Fixed sequence
		class FixedValue : public Node
		{
		public:
			inline FixedValue( const substring& source ) : Node( source ) { Type = NodeType::val_fixed; }
			inline FixedValue( const FixedValue& other ) { *this = other; }
			inline FixedValue( FixedValue&& other ) noexcept { *this = std::move( other ); }
			inline FixedValue( string&& value, const substring& source ) : Node( source ) {
				Type = NodeType::val_fixed; Value = std::move( value ); }
			virtual ~FixedValue() = default;

			inline Node* copy() const override { return new FixedValue( *this ); }

			inline FixedValue& operator=( const FixedValue& other ) {
				*static_cast<Node*>( this ) = other; Value = other.Value; return *this; }
			inline FixedValue& operator=( FixedValue&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other ); Value = std::move( other.Value ); return *this; }

			inline const string& value() const noexcept { return Value; }

		private:
			bool _match( RxData& data, size_t steps ) override;

			string _strStruct() const override;

			inline bool _equal( const Node& other, cmpflag flags ) const noexcept override {
				return Value == dynamic_cast<const FixedValue*>( &other )->Value; }
			inline size_t _countMinCharsRemaining() noexcept override {
				return MinCharsRemaining = Value.numChars() * Quant.minQ() + ( Next ? Next->_countMinCharsRemaining() : 0 ); }
			Node* _exposeLiterals() override;

		private:
			string Value;
		};
	}
}
