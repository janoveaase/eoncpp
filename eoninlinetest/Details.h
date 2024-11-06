#pragma once

#include <eonstring/String.h>
#include <eonterminal/Terminal.h>
#include "TestName.h"



///////////////////////////////////////////////////////////////////////////////
//
// The 'eonitest' namespace encloses all public inline test functionality
//
namespace eonitest
{
	enum class style
	{
		normal,
		error,
		warning,
		item,
		item_1,
		item_2,
		item_3,
		key,
		key_1,
		key_2,
		key_3,
		value,
		value_1,
		value_2,
		value_3
	};
	inline bool keyStyle( style s ) noexcept {
		return s == style::key || s == style::key_1 || s == style::key_2 || s == style::key_3; }
	inline bool valueStyle( style s ) noexcept {
		return s == style::value || s == style::value_1 || s == style::value_2 || s == style::value_3; }

	struct LogItem
	{
		LogItem() = default;
		inline LogItem( eon::string item, eon::style style = eon::style::normal ) noexcept
			: Item( std::move( item ) ), Style( style ) {}
		inline LogItem( const eon::substring& item, eon::style style = eon::style::normal ) noexcept
			: Item( item ), Style( style ) {}
		inline LogItem( const char* item, eon::style style = eon::style::normal ) noexcept : Item( item ), Style( style ) {}

		eon::string Item;
		eon::style Style{ eon::style::normal };
	};

	struct LogLine
	{
		inline LogLine& operator<<( LogItem item ) { Items.push_back( std::move( item ) ); return *this; }

		std::vector<LogItem> Items;
	};

	struct TestLog
	{
		inline TestLog& operator<<( const LogItem& item )
		{
			if( Lines.empty() )
				Lines.emplace_back();
			Lines[ Lines.size() - 1 ] << item;
			return *this;
		}
		inline TestLog& operator<<( style style ) { NextStyle = style; return *this; }
		TestLog& operator<<( eon::string text );
		inline TestLog& operator<<( std::string text ) { *this << eon::string( std::move( text ) ); return *this; }
		inline TestLog& operator<<( const char* text ) { *this << eon::string( text ); return *this; }
		inline TestLog& operator<<( char chr ) {
			NextStyle = style::normal; return chr == '\n' ? endl() : *this << eon::string( eon::char_t( chr ) ); }
		inline TestLog& endl() { Lines.emplace_back(); return *this; }
		inline void _finalizeKey( eon::index_t used_width )
		{
			if( used_width < 14 )
				*this << LogItem( eon::string().padRight( 14 - used_width ) );
			*this << LogItem( ": " );
		}

		void _pushValue( eon::string text, eon::style value_style );

		std::vector<LogLine> Lines;
		style NextStyle{ style::normal };
	};


	// Details of a test
	class TestDetails
	{
	public:
		TestName Name;
		bool Success{ false };
		TestLog Log;
	};
}
