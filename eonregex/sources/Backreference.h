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
		// Backreference value group
		// Matches a previous capture
		class Backreference : public Node
		{
		public:
			inline Backreference( const substring& source ) : Node( source ) { Type = NodeType::val_backref; }
			inline Backreference( const Backreference& other ) { *this = other; }
			inline Backreference( Backreference&& other ) noexcept { *this = std::move( other ); }
			inline Backreference( name_t name, const substring& source ) noexcept : Node( source ) {
				Name = name; Type = NodeType::val_backref; }
			virtual ~Backreference() = default;

			inline Node* copy() const override { return new Backreference( *this ); }

			inline Backreference& operator=( const Backreference& other ) {
				*static_cast<Node*>( this ) = other; Name = other.Name; return *this; }
			inline Backreference& operator=( Backreference&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other ); Name = std::move( other.Name ); return *this; }

		private:
			bool _match( RxData& data, size_t steps ) override;
			inline string _strStruct() const override { return "@:<" + str( Name ) + ">"; }
			inline bool _equal( const Node& other, cmpflag flags ) const noexcept override {
				return Name == dynamic_cast<const Backreference*>( &other )->Name; }
			inline size_t _countMinCharsRemaining() noexcept override { return MinCharsRemaining = 0; }

		private:
			name_t Name{ no_name };
		};
	}
}
