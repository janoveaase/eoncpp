#include "Regression.h"



namespace eon
{
	TEST( TokenizerTest, basic )
	{
		string raw{ "This is a 02   \tline test\nLine #2!" };
		source src( "test", std::move( raw ) );
		auto tokens = Tokenizer()( src );
		REQUIRE_EQ( 18, tokens.size() ) << "Wrong number of tokens";

		string expected{
			"This; ;is; ;a; ;02;   ;\t;line; ;test;\n;Line; ;#;2;!" };
		string actual;
		for( auto& token : tokens )
		{
			if( !actual.empty() )
				actual += ";";
			actual += token.substr();
		}
		WANT_EQ( expected.stdstr(), actual.stdstr() );
	}

	TEST( TokenizerTest, larger )
	{
		string raw{ "TEST( TokenizerTest, basic )\n"
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
			"	WANT_EQ( expected.stdstr(), actual.stdstr() );\n"
			"}" };
		source src( "test", std::move( raw ) );
		auto tokens = Tokenizer()( src );
		REQUIRE_EQ( 250, tokens.size() ) << "Wrong number of tokens";

		string expected{
			"TEST|(| |TokenizerTest|,| |basic| |)|\n"
			"|{|\n"
			"|\t|string| |raw|{| |\"|This| |is| |a| |02|   |\\|tline| |test|\\|nLine| |#|2|!|\"| |}|;|\n"
			"|\t|source| |src|(| |\"|test|\"|,| |std|::|move|(| |raw| |)| |)|;|\n"
			"|\t|auto| |tokens| |=| |Tokenizer|(|)|(| |src| |)|;|\n"
			"|\t|REQUIRE|_|EQ|(| |18|,| |tokens|.|size|(|)| |)| |<<| |\"|Wrong| |number| |of| |tokens|\"|;|\n"
			"|\n"
			"|\t|string| |expected|{|\n"
			"|\t\t|\"|This|;| |;|is|;| |;|a|;| |;|02|;|   |;|\\|t|;|line|;| |;|test|;|\\|n|;|Line|;| |;|#|;|2|;|!|\"| |}|;|\n"
			"|\t|string| |actual|;|\n"
			"|\t|for|(| |auto|&| |token| |:| |tokens| |)|\n"
			"|\t|{|\n"
			"|\t\t|if|(| |!|actual|.|empty|(|)| |)|\n"
			"|\t\t\t|actual| |+|=| |\"|;|\"|;|\n"
			"|\t\t|actual| |+|=| |token|.|substr|(|)|;|\n"
			"|\t|}|\n"
			"|\t|WANT|_|EQ|(| |expected|.|stdstr|(|)|,| |actual|.|stdstr|(|)| |)|;|\n"
			"|}" };
		string actual;
		for( auto& token : tokens )
		{
			if( !actual.empty() )
				actual += "|";
			actual += token.substr();
		}
		WANT_EQ( expected.stdstr(), actual.stdstr() );
	}
}
