#include "String.h"
#include <eoninlinetest/InlineTest.h>
#include <cctype>
#include <regex>
#include <unordered_map>


namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// Static utility methods
	//

	string string::toString( double value )
	{
		static char digits[ 480 ];
#ifdef EON_WINDOWS
		sprintf_s( digits, 480, "%.10f", value );
#else
		snprintf( digits, 480, "%.10f", value );
#endif
		auto size = strlen( digits );
		for( ; digits[ size - 1 ] == '0' && digits[ size - 2 ] != '.';
			--size )
			;
		return string( std::string( digits, size ), true );
	}

	string string::toString( long double value )
	{
		static char digits[ 480 ];
#ifdef EON_WINDOWS
		sprintf_s( digits, 480, "%.10Lf", value );
#else
		snprintf( digits, 480, "%.10Lf", value );
#endif
		auto size = strlen( digits );
		for( ; digits[ size - 1 ] == '0' && digits[ size - 2 ] != '.';
			--size )
			;
		return string( std::string( digits, size ), true );
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


	inline bool _isWordSeparator( const Characters& chars, char_t chr ) {
		return chars.isSeparator( chr ) || chars.isPunctuation( chr ); }

	string::iterator string::findWordStart( iterator start, iterator end )
	{
		auto& chars = Characters::get();

		// If first character is the first in the string and also a letter,
		// then we need not search any further.
		bool is_first = start == start.getFirst();
		if( is_first && chars.isLetter( *start ) )
			return start;

		// Next word must be after a space or a punctuation (and must start
		// with a letter).
		bool separator = is_first ? false : _isWordSeparator( chars, *( start - 1 ) );
		for( auto i = start; i != end; ++i )
		{
			if( chars.isLetter( *i ) && separator )
				return i;
			else
				separator = _isWordSeparator( chars, *i );
		}

		return end.getEnd();
	}

	inline bool _isSentenceSeparator( const Characters& chars, char_t chr ) {
		return chr == '.' || chr == ':' || chr == '!' || chr == '?' || chr == '\n'; }

	inline bool _haveSeparator( const Characters& chars, string::iterator i )
	{
		for( ; i; --i )
		{
			if( _isSentenceSeparator( chars, *i ) )
				return true;
			else if( !chars.isSeparator( *i ) )
				break;
		}
		return false;
	}

	string::iterator string::findSentenceStart( iterator start, iterator end )
	{
		auto& chars = Characters::get();

		// If first character is the first in the string and also a letter,
		// then we need not search any further.
		bool is_first = start == start.getFirst();
		if( is_first && chars.isLetter( *start ) )
			return start;

		// Next sentence must start with a letter after '.' or ':' or '!' or '?' or '\n'.
		// White-spaces can occur in-between.
		bool separator = false;
		if( !is_first )
			separator = _haveSeparator( chars, start - 1 );

		for( auto i = start; i != end; ++i )
		{
			if( chars.isLetter( *i ) && separator )
				return i;
			else if( !chars.isSeparator( *i ) )
				separator = _isSentenceSeparator( chars, *i );
		}

		return end.getEnd();
	}
}
