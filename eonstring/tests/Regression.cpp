#include "Regression.h"
#include <set>
#include <cfloat>
#include <fstream>



namespace eon
{
	TEST( Substring, order )
	{
		eon::string str{ "12345" };
		auto sub = str.substr();
		REQUIRE_EQ( "12345", sub.stdstr() );
		auto rsub = sub.highToLow();
		REQUIRE_EQ( "54321", rsub.stdstr() ) << "Failed!";
		auto rrsub = rsub.lowToHigh();
		REQUIRE_EQ( "12345", rrsub.stdstr() );
	}
	TEST( Substring, unicode )
	{
		const char* str{ "β" };
		auto sub = eon::substring( str );
		WANT_EQ( 0, sub.begin().numChar() );
		WANT_EQ( 1, sub.end() - sub.begin() );
		WANT_EQ( 2, sub.end().numByte() - sub.begin().numByte() );
	}




	TEST( StaticString, toString )
	{
		WANT_EQ( "1234", eon::string::toString( 1234 ) );
		WANT_EQ( "-1.23", eon::string::toString( -1.230 ) );
	}
	TEST( StaticString, isLetter )
	{
		WANT_FALSE( eon::string::isLetter( '_' ) );
	}

	TEST( String, invalid_utf8 )
	{
		std::string invalid{ "ab\xB6" };
		eon::string valid( invalid.c_str(), invalid.size(), "X" );
		WANT_EQ( "abX", valid );
	}

	TEST( String, find )
	{
		eon::string str{ "123454321" };
		auto first = str.findFirst( '3' );
		auto last = str.findLast( '3' );
		WANT_EQ( 2, first.begin().numChar() ) << "Wrong first";
		WANT_EQ( 6, last.begin().numChar() ) << "Wrong last";
	}
	TEST( String, find_in_sub )
	{
		eon::string str{ "one two three four five six seven eight nine" };
		auto sub1 = str.substr( str.begin() + 4, str.begin() + 23 );
		auto sub2 = str.substr( str.begin() + 14 );
		auto bad = sub1.findFirst( eon::string( "six" ).substr() );
		auto good = sub2.findFirst( eon::string( "six" ).substr() );
		WANT_FALSE( static_cast<bool>( bad ) );
		WANT_TRUE( static_cast<bool>( good ) );
	}
	TEST( String, split )
	{
		eon::string str{ "alpha beta gamma delta" };
		auto elms = str.splitSequential<std::list<eon::substring>>( " " );
		WANT_EQ( 4, elms.size() );
		std::vector<eon::string> exp{ "alpha", "beta", "gamma", "delta" };
		auto a = elms.begin();
		auto e = exp.begin();
		for( ; a != elms.end(); ++a, ++e )
		{
			WANT_EQ( *e, eon::string( *a ) );
		}
	}
	TEST( String, join )
	{
		std::set<eon::string> elms{ "alpha", "beta", "gamma", "delta" };
		auto act = eon::string( "--" ).join( elms.begin(), elms.end() );
		auto exp = "alpha--beta--delta--gamma";
		WANT_EQ( exp, act );
	}


	TEST( String, move )
	{
		eon::string str{ "john doe" };
		str = eon::string( std::move( str ) ).ucWords();
		WANT_EQ( "John Doe", str );
	}
	TEST( String, bool )
	{
		eon::string ok( true );
		eon::string bad( false );
		WANT_EQ( "true", ok );
		WANT_EQ( "false", bad );

		ok = false;
		bad = true;
		WANT_EQ( "false", ok );
		WANT_EQ( "true", bad );
	}

	TEST( String, char )
	{
		eon::string a{ 'a' };
		eon::string b{ 'B' };
		WANT_EQ( "a", a );
		WANT_EQ( "B", b );

		a = 'B';
		b = 'a';
		WANT_EQ( "B", a );
		WANT_EQ( "a", b );
	}

	TEST( String, numeralsOnly )
	{
		eon::string rpos( "123456" );
		eon::string rneg( "123456" );
		eon::string rlead( "000123456" );
		eon::string rtrail( "123456000" );
		eon::string rltpos( "0012345600" );
		eon::string rltneg( "0012345600" );
		eon::string ipos( "123" );
		eon::string ineg( "123" );
		eon::string invalid1( "1.2.3" );
		eon::string invalid2( "123a456" );

		WANT_TRUE( rpos.numeralsOnly() ) << "Failed: rpos";
		WANT_TRUE( rneg.numeralsOnly() ) << "Failed: rneg";
		WANT_TRUE( rlead.numeralsOnly() ) << "Failed: rlead";
		WANT_TRUE( rtrail.numeralsOnly() ) << "Failed: rtrail";
		WANT_TRUE( rltpos.numeralsOnly() ) << "Failed: rltpos";
		WANT_TRUE( rltneg.numeralsOnly() ) << "Failed: rltneg";
		WANT_TRUE( ipos.numeralsOnly() ) << "Failed: ipos";
		WANT_TRUE( ineg.numeralsOnly() ) << "Failed: ineg";
		WANT_FALSE( invalid1.numeralsOnly() ) << "Failed: invalid1";
		WANT_FALSE( invalid2.numeralsOnly() ) << "Failed: invalid2";
	}
	TEST( String, isFloat )
	{
		eon::string rpos( "123.456" );
		eon::string rneg( "-123.456" );
		eon::string rlead( "000123.456" );
		eon::string rtrail( "123.456000" );
		eon::string rltpos( "+00123.45600" );
		eon::string rltneg( "-00123.45600" );
		eon::string ipos( "123" );
		eon::string ineg( "-123" );
		eon::string invalid1( "1.2.3" );
		eon::string invalid2( "123a456" );
		eon::string sub( "12.3X-4.56" );

		WANT_TRUE( rpos.isFloat() ) << "Failed: rpos";
		WANT_TRUE( rneg.isFloat() ) << "Failed: rneg";
		WANT_TRUE( rlead.isFloat() ) << "Failed: rlead";
		WANT_TRUE( rtrail.isFloat() ) << "Failed: rtrail";
		WANT_TRUE( rltpos.isFloat() ) << "Failed: rltpos";
		WANT_TRUE( rltneg.isFloat() ) << "Failed: rltneg";
		WANT_FALSE( ipos.isFloat() ) << "Failed: ipos";
		WANT_FALSE( ineg.isFloat() ) << "Failed: ineg";
		WANT_FALSE( invalid1.isFloat() ) << "Failed: invalid1";
		WANT_FALSE( invalid2.isFloat() ) << "Failed: invalid2";
		WANT_TRUE( eon::substring( sub.begin(), sub.begin() + 4 ).isFloat() ) << "Failed: sub #1";
		WANT_TRUE( eon::substring( sub.begin() + 5, sub.end() ).isFloat() ) << "Failed: sub #2";
	}
	TEST( String, isInt )
	{
		eon::string rpos( "123.456" );
		eon::string rneg( "-123.456" );
		eon::string rlead( "000123.456" );
		eon::string rtrail( "123.456000" );
		eon::string rltpos( "+00123.45600" );
		eon::string rltneg( "-00123.45600" );
		eon::string ipos( "+123" );
		eon::string ineg( "-123" );
		eon::string invalid1( "1.2.3" );
		eon::string invalid2( "123a456" );
		eon::string sub( "+123X-456" );

		WANT_FALSE( rpos.isInt() ) << "Failed: rpos";
		WANT_FALSE( rneg.isInt() ) << "Failed: rneg";
		WANT_FALSE( rlead.isInt() ) << "Failed: rlead";
		WANT_FALSE( rtrail.isInt() ) << "Failed: rtrail";
		WANT_FALSE( rltpos.isInt() ) << "Failed: rltpos";
		WANT_FALSE( rltneg.isInt() ) << "Failed: rltneg";
		WANT_TRUE( ipos.isInt() ) << "Failed: ipos";
		WANT_TRUE( ineg.isInt() ) << "Failed: ineg";
		WANT_FALSE( invalid1.isInt() ) << "Failed: invalid1";
		WANT_FALSE( invalid2.isInt() ) << "Failed: invalid2";
		WANT_TRUE( eon::substring( sub.begin(), sub.begin() + 4 ).isInt() ) << "Failed: sub #1";
		WANT_TRUE( eon::substring( sub.begin() + 5, sub.end() ).isInt() ) << "Failed: sub #2";
	}
	TEST( String, isUInt )
	{
		eon::string rpos( "123.456" );
		eon::string rneg( "-123.456" );
		eon::string rlead( "000123.456" );
		eon::string rtrail( "123.456000" );
		eon::string rltpos( "+00123.45600" );
		eon::string rltneg( "-00123.45600" );
		eon::string ipos( "123" );
		eon::string ineg( "-123" );
		eon::string invalid1( "1.2.3" );
		eon::string invalid2( "123a456" );
		eon::string sub( "123X-456" );

		WANT_FALSE( rpos.isUInt() ) << "Failed: rpos";
		WANT_FALSE( rneg.isUInt() ) << "Failed: rneg";
		WANT_FALSE( rlead.isUInt() ) << "Failed: rlead";
		WANT_FALSE( rtrail.isUInt() ) << "Failed: rtrail";
		WANT_FALSE( rltpos.isUInt() ) << "Failed: rltpos";
		WANT_FALSE( rltneg.isUInt() ) << "Failed: rltneg";
		WANT_TRUE( ipos.isUInt() ) << "Failed: ipos";
		WANT_FALSE( ineg.isUInt() ) << "Failed: ineg";
		WANT_FALSE( invalid1.isUInt() ) << "Failed: invalid1";
		WANT_FALSE( invalid2.isUInt() ) << "Failed: invalid2";
		WANT_TRUE( eon::substring( sub.begin(), sub.begin() + 3 ).isUInt() ) << "Failed: sub #1";
		WANT_TRUE( eon::substring( sub.begin() + 5, sub.end() ).isUInt() ) << "Failed: sub #2";
		WANT_FALSE( eon::substring( sub.begin() + 4, sub.end() ).isUInt() ) << "Failed: sub #3";
	}

	TEST( String, toNumber )
	{
		int32_t i32 = -1234;
		int64_t i64 = -123456789012LL;
		uint32_t ui32 = 1234567890;
		uint64_t ui64 = 1234567890123456LL;
		double dbl = -1234.5;
		WANT_EQ( i32, eon::string( "-1234" ).toInt32() ) << "Wrong int32_t";
		WANT_EQ( ui32, eon::string( "1234567890" ).toUInt32() ) << "Wrong uint32_t";
		WANT_EQ( i64, eon::string( "-123456789012" ).toInt64() ) << "Wrong int64_t";
		WANT_EQ( ui64, eon::string( "1234567890123456" ).toUInt64() ) << "Wrong uint64_t";
		WANT_EQ( dbl, eon::string( "-1234.5" ).toDouble() ) << "Wrong double";
	}

	TEST( String, from_number )
	{
		int32_t i32 = -1234;
		int64_t i64 = -123456789012LL;
		uint32_t ui32 = 1234567890;
		uint64_t ui64 = 1234567890123456LL;
		size_t sz = 9988;
		double dbl = -1234.5;
		WANT_EQ( i32, eon::string( static_cast<int32_t>( i32 ) ).toInt32() ) << "Wrong int32_t";
		WANT_EQ( ui32, eon::string( static_cast<uint32_t>( ui32 ) ).toUInt32() ) << "Wrong uint32_t";
		WANT_EQ( i64, eon::string( i64 ).toInt64() ) << "Wrong int64_t";
		WANT_EQ( ui64, eon::string( ui64 ).toUInt64() ) << "Wrong uint64_t";
		WANT_EQ( dbl, eon::string( dbl ).toDouble() ) << "Wrong double";
		WANT_EQ( sz, eon::string( static_cast<size_t>( sz ) ).toUInt32() ) << "Wrong size_t";
	}


	TEST( String, trim )
	{
		eon::string str;

		str = "no trim";
		WANT_EQ( "no trim", str.trim().stdstr() ) << "Clean string was trimmed";
		str = " \t\nleft trim";
		WANT_EQ( "left trim", str.trim().stdstr() ) << "Left string was not trimmed";
		str = " \t\nleft trim";
		WANT_EQ( "left trim", str.trimLeading().stdstr() ) << "Left string was not left trimmed";
		str = " \t\nleft trim";
		WANT_EQ( " \t\nleft trim", str.trimTrailing().stdstr() ) << "Left string was right trimmed";
		str = "right trim\b ";
		WANT_EQ( "right trim", str.trim().stdstr() ) << "Right string was not trimmed";
		str = "right trim\b ";
		WANT_EQ( "right trim\b ", str.trimLeading().stdstr() ) << "Right string was left trimmed";
		str = "right trim\b ";
		WANT_EQ( "right trim", str.trimTrailing().stdstr() ) << "Right string was not right trimmed";
		str = " left and right trim ";
		WANT_EQ( "left and right trim", str.trim().stdstr() ) << "Both string was not trimmed";
		str = "\n";
		WANT_EQ( "", str.trim().stdstr() ) << "Whitespace string was not trimmed";
	}

	TEST( String, trimNumber )
	{
		eon::string basic( "123.456" );
		eon::string leading( "000123.456" );
		eon::string trailing( "123.456000" );
		eon::string positive( "+00123.45600" );
		eon::string negative( "-00123.45600" );
		eon::string shortflt( "1.9" );
		eon::string allzero1( "0000" );
		eon::string allzero2( "0.0000" );
		eon::string invalid( "1.2.3" );

		auto str = eon::string( basic.trimNumber() );
		WANT_EQ( "123.456", str ) << "Wrong basic";
		str = leading.trimNumber();
		WANT_EQ( "123.456", str ) << "Wrong leading";
		str = trailing.trimNumber();
		WANT_EQ( "123.456", str ) << "Wrong trailing";
		str = positive.trimNumber();
		WANT_EQ( "+00123.456", str ) << "Wrong positive";
		str = negative.trimNumber();
		WANT_EQ( "-00123.456", str ) << "Wrong negative";
		str = shortflt.trimNumber();
		WANT_EQ( "1.9", str ) << "Wrong shortflt";
		str = allzero1.trimNumber();
		WANT_EQ( "0", str ) << "Wrong allzero1";
		str = allzero2.trimNumber();
		WANT_EQ( "0", str ) << "Wrong allzero2";
		str = invalid.trimNumber();
		WANT_EQ( "1.2.3", str ) << "Invalid number string was trimmed";
	}

	TEST( String, separateThousands )
	{
		eon::string trivial( "12" );
		eon::string basic( "123.456" );
		eon::string medium( "123456.78" );
		eon::string large( "12345678901.2345" );

		auto str = trivial.separateThousands();
		WANT_EQ( "12", str ) << "Wrong trivial";
		str = basic.separateThousands();
		WANT_EQ( "123.456", str ) << "Wrong basic";
		str = medium.separateThousands();
		WANT_EQ( "123,456.78", str ) << "Wrong medium";
		str = large.separateThousands( ' ' );
		WANT_EQ( "12 345 678 901.2345", str ) << "Wrong large";
	}

	TEST( String, roundNumber )
	{
		eon::string num, str;

		num = "123";
		str = num.roundNumber();
		WANT_EQ( "123", str );
		num = "123";
		str = num.roundNumber( 2 );
		WANT_EQ( "123", str );

		num = "123.456";
		str = num.roundNumber();
		WANT_EQ( "123", str );
		num = "123.543";
		str = num.roundNumber();
		WANT_EQ( "124", str );
		num = "123.456";
		str = num.roundNumber( 2 );
		WANT_EQ( "123.46", str );
		num = "0.456";
		str = num.roundNumber( 2 );
		WANT_EQ( "0.46", str );
	}

	TEST( String, lower )
	{
		eon::string str{ "Hello WORLD and hello OpeniT!" };
		WANT_EQ( "hello world and hello openit!", str.lower() ) << "Wrong lower";
		str = "HELLO WORLD AND HELLO OPENIT!";
		WANT_EQ( "Hello world and hello openiT!", str.lower( eon::substring( str.begin() + 1, str.end() - 2 ) ) )
			<< "Wrong lower limited";
	}
	TEST( String, upper )
	{
		eon::string str;
		str = "hello WORLD and hello Openit!";
		WANT_EQ( "HELLO WORLD AND HELLO OPENIT!", str.upper() ) << "Wrong upper";
		str = "hello WORLD and hello Openit!";
		WANT_EQ( "hELLO WORLD AND HELLO OPENIt!", str.upper( eon::substring( str.begin() + 1, str.end() - 2 ) ) )
			<< "Wrong upper limited";
	}
	TEST( String, ucFirst )
	{
		eon::string str;
		str = "hello WORLD and hello OpeniT! this is a test. TEST ENDS.";
		WANT_EQ( "Hello WORLD and hello OpeniT! this is a test. TEST ENDS.", str.ucFirst() );
		WANT_EQ( "hello WORLD and hello OpeniT! This is a test. TEST ENDS.",
			str.ucFirst( eon::substring( str.begin() + 30, str.end() ) ) );
	}
	TEST( String, ucWords )
	{
		eon::string str;
		str = "hello WORLD and hello OpeniT! this is a test. TEST ENDS.";
		WANT_EQ( "Hello WORLD And Hello OpeniT! This Is A Test. TEST ENDS.", str.ucWords() );
		WANT_EQ( "hello WORLD and hello OpeniT! This Is A Test. TEST ENDS.",
			str.ucWords( eon::substring( str.begin() + 30, str.end() ) ) );
	}
	TEST( String, ucSentences )
	{
		eon::string str;
		str = "hello.WORLD and hello OpeniT! this is a test.  tEST ENDS.";
		WANT_EQ( "Hello.WORLD and hello OpeniT! This is a test.  TEST ENDS.", str.ucSentences() );
		WANT_EQ( "hello.WORLD and hello OpeniT! This is a test.  TEST ENDS.",
			str.ucSentences( eon::substring( str.begin() + 30, str.end() ) ) );
		WANT_EQ( "Hello.WORLD and hello OpeniT! this is a test.  tEST ENDS.",
			str.ucSentences( eon::substring( str.begin(), str.begin() + 30 ) ) );
	}

	TEST( String, substr )
	{
		eon::string str{ "Hello World!" };
		WANT_EQ( "ello World", eon::string( str.substr( str.begin() + 1, str.last() ) ) );
	}
	TEST( String, slice )
	{
		eon::string str{ "Hello World!" };
		WANT_EQ( "ello World", eon::string( str.slice( 1, -2 ) ) );
		WANT_EQ( "olle", eon::string( str.slice( 4, 1 ) ) );
	}

	TEST( String, count )
	{
		eon::string str{ "abacad" };
		WANT_EQ( 3, str.count( char_t( 'a' ) ) ) << "Wrong char count";
		WANT_EQ( 1, str.count( char_t( 'a' ), eon::substring( str.begin() + 1, str.begin() + 3 ) ) )
			<< "Wrong limited char count";

		str = "alpha beta alpha gamma alpha delta";
		WANT_EQ( 3, str.count( "alpha" ) ) << "Wrong string count";
		WANT_EQ( 1, str.count( "alpha", eon::substring( str.begin() + 1, str.begin() + 27 ) ) )
			<< "Wrong limited string count";
	}

	TEST( String, hash )
	{
		std::string str( "Hello World!" );
		size_t expected = sizeof( size_t ) == 4 ? 2984921202 : 10092224619179044402llu;
		WANT_EQ( expected, eon::string( str ).hash() ) << "Wrong hash value";
	}
	TEST( String, hash_speed )
	{
		std::vector<std::string> s_strings{
			"alpha", "beta", "gamma", "delta", "epsilon", "zeta",
			"eta", "theta", "iota", "kappa", "lambda", "mu", "nu",
			"xi", "omicron", "pi", "rho", "sigma", "tau", "upsilon",
			"phi", "chi", "psi", "omega",
			"Norway", "United States of America", "France", "United Kingdom",
			"Sweeden", "Denmark", "Italy", "Iceland", "Germany", "Australia",
			"Philippines", "Peru", "Puerto Rico", "North Macedonia",
			"New Caledonia", "Republic of Korea", "Romania", "Saint Helena",
			"Ascension Island", "Saint Kitts and Nevis", "Sudan",
			"Switzerland", "Trinidad and Tobago", "Uruguay", "Uzbekistan",
			"Zambia", "Zimbabwe", "Western Sahara", "Djibouti",
			"Dominican Republic", "Egypt",
			"The quick brown fox jumps over the lazy dog"
		};
		std::vector<string> e_strings;
		for( auto& str : s_strings )
			e_strings.push_back( string( str ) );

		size_t rounds = 1000000;
#ifdef _DEBUG
		rounds /= 10;
#endif
		size_t e_sum{ 0 };
		std::hash<string> e_hasher;
		std::chrono::steady_clock clock;
		auto e_start = clock.now();
		for( size_t i = 0; i < rounds; ++i )
		{
			for( auto& e_str : e_strings )
				e_sum += e_hasher( e_str );
		}
		auto e_end = clock.now();

		size_t s_sum{ 0 };
		std::hash<std::string> s_hasher;
		auto s_start = clock.now();
		for( size_t i = 0; i < rounds; ++i )
		{
			for( auto& s_str : s_strings )
				s_sum += s_hasher( s_str );
		}
		auto s_end = clock.now();

		auto e_time = e_end - e_start;
		auto s_time = s_end - s_start;
		auto e_ms = std::chrono::duration_cast<std::chrono::milliseconds>( e_time );
		auto s_ms = std::chrono::duration_cast<std::chrono::milliseconds>( s_time );

		eon::term << "Eon string (" << e_sum << "): " << string::toString( e_ms.count() ) << "ms\n";
		eon::term << "Std string (" << s_sum << "): " << string::toString( s_ms.count() ) << "ms\n";
	}

	TEST( String, replace )
	{
		WANT_EQ( "elphe bete gemme", eon::string( "alpha beta gamma" ).replace( 'a', 'e' ) ) << "Wrong char";
		eon::string str{ "alpha beta gamma" };
		WANT_EQ( "alphe bete gemma", str.replace( 'a', 'e', eon::substring( str.begin() + 1, str.begin() + 14 ) ) )
			<< "Wrong char limited";

		WANT_EQ( "a and b and c", eon::string( "a or b or c" ).replace( "or", "and" ) ) << "Wrong string";
		str = "a or b or c";
		WANT_EQ( "a or b and c", str.replace( "or", "and", eon::substring( str.begin() + 3, str.end() ) ) )
			<< "Wrong string limited";
		str = "one to one";
		WANT_EQ( "many to many", str.replace( "one", "many" ) ) << "Wrong string limited";
	}
	TEST( String, replace2 )
	{
		WANT_EQ( "a--b", eon::string( "a,,b" ).replace( ',', '-' ) );
		WANT_EQ( "a--b", eon::string( "a::::b" ).replace( "::", "-" ) );
	}
	TEST( String, replace_speed )
	{
		eon::string base{ "one one-thousand two one-thousand three one-thousand four one-thousand five one-thousand size one-"
			"thousand seven one-thousand eight one-thousand nine one-thousand" };
		eon::string exp{ "two two-thousand two two-thousand three two-thousand four two-thousand five two-thousand size two-"
			"thousand seven two-thousand eight two-thousand nine two-thousand" };
		auto start = std::chrono::high_resolution_clock::now();
#ifdef _DEBUG
		size_t max = 10000;
#else
		size_t max = 100000;
#endif
		eon::term << "Running " << max << " iterations ...\n";
		for( size_t i = 0; i < max; ++i )
		{
			WANT_EQ( exp, base.replace( "one", "two" ) ) << "Wrong value in iteration " + std::to_string( i );
		}
		auto end = std::chrono::high_resolution_clock::now();
		eon::term << "Replace took " << std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count()
			<< " msec\n";
	}

	TEST( String, remove )
	{
		WANT_EQ( "lph bet gmm", eon::string( "alpha beta gamma" ).remove( 'a' ) ) << "Wrong char";
		eon::string str{ "alpha beta gamma" };
		WANT_EQ( "alph bet gmma", str.remove( 'a', eon::substring( str.begin() + 1, str.begin() + 14 ) ) )
			<< "Wrong char limited";

		WANT_EQ( "a  b  c", eon::string( "a and b and c" ).remove( "and" ) ) << "Wrong string";
		str = "a and b and c";
		WANT_EQ( "a and b  c", str.remove( "and", eon::substring( str.begin() + 3, str.end() ) ) )
			<< "Wrong string limited";
	}

	TEST( String, quote )
	{
		eon::string str{ "Hello World!" };

		WANT_EQ( "\"Hello World!\"", str.doubleQuote() ) << "Failed to quote";
		WANT_EQ( "He\"llo Wor\"ld!", str.doubleQuote( eon::substring( str.begin() + 2, str.begin() + 9 ) ) )
			<< "Failed to quote limited";

		WANT_EQ( "'Hello World!'", str.singleQuote() ) << "Failed to quote";
		WANT_EQ( "He'llo Wor'ld!", str.singleQuote( eon::substring( str.begin() + 2, str.begin() + 9 ) ) )
			<< "Failed to quote limited";
	}
	TEST( String, unquote )
	{
		WANT_EQ( "Hello World!", eon::string( "\"Hello World!\"" ).unQuote() ) << "Failed to unquote";
		eon::string str{ "He\"llo Wor\"ld!" };
		WANT_EQ( "Hello World!", str.unQuote( eon::substring( str.begin() + 2, str.begin() + 11 ) ) )
			<< "Failed to unquote limited";

		WANT_EQ( "Hello World!", eon::string( "'Hello World!'" ).unQuote() ) << "Failed to unquote";
		str = "He'llo Wor'ld!";
		WANT_EQ( "Hello World!", str.unQuote( eon::substring( str.begin() + 2, str.begin() + 11 ) ) )
			<< "Failed to unquote limited";
	}

	TEST( String, reverse )
	{
		eon::string str{ "abcdefgh" };
		WANT_EQ( "hgfedcba", str.reverse() ) << "Wrong reverse";
		WANT_EQ( "abfedcgh", str.reverse( eon::substring( str.begin() + 2, str.end() - 2 ) ) ) << "Wrong reverse limited";
	}

	TEST( String, pad )
	{
		WANT_EQ( "   pad", eon::string( "pad" ).padLeft( 6 ) ) << "Wrong left pad";
		WANT_EQ( "___pad", eon::string( "pad" ).padLeft( 6, '_' ) ) << "Wrong left pad char";
		WANT_EQ( "pad   ", eon::string( "pad" ).padRight( 6 ) ) << "Wrong right pad";
		WANT_EQ( "pad___", eon::string( "pad" ).padRight( 6, '_' ) ) << "Wrong right pad char";
		WANT_EQ( "  pad   ", eon::string( "pad" ).padLeftAndRight( 8 ) ) << "Wrong left-right pad";
		WANT_EQ( "___pad___", eon::string( "pad" ).padLeftAndRight( 9, '_' ) ) << "Wrong left-right pad char";
		WANT_EQ( "___pad____", eon::string( "pad" ).padLeftAndRight( 10, '_' ) ) << "Wrong left-right uneven pad char";
	}
	TEST( String, indentLines )
	{
		WANT_EQ( "  line1\n  line2", eon::string( "line1\nline2" ).indentLines( 2, eon::SpaceChr ) ) << "Wrong indent 1";
		WANT_EQ( "  line1\n\n   line2", eon::string( "  line1\n\n line2" ).indentLines( 2, eon::SpaceChr ) )
			<< "Wrong indent 2";
		WANT_EQ( "\n line1\n line2", eon::string( "\nline1\nline2" ).indentLines( 1, eon::SpaceChr ) ) << "Wrong indent 2";
	}

	TEST( String, findFirst )
	{
		eon::string str( "34312345334" );
		WANT_TRUE( str.begin() == str.findFirst( '3' ).begin() );
		WANT_TRUE( str.begin() == str.findFirst( "34" ).begin() );
		WANT_TRUE( str.begin() + 5 == str.findFirst( '3', eon::substring( str.begin() + 3, str.end() ) ).begin() );
		WANT_TRUE( str.begin() + 5 == str.findFirst( "34", eon::substring( str.begin() + 2, str.end() ) ).begin() );

		WANT_FALSE( str.findFirst( '1', eon::substring( str.begin() + 4, str.end() ) ) );
		WANT_FALSE( str.findFirst( "34", eon::substring( str.begin() + 1, str.begin() + 6 ) ) );
	}

	TEST( String, findFirstNotQuoted )
	{
		eon::string str{ "abc\"xyz\"efg" };
		WANT_FALSE( str.findFirstNotDoubleQuoted( 'x' ) ) << "Found char which was not in the unquoted part of the string";
		str = "\"x\"\"x\"xy";
		WANT_TRUE( str.begin() + 6 == str.findFirstNotDoubleQuoted( 'x' ) )
			<< "Didn't find char which was outside of quotes";
		str = "\"x\"\"x\"zyx";
		WANT_FALSE( eon::substring( str.begin(), str.begin() + 8 ).findFirstIgnoreSections( 'x', '"' ) )
			<< "Found char not in the unquoted limited part of the string";
		str = "xyz\"xyz\"zyx";
		WANT_TRUE( str.begin() + 10 == eon::substring( str.begin() + 1, str.end() ).findFirstIgnoreSections( 'x', '"' ) )
			<< "Didn't find char limited which was outside of quotes";

		WANT_FALSE( eon::string( "abc\"TESTyz\"efg" ).findFirstNotDoubleQuoted( "TEST" ) )
			<< "Found string which was not in the unquoted part of the string";
		str = "\"TEST\"\"TEST\"TESTy";
		WANT_TRUE( str.begin() + 12 == str.findFirstNotDoubleQuoted( "TEST" ) )
			<< "Didn't find string which was outside of quotes";
		str = "\"TEST\"\"TEST\"zyTEST";
		WANT_FALSE( eon::substring( str.begin(), str.begin() + 7 ).findFirstIgnoreSections(
			eon::substring( "TEST" ), '"' ) ) << "Found string which was not in the unquoted limited part of the string";
		str = "TESTyz\"TESTyz\"zyTEST";
		WANT_TRUE( str.begin() + 16 == eon::substring( str.begin() + 1, str.end() ).findFirstIgnoreSections(
			eon::substring( "TEST" ), '"' ) ) << "Didn't find string limited which was outside of quotes";
	}

	TEST( String, compare_diffpos )
	{
		WANT_EQ( 0, eon::string( "alpha beta" ).compare( "alpha beta", eon::CompareType::diff_pos ) )
			<< "Failed to compare equal";
		eon::string str{ "alpha beta" };
		WANT_EQ( 6, str.compare( "alpha", eon::CompareType::diff_pos ) ) << "Failed to compare substring from start";
		str = "alpha beta";
		WANT_EQ( 0, eon::substring( str.begin(), str.begin() + 5 ).compare(
			eon::substring( "alpha" ), eon::CompareType::diff_pos ) ) << "Failed to compare shorter left";
		WANT_EQ( 0, eon::substring( str.begin() + 6, str.end() ).compare(
				eon::substring( "beta" ), eon::CompareType::diff_pos ) ) << "Failed to compare shorter right";
		WANT_EQ( -10, eon::string( "alpha betA" ).compare( "alpha beta", eon::CompareType::diff_pos ) )
			<< "Failed to compare lesser";
		WANT_EQ( 10, eon::string( "alpha beta" ).compare( "alpha betA", eon::CompareType::diff_pos ) )
			<< "Failed to compare greater";

		WANT_EQ( 0, eon::substring( str.begin() + 2, str.end() - 2 ).compare(
			eon::substring( "pha be" ), eon::CompareType::diff_pos ) ) << "Failed to compare limited equal";
		str = "alpha bEta";
		WANT_EQ( -6, eon::substring( str.begin() + 2, str.end() - 2 ).compare(
			eon::substring( "pha be" ), eon::CompareType::diff_pos ) ) << "Failed to compare limited lesser";
		str = "alpha beta";
		WANT_EQ( 6, eon::substring( str.begin() + 2, str.end() - 2 ).compare(
			eon::substring( "pha bE" ), eon::CompareType::diff_pos ) ) << "Failed to compare limited greater";

		str = "Hello Big World!";
		eon::string str2{ "Big and bad" };
		WANT_EQ( 0, eon::substring( str.begin() + 6, str.begin() + 10 ).compare( eon::substring( str2.begin(),
			str2.begin() + 4 ), eon::CompareType::diff_pos ) ) << "Failed to compare big and bad";

		str = "";
		str2 = "Someting";
		WANT_EQ( -1, str.compare( str2, eon::CompareType::diff_pos ) ) << "Failed to compare empty";

		WANT_EQ( -5, eon::string( "beta" ).compare( "beta/gamma", eon::CompareType::diff_pos ) )
			<< "Failed to compare correctly when rhs is identical to lhs but longer";
	}
#ifndef __GNUC__
	TEST( String, compare_faster )
	{
		WANT_EQ( 0, eon::string( "alpha beta" ).compare( "alpha beta" ) ) << "Failed to compare equal";
		eon::string str{ "alpha beta" };
		WANT_EQ( 1, str.compare( "alpha" ) ) << "Failed to compare substring from start";
		str = "alpha beta";
		WANT_EQ( 0, eon::substring( str.begin(), str.begin() + 5 ).compare( eon::substring( "alpha" ) ) )
			<< "Failed to compare shorter left";
		WANT_EQ( 0, eon::substring( str.begin() + 6, str.end() ).compare( eon::substring( "beta" ) ) )
			<< "Failed to compare shorter right";
		WANT_EQ( -1, eon::string( "alpha betA" ).compare( "alpha beta" ) ) << "Failed to compare lesser";
		WANT_EQ( 1, eon::string( "alpha beta" ).compare( "alpha betA" ) ) << "Failed to compare greater";

		WANT_EQ( 0,
			eon::substring( str.begin() + 2, str.end() - 2 ).compare( eon::substring( "pha be" ) ) )
			<< "Failed to compare limited equal";
		str = "alpha bEta";
		WANT_EQ( -1, eon::substring( str.begin() + 2, str.end() - 2 ).compare( eon::substring( "pha be" ) ) )
			<< "Failed to compare limited lesser";
		str = "alpha beta";
		WANT_EQ( 1, eon::substring( str.begin() + 2, str.end() - 2 ).compare( eon::substring( "pha bE" ) ) )
			<< "Failed to compare limited greater";

		str = "Hello Big World!";
		eon::string str2{ "Big and bad" };
		WANT_EQ( 0, eon::substring( str.begin() + 6, str.begin() + 10 ).compare(
			eon::substring( str2.begin(), str2.begin() + 4 ) ) ) << "Failed to compare big and bad";

		str = "";
		str2 = "Someting";
		WANT_EQ( -1, str.compare( str2 ) ) << "Failed to compare empty";

		WANT_EQ( -1, eon::string( "beta" ).compare( "beta/gamma" ) )
			<< "Failed to compare correctly when rhs is identical to lhs but longer";
	}
#endif
	TEST( String, iCompare )
	{
		WANT_EQ( 0, eon::string( "alpha" ).iCompare( "ALPHA" ) );
		WANT_EQ( 5, eon::string( "alpha" ).iCompare( "alPh" ) );
		WANT_EQ( -5, eon::string( "alPh" ).iCompare( "alpha" ) );
	}

	TEST( String, escape )
	{
		WANT_EQ( "a	b\\nc\\\\", eon::string( "a\tb\nc\\" ).escape() ) << "Failed to escape";
		WANT_EQ( "a\\tb\\nc\\\\", eon::string( "a\tb\nc\\" ).escapeAll() ) << "Failed to escape";
		WANT_EQ( "a\tb\nc\\", eon::string( "a\\tb\\nc\\\\" ).unescape() ) << "Failed to unescape";
		auto esc = eon::string( "C:\\\\flex\\log \\n\\ntest" ).unescape();
		WANT_EQ( "C:\\flex\\log \n\ntest", esc ) << "Failed to properly unescape";

		string uexp{ char_t( 128522 ) };	// Smileyface
		WANT_TRUE( string( "\\U1F60A" ).unescape() == uexp ) << "Failed to unescape unicode";
		
		string hexexp{ "0Az" };
		WANT_TRUE( string( "\\x30\\x41\\x7A" ).unescape() == hexexp ) << "Failed to unescape hex";

		string octexp{ "0Az" };
		octexp += char_t( 277 );	// e with breve
		octexp += NullChr;
		auto octact = string( "\\60\\101\\172\\425\\0" ).unescape();
		WANT_TRUE( octact == octexp ) << "Failed to unescape octal";
	}
	TEST( String, escape_nonprintable )
	{
		eon::string base{ '\0', '\v', '\r' };
		base += char_t( 128522 );	// Smileyface
//		base += u8"😊";
		auto esc = base.escapeNonPrintable();
		WANT_EQ( "&#0;&#11;&#13;&#128522;", esc ) << "Failed to escape";
		auto unesc = esc.unescapeNonPrintable();
		WANT_EQ( base, unesc ) << "Failed to unescape";
	}

	TEST( String, utf8 )
	{
		std::vector<char32_t> codepoints;
		codepoints.push_back( 0x0041U );	// 'A'
		codepoints.push_back( 0x00DFU );	// Sharp 's'
		codepoints.push_back( 0x0100U );	// 'A' with macron
		codepoints.push_back( 0x0394U );	// Open triangle, upwards
		codepoints.push_back( 0x050EU );	// Cyrillic captital komi tje

		eon::string str;
		for( auto i : codepoints )
		{
			REQUIRE_NO_EXCEPT( str += char_t( i ) ) << "Failed to add unicode character &#" << std::to_string( i );
		}
		REQUIRE_EQ( codepoints.size(), str.numChars() ) << "Wrong size";

		size_t i = 0;
		for( auto char_t : str )
		{
			if( char_t != codepoints[ i ] ) ADD_FAILURE() << "Wrong code point at index " << std::to_string( i );
			++i;
		}
	}

	TEST( String, toString_int )
	{
		int64_t i64_1{ 1 }, i64_2{ -56746754767LL };
		uint64_t ui64{ 9034658634325425ULL };
		double dbl_1{ 1.23 }, dbl_2{ -3546346.02341435536 };
		WANT_EQ( "1", string::toString( i64_1 ) ) << "Wrong int64_t value";
		WANT_EQ( "-56746754767", string::toString( i64_2 ) ) << "Wrong int64_t value";
		WANT_EQ( "9034658634325425", string::toString( ui64 ) ) << "Wrong uint64_t value";
		WANT_EQ( "1.23", string::toString( dbl_1 ) ) << "Wrong double value";
		WANT_EQ( "-3546346.02341436", string::toString( dbl_2 ) ) << "Wrong double value";
		WANT_EQ( 311, string::toString( DBL_MAX ).numChars() ) << "Wrong double max";
	}

	TEST( String, real_issue_seen1 )
	{
		eon::string source{ "log/test/function/unit_context/gt_context.cpp:340" };
		eon::string exp = source;
		eon::substring sub;
		sub.begin() = eon::string_iterator( source.c_str(), 93, source.numChars(), source.c_str() + 93, 93 );
		sub.end() = eon::string_iterator( source.c_str(), 103, source.numChars(), source.c_str() + 103, 103 );
		source.erase( sub );
		WANT_EQ( exp, source );
	}

	TEST( String, speed_test )
	{
		// Create and destroy a different string a large number of times,
		// access the 10'th and the last character of each and add their integer
		// values to a sum..

		size_t num_strings = 2000000;
#ifdef _DEBUG
		num_strings /= 10;
#endif
		std::chrono::steady_clock clock;

		// eon
		size_t e_sum = 0;
		auto e_start = clock.now();
		{
			for( size_t i = 0; i < num_strings; ++i )
			{
				eon::string str = "This is something else #" + eon::string( i );
				e_sum += static_cast<size_t>( *( str.begin() + 10 ) + *str.last() );
				str.clear();
			}
		}
		auto e_end = clock.now();

		// std
		size_t s_sum = 0;
		auto s_start = clock.now();
		{
			for( size_t i = 0; i < num_strings; ++i )
			{
				std::string str = "This is something else #" + std::to_string( i );
				s_sum += static_cast<size_t>( str[ 10 ] + str[ str.size() - 1 ] );
				str.clear();
			}
		}
		auto s_end = clock.now();

		auto e_time = e_end - e_start;
		auto s_time = s_end - s_start;
		auto e_ms = std::chrono::duration_cast<std::chrono::milliseconds>( e_time );
		auto s_ms = std::chrono::duration_cast<std::chrono::milliseconds>( s_time );

		eon::term << "Eon string (" << e_sum << "): " << string::toString( e_ms.count() ) << "ms\n";
		eon::term << "Std string (" << s_sum << "): " << string::toString( s_ms.count() ) << "ms\n";
	}



	struct name_tt
	{
		std::uint32_t Value{ 0 };
	};


	TEST( Name, basic )
	{
		name_t  n1{ no_name }, n2{ name_name };
		n1 = name( "name" );
		WANT_EQ( n1, n2 );
		WANT_EQ( "name", str( n2 ) );
	}
	TEST( Name, compiler )
	{
		auto n1 = compilerName( "$name" );
		auto n2 = compilerName( "@name" );
		auto n3 = compilerName( "#name" );
		auto n4 = compilerName( "0$@#0" );
		REQUIRE_FALSE( n1 == no_name );
		REQUIRE_FALSE( n2 == no_name );
		REQUIRE_FALSE( n3 == no_name );
		REQUIRE_FALSE( n4 == no_name );
		WANT_EQ( "$name", str( n1 ) );
		WANT_EQ( "@name", str( n2 ) );
		WANT_EQ( "#name", str( n3 ) );
		WANT_EQ( "0$@#0", str( n4 ) );
	}

	TEST( Name, path )
	{
		namepath ref{ "one/two/three" };
		WANT_EQ( "@one/two/three", ref.str() );
		REQUIRE_EQ( 3, ref.numElms() );
		WANT_EQ( "one", str( ref.at( 0 ) ) );
		WANT_EQ( "two", str( ref.at( 1 ) ) );
		WANT_EQ( "three", str( ref.at( 2 ) ) );
	}
}
