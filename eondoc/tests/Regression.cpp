#include "Regression.h"



namespace eon
{
	TEST( EdocTest, empty )
	{
		string in{ "" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "", docs.str( "test" ).stdstr() );
	}

	TEST( EdocTest, title )
	{
		string in{ ">> This is a Title <<" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( ">> This is a Title <<\n", docs.str( "test" ).stdstr() );
	}

	TEST( EdocTest, heading_1 )
	{
		string in{ ">> This is a heading" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( ">> This is a heading\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, heading_2 )
	{
		string in{ ">> > This is a heading" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( ">> > This is a heading\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, heading_3 )
	{
		string in{ ">> >> This is a heading" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( ">> >> This is a heading\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, heading_4 )
	{
		string in{ ">> >>> This is a heading" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( ">> >>> This is a heading\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, heading_5 )
	{
		string in{ ">> >>>> This is a heading" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( ">> >>>> This is a heading\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, heading_6 )
	{
		string in{ ">> >>>>> This is a heading" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( ">> >>>>> This is a heading\n", docs.str( "test" ).stdstr() );
	}

	TEST( EdocTest, paragraph_basic )
	{
		string in{ "This is a paragraph" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "This is a paragraph\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, paragraph_softbreak )
	{
		string in{ "This is a paragraph\nwith a soft line-break!" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "This is a paragraph with a soft line-break!\n",
			docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, paragraph_hardbreak )
	{
		string in{ "This is a paragraph\n  with a hard line-break!" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "This is a paragraph\n  with a hard line-break!\n",
			docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, paragraph_emphasize_word )
	{
		string in{ "This is an *emphasized* word" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "This is an *emphasized* word\n",
			docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, paragraph_emphasize_phrase )
	{
		string in{ "This is an *emphasized phrase of many words*" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "This is an *emphasized phrase of many words*\n",
			docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, paragraph_no_emphasize )
	{
		string in{ "This is not an *emphasized phrase of many words" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "This is not an *emphasized phrase of many words\n",
			docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, paragraph_specialchars )
	{
		string in{ "&lt;&#A9;&gt;" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "&lt;&#A9;&gt;\n",
			docs.str( "test" ).stdstr() );
	}

	TEST( EdocTest, listing_basic )
	{
		string in{ "  - one\n  - two" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  - one\n  - two\n",
			docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, listing_numbered )
	{
		string in{ "  1 one\n  1 two" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  1 one\n  2 two\n",
			docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, listing_multiline )
	{
		string in{ "  - one\n    1\n  - two\n    2" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  - one 1\n  - two 2\n",
			docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, listing_multiline_long )
	{
		string in{ "  - This is a very long line that should be split up when parsed and converted back to string again.\n  - Another somewhat long line that should be split up over two lines in the final output." };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  - This is a very long line that should be split up when parsed and converted\n    back to string again.\n  - Another somewhat long line that should be split up over two lines in the\n    final output.\n",
			docs.str( "test" ).stdstr() );
	}

	TEST( EdocTest, definition_singleword_singleline )
	{
		string in{ "  what: definition" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  what: definition\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, definition_multiword_singleline )
	{
		string in{ "  what is this: definition" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  what is this: definition\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, definition_singleword_secondline )
	{
		string in{ "  what:\n    definition of something *very* strange that defies description!" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  what: definition of something *very* strange that defies description!\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, definition_singleword_multiline )
	{
		string in{ "  what:\n    definition of something *very* strange that defies description\n    but for which we add a lot of text anyways!" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  what:\n    definition of something *very* strange that defies description but for\n    which we add a lot of text anyways!\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, definition_multiword_multiline )
	{
		string in{ "  what is this:\n    definition of something *very* strange that defies description\n    but for which we add a lot of text anyways!" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  what is this:\n    definition of something *very* strange that defies description but for\n    which we add a lot of text anyways!\n", docs.str( "test" ).stdstr() );
	}

	TEST( EdocTest, insert_example_simple1 )
	{
		string in{ "  --example-->Hello<--" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  --example-->Hello<--\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, insert_example_simple2 )
	{
		string in{ "  --example--> Hello  <--" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  --example--> Hello  <--\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, insert_example_simple3 )
	{
		string in{ "  --example-->\n  Hello\n  <--" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  --example-->Hello<--\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, insert_example_complex )
	{
		string in{ "  --example-->\n  Hello\n  The second line!\n  A third line\n  Last line.\n  <--" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  --example-->\n  Hello\n  The second line!\n  A third line\n  Last line.\n  <--\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, insert_example_heading_simple )
	{
		string in{ "  --example, with a heading-->\n  Hello\n  <--" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  --example, with a heading-->Hello<--\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, insert_example_heading_complex )
	{
		string in{ "  --example,    with a heading   -->\n  Hello\n  The second line!\n  A third line\n  Last line.\n  <--" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  --example, with a heading-->\n  Hello\n  The second line!\n  A third line\n  Last line.\n  <--\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, insert_code_cpp )
	{
		string in{ "  --code C++-->\n    int x = 33 + y;\n    if( x > 100 )\n      reachedMax();\n  <--" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  --code C++-->\n    int x = 33 + y;\n    if( x > 100 )\n      reachedMax();\n  <--\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, insert_code_cpp_heading )
	{
		string in{ "  --code C++,  Just a little snippet    -->\n    int x = 33 + y;\n    if( x > 100 )\n      reachedMax();\n  <--" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  --code C++, Just a little snippet-->\n    int x = 33 + y;\n    if( x > 100 )\n      reachedMax();\n  <--\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, insert_code_nolang )
	{
		string in{ "  --code->\n    int x = 33 + y;\n    if( x > 100 )\n      reachedMax();\n  <--" };
		edoc docs;
		REQUIRE_EXCEPT( docs.parse( in, "test" ), EdocBadSyntax )
			<< "Failed detect error";
	}
	TEST( EdocTest, insert_code_nolang_heading )
	{
		string in{ "  --code, Just a little snippet->\n    int x = 33 + y;\n    if( x > 100 )\n      reachedMax();\n  <--" };
		edoc docs;
		REQUIRE_EXCEPT( docs.parse( in, "test" ), EdocBadSyntax )
			<< "Failed detect error";
	}
	TEST( EdocTest, insert_quote )
	{
		string in{ "  --quote From nowhere-->Yes<--" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  --quote From nowhere-->Yes<--\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, insert_quote_nosource )
	{
		string in{ "  --quote-->Yes<--" };
		edoc docs;
		REQUIRE_EXCEPT( docs.parse( in, "test" ), EdocBadSyntax )
			<< "Failed detect error";
	}
	TEST( EdocTest, insert_image )
	{
		string in{ "  --insert image-->http://some-address.com<--" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  --insert image-->http://some-address.com<--\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, insert_image_heading )
	{
		string in{ "  --insert image, Some dummy-->http://some-address.com<--" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  --insert image, Some dummy-->http://some-address.com<--\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, insert_image_nourl )
	{
		string in{ "  --insert image--><--" };
		edoc docs;
		REQUIRE_EXCEPT( docs.parse( in, "test" ), EdocBadSyntax )
			<< "Failed detect error";
	}
	TEST( EdocTest, insert_toc )
	{
		string in{ "  --insert TOC--><--" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  --insert TOC--><--\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, insert_toc_heading )
	{
		string in{ "  --insert TOC, Table of Contents--><--" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  --insert TOC, Table of Contents--><--\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, insert_toc_contents )
	{
		string in{ "  --insert TOC-->This is not legal!<--" };
		edoc docs;
		REQUIRE_EXCEPT( docs.parse( in, "test" ), EdocBadSyntax )
			<< "Failed detect error";
	}
	TEST( EdocTest, insert_ctoc )
	{
		string in{ "  --insert CTOC--><--" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  --insert CTOC--><--\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, insert_ctoc_contents )
	{
		string in{ "  --insert CTOC-->This is not legal!<--" };
		edoc docs;
		REQUIRE_EXCEPT( docs.parse( in, "test" ), EdocBadSyntax )
			<< "Failed detect error";
	}
	TEST( EdocTest, insert_unsupported)
	{
		string in{ "  --insert bad--><--" };
		edoc docs;
		REQUIRE_EXCEPT( docs.parse( in, "test" ), EdocBadSyntax )
			<< "Failed detect error";
	}

	TEST( EdocTest, exclamation_tip )
	{
		string in{ "TIP: This is a tip!" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "TIP: This is a tip!\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, exclamation_note )
	{
		string in{ "NOTE: This is a note!" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "NOTE: This is a note!\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, exclamation_warning )
	{
		string in{ "WARNING: This is a warning!" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "WARNING: This is a warning!\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, exclamation_invalid )
	{
		string in{ "tip: This is a tip!" };
		edoc docs;
		REQUIRE_EXCEPT( docs.parse( in, "test" ), EdocBadSyntax )
			<< "Failed to detect error";
	}

	TEST( EdocTest, anchor_paragraph )
	{
		string in{ "@test anchor:\nThis is a paragraph!" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "@test_anchor:\nThis is a paragraph!\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, anchor_defintion )
	{
		string in{ "@defs:\n  uno: one\n  dos: two" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "@defs:\n  uno: one\n  dos: two\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, anchor_list)
	{
		string in{ "@anchor:\n  - One\n  - Two" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "@anchor:\n  - One\n  - Two\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, anchor_hidden )
	{
		string in{ "  --insert hidden @test:-->This is hidden data!<--" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  --insert hidden @test:-->This is hidden data!<--\n", docs.str( "test" ).stdstr() );
	}
	// Anchor table

	TEST( EdocTest, link_basic )
	{
		string in{ "Go back to @[Top_of_page]." };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "Go back to @[Top_of_page].\n", docs.str( "test" ).stdstr() );
	}
	TEST( EdocTest, link_spaces )
	{
		string in{ "Go back to @[   Top_of_page   ]." };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "Go back to @[Top_of_page].\n", docs.str( "test" ).stdstr() );
	}

/*	TEST( EdocTest, table_simple )
	{
		string in{ "  |++++++++++++|++++|\n  | 2019-12-12   43" };
		edoc docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, "test" ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( "test" ) );
		WANT_EQ( "  |++++++++++++|++++|\n  | 2019-12-12   43\n", docs.str( "test" ).stdstr() );
	}*/
}
