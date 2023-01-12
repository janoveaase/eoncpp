#include "Regression.h"



namespace eon
{
	TEST( EdocTest, headers )
	{
		edoc doc;
		auto output = doc.parse( ">> Header1\n>> > Header2\n>> >> Header 3\n>> >>> Header #4" );
		Stringifier strf;
		output.str( strf );
		auto act = strf.generateString();
		auto exp{
			"data(\n"
			"  (type=h1, value=\"Header1\"),"
			" (type=h2, value=\"Header2\"),"
			" (type=h3, value=\"Header 3\"),"
			" (type=h4, value=\"Header #4\"))"};
		WANT_EQ( exp, act );
	}

	TEST( EdocTest, bullet_list )
	{
		edoc doc;
		auto output = doc.parse( "  * Item 1\n  * Item 2\n  * Item 3\n" );
		Stringifier strf;
		output.str( strf );
		auto act = strf.generateString();
		auto exp{
			"data(\n"
			"  (type=list, list=bullet,\n"
			"    value:\n"
			"      (type=text, value=\"Item 1\"), (type=text, value=\"Item 2\"), (type=text, value=\"Item 3\")))" };
		WANT_EQ( exp, act );
	}
	TEST( EdocTest, dash_list )
	{
		edoc doc;
		auto output = doc.parse( "  - Item 1\n  - Item 2\n  - Item 3\n" );
		Stringifier strf;
		output.str( strf );
		auto act = strf.generateString();
		auto exp{
			"data(\n"
			"  (type=list, list=dash,\n"
			"    value:\n"
			"      (type=text, value=\"Item 1\"), (type=text, value=\"Item 2\"), (type=text, value=\"Item 3\")))" };
		WANT_EQ( exp, act );
	}
	TEST( EdocTest, enum_list )
	{
		edoc doc;
		auto output = doc.parse( "  1 Item 1\n  # Item 2\n  33 Item 3\n" );
		Stringifier strf;
		output.str( strf );
		auto act = strf.generateString();
		auto exp{
			"data(\n"
			"  (type=list, list=enum,\n"
			"    value:\n"
			"      (type=text, value=\"Item 1\"), (type=text, value=\"Item 2\"), (type=text, value=\"Item 3\")))" };
		WANT_EQ( exp, act );
	}

	TEST( EdocTest, exclamation )
	{
		edoc doc;
		auto output = doc.parse( "NOTE: This is a note!\nWARNING: A warning\nTODO: Do this!\nTIP: Nice tip!\n"
			"NOTE: Can span\n  multiple lines!" );
		Stringifier strf;
		strf.hardLineWidth( 80 );
		output.str( strf );
		auto act = strf.generateString();
		auto exp{
			"data(\n"
			"  (type=note, value=\"This is a note!\"), (type=warning, value=\"A warning\"),\n"
			"  (type=todo, value=\"Do this!\"), (type=tip, value=\"Nice tip!\"), (type=note,\n"
			"  value=\"Can span multiple lines!\"))" };
		WANT_EQ( exp, act );
	}

	TEST( EdocTest, definition1 )
	{
		edoc doc;
		auto output = doc.parse( "  alpha: The first letter in the Greek alphabet\n"
			"  two words: Example of two word definition\n  multiline:\n    Line 1\n    Line 2\n"
			"  !anonymous def: One\n  \"double quoted\":\n    Indented and anonymous" );
		Stringifier strf;
		strf.hardLineWidth( 80 );
		output.str( strf );
		auto act = strf.generateString();
		auto exp =
			"data(\n"
			"  (type=definition, phrase=\"alpha\",\n"
			"  text=\"The first letter in the Greek alphabet\",\n"
			"  value=\"The first letter in the Greek alphabet\"), (type=definition,\n"
			"  phrase=\"two words\", text=\"Example of two word definition\",\n"
			"  value=\"Example of two word definition\"), (type=definition, phrase=\"multiline\",\n"
			"  text=\"Line 1 Line 2\", value=\"Line 1 Line 2\"), (type=definition, anonymous,\n"
			"  phrase=\"anonymous def\", value=\"One\"), (type=definition, quoted,\n"
			"  phrase=\"double quoted\", value=\"Indented and anonymous\"))";
		WANT_EQ( exp, act );
	}
	TEST( EdocTest, definition2 )
	{
		edoc doc;
		auto output = doc.parse(
			"Plain text.\n"
			"\n"
			"  \"{A}\":\n"
			"    Match element exactly A number of times.\n"
			"  B:\n"
			"    A B!" );
		Stringifier strf;
		strf.hardLineWidth( 80 );
		output.str( strf );
		auto act = strf.generateString();
		auto exp =
			"data(\n"
			"  (type=paragraph,\n"
			"    value:\n"
			"      (type=text, value=\"Plain text.\")),\n"
			"  (type=definition, quoted, phrase=\"{A}\",\n"
			"  value=\"Match element exactly A number of times.\"), (type=definition,\n"
			"  phrase=\"B\", text=\"A B!\", value=\"A B!\"))";
		WANT_EQ( exp, act );
	}
	TEST( EdocTest, definition3 )
	{
		edoc doc;
		auto output = doc.parse(
			"  \"@<A>(B)\":\n"
			"    Match B (a sub-expression) and record using the group name A. Example: \"@<eonname>(\\w+{name})\"." );
		Stringifier strf;
		strf.hardLineWidth( 80 );
		output.str( strf );
		auto act = strf.generateString();
		auto exp =
			"data(\n"
			"  (type=definition, quoted, phrase=\"@<A>(B)\",\n"
			"    value:\n"
			"      (type=text,\n"
			"      value=\"\n"
			"      Match B (a sub-expression) and record using the group name A. Example: \"),\n"
			"      (type=quoted, value=\"@<eonname>(\\w+{name})\"), (type=text, value=\".\")))";
		WANT_EQ( exp, act );
	}

	TEST( EdocTest, insert1 )
	{
		edoc doc;
		auto output = doc.parse(
			"  --code C++ A code sample-->One-liner<--\n"
			"  --example Something amazing-->\n  This is a line.\n  Another line.<--\n"
			"  --quote Someone famous-->\n  The quote!\n  <--\n"
			"  --toc level=2 Table of Contents-->Text below<--\n"
			"  --image source=nice.png align=right A nice image-->Probably a non-existing image<--\n"
			"  --hidden Don't peek!-->This text is hidden!<--\n" );
		Stringifier strf;
		strf.hardLineWidth( 80 );
		output.str( strf );
		auto act = strf.generateString();
		auto exp =
			"data(\n"
			"  (type=insert, insert=code, lang=\"C++\", title=\"A code sample\",\n"
			"  value=\"One-liner\"), (type=insert, insert=example, title=\"Something amazing\",\n"
			"    value:\n"
			"      \"This is a line.\", \"Another line.\"),\n"
			"  (type=insert, insert=quote, source=\"Someone famous\", value=\"The quote!\"),\n"
			"  (type=insert, insert=toc, level=2, title=\"Table of Contents\",\n"
			"  value=\"Text below\"), (type=insert, insert=image, source=\"nice.png\",\n"
			"  align=right, title=\"A nice image\", value=\"Probably a non-existing image\"),\n"
			"  (type=insert, insert=hidden, title=\"Don't peek!\",\n"
			"  value=\"This text is hidden!\"))";
		WANT_EQ( exp, act );
	}
	TEST( EdocTest, insert2 )
	{
		edoc doc;
		auto output = doc.parse(
			u8"  --code Æon A code sample-->\n"
			u8"  for each i in range(from 1 to 100):\n"
			u8"    write(i to stdout);\n"
			u8"  <--\n" );
		Stringifier strf;
		output.str( strf );
		auto act = strf.generateString();
		auto exp =
			u8"data(\n"
			u8"  (type=insert, insert=code, lang=\"Æon\", title=\"A code sample\",\n"
			u8"    value:\n"
			u8"      \"for each i in range(from 1 to 100):\", \"  write(i to stdout);\"))";
		WANT_EQ( exp, act );
	}

	TEST( EdocTest, plain_text )
	{
		edoc doc;
		auto output = doc.parse( "Plain text\nMore plain text.\n\nNew paragraph!" );
		Stringifier strf;
		strf.hardLineWidth( 80 );
		output.str( strf );
		auto act = strf.generateString();
		auto exp{
			"data(\n"
			"  (type=paragraph,\n"
			"    value:\n"
			"      (type=text, value=\"Plain text\"), (type=text,\n"
			"        value:\n"
			"          (type=newline), (type=text, value=\"More plain text.\"))),\n"
			"  (type=paragraph,\n"
			"    value:\n"
			"      (type=text, value=\"New paragraph!\")))" };
		WANT_EQ( exp, act );
	}
	TEST( EdocTest, emphasize )
	{
		edoc doc;
		auto output = doc.parse( "*BOLD*\nA *bold* move with *bold* text.\nalpha !*beta* *gamma* * delta *epsilon*\n not*bold*" );
		Stringifier strf;
		strf.hardLineWidth( 80 );
		output.str( strf );
		auto act = strf.generateString();
		auto exp =
			"data(\n"
			"  (type=paragraph,\n"
			"    value:\n"
			"      (type=text,\n"
			"        value:\n"
			"          (type=emphasized, value=\"BOLD\")),\n"
			"      (type=text,\n"
			"        value:\n"
			"          (type=newline), (type=text, value=\"A \"), (type=emphasized,\n"
			"          value=\"bold\"), (type=text, value=\" move with \"), (type=emphasized,\n"
			"          value=\"bold\"), (type=text, value=\" text.\")),\n"
			"      (type=text,\n"
			"        value:\n"
			"          (type=newline), (type=text, value=\"alpha *beta* \"), (type=emphasized,\n"
			"          value=\"gamma\"), (type=text, value=\" * delta \"), (type=emphasized,\n"
			"          value=\"epsilon\")),\n"
			"      (type=text,\n"
			"        value:\n"
			"          (type=newline), (type=text, value=\" not*bold*\"))))";
		WANT_EQ( exp, act );
	}
	TEST( EdocTest, emphasize_1 )
	{
		edoc doc;
		auto output = doc.parse( "(*BOLD*)" );
		Stringifier strf;
		output.str( strf );
		auto act = strf.generateString();
		auto exp =
			"data(\n"
			"  (type=paragraph,\n"
			"    value:\n"
			"      (type=text,\n"
			"        value:\n"
			"          (type=text, value=\"(\"), (type=emphasized, value=\"BOLD\"), (type=text, value=\")\"))))";
		WANT_EQ( exp, act );
	}
	TEST( EdocTest, quote )
	{
		edoc doc;
		auto output = doc.parse(
			"\"QUOTED\"\nA \"quoted\" text with \" quoted contents\".\nalpha \"beta\" \"gamma  delta \"epsilon\"" );
		Stringifier strf;
		strf.hardLineWidth( 80 );
		output.str( strf );
		auto act = strf.generateString();
		auto exp =
			"data(\n"
			"  (type=paragraph,\n"
			"    value:\n"
			"      (type=text,\n"
			"        value:\n"
			"          (type=quoted, value=\"QUOTED\")),\n"
			"      (type=text,\n"
			"        value:\n"
			"          (type=newline), (type=text, value=\"A \"), (type=quoted,\n"
			"          value=\"quoted\"), (type=text, value=\" text with \"), (type=quoted,\n"
			"          value=\" quoted contents\"), (type=text, value=\".\")),\n"
			"      (type=text,\n"
			"        value:\n"
			"          (type=newline), (type=text, value=\"alpha \"), (type=quoted,\n"
			"          value=\"beta\"), (type=text, value=\" \"), (type=quoted,\n"
			"          value=\"gamma  delta \"), (type=text, value=\"epsilon\\\"\"))))";
		WANT_EQ( exp, act );
	}
	TEST( EdocTest, reference )
	{
		edoc doc;
		auto output = doc.parse(
			"[REFERENCE]\n"
			"A [reference] text with [key:references in it].\nalpha ![noref] ![nokey:noreference]\n"
			"A [http:http://somewhere.com] reference." );
		Stringifier strf;
		strf.hardLineWidth( 80 );
		output.str( strf );
		auto act = strf.generateString();
		auto exp =
			"data(\n"
			"  (type=paragraph,\n"
			"    value:\n"
			"      (type=text,\n"
			"        value:\n"
			"          (type=reference, target=\"REFERENCE\")),\n"
			"      (type=text,\n"
			"        value:\n"
			"          (type=newline), (type=text, value=\"A \"), (type=reference,\n"
			"          target=\"reference\"), (type=text, value=\" text with \"),\n"
			"          (type=reference, caption=\"key\", target=\"references in it\"),\n"
			"          (type=text, value=\".\")),\n"
			"      (type=text,\n"
			"        value:\n"
			"          (type=newline), (type=text,\n"
			"          value=\"alpha [noref] [nokey:noreference]\")),\n"
			"      (type=text,\n"
			"        value:\n"
			"          (type=newline), (type=text, value=\"A \"), (type=reference,\n"
			"          caption=\"http\", target=\"http://somewhere.com\"), (type=text,\n"
			"          value=\" reference.\"))))";
		WANT_EQ( exp, act );
	}
	TEST( EdocTest, http )
	{
		edoc doc;
		auto output = doc.parse(
			"ftp://here.we.are\nA http://link.com with https://some.or-other?thing&todo+here.\n!http://nolink.com" );
		Stringifier strf;
		strf.hardLineWidth( 80 );
		output.str( strf );
		auto act = strf.generateString();
		auto exp =
			"data(\n"
			"  (type=paragraph,\n"
			"    value:\n"
			"      (type=text,\n"
			"        value:\n"
			"          (type=reference, target=\"ftp://here.we.are\")),\n"
			"      (type=text,\n"
			"        value:\n"
			"          (type=newline), (type=text, value=\"A \"), (type=reference,\n"
			"          target=\"http://link.com\"), (type=text, value=\" with \"),\n"
			"          (type=reference, target=\"https://some.or-other?thing&todo+here\"),\n"
			"          (type=text, value=\".\")),\n"
			"      (type=text,\n"
			"        value:\n"
			"          (type=newline), (type=text, value=\"http://nolink.com\"))))";
		WANT_EQ( exp, act );
	}


	TEST( ToHtmlTest, headers )
	{
		auto output = edoc().parse( ">> Header1\n>> > Header2\n>> >> Header 3\n>> >>> Header #4" );
		ToHtml tohtml;
		string act = tohtml.convert( output );
		string exp{
			"<div class=\"eon_frame\">\n"
			"<h1 class=\"eon_h1\" id=\"Header1\">Header1</h1>\n"
			"<h2 class=\"eon_h2\" id=\"Header2\">Header2</h2>\n"
			"<h3 class=\"eon_h3\" id=\"Header_3\">Header 3</h3>\n"
			"<h4 class=\"eon_h4\" id=\"Header_#4\">Header #4</h4>\n"
			"</div>\n" };
		WANT_EQ( exp, act );
	}

	TEST( ToHtmlTest, bullet_list )
	{
		auto output = edoc().parse( "  * Item 1\n  * Item 2\n  * Item 3\n" );
		ToHtml tohtml;
		string act = tohtml.convert( output );
		string exp{
			"<div class=\"eon_frame\">\n"
			"<ul style=\"list-style-type:disc;\">\n"
			"  <li>Item 1</li>\n"
			"  <li>Item 2</li>\n"
			"  <li>Item 3</li>\n"
			"</ul>\n"
			"</div>\n" };
		WANT_EQ( exp, act );
	}
	TEST( ToHtmlTest, dash_list )
	{
		auto output = edoc().parse( "  - Item 1\n  - Item 2\n  - Item 3\n" );
		ToHtml tohtml;
		string act = tohtml.convert( output );
		string exp{ "<div class=\"eon_frame\">\n<ul style=\"list-style-type:square;\">\n"
			"  <li>Item 1</li>\n  <li>Item 2</li>\n  <li>Item 3</li>\n</ul>\n</div>\n" };
		WANT_EQ( exp, act );
	}
	TEST( ToHtmlTest, dash_enum )
	{
		auto output = edoc().parse( "  1 Item 1\n  2 Item 2\n  33 Item 3\n" );
		ToHtml tohtml;
		string act = tohtml.convert( output );
		string exp{ "<div class=\"eon_frame\">\n<ol>\n"
			"  <li>Item 1</li>\n  <li>Item 2</li>\n  <li>Item 3</li>\n</ol>\n</div>\n" };
		WANT_EQ( exp, act );
	}

	TEST( ToHtmlTest, exclamation )
	{
		auto output = edoc().parse( "NOTE: This is a note!\nWARNING: A warning\nTODO: Do this!\nTIP: Nice tip!\n"
			"NOTE: Can span\n  multiple lines!" );
		ToHtml tohtml;
		string act = tohtml.convert( output );
		string exp{ "<div class=\"eon_frame\">\n"
			"<div class=\"eon_shout\">\n"
			"  <div class=\"eon_note\">NOTE!</div>\n  <div class=\"eon_note_text\">This is a note!</div>\n</div>\n"
			"<div class=\"eon_shout\">\n"
			"  <div class=\"eon_warning\">WARNING!</div>\n  <div class=\"eon_warning_text\">A warning</div>\n</div>\n"
			"<div class=\"eon_shout\">\n"
			"  <div class=\"eon_todo\">TODO!</div>\n  <div class=\"eon_todo_text\">Do this!</div>\n</div>\n"
			"<div class=\"eon_shout\">\n"
			"  <div class=\"eon_tip\">TIP!</div>\n  <div class=\"eon_tip_text\">Nice tip!</div>\n</div>\n"
			"<div class=\"eon_shout\">\n"
			"  <div class=\"eon_note\">NOTE!</div>\n  <div class=\"eon_note_text\">Can span multiple lines!</div>\n</div>\n"
			"</div>\n" };
		WANT_EQ( exp, act );
	}

	TEST( ToHtmlTest, definition )
	{
		auto output = edoc().parse( "  alpha: The first letter in the Greek alphabet\n"
			"  two words: Example of two word definition\n  multiline:\n    Line 1\n    Line 2\n"
			"  !anonymous def: One\n  \"double quoted\":\n    Indented and anonymous\n\nWhat is alpha? Is it two words?" );
		ToHtml tohtml;
		string act = tohtml.convert( output );
		string exp{
			"<div class=\"eon_frame\">\n"
			"<div class=\"eon_define\" id=\"alpha\">\n"
			"  <div class=\"eon_phrase\"><span class=\"eon_phrase_normal\">alpha</span> :</div>\n"
			"  <div class=\"eon_definition\">The first letter in the Greek alphabet</div>\n"
			"</div>\n"
			"<div class=\"eon_define\" id=\"two_words\">\n"
			"  <div class=\"eon_phrase\"><span class=\"eon_phrase_normal\">two words</span> :</div>\n"
			"  <div class=\"eon_definition\">Example of two word definition</div>\n"
			"</div>\n"
			"<div class=\"eon_define\" id=\"multiline\">\n"
			"  <div class=\"eon_phrase\"><span class=\"eon_phrase_normal\">multiline</span> :</div>\n"
			"  <div class=\"eon_definition\">Line 1 Line 2</div>\n"
			"</div>\n"
			"<div class=\"eon_define\">\n"
			"  <div class=\"eon_phrase\"><span class=\"eon_phrase_anonymous\">anonymous def</span> :</div>\n"
			"  <div class=\"eon_definition\">One</div>\n"
			"</div>\n"
			"<div class=\"eon_define\">\n"
			"  <div class=\"eon_phrase\"><span class=\"eon_phrase_quoted\">double quoted</span> :</div>\n"
			"  <div class=\"eon_definition\">Indented and anonymous</div>\n</div>\n"
			"<p class=\"eon_paragraph\">\n"
			"  What is <span class=\"eon_def_popup\" title=\"The first letter in the Greek alphabet\">alpha</span>? Is it "
				"<span class=\"eon_def_popup\" title=\"Example of two word definition\">two words</span>?\n</p>\n"
			"</div>\n" };
		WANT_EQ( exp, act );
	}

	TEST( ToHtmlTest, insert )
	{
		auto output = edoc().parse(
			"  --code C++ A code sample-->One-liner<--\n"
			"  --example Something amazing-->\n  This is a line.\n  Another line.<--\n"
			"  --quote Someone famous-->\n  The quote!\n  <--\n"
			"  --toc level=2 Table of Contents-->Text below<--\n"
			"  --image source=nice.png align=right A nice image-->Probably a non-existing image<--\n"
			"  --hidden Don't peek!-->This text is hidden!<--\n" );
		ToHtml tohtml;
		string act = tohtml.convert( output );
		string exp{
			"<div class=\"eon_frame\">\n"
			"<div class=\"eon_insert_frame\">\n"
			"<div class=\"eon_insert_header_line\">"
				"<span class=\"eon_insert_lead\">Code:</span>"
				"<span class=\"eon_code_lang\">C++</span><span class=\"eon_insert_title\">A code sample</span>"
				"</div>\n"
			"<pre class=\"eon_code\">\n"
			"One-liner</pre>\n"
			"</div>\n"
			"<div class=\"eon_insert_frame\">\n"
			"<div class=\"eon_insert_header_line\">"
				"<span class=\"eon_insert_lead\">Example!</span>"
				"<span class=\"eon_insert_title\">Something amazing</span>"
				"</div>\n"
			"  <div class=\"eon_example\">\n"
			"    This is a line.<br />\n"
			"    Another line.\n"
			"  </div>\n"
			"</div>\n"
			"<div class=\"eon_insert_frame\">\n"
			"<div class=\"eon_insert_header_line\">"
				"<span class=\"eon_insert_lead\">Quote!</span>"
				"<span class=\"eon_insert_title\">Someone famous</span>"
				"</div>\n"
			"  <div class=\"eon_quote\">\n"
			"    The quote!\n"
			"  </div>\n"
			"</div>\n"
			"<div class=\"eon_insert_frame\">\n"
			"<div class=\"eon_insert_header_line\">"
				"<span class=\"eon_insert_title\">Table of Contents</span></div>\n"
			"  <div class=\"eon_toc\">\n"
			"  </div>\n"
			"  <div class=\"eon_insert_subtext\">\n"
			"    Text below\n"
			"  </div>\n"
			"</div>\n"
			"<div class=\"eon_insert_frame\">\n"
			"<div class=\"eon_insert_header_line\">"
				"<span class=\"eon_insert_title\">A nice image</span></div>\n"
			"  <div class=\"eon_image\" style=\"float:right;\"><img src=\"nice.png\" alt=\"A nice image\" /></div>\n"
			"  <div class=\"eon_insert_subtext\">\n    Probably a non-existing image\n  </div>\n</div>\n"
			"</div>\n" };
		WANT_EQ( exp, act );
	}

	TEST( ToHtmlTest, toc )
	{
		auto output = edoc().parse(
			">> !Introduction\n"
			"  --toc level=2 Table of Contents-->Text below<--\n"
			">> One\n"
			">> > One point one\n"
			">> >> One point one point one\n"
			">> > One point two\n"
			">> Two\n" );
		ToHtml tohtml;
		string act = tohtml.convert( output );
		string exp{
			"<div class=\"eon_frame\">\n"
			"<h1 class=\"eon_h1\" id=\"Introduction\">Introduction</h1>\n"
			"<div class=\"eon_insert_frame\">\n"
			"<div class=\"eon_insert_header_line\">"
				"<span class=\"eon_insert_title\">Table of Contents</span></div>\n"
			"  <div class=\"eon_toc\">\n"
			"    <div class=\"eon_toc1\"><a href=\"#One\">One</a></div>\n"
			"    <div class=\"eon_toc2\"><a href=\"#One_point_one\">One point one</a></div>\n"
			"    <div class=\"eon_toc2\"><a href=\"#One_point_two\">One point two</a></div>\n"
			"    <div class=\"eon_toc1\"><a href=\"#Two\">Two</a></div>\n"
			"  </div>\n"
			"  <div class=\"eon_insert_subtext\">\n    Text below\n  </div>\n"
			"</div>\n"
			"<h1 class=\"eon_h1\" id=\"One\">One</h1>\n"
			"<h2 class=\"eon_h2\" id=\"One_point_one\">One point one</h2>\n"
			"<h3 class=\"eon_h3\" id=\"One_point_one_point_one\">One point one point one</h3>\n"
			"<h2 class=\"eon_h2\" id=\"One_point_two\">One point two</h2>\n"
			"<h1 class=\"eon_h1\" id=\"Two\">Two</h1>\n"
			"</div>\n" };
		WANT_EQ( exp, act );
	}

	TEST( ToHtmlTest, plain_text )
	{
		auto output = edoc().parse( "Plain text\nMore plain text.\n\nNew paragraph!" );

		ToHtml tohtml;
		string act = tohtml.convert( output );
		string exp{ "<div class=\"eon_frame\">\n"
			"<p class=\"eon_paragraph\">\n"
			"  Plain text<br />\n"
			"  More plain text.\n"
			"</p>\n"
			"<p class=\"eon_paragraph\">\n  New paragraph!\n</p>\n"
			"</div>\n" };
		WANT_EQ( exp, act );
	}
	TEST( ToHtmlTest, emphasize )
	{
		auto output = edoc().parse(
			"*BOLD*\nA *bold* move with *bold* text.\nalpha !*beta* *gamma* * delta *epsilon*\n not*bold*" );
		ToHtml tohtml;
		string act = tohtml.convert( output );
		string exp{
			"<div class=\"eon_frame\">\n"
			"<p class=\"eon_paragraph\">\n"
			"  <span class=\"eon_emphasized\">BOLD</span><br />\n"
			"  A <span class=\"eon_emphasized\">bold</span> move with <span class=\"eon_emphasized\">bold</span> text.<br />\n"
			"  alpha *beta* <span class=\"eon_emphasized\">gamma</span> * delta <span class=\"eon_emphasized\">epsilon</span><br />\n"
			"   not*bold*\n"
			"</p>\n"
			"</div>\n" };
			WANT_EQ( exp, act );
	}
	TEST( ToHtmlTest, quote )
	{
		auto output = edoc().parse(
			"\"QUOTED\"\nA \"quoted\" text with \" quoted contents\".\nalpha \"beta\" \"gamma  delta \"epsilon\"" );
		ToHtml tohtml;
		string act = tohtml.convert( output );
		string exp{
			"<div class=\"eon_frame\">\n"
			"<p class=\"eon_paragraph\">\n"
			"  <span class=\"eon_quoted\">\"QUOTED\"</span><br />\n"
			"  A <span class=\"eon_quoted\">\"quoted\"</span> text with <span class=\"eon_quoted\">\" quoted contents\"</span>.<br />\n"
			"  alpha <span class=\"eon_quoted\">\"beta\"</span> <span class=\"eon_quoted\">\"gamma  delta \"</span>epsilon\"\n"
			"</p>\n"
			"</div>\n" };
		WANT_EQ( exp, act );
	}
	TEST( ToHtmlTest, reference )
	{
		auto output = edoc().parse(
			"[REFERENCE]\n"
			"A [reference] text with [key:references in it].\nalpha ![noref] ![nokey:noreference]\n"
			"A [http:http://somewhere.com] reference." );
		ToHtml tohtml;
		string act = tohtml.convert( output );
		string exp{
			"<div class=\"eon_frame\">\n"
			"<p class=\"eon_paragraph\">\n"
			"  <a href=\"#REFERENCE\" class=\"eon_reference\">REFERENCE</a><br />\n"
			"  A <a href=\"#reference\" class=\"eon_reference\">reference</a> text with <a href=\"#references_in_it\" class=\"eon_reference\">key</a>.<br />\n"
			"  alpha [noref] [nokey:noreference]<br />\n"
			"  A <a href=\"http://somewhere.com\" class=\"eon_reference\">http</a> reference.\n"
			"</p>\n"
			"</div>\n" };
		WANT_EQ( exp, act );
	}
	TEST( ToHtmlTest, http )
	{
		auto output = edoc().parse(
			"ftp://here.we.are\nA http://link.com with https://some.or-other?thing&todo+here.\n!http://nolink.com" );
		ToHtml tohtml;
		string act = tohtml.convert( output );
		string exp{
			"<div class=\"eon_frame\">\n"
			"<p class=\"eon_paragraph\">\n"
			"  <a href=\"ftp://here.we.are\" class=\"eon_reference\">ftp://here.we.are</a><br />\n"
			"  A <a href=\"http://link.com\" class=\"eon_reference\">http://link.com</a> with <a href=\"https://some.or-other?thing&todo+here\" class=\"eon_reference\">https://some.or-other?thing&todo+here</a>.<br />\n"
			"  http://nolink.com\n"
			"</p>\n"
			"</div>\n" };
		WANT_EQ( exp, act );
	}

	TEST( ToHtmlFileTest, document )
	{
		string input{
			u8">> !This is a Test\n"
			"  --toc Table of Contents-->Text below<--\n"
			">> First Chapter\n"
			"This is the first chapter.\n"
			"It has two lines.\n"
			">> > New header under first chapter\n"
			"More text, *some is emphasized*, we also have a [link that leads nowhere].\n"
			"But a link to [First header:This is a Test] should work!\n"
			"\n"
			"New paragraph. It should be thoroughly separated from the previous text.\n"
			"  --code Æon Let's have some code-->\n"
			"  mylist.append( elm ) for each elm in yourlist;\n"
			"  if not empty(mylist):\n"
			"    write(mylist's size to stdout);\n"
			"  <--\n"
			"\n"
			">> New header\n"
			"Here is a bullet list:\n"
			"  * First item\n"
			"  * Second item from the top\n"
			"  * Third and last item\n"
			"\n"
			"Let's also see a dash list (which will have squares in HTML):\n"
			"  - Dashing item number one\n"
			"  - The second-most dashing *bold* item!\n"
			"\n"
			"We must also have an enumerated list:\n"
			"  1 First item\n"
			"  33 Second item\n"
			"  # Third item\n"
			"  9 Fourth item\n"
			"\n"
			"NOTE: This is a note spanning two lines\n"
			"      in the input!\n"
			"WARNING: Be warned about crazy things!\n"
			"TODO:\n"
			"  Check if things are OK\n"
			"TIP: Follow this up\n"
			"\n"
			">> > Definitions\n"
			"Below, we define the phrase \"alpha\", but before we have used alpha, it will not be available for lookup.\n"
			"\n"
			"  alpha: The first letter in the Greek alphabet\n"
			"\n"
			"Now, alpha should have a tooltip, at least!\n"
			"\n"
			"  !This is not for lookup: Just an indented line with colon and following text.\n"
			"  This should be: Simple phrase to look up.\n"
			"  \"Should not be\": Another inactive definition.\n"
			"\n"
			">> >> Down to third level\n"
			"In the following, we shall look at more inserts.\n"
			"\n"
			">> >>> Example\n"
			"This is an example:\n"
			"  --example Showing an example-->This is the example text!<--\n"
			"\n"
			"  --example Another one-->\n"
			"  Here, we have\n"
			"  two lines!\n"
			"  <--\n"
			">> >>> A Quote\n"
			"  --quote Shakespear-->To be or not to be<--\n"
			"\n"
			">> >>> Image\n"
			"  --image source=http://aaseweb.no/eon_logo_64.png The Æon logo-->It is very small!<--\n"
			">> Another top level section\n"
			"We can add a link to https://wikipedia.org, or we can add the same link with a link [caption:https://wikipedia.org].\n"
		};
		Tuple dt;
		REQUIRE_NO_EXCEPT( dt = edoc().parse( input ) ) << "Failed to parse EDOC!";
		file output{ path( sandbox() ) / "output.html" };
		REQUIRE_NO_EXCEPT( output.save( ToHtml().makeDocument( dt, "This is a test" ) ) );
	}
}
