#include "String.h"
#include <eoninlinetest/InlineTest.h>
#include <cctype>
#include <regex>
#include <unordered_map>


namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// Modifier Methods
	//

	EON_TEST_3STEP( string, clear, basic,
		string obj( "abcdef" ),
		obj.clear(),
		EON_EQ( "", obj ) );

	EON_NO_TEST( string, reserve );		// Cannot predict what std::string will do here!

	EON_TEST_2STEP( string, operator_lsh, string,
		string obj = "abc",
		EON_EQ( "abcdef", obj << string( "def" ) ) );
	EON_TEST_2STEP( string, operator_lsh, substring,
		string obj = "abc",
		EON_EQ( "abcdef", obj << substring( "def" ) ) );
	EON_TEST_2STEP( string, operator_lsh, stdstring,
		string obj = "abc",
		EON_EQ( "abcdef", obj << std::string( "def" ) ) );
	EON_TEST_2STEP( string, operator_lsh, cstring,
		string obj = "abc",
		EON_EQ( "abcdef", obj << "def" ) );
	EON_TEST_2STEP( string, operator_lsh, template,
		string obj = "abc",
		EON_EQ( "abc99", obj << static_cast<index_t>( 99 ) ) );

	EON_TEST_2STEP( string, operator_plusasgn, string,
		string obj = "abc",
		EON_EQ( "abcdef", obj += string( "def" ) ) );

	string& string::operator+=( const substring& input )
	{
		if( input.validUTF8() )
		{
			Bytes.append( input.begin().byteData(), input.numBytes() );
			NumChars += input.numChars();
			return *this;
		}
		else
			return *this += string( input );
	}
	EON_TEST_2STEP( string, substring_plusasgn, empty_to_empty,
		string obj,
		EON_EQ( string(), obj += substring( "" ) ) );
	EON_TEST_2STEP( string, substring_plusasgn, something_to_empty,
		string obj,
		EON_EQ( string( "ab" ), obj += substring( "ab" ) ) );
	EON_TEST_2STEP( string, substring_plusasgn, empty_to_something,
		string obj( "ab" ),
		EON_EQ( string( "ab" ), obj += substring( "" ) ) );
	EON_TEST_2STEP( string, substring_plusasgn, something_to_something,
		string obj( "ab" ),
		EON_EQ( string( "abcd" ), obj += substring( "cd" ) ) );

	EON_TEST_2STEP( string, operator_plusasgn, stdstring,
		string obj = "abc",
		EON_EQ( "abcdef", obj += std::string( "def" ) ) );

	EON_TEST_2STEP( string, operator_plusasgn, cstring,
		string obj = "abc",
		EON_EQ( "abcdef", obj += "def" ) );

	EON_TEST_2STEP( string, operator_plusasgn, char_t,
		string obj = "abc",
		EON_EQ( "abcd", obj += char_t( 'd' ) ) );

	EON_TEST_2STEP( string, operator_plusasgn, char,
		string obj = "abc",
		EON_EQ( "abcd", obj += 'd' ) );

	EON_TEST_2STEP( string, operator_plusasgn, unsigned_char,
		string obj = "abc",
		EON_EQ( "abcd", obj += (unsigned char)'d' ) );

	string::iterator string::insert( const iterator& pos, const string& substr )
	{
		pos.assertSameSource( Bytes.c_str() );
		if( !pos )
		{
			*this += substr;
			return iterator( Bytes.c_str(), Bytes.size(), NumChars, Bytes.c_str() + pos.numByte() );
		}
		Bytes.insert( pos.numByte(), substr.Bytes );
		NumChars += substr.NumChars;
		return iterator( Bytes.c_str(), Bytes.size(), NumChars, Bytes.c_str() + pos.numByte() );
	}
	EON_TEST_3STEP( string, insert, nothing_to_empty,
		string obj,
		obj.insert( obj.begin(), "" ),
		EON_EQ( string(), obj ) );
	EON_TEST_3STEP( string, insert, something_to_empty,
		string obj,
		obj.insert( obj.begin(), "ab" ),
		EON_EQ( string( "ab" ), obj ) );
	EON_TEST_3STEP( string, insert, nothing_to_something,
		string obj( "ab" ),
		obj.insert( obj.begin(), "" ),
		EON_EQ( string( "ab" ), obj ) );
	EON_TEST_3STEP( string, insert, something_to_something,
		string obj( "ab" ),
		obj.insert( obj.begin() + 1, "cd" ),
		EON_EQ( string( "acdb" ), obj ) );

	EON_TEST_3STEP( string, insert, char_t,
		string obj( "abde" ),
		obj.insert( obj.begin() + 2, char_t( 'c' ) ),
		EON_EQ( "abcde", obj ) );

	string& string::erase( const substring& sub ) noexcept
	{
		if( sub.empty() )
			return *this;
		auto area = sub.lowToHigh();
		if( area.begin().numByte() >= Bytes.size() )
			return *this;
		if( area.begin().numByte() + area.numBytes() > Bytes.size() )
			area.end() = end();
		Bytes.erase( area.begin().numByte(), area.numBytes() );
		NumChars -= area.numChars();
		return *this;
	}
	EON_TEST_2STEP( string, erase, nothing_from_nothing,
		string obj,
		EON_EQ( string(), obj.erase( substring() ) ) );
	EON_TEST_2STEP( string, erase, nothing_from_something,
		string obj( "abcd" ),
		EON_EQ( string( "abcd" ), obj.erase( substring() ) ) );
	EON_TEST_2STEP( string, erase, something_from_something,
		string obj( "abcd" ),
		EON_EQ( string( "ad" ), obj.erase( obj.substr( obj.begin() + 1, obj.end() - 1) ) ) );
}
