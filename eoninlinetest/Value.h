#pragma once

#include <eonstring/Name.h>
#include <eonstring/String.h>
#include "TestOptions.h"



///////////////////////////////////////////////////////////////////////////////
//
// The 'eonitest' namespace encloses all public inline test functionality
//
namespace eonitest
{
	///////////////////////////////////////////////////////////////////////////
	//
	// The __Value class accepts a value of some kind and converts it into a
	// std::string, which can be used for comparison/testing.
	//
	class __Value
	{
	public:

		// Must be constructed with a value.
		__Value() = delete;

		inline explicit __Value( bool value, const __TestOptions& options = __TestOptions() ) { Value = value; }
		inline explicit __Value( eon::byte_t value, const __TestOptions& options = __TestOptions() ) { Value = value; }
		inline explicit __Value( eon::char_t value, const __TestOptions& options = __TestOptions() ) { Value = value; }
		inline __Value( int value, const __TestOptions& options = __TestOptions() ) { Value = std::to_string( value ); }
		inline __Value( double value, const __TestOptions& options = __TestOptions() ) {
			Value = eon::string( std::to_string( value ) ).roundNumber( options.roundOff() ); }
		inline __Value( eon::index_t value, const __TestOptions& options = __TestOptions() ) { Value = std::to_string( value ); }
		inline __Value( int64_t value, const __TestOptions& options = __TestOptions() ) { Value = std::to_string( value ); }
		inline explicit __Value( const char* value, const __TestOptions& options = __TestOptions() ) { Value = value; }
		inline explicit __Value( const std::string& value, const __TestOptions& options = __TestOptions() ) { Value = value; }
		inline __Value( eon::string value, const __TestOptions& options = __TestOptions() ) { Value = std::move( value ); }



		// Comparison.
		inline int compare( const __Value& other ) const noexcept { return Value.compare( other.Value ); }
		inline bool operator<( const __Value& other ) const noexcept { return compare( other ) < 0; }
		inline bool operator<=( const __Value& other ) const noexcept { return compare( other ) <= 0; }
		inline bool operator>( const __Value& other ) const noexcept { return compare( other ) > 0; }
		inline bool operator>=( const __Value& other ) const noexcept { return compare( other ) >= 0; }
		inline bool operator==( const __Value& other ) const noexcept { return compare( other ) == 0; }
		inline bool operator!=( const __Value& other ) const noexcept { return compare( other ) != 0; }

	public:
		eon::string Value;
	};
}
