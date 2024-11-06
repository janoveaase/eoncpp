#pragma once
#include "NameDefs.h"
#include "String.h"
#include "DefaultNames.h"
#include "NameData.h"
#include <set>
#include <mutex>
#include <unordered_map>


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	// Eon defines 'a name' as any sequence of letters (all [eon::charcat]
	// letter categories), numbers (all [eon::charcat] number categories) and
	// underscore. The only limitation is that the name cannot consist entirely
	// of numbers - there must be at least one letter or underscore. A name can
	// start with a number, though.
	//
	// The 'eon::name_t' type is at core just a 32 bit integer, refering to a
	// string inside an array, making it easy to "convert" name to string.
	//
	// Every unique name is guaranteed to have the same integer value, and so
	// any two names can be compared equal/not equal as a basic integer compare.
	//
	// WARNING: While comparing equal/not equal is both fast and trivial,
	//          comparing less than/greater than alpha-numerically requires
	//          string conversion and [eon::string::compare] to be involved!
	//          This is significantly more costly!
	//
	// Where alpha-numerical comparison is not required, fast integer comparsion
	// can be used for added speed.
	//
	// Uses: As keys in 'std::map', 'std::multimap', 'std::unordered_map',
	//       'std::unordered_multimap', as quick references, etc.
	//
	// NOTE: Converting to and from string is thread-safe!
	//
	// WARNING: While the [eon::string] value of a name will be the same across
	//          processes, the integer value will not! Do not use the integer
	//          value when streaming/saving etc.!
	//
	// NOTE: There is a small cost every time you create a new 'name' from a
	// 'string' since the name has to be looked up and/or inserted into a
	// data structure.
	//




	///////////////////////////////////////////////////////////////////////////
	//
	// Convert between [eon::name_t] and [eon::stringt]
	//

	// Convert name into string
	// Will return empty string if not a valid name
	const string& str( const name_t& name );

	// Convert string into name
	// Will return [eon::no_name] if the string is not valid as name
	name_t name( string&& str ) noexcept;
	inline name_t name( const string& str ) { return name( string( str ) ); }
	inline name_t name( const std::string& str ) { return name( string( str ) ); }
	inline name_t name( const char* str ) { return name( string( substring( str ) ) ); }

	// Check if string is valid as name
	inline bool validName( const string& str ) noexcept { return NameData::validName( str ); }

	// Convert string into name for compiler use
	// Such names can contain non-name characters
	name_t compilerName( string&& str );

	// Convert string into name for compiler use
	// Such names can contain non-name characters
	inline name_t compilerName( const char* str ) { return compilerName( string( str ) ); }

#ifdef _DEBUG
	static std::unique_ptr<NameData> Data;
#endif
}
