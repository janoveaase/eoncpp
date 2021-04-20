#include "String.h"
#include <cctype>
#include <regex>


namespace eon
{
	const string string::Empty;


	string& string::assign( const eon_char* codepoints, size_t size )
	{
		clear();
		reserve( size );
		for( size_t i = 0; i < size; ++i )
			*this += codepoints[ i ];
		return *this;
	}
	string& string::assign( const char* chars, size_t size )
	{
		iterator i( chars, size );
		if( !i.validUTF8() )
			throw InvalidUTF8();
		NumChars = i.numChars();
		Bytes.assign( chars, size );
		return *this;
	}

	string& string::assign( size_t copies, eon_char codepoint )
	{
		uint32_t bytes;
		auto size = iterator::unicodeToBytes( codepoint, bytes );
		Bytes.reserve( size * copies );
		for( size_t i = 0; i < copies; ++i )
			Bytes.append( (const char*)&bytes, size );
		NumChars = copies;
		return *this;
	}
	string& string::assign( size_t copies, const string& other )
	{
		// Make sure 'other' and 'this' are not the same!
		if( &other.Bytes != &Bytes )
		{
			Bytes.reserve( other.numBytes() * copies );
			for( size_t i = 0; i < copies; ++i )
				Bytes.append( other.Bytes );
			NumChars = other.NumChars * copies;
			return *this;
		}
		else
			return assign( copies, string( other ) );
	}
	string& string::assign( size_t copies, const std::string& stdstr )
	{
		// Make sure 'other' is not our own buffer!
		if( &stdstr != &Bytes )
		{
			iterator i( stdstr );
			if( !i.validUTF8() )
				throw InvalidUTF8();
			Bytes.reserve( stdstr.size() * copies );
			for( size_t i = 0; i < copies; ++i )
				Bytes.append( stdstr );
			NumChars = i.numChar() * copies;
			return *this;
		}
		else
			return assign( copies, std::string( stdstr ) );
	}
	string& string::assign( size_t copies, const substring& sub )
	{
		// Make sure 'sub' is not from 'this'!
		if( !sub.sameBuffer( Bytes.c_str() ) )
		{
			if( !sub.begin().validUTF8() )
				throw InvalidUTF8();
			clear();
			Bytes.reserve( sub.toSize() * copies );
			for( size_t i = 0; i < copies; ++i )
				*this += sub;
			return *this;
		}
		else
			return assign( copies, string( sub ) );
	}

	string& string::operator=( const substring& sub )
	{
		if( !sub.validUTF8() )
			throw InvalidUTF8();
		if( sub.isHighToLow() )
		{
			clear();
			for( auto i = sub.begin(); i != sub.end(); --i )
				*this += *i;
		}
		else
		{
			NumChars = sub.numChars();
			Bytes.assign( sub.begin().byteData(), sub.numBytes() );
		}
		return *this;
	}

	string& string::operator=(
		const std::initializer_list<eon_char>& codepoints )
	{
		clear();
		reserve( codepoints.size() );
		for( auto& codepoint : codepoints )
			*this += codepoint;
		return *this;
	}
	string& string::operator=( const std::vector<eon_char>& codepoints )
	{
		clear();
		reserve( codepoints.size() );
		for( auto& codepoint : codepoints )
			*this += codepoint;
		return *this;
	}
	string& string::operator=( const std::initializer_list<char>& chars )
	{
		clear();
		reserve( chars.size() );
		for( auto& chr : chars )
			*this += static_cast<eon_byte>( chr );
		return *this;
	}
	string& string::operator=(
		const std::initializer_list<unsigned char>& chars )
	{
		clear();
		reserve( chars.size() );
		for( auto& chr : chars )
			*this += static_cast<eon_byte>( chr );
		return *this;
	}




	string::iterator string::decodeIterator( const string& encoded_iterator )
	{
		std::regex pattern{ R"(^(\d+):(\d+)$)" };
		std::smatch match;
		if( std::regex_match( encoded_iterator.stdstr(), match, pattern ) )
		{
			return iterator( Bytes.c_str(), Bytes.size(), NumChars,
				Bytes.c_str() + std::atoi( match[ 1 ].str().c_str() ),
				std::atoi( match[ 2 ].str().c_str() ) );
		}
		else
			return end();
	}
	substring string::decodeSubstring( const string& encode_substring )
	{
		std::regex pattern{ R"(^(\d+):(\d+)\-(\d+):(\d+)$)" };
		std::smatch match;
		if( std::regex_match( encode_substring.stdstr(), match, pattern ) )
		{
			return substring(
				iterator( Bytes.c_str(), Bytes.size(), NumChars, Bytes.c_str()
					+ std::atoi( match[ 1 ].str().c_str() ), std::atoi(
						match[ 2 ].str().c_str() ) ),
				iterator( Bytes.c_str(), Bytes.size(), NumChars, Bytes.c_str()
					+ std::atoi( match[ 3 ].str().c_str() ), std::atoi(
						match[ 4 ].str().c_str() ) ) );
		}
		else
			return substring( end() );
	}




	string::iterator string::bytePos( eon_pos pos, iterator start ) const
	{
		if( start )
			start.assertSameBuffer( Bytes.c_str() );
		else
			start = begin();
		if( _ascii() )		// Optimized ASCII handling
		{
			if( start.numByte() + pos < Bytes.size() )
				return iterator( Bytes.c_str(), Bytes.size(), start.numChar()
					+ pos, Bytes.c_str() + start.numChar() + pos );
			else
				return end();
		}

		// We have to count
		for( ; start != end() && pos > 0; ++start, --pos )
			;
		return start;
	}

	string::iterator string::rebase( const iterator& other ) const noexcept
	{
		if( other
			&& other.numByte() < numBytes() && other.numChar() < NumChars )
		{
			// The iterator is inside of our range
			return iterator( Bytes.c_str(), numBytes(), NumChars,
				Bytes.c_str() + other.numByte() );
		}
		return end();
	}

	string::iterator string::rebaseMoved( const iterator& other ) const noexcept
	{
		if( other )
			return iterator( Bytes.c_str(), numBytes(), NumChars,
				Bytes.c_str() + other.numByte(), other.numChar() );
		else
			return end();
	}




	string::iterator string::insert(
		const iterator& pos, const string& substr )
	{
		pos.assertSameBuffer( Bytes.c_str() );
		if( !pos )
		{
			*this += substr;
			return iterator( Bytes.c_str(), Bytes.size(), NumChars,
				Bytes.c_str() + pos.numByte() );
		}
		Bytes.insert( pos.numByte(), substr.Bytes );
		NumChars += substr.NumChars;
		return iterator( Bytes.c_str(), Bytes.size(), NumChars,
			Bytes.c_str() + pos.numByte() );
	}

	string& string::erase( const substring& sub ) noexcept
	{
		if( sub.empty() )
			return *this;
		auto area = sub.lowToHigh();
		Bytes.erase( area.begin().numByte(), area.numBytes() );
		NumChars -= area.numChars();
		return *this;
	}




	string string::upper( const substring& sub ) const
	{
		if( sub.empty() )
			return *this;
		auto area = sub.lowToHigh();
		string result;
		if( area.begin() > begin() )
			result += substr( begin(), area.begin() );
		for( auto i = area.begin(); i != area.end(); ++i )
		{
			if( *i <= 0xFFFF )
				result += static_cast<eon_char>(
					std::toupper( static_cast<int>( *i ) ) );
			else
				result += *i;
		}
		if( area.end() != end() )
			result += substr( area.end() );
		return result;
	}
	string string::lower( const substring& sub ) const
	{
		if( sub.empty() )
			return *this;
		auto area = sub.lowToHigh();
		string result;
		if( area.begin() > begin() )
			result += substr( begin(), area.begin() );
		for( auto i = area.begin(); i != area.end(); ++i )
		{
			if( *i <= 0xFFFF )
				result += static_cast<eon_char>(
					std::tolower( static_cast<int>( *i ) ) );
			else
				result += *i;
		}
		if( area.end() != end() )
			result += substr( area.end() );
		return result;
	}

	string string::ucFirst( const substring& sub ) const
	{
		if( sub.empty() )
			return *this;
		auto area = sub.lowToHigh();
		if( area.numBytes() == 1 )
			return upper( area );
		else
			return string( substr( begin(), area.begin() ) )
			+ string( substr( area.begin(), area.begin() + 1 ) ).upper()
			+ string( substr( area.begin() + 1 ) );
	}
	string string::ucWords( const substring& sub ) const
	{
		if( sub.empty() )
			return *this;
		auto area = sub.lowToHigh();
		string result;
		if( area.begin() > begin() )
			result += substr( begin(), area.begin() );
		bool new_word = true;
		auto& chars = Characters::get();
		for( auto i = area.begin(); i != area.end(); ++i )
		{
			if( new_word )
			{
				if( chars.isLetterLowerCase( *i ) )
					result += static_cast<eon_char>( std::toupper( *i ) );
				else
					result += *i;
				if( chars.isLetter( *i ) )
					new_word = false;
			}
			else
			{
				result += *i;
				if( chars.isSeparator( *i ) || chars.isPunctuation( *i ) )
					new_word = true;
			}
		}
		if( area.end() != end() )
			result += substr( area.end() );
		return result;
	}
	string string::ucSentences( const substring& sub ) const
	{
		if( sub.empty() )
			return *this;
		auto area = sub.lowToHigh();
		string result;
		if( area.begin() > begin() )
			result += substr( begin(), area.begin() );
		bool new_sentence = true;
		auto& chars = Characters::get();
		for( auto i = area.begin(); i != area.end(); ++i )
		{
			if( new_sentence )
			{
				if( chars.isLetterLowerCase( *i ) )
					result += static_cast<eon_char>( std::toupper( *i ) );
				else
					result += *i;
				if( chars.isLetter( *i ) )
					new_sentence = false;
			}
			else
			{
				result += *i;
				if( *i == '.' || *i == ':' || *i == '!' || *i == '?' )
					new_sentence = true;
			}
		}
		if( area.end() != end() )
			result += substr( area.end() );
		return result;
	}

	substring string::slice( int64_t start, int64_t end ) const
	{
		auto s = static_cast<size_t>( start >= 0 ? start : NumChars + start );
		auto e = static_cast<size_t>( end >= 0 ? end : NumChars + end );
		if( s <= e )
			++e;
		else
			--e;
		auto sub = substr();
		return substring( sub.iterator( s ), sub.iterator( e ) );
	}

	substring string::beforeFirst( const string& delimiter ) const
	{
		auto found = findFirst( delimiter );
		if( found )
			return substr( begin(), found.begin() );
		else
			return substring( end() );
	}
	substring string::beforeFirst( eon_char delimiter ) const
	{
		auto found = findFirst( delimiter );
		if( found )
			return substr( begin(), found.begin() );
		else
			return substring( end() );
	}
	substring string::beforeLast( const string& delimiter ) const
	{
		auto found = findLast( delimiter );
		if( found )
			return substr( begin(), found.begin() );
		else
			return substring( end() );
	}
	substring string::beforeLast( eon_char delimiter ) const
	{
		auto found = findLast( delimiter );
		if( found )
			return substr( begin(), found.begin() );
		else
			return substring( end() );
	}
	substring string::afterFirst( const string& delimiter ) const
	{
		auto found = findFirst( delimiter );
		if( found )
			return substr( found.end() );
		else
			return substring( end() );
	}
	substring string::afterFirst( eon_char delimiter ) const
	{
		auto found = findFirst( delimiter );
		if( found )
			return substr( found.end() );
		else
			return substring( end() );
	}
	substring string::afterLast( const string& delimiter ) const
	{
		auto found = findLast( delimiter );
		if( found )
			return substr( found.end() );
		else
			return substring( end() );
	}
	substring string::afterLast( eon_char delimiter ) const
	{
		auto found = findLast( delimiter );
		if( found )
			return substr( found.end() );
		else
			return substring( end() );
	}

	string string::replace( const string& find, const string& replacement,
		const substring& sub ) const
	{
		if( sub.empty() )
			return *this;
		auto area = sub.lowToHigh();
		auto found = this->findFirst( find, area );
		if( !found )
			return *this;

		string result;
		if( found.begin() != begin() )
			result = substr( begin(), found.begin() );
		else if( area.begin() > begin() )
			result += substr( begin(), area.begin() );
		auto pos = end();
		while( found )
		{
			result += replacement;
			pos = found.end();
			found = this->findFirst( find, substring( pos, area.end() ) );
			if( found && found.begin() > pos )
				result += substr( pos, found.begin() );
		}
		if( pos != end() )
			result += substr( pos );
		return result;
	}
	string string::replace( eon_char find, eon_char replacement,
		const substring& sub ) const
	{
		if( sub.empty() )
			return *this;
		auto area = sub.lowToHigh();
		string result;
		if( area.begin() > begin() )
			result += substr( begin(), area.begin() );
		for( auto i = area.begin(); i != area.end(); ++i )
		{
			if( *i == find )
				result += replacement;
			else
				result += *i;
		}
		if( area.end() != end() )
			result += substr( area.end() );
		return result;
	}

	string string::escape( const substring& sub ) const
	{
		if( sub.empty() )
			return *this;
		auto area = sub.lowToHigh();
		string result;
		for( auto i = area.begin(); i != area.end(); ++i )
		{
			switch( *i )
			{
				case '\a':
					result += "\\a";
					break;
				case '\b':
					result += "\\b";
					break;
				case '\f':
					result += "\\f";
					break;
				case '\n':
					result += "\\n";
					break;
				case '\r':
					result += "\\r";
					break;
				case '\t':
					result += "\\t";
					break;
				case '\v':
					result += "\\v";
					break;
				case '\\':
					result += "\\\\";
					break;
				case '\"':
					result += "\\\"";
					break;
				case '\?':
					result += "\\?";
					break;
				default:
					result += *i;
			}
		}
		return result;
	}
	string string::unescape( const substring& sub ) const
	{
		if( sub.empty() )
			return *this;
		auto area = sub.lowToHigh();
		string result;
		bool esc = false;
		for( auto i = area.begin(); i != area.end(); ++i )
		{
			if( !esc )
			{
				if( *i == '\\' )
					esc = true;
				else
					result += *i;
			}
			else
			{
				switch( *i )
				{
					case 'a':
						result += "\a";
						break;
					case 'b':
						result += "\b";
						break;
					case 'f':
						result += "\f";
						break;
					case 'n':
						result += "\n";
						break;
					case 'r':
						result += "\r";
						break;
					case 't':
						result += "\t";
						break;
					case 'v':
						result += "\v";
						break;
					case '\\':
						result += "\\";
						break;
					case '"':
						result += "\"";
						break;
					case '?':
						result += "\?";
						break;
					default:
						result += "\\";
						result += *i;
				}
				esc = false;
			}
		}
		return result;
	}

	string string::escapeNonPrintable( const substring& sub ) const
	{
		if( sub.empty() )
			return *this;
		auto area = sub.lowToHigh();
		string result;
		for( auto codepoint : area )
		{
			if( codepoint < '\n'
				|| ( codepoint > '\n' && codepoint < ' ' )
				|| codepoint > 126 )
				result += "&#" + string( static_cast<int32_t>( codepoint ) )
				+ SemiColonChr;
			else
				result += codepoint;
		}
		return result;
	}
	string string::unescapeNonPrintable( const substring& sub ) const
	{
		if( sub.empty() )
			return *this;
		auto area = sub.lowToHigh();
		string result;
		for( auto i = area.begin(); i != area.end(); ++i )
		{
			if( *i == '&' && i.numChar() < NumChars - 4 && *( i + 1 ) == '#' )
			{
				auto end = substring( i, area.end() ).findFirst(
					SemiColonChr );
				if( end )
				{
					auto num = substr( i + 2, end.begin() );
					if( num.findFirstOtherThan( charcat::number_ascii_digit ) == num.end() )
					{
						result += static_cast<eon_char>( num.toUInt32() );
						i = end.begin();
						continue;
					}
				}
			}
			result += *i;
		}
		return result;
	}

	string string::doubleQuote( const substring& sub ) const
	{
		if( sub.empty() )
			return *this;
		auto area = sub.lowToHigh();
		string result;
		if( area.begin() > begin() )
			result = substr( begin(), area.begin() );
		if( !isDoubleQuoted( area ) )
			result += DblQuoteChr + string( area ) + DblQuoteChr;
		if( area.end() != end() )
			result += substr( area.end() );
		return result;
	}
	string string::singleQuote( const substring& sub ) const
	{
		if( sub.empty() )
			return *this;
		auto area = sub.lowToHigh();
		string result;
		if( area.begin() > begin() )
			result = substr( begin(), area.begin() );
		if( !isSingleQuoted( area ) )
			result += SglQuoteChr + string( area ) + SglQuoteChr;
		if( area.end() != end() )
			result += substr( area.end() );
		return result;
	}

	string string::unQuote( const substring& sub ) const
	{
		if( sub.empty() )
			return *this;
		auto area = sub.lowToHigh();
		string result;
		if( area.begin() > begin() )
			result = substr( begin(), area.begin() );
		if( isDoubleQuoted( area ) || isSingleQuoted( area ) )
			result += substr( area.begin() + 1, area.end() - 1 );
		if( area.end() != end() )
			result += substr( area.end() );
		return result;
	}

	string string::reverse( const substring& sub ) const
	{
		if( sub.empty() )
			return *this;
		auto area = sub.lowToHigh();
		auto revs = sub.highToLow();
		string result;
		if( area.begin() > begin() )
			result = substr( begin(), area.begin() );
		for( auto i = revs.begin(); i != revs.end(); --i )
			result += *i;
		if( area.end() != end() )
			result += substr( area.end() );
		return result;
	}

	string string::padLeftAndRight( size_t target_size, eon_char fill ) const
	{
		if( numChars() >= target_size )
			return *this;

		auto diff = target_size - numChars(), half = diff / 2;
		return string( half, fill ) + *this + string( diff - half, fill );
	}

	string string::indentLines( size_t indentation_level,
		eon_char indentation_char ) const
	{
		auto lines = splitSequential<std::list<substring>>( NewlineChr );
		string result;
		bool first_line = true;

		auto line = lines.begin();
		auto level = line->indentationLevel( indentation_char );
		if( level == indentation_level )
		{
			result += *line;
			++line;
			first_line = false;
		}

		for( ; line != lines.end(); ++line )
		{
			if( first_line )
				first_line = false;
			else
				result += NewlineChr;
			if( !line->empty() )
				result += string(
					indentation_level, indentation_char ) + *line;
		}
		return result;
	}




	string string::separateThousands( eon_char thousands_sep,
		eon_char decimal_separator ) const
	{
		if( !isFloat() && !isUInt() && !isInt() )
			return *this;

		auto sep = findFirst( decimal_separator );
		if( !sep )
			sep = substring( end() );

		if( sep.begin() - begin() < 4 )
			return *this;

		string formatted(
			substr( begin(), begin() + sep.begin().numChar() % 3 ) );
		auto pos = begin() + formatted.numChars();
		while( pos != sep.begin() )
		{
			if( !formatted.empty() )
				formatted += thousands_sep;
			formatted += substr( pos, pos + 3 );
			pos += 3;
		}
		formatted += substr( sep.begin() );
		return formatted;
	}
	string string::roundNumber( size_t max_decimals,
		eon_char decimal_separator ) const
	{
		if( !isFloat() && !isUInt() && !isInt() )
			return *this;

		auto digits = chars();
		auto sep = _findDecimalSeparator( digits, decimal_separator);
		if( sep == digits.size() )
			return *this;

		auto num_decimals = digits.size() - sep - 1;
		if( num_decimals <= max_decimals )
			return *this;

		auto digit = digits[ sep + 1 + max_decimals ] - ZeroChr;
		if( max_decimals == 0 )
			digits.erase( digits.begin() + sep, digits.end() );
		else
			digits.erase( digits.begin() + sep + 1 + max_decimals,
				digits.end() );
		if( digit >= 5 )
			_roundUp( digits, digits.size() - 1 );

		return string( digits );
	}




	string& string::operator=( const std::wstring& stdwstr )
	{
		static std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return *this = conv.to_bytes( stdwstr );
	}
	string& string::operator=( const wchar_t* cstr )
	{
		static std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return *this = conv.to_bytes( cstr );
	}
	std::wstring string::wstr() const
	{
		static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		return conv.from_bytes( Bytes );
	}




	string& string::push_back( const string& element, const string& separator )
	{
		if( !empty() )
			*this += separator;
		*this += element;
		return *this;
	}
	string& string::push_front( const string& element,
		const string& separator )
	{
		if( !empty() )
			*this = element + separator + *this;
		else
			*this = element;
		return *this;
	}
	string& string::orderList( const string& separator )
	{
		auto elements = substr().splitNonsequential<std::set<substring>>(
			separator.substr() );
		return *this = separator.join( elements );
	}




	void string::setLocale( const std::string& name )
	{
		static std::string locale_name;
		if( name != locale_name )
		{
			locale_name = name;
			std::locale::global( std::locale( locale_name ) );
		}
	}

	const std::string& string::bom()
	{
		static std::string _bom;
		if( _bom.empty() )
		{
			_bom = (char)0xEF;
			_bom += (char)0xBB;
			_bom += (char)0xBF;
		}
		return _bom;
	}




	size_t string::_findDecimalSeparator( std::vector<eon_char>& digits,
		eon_char decimal_separator ) noexcept
	{
		for( size_t i = 0; i < digits.size(); ++i )
		{
			if( digits[ i ] == decimal_separator )
				return i;
		}
		return digits.size();
	}

	void string::_roundUp( std::vector<eon_char>& digits, size_t i ) noexcept
	{
		// The digit at i + 1 is greater than 5, so we have to round up.
		if( digits[ i ] == PointChr )
			--i;
		auto digit = digits[ i ] - ZeroChr;
		if( i == 0 )
		{
			digits[ i ] = '9';
			digits.insert( digits.begin(), static_cast<eon_byte>( '1' ) );
			return;
		}
		if( digit == 9 )
		{
			digits[ i ] = '0';
			_roundUp( digits, i - 1 );
		}
		else
			digits[ i ] = static_cast<eon_byte>( ZeroChr + digit + 1 );
	}
}
