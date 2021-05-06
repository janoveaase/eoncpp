#pragma once

#include "Node.h"


namespace eonrx
{
	/*
	* Value node
	* Matches the escape character
	*/
	class ValBackslash : public Node
	{
	public:
		ValBackslash( const SourceRef& source ) : Node( source ) {
			Type = NodeType::val_backslash; }
		ValBackslash( ValBackslash& other ) : Node( other ) {
			Type = NodeType::val_backslash; }
		ValBackslash( ValBackslash&& other ) noexcept
			: Node( std::move( other ) ) { Type = NodeType::val_backslash; }
		virtual ~ValBackslash() = default;

		inline ValBackslash& operator=( const ValBackslash& other ) {
			*static_cast<Node*>( this ) = other; return *this; }
		inline ValBackslash& operator=( ValBackslash&& other ) noexcept {
			*static_cast<Node*>( this ) = std::move( other ); return *this; }

	private:
		inline bool _match( Param& param, size_t steps ) override {
			return param && param() == '\\' ? param.advance() : false; }
	};

	/*
	* Value node
	* Matches the double quote character
	*/
	class ValDQuote : public Node
	{
	public:
		ValDQuote( const SourceRef& source ) : Node( source ) {
			Type = NodeType::val_dquote; }
		ValDQuote( ValDQuote& other ) : Node( other ) {
			Type = NodeType::val_dquote; }
		ValDQuote( ValDQuote&& other ) noexcept : Node( std::move( other ) ) {
			Type = NodeType::val_dquote; }
		virtual ~ValDQuote() = default;

		inline ValDQuote& operator=( const ValDQuote& other ) {
			*static_cast<Node*>( this ) = other; return *this; }
		inline ValDQuote& operator=( ValDQuote&& other ) noexcept {
			*static_cast<Node*>( this ) = std::move( other ); return *this; }

	private:
		inline bool _match( Param& param, size_t steps ) override {
			return param && param() == '\"' ? param.advance() : false; }
	};
};
