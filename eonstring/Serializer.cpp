#include "Serializer.h"


namespace eon
{
	void serializer::_encode( bool value, std::string& bytes )
	{
		bytes += "B!";
		bytes += ( value ? "1" : "0" );
	}
	void serializer::_encode( byte_t value, std::string& bytes )
	{
		bytes += "b!";
		bytes += hex::toHex( value ).stdstr();
	}
	void serializer::_encode( char_t value, std::string& bytes )
	{
		bytes += "c!";
		bytes += std::to_string( static_cast<int>( value ) );
		bytes += record_sep;
	}
	void serializer::_encode( int_t value, std::string& bytes )
	{
		bytes += "i!";
		bytes += std::to_string( value );
		bytes += record_sep;
	}
	void serializer::_encode( short_t value, std::string& bytes )
	{
		bytes += "i<";
		bytes += std::to_string( value );
		bytes += record_sep;
	}
	void serializer::_encode( long_t value, std::string& bytes )
	{
		bytes += "i>";
		bytes += std::to_string( value );
		bytes += record_sep;
	}
	void serializer::_encode( flt_t value, std::string& bytes )
	{
		bytes += "f!";
		bytes += std::to_string( value );
		bytes += record_sep;
	}
	void serializer::_encode( low_t value, std::string& bytes )
	{
		bytes += "f<";
		bytes += std::to_string( value );
		bytes += record_sep;
	}
	void serializer::_encode( high_t value, std::string& bytes )
	{
		bytes += "f>";
		bytes += std::to_string( value );
		bytes += record_sep;
	}
	void serializer::_encode( index_t value, std::string& bytes )
	{
		bytes += "I!";
		bytes += std::to_string( value );
		bytes += record_sep;
	}
	void serializer::_encode( name_t value, std::string& bytes )
	{
		bytes += "n!";
		bytes += ( value != no_name ? str( value ).stdstr() : "" );
		bytes += record_sep;
	}
	void serializer::_encode( const std::string& value, std::string& bytes )
	{
		bytes += "b+";
		for( auto c : value )
		{
			if( c == '\\' )
				bytes += "\\\\";
			else if( c < 32 || c > 127 )
				bytes += "\\" + hex::toHex( c ).stdstr();
			else
				bytes += c;
		}
		bytes += record_sep;
	}
	void serializer::_encode( const string& value, std::string& bytes )
	{
		bytes += "s!";
		for( auto c : value.stdstr() )
		{
			if( c == '\\' )
				bytes += "\\\\";
			else if( c < 32 || c > 127 )
				bytes += "\\" + hex::toHex( c ).stdstr();
			else
				bytes += c;
		}
		bytes += record_sep;
	}
	void serializer::_encode( const char* value, std::string& bytes )
	{
		bytes += "b+";
		for( auto c = value; *c != '\0'; ++c )
		{
			if( *c == '\\' )
				bytes += "\\\\";
			else if( *c < 32 || *c > 127 )
				bytes += "\\" + hex::toHex( *c ).stdstr();
			else
				bytes += *c;
		}
		bytes += record_sep;
	}


	bool serializer::_decodeBool( std::string::const_iterator& pos, std::string::const_iterator end )
	{
		pos = _expectType( name_bool, pos, end );
		return *pos++ == '1';
	}
	byte_t serializer::_decodeByte( std::string::const_iterator& pos, std::string::const_iterator end )
	{
		pos = _expectType( name_byte, pos, end );
		hex::digits hd( *pos, *( pos + 1 ) );
		++pos; ++pos;
		return hex::toByte( hd );
	}
	char_t serializer::_decodeChar( std::string::const_iterator& pos, std::string::const_iterator end )
	{
		pos = _expectType( name_char, pos, end );
		string num;
		for( ; pos != end && *pos != record_sep; ++pos )
			num += *pos;
		++pos;
		return static_cast<char_t>( num.toIndex() );
	}
	int_t serializer::_decodeInt( std::string::const_iterator& pos, std::string::const_iterator end )
	{
		pos = _expectType( name_int, pos, end );
		string num;
		for( ; pos != end && *pos != record_sep; ++pos )
			num += *pos;
		++pos;
		return num.toInt();
	}
	short_t serializer::_decodeShort( std::string::const_iterator& pos, std::string::const_iterator end )
	{
		pos = _expectType( name_short, pos, end );
		string num;
		for( ; pos != end && *pos != record_sep; ++pos )
			num += *pos;
		++pos;
		return num.toShort();
	}
	long_t serializer::_decodeLong( std::string::const_iterator& pos, std::string::const_iterator end )
	{
		pos = _expectType( name_long, pos, end );
		string num;
		for( ; pos != end && *pos != record_sep; ++pos )
			num += *pos;
		++pos;
		return num.toLong();
	}
	flt_t serializer::_decodeFloat( std::string::const_iterator& pos, std::string::const_iterator end )
	{
		pos = _expectType( name_float, pos, end );
		string num;
		for( ; pos != end && *pos != record_sep; ++pos )
			num += *pos;
		++pos;
		return num.toFloat();
	}
	low_t serializer::_decodeLow( std::string::const_iterator& pos, std::string::const_iterator end )
	{
		pos = _expectType( name_low, pos, end );
		string num;
		for( ; pos != end && *pos != record_sep; ++pos )
			num += *pos;
		++pos;
		return num.toLow();
	}
	high_t serializer::_decodeHigh( std::string::const_iterator& pos, std::string::const_iterator end )
	{
		pos = _expectType( name_high, pos, end );
		string num;
		for( ; pos != end && *pos != record_sep; ++pos )
			num += *pos;
		++pos;
		return num.toHigh();
	}
	index_t serializer::_decodeIndex( std::string::const_iterator& pos, std::string::const_iterator end )
	{
		pos = _expectType( name_index, pos, end );
		string num;
		for( ; pos != end && *pos != record_sep; ++pos )
			num += *pos;
		++pos;
		return num.toIndex();
	}
	name_t serializer::_decodeName( std::string::const_iterator& pos, std::string::const_iterator end )
	{
		pos = _expectType( name_name, pos, end );
		string nam;
		for( ; pos != end && *pos != record_sep; ++pos )
			nam += *pos;
		++pos;
		return name( nam );
	}
	std::string serializer::_decodeBytes( std::string::const_iterator& pos, std::string::const_iterator end )
	{
		pos = _expectType( name_bytes, pos, end );
		std::string value;
		bool esc{ false };
		for( ; pos != end && *pos != record_sep; ++pos )
		{
			if( !esc )
			{
				if( *pos == '\\' )
					esc = true;
				else
					value += *pos;
			}
			else
			{
				if( *pos == '\\' )
					value += *pos;
				else
					value += hex::toByte( hex::digits( *pos, *++pos ) );
				esc = false;
			}
		}
		++pos;
		return value;
	}
	string serializer::_decodeStr( std::string::const_iterator& pos, std::string::const_iterator end )
	{
		pos = _expectType( name_string, pos, end );
		std::string raw;
		bool esc{ false };
		for( ; pos != end && *pos != record_sep; ++pos )
		{
			if( !esc )
			{
				if( *pos == '\\' )
					esc = true;
				else
					raw += *pos;
			}
			else
			{
				if( *pos == '\\' )
					raw += *pos;
				else
				{
					hex::digits digs( *pos, *( pos + 1 ) );
					++pos;
					raw += hex::toByte( digs );
				}
				esc = false;
			}
		}
		++pos;
		return string( std::move( raw ) );
	}

	std::string::const_iterator serializer::_expectType( name_t exp_type,
		std::string::const_iterator pos, std::string::const_iterator end )
	{
		auto t1 = *pos;
		name_t act_type{ no_name };
		if( ++pos != end )
		{
			switch( t1 )
			{
				case 'B':
					act_type = *pos == '!' ? name_bool : *pos == '+' ? name_bits : no_name;
					break;
				case 'b':
					act_type = *pos == '!' ? name_byte : *pos == '+' ? name_bytes : no_name;
					break;
				case 'c':
					act_type = *pos == '!' ? name_char : *pos == '+' ? name_chars : no_name;
					break;
				case 'i':
					act_type = *pos == '!' ? name_int : *pos == '<' ? name_short : *pos == '>' ? name_long : no_name;
					break;
				case 'f':
					act_type = *pos == '!' ? name_float : *pos == '<' ? name_low : *pos == '>' ? name_high : no_name;
					break;
				case 'I':
					act_type = *pos == '!' ? name_index : no_name;
					break;
				case 'n':
					act_type = *pos == '!' ? name_name : no_name;
					break;
				case 's':
					act_type = *pos == '!' ? name_string : no_name;
					break;
				case 'r':
					act_type = *pos == '!' ? name_regex : no_name;
					break;
				case 'e':
					act_type = *pos == '!' ? name_expression : no_name;
					break;
				default:
					break;
			}
		}
		if( exp_type == act_type )
			return ++pos;
		else
			throw WrongType( "Serializer expected to de-serialize '" + str( exp_type ) + "', got '"
				+ ( act_type != no_name ? str( act_type ) : "N/A" ) + "' instead!" );
	}
}
