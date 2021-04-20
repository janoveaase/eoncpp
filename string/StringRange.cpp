/*#include "StringRange.h"


range StringRange::trim( const std::string& source, const range& element )
{
	static const char* whitespace{ " \t\r\n\v" };
	static const char* whitespace_end = whitespace + 5;
	range result = element;
	while( result.Size > 0 )
	{
		if( hasChar( whitespace, whitespace_end, source[ result.Begin ] ) )
		{
			++result.Begin;
			--result.Size;
		}
	}
	while( result.Size > 0 )
	{
		if( hasChar( whitespace, whitespace_end, source[ result.end() - 1 ] ) )
			--result.Size;
	}
	return result;
}
BlockArray<range> StringRange::trim( const std::string& source, const BlockArray<range>& elements )
{
	BlockArray<range> result;
	for( size_t i = 0; i < elements.size(); ++i )
		result.append( trim( source, elements[ i ] ) );
	return std::move( result );
}

BlockArray<range> StringRange::removeEmpty( const BlockArray<range>& elements )
{
	BlockArray<range> result;
	for( size_t i = 0; i < elements.size(); ++i )
	{
		if( !elements[ i ].empty() )
			result.append( elements[ i ] );
	}
	return std::move( result );
}


BlockArray<range> StringRange::reSplit( const std::string& source, const BlockArray<range>& elements, char delimiter )
{
	BlockArray<range> result;
	for( size_t i = 0; i < elements.size(); ++i )
	{
		auto& elm = elements[ i ];
		auto found = findChar( source.c_str() + elm.Begin,
			source.c_str() + elm.end(), delimiter );
		if( found != nullptr )
		{
			auto end1 = found - source.c_str();
			auto beg2 = end1 + 1;
			result.append( range( elm.Begin, end1 - elm.Begin ) );
			result.append( range( beg2, elm.end() - beg2 ) );
		}
		else
		{
			result.append( range( elm ) );
			result.append( range() );
		}
	}
	return std::move( result );
}
BlockArray<range> StringRange::reSplit( const std::string& source, const BlockArray<range>& elements, const char* delimiter )
{
	BlockArray<range> result;
	auto delim_size = strlen( delimiter );
	for( size_t i = 0; i < elements.size(); ++i )
	{
		auto& elm = elements[ i ];
		auto found = findSubstr( source.c_str() + elm.Begin,
			source.c_str() + elm.end(), delimiter, delim_size );
		if( found != nullptr )
		{
			auto end1 = found - source.c_str();
			auto beg2 = end1 + delim_size;
			result.append( range( elm.Begin, end1 - elm.Begin ) );
			result.append( range( beg2, elm.end() - beg2 ) );
		}
		else
		{
			result.append( range( elm ) );
			result.append( range() );
		}
	}
	return std::move( result );
}
BlockArray<range> StringRange::reSplit( const std::string& source, const BlockArray<range>& elements, std::string& delimiter )
{
	BlockArray<range> result;
	for( size_t i = 0; i < elements.size(); ++i )
	{
		auto& elm = elements[ i ];
		auto found = findSubstr( source.c_str() + elm.Begin,
			source.c_str() + elm.end(), delimiter.c_str(), delimiter.size() );
		if( found != nullptr )
		{
			auto end1 = found - source.c_str();
			auto beg2 = end1 + delimiter.size();
			result.append( range( elm.Begin, end1 - elm.Begin ) );
			result.append( range( beg2, elm.end() - beg2 ) );
		}
		else
		{
			result.append( range( elm ) );
			result.append( range() );
		}
	}
	return std::move( result );
}
BlockArray<range> StringRange::reSplit( const std::string& source, const BlockArray<range>& elements, std::regex& delimiter )
{
	BlockArray<range> result;
	for( size_t i = 0; i < elements.size(); ++i )
	{
		auto& elm = elements[ i ];
		std::smatch match;
		if( std::regex_search( source.begin() + elm.Begin, source.begin() + elm.end(), match, delimiter ) )
		{
			auto end1 = match[ 0 ].first - source.begin();
			auto beg2 = match[ 0 ].second - source.begin();
			result.append( range( elm.Begin, end1 - elm.Begin ) );
			result.append( range( beg2, elm.end() - beg2 ) );
		}
		else
		{
			result.append( range( elm ) );
			result.append( range() );
		}
	}
	return std::move( result );
}
*/