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
		* Operator node
		* Match one of the alternates
		*/
		class OpOr : public Node
		{
		public:
			OpOr( const substring& source ) : Node( source ) {
				Type = NodeType::op_or; }
			OpOr( const OpOr& other ) : Node( other ) { *this = other; }
			OpOr( OpOr&& other ) noexcept : Node( std::move( other ) ) {
				*this = std::move( other ); }
			virtual ~OpOr() { clear(); }

			inline Node* copy() const override { return new OpOr( *this ); }

			inline void clear() noexcept {
				for( auto opt : Optionals ) delete opt; }

			OpOr& operator=( const OpOr& other );
			inline OpOr& operator=( OpOr&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other );
				Optionals = std::move( other.Optionals ); return *this; }

		private:
			bool _match( RxData& param, size_t steps ) override;

		private:
			std::vector<Node*> Optionals;

			friend class Graph;
		};
	}
}
