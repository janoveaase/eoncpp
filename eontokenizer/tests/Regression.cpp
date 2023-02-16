#include "Regression.h"



namespace eon
{
	TEST( TokenizerTest, basic )
	{
		source::String src( "test", "This is a 02   \tline test\nLine #2!" );
		auto tokens = Tok( src );
		string expected{
			"letters=This|space= |letters=is|space= |letters=a|space= |digits=02|space=   |tab=\t|letters=line|space= |"
			"letters=test|newline=\n|letters=Line|space= |symbol=#|digits=2|symbol=!" };
		string actual = fullJoin( tokens );
		WANT_EQ( expected, actual );
	}

	TEST( TokenizerTest, name )
	{
		string raw{ "n_0" };
		source::String src( "test", std::move( raw ) );
		Tok.registerEonNamesAsTokens();
		auto tokens = Tok( src );
		string expected{ "n_0" };
		string actual = simpleJoin( tokens );
		WANT_EQ( expected, actual );
	}
	TEST( TokenizerTest, names1 )
	{
		string raw{ "name 0name0 00 _0 0_ _name name_00" };
		source::String src( "test", std::move( raw ) );
		Tok.registerEonNamesAsTokens();
		auto tokens = Tok( src );
		string expected{ "name| |0name0| |00| |_0| |0_| |_name| |name_00" };
		string actual = simpleJoin( tokens );
		WANT_EQ( expected, actual );
	}
	TEST( TokenizerTest, names2 )
	{
		string raw{ "_sum_1+=2+2num" };
		source::String src( "test", std::move( raw ) );
		Tok.registerEonNamesAsTokens();
		auto tokens = Tok( src );
		string expected{ "name=_sum_1|operator=+=|digits=2|operator=+|name=2num" };
		string actual = fullJoin( tokens );
		WANT_EQ( expected, actual );
	}

	TEST( TokenizerTest, special )
	{
		string raw{ "++(())<<>>[[]]{{}}--**//" };
		source::String src( "test", std::move( raw ) );
		auto tokens = Tok( src );
		WANT_EQ( 24, tokens.size() ) << "Wrong number of tokens";
		string expected{ "+|+|(|(|)|)|<|<|>|>|[|[|]|]|{|{|}|}|-|-|*|*|/|/" };
		string actual = simpleJoin( tokens );
		WANT_EQ( expected, actual );
	}

	TEST( TokenizerTest, larger )
	{
		string raw{
			"TEST( TokenizerTest, basic )\n"
			"{\n"
			"	string raw{ \"This is a 02   \\tline test\\nLine #2!\" };\n"
			"	source src( \"test\", std::move( raw ) );\n"
			"	auto tokens = Tokenizer()( src );\n"
			"	REQUIRE_EQ( 18, tokens.size() ) << \"Wrong number of tokens\";\n"
			"\n"
			"	string expected{\n"
			"		\"This; ;is; ;a; ;02;   ;\\t;line; ;test;\\n;Line; ;#;2;!\" };\n"
			"	string actual;\n"
			"	for( auto& token : tokens )\n"
			"	{\n"
			"		if( !actual.empty() )\n"
			"			actual += \";\";\n"
			"		actual += token.substr();\n"
			"	}\n"
			"	WANT_EQ( expected, actual );\n"
			"}" };
		source::String src( "test", std::move( raw ) );
		auto tokens = Tok( src );

		string expected{
			"TEST|(| |TokenizerTest|,| |basic| |)|\n"
			"|{|\n"
			"|\t|string| |raw|{| |\"|This| |is| |a| |02|   |\\|tline| |test|\\|nLine| |#|2|!|\"| |}|;|\n"
			"|\t|source| |src|(| |\"|test|\"|,| |std|:|:|move|(| |raw| |)| |)|;|\n"
			"|\t|auto| |tokens| |=| |Tokenizer|(|)|(| |src| |)|;|\n"
			"|\t|REQUIRE|_|EQ|(| |18|,| |tokens|.|size|(|)| |)| |<|<| |\"|Wrong| |number| |of| |tokens|\"|;|\n"
			"|\n"
			"|\t|string| |expected|{|\n"
			"|\t\t|\"|This|;| |;|is|;| |;|a|;| |;|02|;|   |;|\\|t|;|line|;| |;|test|;|\\|n|;|Line|;| |;|#|;|2|;|!|\"| |}|;|\n"
			"|\t|string| |actual|;|\n"
			"|\t|for|(| |auto|&| |token| |:| |tokens| |)|\n"
			"|\t|{|\n"
			"|\t\t|if|(| |!|actual|.|empty|(|)| |)|\n"
			"|\t\t\t|actual| |+=| |\"|;|\"|;|\n"
			"|\t\t|actual| |+=| |token|.|substr|(|)|;|\n"
			"|\t|}|\n"
			"|\t|WANT|_|EQ|(| |expected|,| |actual| |)|;|\n"
			"|}" };
		string actual = simpleJoin( tokens );
		WANT_EQ( expected, actual );
	}


	TEST( TokenParserTest, basic )
	{
		string raw{ "This is a  02   \tline test\nLine #2!" };
		source::String src( "test", std::move( raw ) );
		auto tokens = Tok( src );
		TokenParser parser( std::move( tokens ) );

		REQUIRE_EQ( "This", parser.viewed().str() ) << "Wrong first token";
		parser.forward();
		REQUIRE_EQ( " ", parser.viewed().str() ) << "Wrong second token";
		parser.forward();
		REQUIRE_EQ( "is", parser.viewed().str() ) << "Wrong third token";

		WANT_EQ( 0, parser.lineStart() ) << "Wrong first line start";

		parser.backward();
		parser.backward();
		REQUIRE_EQ( "This", parser.viewed().str() ) << "Wrong first token - again";

		TokenMatcher matcher;
		REQUIRE_NO_EXCEPT(
			matcher = TokenMatcher(
				"(letters 'T*') ?(space) opt((letters 'was') (letters 'is')) ?(space) ?opt((letters 'x') "
				"(letters 'a')) (space '  ') (digits '*2') (space)" ) );
		WANT_TRUE( matcher.match( parser ) ) << "Failed to match group1";

		REQUIRE_NO_EXCEPT(
			matcher = TokenMatcher( "!(letters 't*') (letters) (space) -(digits) (space) !(letters 'are')" ) );
		WANT_TRUE( matcher.match( parser ) ) << "Failed to match group2";

		parser.setView( 8 );
		REQUIRE_EQ( "\t", parser.viewed().str() ) << "Wrong eigth token";

		REQUIRE_EQ( "test", parser.peekAhead( 3 ).str() ) << "Wrong eleventh token";

		REQUIRE_EQ( "#", parser.peek( 15 ).str() ) << "Wrong fifteenth token";

		parser.setView( 15 );
		REQUIRE_EQ( 13, parser.lineStart() ) << "Wrong second line start";
	}


	TEST( ReTokenizerTest, singleton )
	{
		string raw{ "2num" };
		source::String src( "test", std::move( raw ) );
		auto tokens = Tok( src );
		TokenParser parser( std::move( tokens ) );

		ReTokenizer retok;
		retok.addRule(
			new ReTokenizer::ComboRule( name_name, { name_letters, name_digits, name_underscore }, regex{ R"(^\d+$)" } ) );
		retok.addRule( new ReTokenizer::RemoveRule( { name_space } ) );
		tokens = retok( parser );

		string expected{ "name=2num" };
		string actual = fullJoin( tokens );
		WANT_EQ( expected, actual );
	}
	TEST( ReTokenizerTest, basic )
	{
		string raw{ "_sum_1 += 2 + 2num" };
		source::String src( "test", std::move( raw ) );
		auto tokens = Tok( src );
		TokenParser parser( std::move( tokens ) );

		ReTokenizer retok;
		retok.addRule(
			new ReTokenizer::ComboRule( name_name, { name_letters, name_digits, name_underscore }, regex{ R"(^\d+$)" } ) );
		retok.addRule( new ReTokenizer::RemoveRule( { name_space } ) );
		tokens = retok( parser );

		string expected{ "name=_sum_1|operator=+=|digits=2|operator=+|name=2num" };
		string actual = fullJoin( tokens );
		WANT_EQ( expected, actual );
	}
	TEST( ReTokenizerTest, enclosed )
	{
		string raw{ "one \"two \\\" three\" (four and five) [six[seven]eight]" };
		source::String src( "test", std::move( raw ) );
		auto tokens = Tok( src );
		TokenParser parser( std::move( tokens ) );

		ReTokenizer retok;
		retok.addRule( new ReTokenizer::EncloseRule( name_string, name_doublequote, name_backslash ) );
		retok.addRule( new ReTokenizer::EncloseRule( name( "parenthesized" ), name_open, name_close, false ) );
		retok.addRule( new ReTokenizer::EncloseRule( name( "nested" ), name_open_square, name_close_square, true ) );
		retok.addRule( new ReTokenizer::RemoveRule( { name_space } ) );
		tokens = retok( parser );

		string expected{ "letters=one|string=two \\\" three|parenthesized=four and five|nested=six[seven]eight" };
		string actual = fullJoin( tokens );
		WANT_EQ( expected, actual );
	}
	TEST( ReTokenizerTest, linestart )
	{
		string raw{ "   one\n  two\n three\nfour" };
		source::String src( "test", std::move( raw ) );
		auto tokens = Tok( src );
		TokenParser parser( std::move( tokens ) );

		ReTokenizer retok;
		retok.addRule( new ReTokenizer::LinestartRule( name_indentation, name_space ) );
		retok.addRule( new ReTokenizer::RemoveRule( { name_newline } ) );
		tokens = retok( parser );

		string expected{
			"indentation=   |letters=one|indentation=  |letters=two|indentation= |letters=three|letters=four" };
		string actual = fullJoin( tokens );
		WANT_EQ( expected, actual );
	}
	TEST( ReTokenizerTest, int_float_name )
	{
		string raw{ "1 1.2 1three" };
		source::String src( "test", std::move( raw ) );
		auto tokens = Tok( src );
		TokenParser parser( std::move( tokens ) );

		ReTokenizer retok;
		retok.addRule( new ReTokenizer::SequenceRule( name_float, { name_digits, name_point, name_digits } ) );
		retok.addRule( new ReTokenizer::ComboRule( name_name, { name_letters, name_digits, name_underscore },
			regex{ R"(^\d+$)" } ) );
		tokens = retok( parser );

		string expected{ "digits=1|space= |float=1.2|space= |name=1three" };
		string actual = fullJoin( tokens );
		WANT_EQ( expected, actual );
	}
}
