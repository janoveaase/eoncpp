#pragma once
#include "NameDefs.h"
#include "String.h"
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
	//
	// Underlying implementation of eon::name handling
	//

	struct hash_name { inline size_t operator()( const string* a ) const noexcept { return a->hash(); } };
	struct eq_name { inline bool operator()( const string* a, const string* b ) const noexcept { return *a == *b; } };
	class NameData
	{
	public:
		NameData() = default;
		~NameData() { std::scoped_lock<std::mutex> lock( Lock ); for( auto elm : Names ) delete elm; }

		inline const string& str( const name_t& name ) { std::scoped_lock<std::mutex> lock( Lock );
			return ( name > no_name && name.value() <= Names.size() ) ? *Names[ name.value() - 1 ] : NullStr; }

		name_t name( string&& str ) noexcept;
		name_t compilerName( string&& str );

		static bool validName( const string& str ) noexcept;

	private:
		std::vector<string*> Names;
		std::unordered_map<const string*, name_t, hash_name, eq_name> Lookup;
		std::mutex Lock;
		static const string NullStr;
	};
}
