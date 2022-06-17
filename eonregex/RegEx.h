#pragma once
#include "Graph.h"
#include "Match.h"
#include "RxData.h"
#include <eonexcept/Exception.h>


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// Eon Regular Expression Class - eon::regex
	//
	// Regular expressions on [eon::string]s.
	//
	class regex
	{
	public:
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//

		// Default constructor for empty expression
		regex() = default;

		// Copy the 'other' expression
		inline regex( const regex& other ) { if( !other.empty() ) *this = other; }

		// Take ownership of the 'other' expression
		inline regex( regex&& other ) noexcept { *this = std::move( other ); }

		// Construct for an [eon::string] 'expression'
		// WARNING: Will throw InvalidExpression if invalid formatting.
		//          The exception message will contain details about what's
		//          wrong with the 'expression' string.
		inline regex( string expression, string flags = string() ) {
			Raw = expression; Flags = flags; Graph.parse( Raw.substr(), Flags.substr() ); }
		inline regex( substring expression, substring flags ) {
			Raw = expression; Flags = flags; Graph.parse( Raw.substr(), Flags.substr() ); }

		// Construct for an std::string 'expression'
		// WARNING: Will throw InvalidExpression if invalid formatting.
		//          The exception message will contain details about what's
		//          wrong with the 'expression' string.
		inline regex( const std::string& expression, std::string flags = std::string() ) {
			Raw = substring( expression ); Flags = substring( flags ); Graph.parse( Raw.substr(), Flags.substr() ); }

		// Construct for a 'const char*' (C-string)
		// WARNING: Will throw InvalidExpression if invalid formatting.
		//          The exception message will contain details about what's
		//          wrong with the 'expression' string.
		inline regex( const char* expression, const char* flags = nullptr ) {
			if( expression ) Raw = substring( expression ); if( flags ) Flags = substring( flags );
			Graph.parse( Raw.substr(), Flags.substr() ); }

		// Construct for a source


		// Default destruction
		virtual ~regex() = default;




		///////////////////////////////////////////////////////////////////////
		//
		// Modifier Methods
		//

		// Copy the 'other' expression
		inline regex& operator=( const regex& other ) {
			Graph = other.Graph; Raw = other.Raw; Flags = other.Flags; return *this; }

		// Take ownership of the details of the 'other' expression
		inline regex& operator=( regex&& other ) noexcept {
			Graph = std::move( other.Graph ); Raw = std::move( other.Raw ); Flags = std::move( other.Flags );
			return *this; }


		// Clear this expression
		inline void clear() noexcept { Graph.clear(); Raw.clear(); Flags.clear(); }
		



		///////////////////////////////////////////////////////////////////////
		//
		// Read-only Methods
		//

		// Check if there is an expression
		inline bool empty() const noexcept { return Raw.empty(); }

		// Get the raw pattern
		inline const string& str() const noexcept { return Raw; }

		// Get the flags
		inline const string& flags() const noexcept { return Flags; }

		// Get the complete (optimized) pattern
		inline string strStruct() const { return Graph.strStruct(); }




		///////////////////////////////////////////////////////////////////////
		//
		// Matching and Searching
		//

		// Match against a specified [eon::substring] 'str'
		// Returns an [eon::rx::match] object that is either 'true' if there
		// were a match or 'false' if not. If 'true', then use
		// [eon::rx::match::all()] to access the entire match (as an
		// [eon::substring]) or [eon::rx::match::group(name)] to get a
		// specific capture.
		// NOTE: The entire match is a group named "complete", so never
		//       specify a group with that name!
		rx::match match( const substring& str ) const;
		inline rx::match match( const string& str ) const { return match( str.substr() ); }
		inline rx::match match( const std::string& str ) const { return match( substring( str ) ); }
		inline rx::match match( const char* str ) const { return match( substring( str ) ); }

		// Find the first section of the [eon::substring] that matches
		// Returns an [eon::rx::match] object that is either 'true' if there
		// were a match or 'false' if not. If 'true', then use
		// [eon::rx::match::all()] to access the entire match (as an
		// [eon::substring]) or [eon::rx::match::group(name)] to get a
		// specific capture.
		rx::match findFirst( const substring& str ) const;
		inline rx::match findFirst( const string& str ) const { return match( str.substr() ); }
		inline rx::match findFirst( const std::string& str ) const { return match( substring( str ) ); }
		inline rx::match findFirst( const char* str ) const { return match( substring( str ) ); }

		// Find the last section of the [eon::substring that matches]
		// Returns an [eon::rx::match] object that is either 'true' if there
		// were a match or 'false' if not. If 'true', then use
		// [eon::rx::match::all()] to access the entire match (as an
		// [eon::substring]) or [eon::rx::match::group(name)] to get a
		// specific capture.
		rx::match findLast( const substring& str ) const;
		inline rx::match findLast( const string& str ) const { return match( str.substr() ); }
		inline rx::match findLast( const std::string& str ) const { return match( substring( str ) ); }
		inline rx::match findLast( const char* str ) const { return match( substring( str ) ); }



	private:
		rx::Graph Graph;
		string Raw, Flags;
		int Marker{ 0 };
	};
}