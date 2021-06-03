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
		* Matches any word character, [0-9A-Za-z_] if in bytes mode
		*/
		class WordChar : public Node
		{
		public:
			WordChar( const substring& source ) : Node( source ) {
				Type = NodeType::val_wordchar; }
			WordChar( const WordChar& other ) : Node( other ) {
				Type = NodeType::val_wordchar; }
			WordChar( WordChar&& other ) noexcept
				: Node( std::move( other ) ) {
				Type = NodeType::val_wordchar; }
			virtual ~WordChar() = default;

			inline Node* copy() const override {
				return new WordChar( *this ); }

			inline WordChar& operator=( const WordChar& other ) {
				*static_cast<Node*>( this ) = other; return *this; }
			inline WordChar& operator=( WordChar&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other );
				return *this; }

		private:
			inline bool _match( RxData& data, size_t steps ) override {
				return string::isWordChar( data() )
					? data.advance() : false; }
		};


		class NotWordChar : public Node
		{
		public:
			NotWordChar( const substring& source ) : Node( source ) {
				Type = NodeType::val_wordchar; }
			NotWordChar( const NotWordChar& other ) : Node( other ) {
				Type = NodeType::val_wordchar; }
			NotWordChar( NotWordChar&& other ) noexcept
				: Node( std::move( other ) ) { Type = NodeType::val_wordchar; }
			virtual ~NotWordChar() = default;

			inline Node* copy() const override {
				return new NotWordChar( *this ); }

			inline NotWordChar& operator=( const NotWordChar& other ) {
				*static_cast<Node*>( this ) = other; return *this; }
			inline NotWordChar& operator=( NotWordChar&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other );
				return *this; }

		private:
			inline bool _match( RxData& data, size_t steps ) override {
				return data && !string::isWordChar( data() )
					? data.advance() : false; }
		};

	}
}
