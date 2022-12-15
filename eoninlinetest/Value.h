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
	private:
		enum class Type
		{
			alphanum,
			sint,
			uint,
			flt
		};

	public:

		// Must be constructed with a value.
		__Value() = delete;

		inline explicit __Value( bool value, const __TestOptions& options = __TestOptions() ) { Value = value; }
		inline explicit __Value( eon::byte_t value, const __TestOptions& options = __TestOptions() ) { Value = value; }
		inline explicit __Value( eon::char_t value, const __TestOptions& options = __TestOptions() ) { Value = value; }
		inline __Value( int value, const __TestOptions& options = __TestOptions() ) {
			Value = std::to_string( value ); VType = Type::sint; }
		inline __Value( double value, const __TestOptions& options = __TestOptions() ) {
			Value = eon::string( std::to_string( value ) ).roundNumber( options.roundOff() ); VType = Type::flt; }
		inline __Value( eon::index_t value, const __TestOptions& options = __TestOptions() ) {
			Value = std::to_string( value ); VType = Type::uint; }
		inline __Value( int64_t value, const __TestOptions& options = __TestOptions() ) {
			Value = std::to_string( value ); VType = Type::sint; }
		inline explicit __Value( const char* value, const __TestOptions& options = __TestOptions() ) { Value = value; }
		inline explicit __Value( const std::string& value, const __TestOptions& options = __TestOptions() ) { Value = value; }
		inline __Value( eon::string value, const __TestOptions& options = __TestOptions() ) { Value = std::move( value ); }



		// Comparison.
		int compare( const __Value& other ) const noexcept
		{
			if( VType == other.VType )
			{
				// Both have same type so do specific compare!
				switch( VType )
				{
					case Type::sint:
						return _compareSInt( other );
					case Type::uint:
						return _compareUInt( other );
					case Type::flt:
						return _compareFlt( other );
					default:
						return _compareAlpha( other );
				}
			}
			else if( VType == Type::alphanum || other.VType == Type::alphanum )
			{
				// One is alphanumerical so do alphanumerical compare!
				return _compareAlpha( other );
			}
			else if( VType == Type::flt || other.VType == Type::flt )
			{
				// One is a float so do float compare!
				return _compareFlt( other );
			}
			else
			{
				// One is an unsigned integer, the other must be a signed integer!
				if( Value.startsWith( "-" ) )
					return -1;
				else if( other.Value.startsWith( "-" ) )
					return 1;
				else
					return _compareSInt( other );
			}
		}
		inline int _compareSInt( const __Value& other ) const noexcept {
			auto v1 = Value.toInt64(), v2 = other.Value.toInt64(); return v1 < v2 ? -1 : v1 == v2 ? 0 : 1; }
		inline int _compareUInt( const __Value& other ) const noexcept {
			auto v1 = Value.toUInt64(), v2 = other.Value.toUInt64(); return v1 < v2 ? -1 : v1 == v2 ? 0 : 1; }
		inline int _compareFlt( const __Value& other ) const noexcept {
			auto v1 = Value.toDouble(), v2 = other.Value.toDouble(); return v1 < v2 ? -1 : v1 == v2 ? 0 : 1; }
		inline int _compareAlpha( const __Value& other ) const noexcept { return Value.compare( other.Value ); }

		inline bool operator<( const __Value& other ) const noexcept { return compare( other ) < 0; }
		inline bool operator<=( const __Value& other ) const noexcept { return compare( other ) <= 0; }
		inline bool operator>( const __Value& other ) const noexcept { return compare( other ) > 0; }
		inline bool operator>=( const __Value& other ) const noexcept { return compare( other ) >= 0; }
		inline bool operator==( const __Value& other ) const noexcept { return compare( other ) == 0; }
		inline bool operator!=( const __Value& other ) const noexcept { return compare( other ) != 0; }

	public:
		eon::string Value;
		Type VType{ Type::alphanum };
	};
}
