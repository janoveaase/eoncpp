#include "Substring.h"


namespace eon
{
	substring substring::lowToHigh() const noexcept
	{
		if( !isHighToLow() )
			return *this;
		return substring( End + 1, Beg + 1 );
	}
	substring substring::highToLow() const noexcept
	{
		if( !isLowToHigh() )
			return *this;
		return substring( End - 1, Beg - 1 );
	}



	size_t substring::numBytes() const noexcept
	{
		if( Beg.atREnd() )
		{
			if( End.atREnd() )
				return 0;
			else
				return End.numByte() - Beg.numByte() + 1;
		}
		else if( End.atREnd() )
			return Beg.numByte() - End.numByte() + 1;
		else if( Beg <= End )
			return End.numByte() - Beg.numByte();
		else
			return Beg.numByte() - End.numByte();
	}

	eon_byte substring::byte( eon_pos pos ) const noexcept
	{
		if( empty() )
			return 0;
		if( Beg < End )
			return pos < static_cast<eon_pos>( End.Begin - Beg.Begin )
			? *( Beg.Begin + pos ) : 0;
		else
			return pos < static_cast<eon_pos>( Beg.Begin - End.Begin )
			? *( End.Begin + pos ) : 0;
	}

	bool substring::blank() const noexcept
	{
		auto& chars = Characters::get();
		for( auto codepoint : *this )
		{
			if( !chars.isSeparatorSpace( codepoint ) )
				return false;
		}
		return true;
	}

	string_iterator substring::iterator( size_t num_char ) const noexcept
	{
		auto num_chars = end().numChar() - begin().numChar();
		if( num_char > num_chars )
			return end().getEnd();

		// If there are less than 10 characters, we can skip optimizations
		if( num_chars >= 10 )
		{
			// Since counting backwards is slightly more costly than counting
			// forwards, we skew the "middle point" to 2/3.
			auto mid_point = ( num_chars / 3 ) * 2;

			// Only count from end if the character is closer to the end
			if( num_char >= mid_point )
				return end() - ( num_chars - num_char );
		}

		// Count from beginning
		return begin() + num_char;
	}

	eon_pos substring::indentationLevel( eon_char indentation_char )
		const noexcept
	{
		eon_pos level = 0;
		for( auto codepoint : *this )
		{
			if( codepoint != indentation_char )
				break;
			++level;
		}
		return level;
	}

	std::vector<eon_char> substring::chars() const
	{
		std::vector<eon_char> result;
		for( auto chr : *this )
			result.push_back( chr );
		return result;
	}


	substring substring::trim() const
	{
		if( empty() )
			return *this;
		auto& chars = Characters::get();
		auto start = begin();
		for( ; start && chars.isSeparatorSpace( *start ); ++start )
			;
		if( !start )
			return substring( end().getEnd() );
		auto end = this->last();
		for( ; end && chars.isSeparatorSpace( *end ); --end )
			;
		return substring( start, end + 1 );
	}
	substring substring::trimLeading() const
	{
		if( empty() )
			return *this;
		auto& chars = Characters::get();
		auto start = begin();
		for( ; start && chars.isSeparatorSpace( *start ); ++start )
			;
		if( start != end() )
			return substring( start, end() );
		else
			return substring( end().getEnd() );
	}
	substring substring::trimTrailing() const
	{
		if( empty() )
			return *this;
		auto& chars = Characters::get();
		auto end = last();
		for( ; end && chars.isSeparatorSpace( *end ); --end )
			;
		if( end )
			return substring( begin(), end + 1 );
		else
			return substring( this->end().getEnd() );
	}




	bool substring::numeralsOnly() const noexcept
	{
		for( auto chr : *this )
		{
			if( !eon::isNumeral( chr ) )
				return false;
		}
		return true;
	}
	bool substring::isUInt() const noexcept
	{
		for( auto chr : *this )
		{
			if( !eon::isDigit( chr ) )
				return false;
		}
		return true;
	}
	bool substring::isFloat( eon_char decimal_separator ) const noexcept
	{
		if( empty() )
			return false;
		int sep = 0;
		auto i = begin();
		if( *i == '+' || *i == '-' )
			++i;
		for( ; i != end(); ++i )
		{
			if( *i == decimal_separator )
			{
				if( ++sep > 1 )
					return false;
			}
			else if( !isDigit( *i ) )
				return false;
		}
		return sep > 0;
	}

	int64_t substring::toInt64() const
	{
		int64_t num = 0;
		if( empty() )
			return num;
		auto i = begin();
		int64_t sign = *i == '+' ? 1 : *i == '-' ? -1 : 0;
		if( sign != 0 )
			++i;
		for( ; i != end(); ++i )
		{
			if( !isDigit( *i ) )
				break;
			num *= 10;
			num += ( *i - ZeroChr );
		}
		return sign ? num * sign : num;
	}
	int64_t substring::toUInt64() const
	{
		int64_t num = 0;
		if( empty() )
			return num;
		for( auto chr : *this )
		{
			if( !isDigit( chr ) )
				break;
			num *= 10;
			num += ( chr - ZeroChr );
		}
		return num;
	}
	double substring::toDouble( eon_char decimal_separator ) const
	{
		if( empty() )
			return 0.0;
		double num = 0.0, dec = 0.0, dec_pow = 1.0;
		int point = 0;
		auto i = begin();
		double sign = *i == '+' ? 1.0 : *i == '-' ? -1.0 : 0.0;
		if( sign != 0.0 )
			++i;
		for( ; i != end(); ++i )
		{
			if( *i == decimal_separator )
			{
				if( ++point > 1 )
					break;
			}
			else
			{
				if( !isDigit( *i ) )
					break;
				if( point == 0 )
				{
					num *= 10.0;
					num += ( *i - ZeroChr );
				}
				else
				{
					dec *= 10.0;
					dec += ( *i - ZeroChr );
					dec_pow *= 10.0;
				}
			}
		}
		if( sign )
			return sign * ( num + ( dec / dec_pow ) );
		else
			return num + ( dec / dec_pow );
	}

	substring substring::trimNumber( eon_char decimal_separator ) const
	{
		// Find first non-zero
		auto first_non_zero = begin();
		while( first_non_zero != end() && *first_non_zero == ZeroChr )
			++first_non_zero;

		// All zeros, then return first zero
		if( first_non_zero == end() )
			return substring( first_non_zero - 1, end() );

		// Find the decimal separator
		auto sep = first_non_zero;
		while( sep != end() && *sep != decimal_separator )
			++sep;

		// If no separator, we have an all integer number
		if( sep == end() )
			return substring( first_non_zero, end() );

		// Find last non-zero
		auto last_non_zero = last();
		while( last_non_zero != sep && *last_non_zero == ZeroChr )
			--last_non_zero;

		// If no trailing fractional zeros, return the integer part
		if( last_non_zero == sep )
			return substring( first_non_zero, sep );

		return substring( first_non_zero, last_non_zero + 1 );
	}
	substring substring::trimFloat( eon_char decimal_separator ) const
	{
		// Find first non-zero
		auto first_non_zero = begin();
		while( first_non_zero != end() && *first_non_zero == ZeroChr )
			++first_non_zero;

		// All zeros, then return empty substring
		if( first_non_zero == end() )
			return substring( end().getEnd() );

		// If first non-zero is the point, then include previous zero (if any)
		if( *first_non_zero == decimal_separator && first_non_zero != begin() )
			--first_non_zero;

		// Find the decimal separator
		auto sep = first_non_zero;
		while( sep != end() && *sep != decimal_separator )
			++sep;

		// If no separator, we have an all integer number
		if( sep == end() )
			return substring( first_non_zero, end() );

		// Find last non-zero
		auto last_non_zero = last();
		while( last_non_zero != sep && *last_non_zero == ZeroChr )
			--last_non_zero;

		// If no trailing fractional zeros
		if( last_non_zero == sep )
		{
			// If one or more trailing zeros, include one - otherwise just
			// include the sep
			if( last_non_zero + 1 != end() )
				return substring( first_non_zero, last_non_zero + 2 );
			else
				return substring( first_non_zero, last_non_zero + 1 );
		}

		return substring( first_non_zero, last_non_zero + 1 );
	}




	uint32_t substring::hash32() const noexcept
	{
		if( empty() )
			return 0;
		uint32_t h = FNV_OFFSET32;
		for( auto c = begin().byteData(); c != end().byteData(); ++c )
		{
			h ^= *c;
			h *= FNV_PRIME32;
		}
		return h;
	}
	uint64_t substring::hash64() const noexcept
	{
		if( empty() )
			return 0;
		uint64_t h = FNV_OFFSET64;
		for( auto c = begin().byteData(); c != end().byteData(); ++c )
		{
			h ^= *c;
			h *= FNV_PRIME64;
		}
		return h;
	}




	substring substring::findFirst( const substring& to_find ) const noexcept
	{
		if( empty() || to_find.empty() )
			return substring( End.getEnd() );

		// Optimize search when bytes-only
		if( Beg.bytesOnly() )
		{
			// If the sub-string isn't bytes-only, we know it cannot exist
			// within the source!
			if( !to_find.begin().bytesOnly() )
				return substring( End.getEnd() );

			auto found = _findFirst( Beg.Pos, numBytes(),
				to_find.begin().Pos, to_find.numBytes() );
			if( found != nullptr )
			{
				return substring(
					string_iterator( Beg, found, found - Beg.Begin ),
					string_iterator( Beg, found + to_find.numBytes(), (
						found + to_find.numBytes() ) - Beg.Begin ) );
			}
			else
				return substring( End.getEnd() );
		}

		// Do normal iterator search
		for( auto i = begin(); i != end(); ++i )
		{
			auto i_beg = i;
			auto j = to_find.begin();
			for( ; j != to_find.end() && i != end() && *i == *j; ++j, ++i )
				;
			if( j )
				return substring( i_beg, i );
		}
		return substring( End.getEnd() );
	}
	substring substring::findFirst( eon_char to_find ) const noexcept
	{
		if( empty() )
			return substring( End.getEnd() );

		// Optimize search when bytes-only
		if( Beg.bytesOnly() )
		{
			// If the codepoint isn't UTF-8, then only accept search if 'this'
			// also isn't UTF-8
			if( to_find > 127 && Beg.ValidUTF8 )
				return substring( End.getEnd() );

			auto found = _findFirst( Beg.Pos, numBytes(),
				static_cast<char>( to_find ) );
			if( found != nullptr )
				return substring(
					string_iterator( Beg, found, found - Beg.Begin ),
					string_iterator( Beg, found + 1,
						( found + 1 ) - Beg.Begin ) );
			else
				return substring( End.getEnd() );
		}

		// Do Beg normal iterator search
		for( auto i = begin(); i != end(); ++i )
		{
			if( *i == to_find )
				return substring( i, i + 1 );
		}
		return substring( End.getEnd() );
	}

	substring substring::findLast( const substring& to_find ) const noexcept
	{
		if( empty() || to_find.empty() )
			return substring( End.getEnd() );

		// Optimize search when bytes-only
		if( Beg.bytesOnly() )
		{
			// If the sub-string isn't bytes-only, we know it cannot exist
			// within the source!
			if( !to_find.begin().bytesOnly() )
				return substring( End.getEnd() );

			auto found = _findLast( Beg.Pos, numBytes(),
				to_find.begin().Pos, to_find.numBytes() );
			if( found != nullptr )
			{
				return substring(
					string_iterator( Beg, found, found - Beg.Begin ),
					string_iterator( Beg, found + to_find.numBytes(), (
						found + to_find.numBytes() ) - Beg.Begin ) );
			}
			else
				return substring( End.getEnd() );
		}

		// Do normal iterator search
		for( auto i = begin(); i != end(); --i )
		{
			auto i_beg = i;
			auto j = to_find.begin();
			for( ; j != to_find.end() && i != end() && *i == *j; --j, --i )
				;
			if( j )
				return substring( i_beg, i );
		}
		return substring( End.getEnd() );
	}
	substring substring::findLast( eon_char to_find ) const noexcept
	{
		if( empty() )
			return substring( End.getEnd() );

		// Optimize search when bytes-only
		if( Beg.bytesOnly() )
		{
			// If the codepoint isn't UTF-8, then only accept search if 'this'
			// also isn't UTF-8
			if( to_find > 127 && Beg.ValidUTF8 )
				return substring( End.getEnd() );

			auto found = _findLast( Beg.Pos, numBytes(),
				static_cast<char>( to_find ) );
			if( found != nullptr )
			{
				if( found < Beg.End )
					return substring(
						string_iterator( Beg, found, found - Beg.Begin ),
						string_iterator( Beg, found + 1, ( found + 1 ) - Beg.Begin ) );
				else
					return substring( string_iterator( Beg, found,
						found - Beg.Begin ), End );
			}
			else
				return substring( End.getEnd() );
		}

		// Do Beg normal iterator search
		for( auto i = Beg; i != End; --i )
		{
			if( *i == to_find )
				return substring( i, i + 1 );
		}
		return substring( End.getEnd() );
	}

	string_iterator substring::findFirstOf( const substring& characters )
		const noexcept
	{
		for( auto i = begin(); i != end(); ++i )
		{
			if( characters.contains( *i ) )
				return i;
		}
		return end();
	}
	string_iterator substring::findLastOf( const substring& characters )
		const noexcept
	{
		for( auto i = begin(); i != end(); --i )
		{
			if( characters.contains( *i ) )
				return i;
		}
		return end();
	}

	string_iterator substring::findFirstNotOf( const substring& characters )
		const noexcept
	{
		for( auto i = begin(); i != end(); ++i )
		{
			if( !characters.contains( *i ) )
				return i;
		}
		return end();
	}
	string_iterator substring::findLastNotOf( const substring& characters )
		const noexcept
	{
		for( auto i = begin(); i != end(); --i )
		{
			if( !characters.contains( *i ) )
				return i;
		}
		return end();
	}

	inline string_iterator substring::findFirstDiff( const substring& other )
		const noexcept
	{
		auto i = begin(), j = other.begin();
		for( ; i != end() && j != other.end(); ++i, ++j )
		{
			if( *i != *j )
				return i;
		}
		if( i != end() )
			return i;
		else
			return end();
	}

	substring substring::findFirstIgnoreSections(
		const substring& other, eon_char start_sect, eon_char end_sect )
		const noexcept
	{
		end_sect = end_sect == same_char ? start_sect : end_sect;
		int sections = 0;
		bool escaped = false;
		for( auto i = begin(); i != end(); ++i )
		{
			if( escaped )
				escaped = false;
			else if( sections > 0 )
			{
				if( start_sect != end_sect )
				{
					// Nesting
					if( *i == start_sect )
						++sections;
				}
				else
				{
					// Check for backslash
					if( *i == BackSlashChr )
						escaped = true;
				}
				if( *i == end_sect )
					--sections;
			}
			else
			{
				if( start_sect == end_sect && *i == BackSlashChr )
					escaped = true;
				else if( *i == start_sect )
					++sections;
				else
				{
					auto j = other.begin(), k = i;
					for( ; j != other.end() && k != end() && *k == *j;
						++j, ++k )
						;
					if( j == other.end() )
						return substring( i, k );
				}
			}
		}
		return substring( End.getEnd() );
	}
	substring substring::findFirstIgnoreSections( eon_char codepoint,
		eon_char start_sect, eon_char end_sect ) const noexcept
	{
		end_sect = end_sect == same_char ? start_sect : end_sect;
		int sections = 0;
		bool escaped = false;
		for( auto i = begin(); i != end(); ++i )
		{
			if( escaped )
				escaped = false;
			else if( sections > 0 )
			{
				if( start_sect != end_sect )
				{
					// Nesting
					if( *i == start_sect )
						++sections;
				}
				else
				{
					// Check for backslash
					if( *i == BackSlashChr )
						escaped = true;
				}
				if( *i == end_sect )
					--sections;
			}
			else
			{
				if( start_sect == end_sect && *i == BackSlashChr )
					escaped = true;
				else if( *i == start_sect )
					++sections;
				else if( *i == codepoint )
					return substring( i, i + 1 );
			}
		}
		return substring( End.getEnd() );
	}

	string_iterator substring::findFirst( charcat category ) const noexcept
	{
		auto& chars = Characters::get();
		for( auto i = begin(); i != end(); ++i )
		{
			if( chars.is( *i, category ) )
				return i;
		}
		return end();
	}
	string_iterator substring::findLast( charcat category ) const noexcept
	{
		auto& chars = Characters::get();
		for( auto i = begin(); i != end(); --i )
		{
			if( chars.is( *i, category ) )
				return i;
		}
		return end();
	}

	string_iterator substring::findFirstOtherThan( charcat category )
		const noexcept
	{
		auto& chars = Characters::get();
		for( auto i = begin(); i != end(); ++i )
		{
			if( !chars.is( *i, category ) )
				return i;
		}
		return end();
	}
	string_iterator substring::findLastOtherThan( charcat category )
		const noexcept
	{
		auto& chars = Characters::get();
		for( auto i = begin(); i != end(); --i )
		{
			if( !chars.is( *i, category ) )
				return i;
		}
		return end();
	}




	size_t substring::count( eon_char to_count ) const noexcept
	{
		size_t cnt = 0;
		for( auto i = begin(); i != end(); ++i )
		{
			if( *i == to_count )
				++cnt;
		}
		return cnt;
	}
	size_t substring::count( const substring& to_count ) const noexcept
	{
		if( numBytes() > 0
			&& static_cast<size_t>( numBytes() ) < to_count.numChars() )
			return 0;

		size_t cnt = 0;
		auto found = findFirst( to_count );
		while( found )
		{
			++cnt;
			found = substring( found.begin() + 1, end() ).findFirst(
				to_count );
		}
		return cnt;
	}



	int substring::compare( const substring& other, CompareType type )
		const noexcept
	{
		if( !*this )
			return other ? -1 : 0;
		else if( !other )
			return 1;

		if( type == CompareType::faster )
		{
			auto cmp = memcmp( Beg.Pos, other.Beg.Pos,
				numBytes() < other.numBytes()
				? numBytes()
				: other.numBytes() );
			return cmp != 0 || numBytes() == other.numBytes()
				? cmp : numBytes() < other.numBytes() ? -1 : 1;
		}
		else
		{
			auto i = Beg, other_i = other.Beg;
			int pos = 1;
			for( ; i != End && other_i != other.end(); ++i, ++other_i, ++pos )
			{
				if( *i < *other_i )
					return -pos;
				else if( *other_i < *i )
					return pos;
			}
			return i == End && other_i == other.End
				? 0 : i != End ? pos : -pos;
		}
	}
	int substring::iCompare( const substring& other ) const noexcept
	{
		auto i = Beg, other_i = other.Beg;
		int pos = 1;
		for( ; i != End && other_i != other.End; ++i, ++other_i, ++pos )
		{
			if( *i != *other_i )
			{
				// We only convert case when actually different
				auto i_lower = std::tolower( *i );
				auto other_lower = std::tolower( *other_i );
				if( i_lower < other_lower )
					return -pos;
				else if( i_lower > other_lower )
					return pos;
			}
		}
		return i == End && other_i == other.End ? 0 : i == End ? -pos : pos;
	}





	const char* substring::_findFirst( const char* source, size_t source_size,
		const char* substr, size_t substr_size ) const noexcept
	{
		const char* end = source + source_size - ( substr_size - 1 );
		for( auto c = _findFirst( source, source_size, *substr );
			c != nullptr && c != end;
			c = _findFirst( c + 1, source_size, *substr ) )
		{
			if( memcmp( c + 1, substr + 1, substr_size - 1 ) == 0 )
				return c;
		}
		return nullptr;
	}
	const char* substring::_findLast( const char* str, size_t str_size,
		char chr )
		noexcept
	{
		for( auto c = str, end = str - str_size; c != end; --c )
			if( *c == chr )
				return c;
		return nullptr;
	}
	const char* substring::_findLast( const char* source, size_t source_size,
		const char* substr, size_t substr_size ) const noexcept
	{
		const char* last = source + source_size - substr_size;
		for( auto c = last; ; --c )
		{
			if( *c != *substr )
				continue;
			if( source_size == 1 )
				return c;
			if( memcmp( c + 1, substr + 1, substr_size - 1 ) == 0 )
				return c;
		}
		return nullptr;
	}

	std::string substring::_getReverse() const
	{
		std::string revs; revs.reserve( numBytes() );
		uint32_t bytes{ 0 };
		for( auto i = begin(); i != end(); --i )
		{
			size_t size = string_iterator::unicodeToBytes( *i, bytes );
			revs += std::string( (const char*)&bytes, size );
		}
		return revs;
	}
}