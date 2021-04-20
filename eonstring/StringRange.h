#pragma once

/*
	Modify string ranges (by range and BlockArray<range> objects) and/or
	extract substrings marked by the ranges from the source string.
*/
/*
#include "Defs.h"
#include "BlockArray.h"
#include <regex>



class StringRange
{
public:
	StringRange() {}
	~StringRange() {}

	// Trim substring (by moving range start and end so that there are no
	// leading or trailing white-spaces)
	range trim( const std::string& source, const range& element );

	// Get a copy of the elements where each element is trimmed
	BlockArray<range> trim( const std::string& source, const BlockArray<range>& elements );

	// Get a copy of the elements, but without any empty
	BlockArray<range> removeEmpty( const BlockArray<range>& elements );


	// Re-split the array of ranges such that each original element is split in
	// two based on the given delimiter arguments (matching those for
	// StringSplit)
	BlockArray<range> reSplit( const std::string& source, const BlockArray<range>& elements, char delimiter );
	BlockArray<range> reSplit( const std::string& source, const BlockArray<range>& elements, const char* delimiter );
	BlockArray<range> reSplit( const std::string& source, const BlockArray<range>& elements, std::string& delimiter );
	BlockArray<range> reSplit( const std::string& source, const BlockArray<range>& elements, std::regex& delimiter );


	// Extract one element (NO ERROR CHECKING!)
	inline std::string first( const std::string& source, const BlockArray<range>& elements )
		{ return source.substr( elements[ 0 ].Begin, elements[ 0 ].Size ); }
	inline std::string second( const std::string& source, const BlockArray<range>& elements )
		{ return source.substr( elements[ 1 ].Begin, elements[ 1 ].Size ); }
	inline std::string third( const std::string& source, const BlockArray<range>& elements )
		{ return source.substr( elements[ 2 ].Begin, elements[ 2 ].Size ); }
	inline std::string fourth( const std::string& source, const BlockArray<range>& elements )
		{ return source.substr( elements[ 3 ].Begin, elements[ 3 ].Size ); }
	inline std::string fifth( const std::string& source, const BlockArray<range>& elements )
		{ return source.substr( elements[ 4 ].Begin, elements[ 4 ].Size ); }
	inline std::string last( const std::string& source, const BlockArray<range>& elements )
		{ return source.substr( elements[ elements.size() - 1 ].Begin, elements[ elements.size() - 1 ].Size ); }

	// Extract elements to a 'push' supported container of std::string elements
	// (Typically BlockArray or std::queue or std::stack.)
	template<typename Container>
	Container push( const std::string& source, const BlockArray<range>& elements )
	{
		Container result;
		for( size_t i = 0; i < elements.size(); ++i )
			result.append( source.substr( elements[ i ].Begin, elements[ i ].Size ) );
		return std::move( result );
	}

	// Extract elements to a 'push_back' supported container of std::string elements
	// (Typically std::vector and std::list.)
	template<typename Container>
	Container push_back( const std::string& source, const BlockArray<range>& elements )
	{
		Container result;
		for( size_t i = 0; i < elements.size(); ++i )
			result.push_back( source.substr( elements[ i ].Begin, elements[ i ].Size ) );
		return std::move( result );
	}

	// Extract elements to an 'insert' supported container of std::string elements
	// (Typically std::set and std::unordered_set - and their multiset counterparts.)
	template<typename Container>
	Container insert( const std::string& source, const BlockArray<range>& elements )
	{
		Container result;
		for( size_t i = 0; i < elements.size(); ++i )
			result.insert( source.substr( elements[ i ].Begin, elements[ i ].Size ) );
		return std::move( result );
	}

	// Extract elements to a 'pair' based container for (std::string, std::string) elements
	// The range array is read with even elements as first pair element and odd as second.
	// Running 'reSplit' will create such an array.
	// (Typically std::map and std::unordered_map - and their multimap counterparts.)
	template<typename Container>
	Container insertPairs( const std::string& source, const BlockArray<range>& elements )
	{
		Container result;
		for( size_t i = 0; i < elements.size(); ++i )
		{
			auto& key = elements[ i ];
			auto& value = elements[ ++i ];
			result.insert( std::make_pair( source.substr( key.Begin, key.Size ), source.substr( value.Begin, value.Size ) ) );
		}
		return std::move( result );
	}



	// Determine whether the given character is inside the given character range
	static inline bool hasChar( const char* range_start, const char* range_end, char to_find )
		{ return findChar( range_start, range_end, to_find ) != nullptr; }

	// Find given character inside given character range
	// Returns nullptr if not found
	static inline const char* findChar( const char* range_start, const char* range_end, char to_find )
	{
		for( auto c = range_start; c != range_end; ++c )
			{ if( *c == to_find ) return c; }
		return nullptr;
	}

	// Find given substring inside given character range
	// Returns nullptr if not found
	static inline const char* findSubstr( const char* range_start, const char* range_end, const char* substr, size_t substr_size )
	{
		if( substr_size > static_cast<size_t>( range_end - range_start ) )
			return nullptr;
		auto end = range_end - substr_size;
		for( auto c = range_start; c != end; ++c )
		{
			if( memcmp( c, substr, substr_size ) == 0 )
				return c;
		}
		return nullptr;
	}

private:
	size_t MaxElm{ SIZE_MAX };
	bool IncludeDelim{ false };
};
*/