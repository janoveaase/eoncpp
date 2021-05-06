#pragma once

#include "Node.h"


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
		* Backreference value group
		* Matches a previous capture
		*/
		class Backreference : public Node
		{
		public:
			Backreference( const substring& source ) : Node( source ) {
				Type = NodeType::val_backref; }
			Backreference( const Backreference& other ) { *this = other; }
			Backreference( Backreference&& other ) noexcept {
				*this = std::move( other ); }
			Backreference( name_t name, const substring& source ) noexcept
				: Node( source ) {
				Name = name; Type = NodeType::val_backref; }
			virtual ~Backreference() = default;

			inline Node* copy() const override {
				return new Backreference( *this ); }

			inline Backreference& operator=( const Backreference& other ) {
				*static_cast<Node*>( this ) = other; Name = other.Name;
				return *this; }
			inline Backreference& operator=( Backreference&& other )
				noexcept {
				*static_cast<Node*>( this ) = std::move( other );
				Name = std::move( other.Name ); return *this; }

		private:
			bool _match( RxData& data, size_t steps ) override;

		private:
			name_t Name;
		};
	}
}
