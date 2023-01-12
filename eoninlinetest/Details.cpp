#include "Details.h"


namespace eonitest
{
	TestLog& TestLog::operator<<( eon::string text )
	{
		auto len = text.numChars();
		switch( NextStyle )
		{
			case style::error:
				*this << LogItem( std::move( text ), eon::style::error );
				break;
			case style::warning:
				*this << LogItem( std::move( text ), eon::style::warning );
				break;
			case style::key:
				*this << LogItem( std::move( text ), eon::style::strong );
				_finalizeKey( len );
				break;
			case style::item:
				*this << LogItem( std::move( text ), eon::style::strong );
				break;
			case style::item_1:
				*this << LogItem( std::move( text ), eon::style::key_1 );
				break;
			case style::item_2:
				*this << LogItem( std::move( text ), eon::style::key_2 );
				break;
			case style::item_3:
				*this << LogItem( std::move( text ), eon::style::key_3 );
				break;
			case style::key_1:
				*this << LogItem( std::move( text ), eon::style::key_1 );
				_finalizeKey( len );
				break;
			case style::key_2:
				*this << LogItem( std::move( text ), eon::style::key_2 );
				_finalizeKey( len );
				break;
			case style::key_3:
				*this << LogItem( std::move( text ), eon::style::key_3 );
				_finalizeKey( len );
				break;
			case style::value:
				_pushValue( text, eon::style::code );
				break;
			case style::value_1:
				_pushValue( text, eon::style::value_1 );
				break;
			case style::value_2:
				_pushValue( text, eon::style::value_2 );
				break;
			case style::value_3:
				_pushValue( text, eon::style::value_3 );
				break;
			default:
				*this << LogItem( std::move( text ) );
				break;
		}
		NextStyle = style::normal;

		if( keyStyle( NextStyle ) || valueStyle( NextStyle ) )
			NextStyle = style::normal;
		if( keyStyle( NextStyle ) )
		{
			if( text.numChars() < 14 )
				*this << eon::string().padRight( 14 - text.numChars() );
			*this << ": ";
		}
		return *this;
	}

	void TestLog::_pushValue( eon::string text, eon::style value_style )
	{
		if( text.isDoubleQuoted() )
		{
			*this << LogItem( "\"", eon::style::weak );
			if( text.numChars() > 2 )
				*this << LogItem( text.slice( 1, -2 ), value_style );
			*this << LogItem( "\"", eon::style::weak );
		}
		else if( text.startsWith( "@type=" ) )
		{
			*this << LogItem( "type=" + text.substr( text.begin() + 6 ) + ": ", eon::style::weak );
			*this << LogItem( "<cannot translate into text>", value_style );
		}
		else if( text.startsWith( "@char=" ) )
		{
			auto details = text.substr( text.begin() + 6 ).splitSequential<std::vector<eon::string>>(
				eon::substring( "::" ) );
			if( !details[ 0 ].empty() )
			{
				*this << LogItem( "'", eon::style::weak );
				*this << LogItem( details[ 0 ], value_style );
				*this << LogItem( "' #", eon::style::weak );
			}
			else
				*this << LogItem( "#", eon::style::weak );
			*this << LogItem( details[ 1 ], value_style );
			*this << LogItem( " 0x", eon::style::weak );
			*this << LogItem( details[ 2 ], value_style );
		}
		else
			*this << LogItem( std::move( text ), value_style );
	}
}
