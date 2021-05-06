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
		* Matches any lower case letter
		*/
		class LowerCase : public Node
		{
		public:
			LowerCase( const substring& source ) : Node( source ) {
				Type = NodeType::val_lower; }
			LowerCase( const LowerCase& other ) : Node( other ) {
				Type = NodeType::val_lower; }
			LowerCase( LowerCase&& other ) noexcept
				: Node( std::move( other ) ) {
				Type = NodeType::val_lower; }
			virtual ~LowerCase() = default;

			inline Node* copy() const override {
				return new LowerCase( *this ); }

			inline LowerCase& operator=( const LowerCase& other ) {
				*static_cast<Node*>( this ) = other; return *this; }
			inline LowerCase& operator=( LowerCase&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other );
				return *this; }

		private:
			inline bool _match( RxData& data, size_t step ) override {
				return string::isLetterLowerCase( data() )
					? data.advance() : false; }
		};

		/*
		* Value node
		* Matches any upper case letter
		*/
		class UpperCase : public Node
		{
		public:
			UpperCase( const substring& source ) : Node( source ) {
				Type = NodeType::val_upper; };
			UpperCase( const UpperCase& other ) : Node( other ) {
				Type = NodeType::val_upper; }
			UpperCase( UpperCase&& other ) noexcept
				: Node( std::move( other ) ) { Type = NodeType::val_upper; }
			virtual ~UpperCase() = default;

			inline Node* copy() const override {
				return new UpperCase( *this ); }

			inline UpperCase& operator=( const UpperCase& other ) {
				*static_cast<Node*>( this ) = other; return *this; }
			inline UpperCase& operator=( UpperCase&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other );
				return *this; }

		private:
			inline bool _match( RxData& data, size_t step ) override {
				return string::isLetterUpperCase( data() )
					? data.advance() : false; }
		};
	}
}
