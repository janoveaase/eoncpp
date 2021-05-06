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
		* Value node
		* Matches any space character
		*/
		class Space : public Node
		{
		public:
			Space( const substring& source ) : Node( source ) {
				Type = NodeType::val_space; }
			Space( const Space& other ) : Node( other ) {
				Type = NodeType::val_space; }
			Space( Space&& other ) noexcept
				: Node( std::move( other ) ) { Type = NodeType::val_space; }
			virtual ~Space() = default;

			inline Node* copy() const override { return new Space( *this ); }

			inline Space& operator=( const Space& other ) {
				*static_cast<Node*>( this ) = other; return *this; }
			inline Space& operator=( Space&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other );
				return *this; }

		private:
			inline bool _match( RxData& data, size_t steps ) override {
				return string::isSpaceChar( data() )
					? data.advance() : false; }
		};

		class NotSpace : public Node
		{
		public:
			NotSpace( const substring& source ) : Node( source ) {
				Type = NodeType::val_space; }
			NotSpace( const NotSpace& other ) : Node( other ) {
				Type = NodeType::val_space;
			}
			NotSpace( NotSpace&& other ) noexcept
				: Node( std::move( other ) ) {
				Type = NodeType::val_space;
			}
			virtual ~NotSpace() = default;

			inline Node* copy() const override { return new NotSpace( *this ); }

			inline NotSpace& operator=( const NotSpace& other ) {
				*static_cast<Node*>( this ) = other; return *this;
			}
			inline NotSpace& operator=( NotSpace&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other ); return *this;
			}

		private:
			inline bool _match( RxData& data, size_t steps ) override {
				return !string::isSpaceChar( data() ) ? data.advance() : false;
			}
		};

		/*
		* Value node
		* Matches any punctuation character
		*/
		class Punctuation : public Node
		{
		public:
			Punctuation( const substring& source ) : Node( source ) {
				Type = NodeType::val_punctuation; }
			Punctuation( const Punctuation& other ) : Node( other ) {
				Type = NodeType::val_punctuation; }
			Punctuation( Punctuation&& other ) noexcept
				: Node( std::move( other ) ) {
				Type = NodeType::val_punctuation; }
			virtual ~Punctuation() = default;

			inline Node* copy() const override {
				return new Punctuation( *this ); }

			inline Punctuation& operator=( const Punctuation& other ) {
				*static_cast<Node*>( this ) = other; return *this; }
			inline Punctuation& operator=( Punctuation&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other );
				return *this; }

		private:
			inline bool _match( RxData& data, size_t steps ) override {
				return string::isPunctuation( data() )
					? data.advance() : false; }
		};

		class NotPunctuation : public Node
		{
		public:
			NotPunctuation( const substring& source ) : Node( source ) {
				Type = NodeType::val_punctuation; }
			NotPunctuation( const NotPunctuation& other ) : Node( other ) {
				Type = NodeType::val_punctuation; }
			NotPunctuation( NotPunctuation&& other ) noexcept
				: Node( std::move( other ) ) {
				Type = NodeType::val_punctuation; }
			virtual ~NotPunctuation() = default;

			inline Node* copy() const override {
				return new NotPunctuation( *this ); }

			inline NotPunctuation& operator=( const NotPunctuation& other ) {
				*static_cast<Node*>( this ) = other; return *this; }
			inline NotPunctuation& operator=( NotPunctuation&& other )
				noexcept { *static_cast<Node*>( this )
				= std::move( other ); return *this; }

		private:
			inline bool _match( RxData& data, size_t steps ) override {
				return !string::isPunctuation( data() )
					? data.advance() : false; }
		};
	}
}
