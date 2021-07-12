#include "Regression.h"



namespace eon
{
	TEST( EtfTest, empty_unnamed_01 )
	{
		string in{ "---" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_FALSE( doc ) << "Got document contents";
	}
	TEST( EtfTest, empty_unnamed_02 )
	{
		string in{ "---\n" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_FALSE( doc ) << "Got document contents";
	}
	TEST( EtfTest, empty_named_01 )
	{
		string in{ "---title" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_title ) );
		auto& doc = docs.doc( name_title );
		REQUIRE_FALSE( doc ) << "Got document contents";
	}
	TEST( EtfTest, empty_named_02 )
	{
		string in{ "---       title              \n" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_title ) );
		auto& doc = docs.doc( name_title );
		REQUIRE_FALSE( doc ) << "Got document contents";
	}
	TEST( EtfTest, empty_multiple_named_01 )
	{
		string in{ "---one\n---two\n" };
		auto one = name::get( "one" );
		auto two = name::get( "two" );
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_TRUE( docs.exists( one ) );
		WANT_TRUE( docs.exists( two ) );
	}

	TEST( IntTest, true )
	{
		string in{ "true" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( IntTest, false )
	{
		string in{ "false" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}

	TEST( CharTest, null )
	{
		string in{ "'\\0'" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( CharTest, F )
	{
		string in{ "'F'" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( CharTest, unicode )
	{
		string in = string( "'\\U050E'" );
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( string( u8"'\U0000050E'" ).stdstr(), out.stdstr() );
	}

	TEST( IntTest, zero )
	{
		string in{ "0" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( IntTest, positive )
	{
		string in{ "+45" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "45", out.stdstr() );
	}
	TEST( IntTest, negative )
	{
		string in{ "-16" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}

	TEST( FloatTest, zero )
	{
		string in{ "0.0" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( FloatTest, trailing_zeros )
	{
		string in{ "0.0001000" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "0.0001", out.stdstr() );
	}
	TEST( FloatTest, positive )
	{
		string in{ "+5.98" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "5.98", out.stdstr() );
	}
	TEST( FloatTest, negative )
	{
		string in{ "-9.87" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}

	TEST( NameTest, basic )
	{
		string in{ "name" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}

	TEST( StringTest, empty )
	{
		string in{ "\"\"" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( StringTest, plain )
	{
		string in{ "\"a string\"" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( StringTest, spaces )
	{
		string in{ "\"      x       \"" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( StringTest, escapes )
	{
		string in{ "\"a \\\"string\\\"\"" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( StringTest, more_escapes )
	{
		string in{ "\"\\tSeveral \\\"\\n escapes'\\b\\v\"" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "\"	Several \\\"\\n escapes'\\b\\v\"", out.stdstr() );
	}

	TEST( RawTest, empty )
	{
		string in{ "|" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( RawTest, basic1 )
	{
		string in{ "|One line of raw" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( RawTest, basic2 )
	{
		string in{ "|\n  One line of raw" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "|One line of raw", out.stdstr() );
	}
	TEST( RawTest, soft_break )
	{
		string in{ "|\n  One line\\\n   of raw" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "|One line of raw", out.stdstr() );
	}
	TEST( RawTest, hard_break )
	{
		string in{ "|\n  Two lines\n  of raw" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( RawTest, multiline )
	{
		string in{ "|\n  one\n  two\n  three\n  four" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}

	TEST( BinaryTest, empty )
	{
		string in{ "%" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( BinaryTest, plain )
	{
		string in{ "%54657374" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( BinaryTest, split_long )
	{
		std::string bin{ "This is a long binary text that we want to see as two lines in ETF!" };
		std::string exp{ "% 546869732069732061206C6F6E672062696E617279207465787420746861742077652077616E\n  7420746F207365652061732074776F206C696E657320696E2045544621" };
		string in{ "%" + hex( bin ).value() };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( exp, out.stdstr() );
		WANT_EQ( bin, doc.attribute( 0 )->binary_value().binary() );
	}

	TEST( RegexTest, empty )
	{
		string in{ R"(?//)" };
		eonof docs;
		REQUIRE_EXCEPT( docs.parseExclusive( in, name_test ), EtfBadSyntax )
			<< "Failed to detect bad syntax";
	}
	TEST( RegexTest, basic )
	{
		string in{ R"(?/./)" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( RegexTest, complex )
	{
		string in{ R"(?/^\d{1,4}\-\d{1,2}\-\d{1,2}T\d{1,2}:\d{1,2}(:\d{1,2})?$/)" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( RegexTest, flags_legal )
	{
		string in{ R"(?/./ilsSo)" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( RegexTest, flags_illegal )
	{
		string in{ R"(?/./ilsxo)" };
		eonof docs;
		REQUIRE_EXCEPT( docs.parseExclusive( in, name_test ), EtfBadSyntax )
			<< "Failed to detect bad syntax";
	}
	TEST( RegexTest, boundary )
	{
		string in{ R"(?~^/b~)" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}

	TEST( RefTest, empty )
	{
		string in{ "@" };
		eonof docs;
		REQUIRE_EXCEPT( docs.parseExclusive( in, name_test ), EtfBadSyntax )
			<< "Failed to detect bad syntax";
	}
	TEST( RefTest, one )
	{
		string in{ "@one" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( RefTest, two )
	{
		string in{ "@one/two" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( RefTest, underscore )
	{
		string in{ "@mappers/scan_include" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( RefTest, ref )
	{
		string in{ "a=one\nb=two\nc=@a" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( RefTest, find_one_step )
	{
		string in{ "a=one\nb=two\nc=@a" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		tup::variables vars;
		REQUIRE_NO_EXCEPT( doc.resolveAllReferences( vars ) );

		auto elm = doc.find( { name::get( "c" ) } );
		REQUIRE_TRUE( elm ) << "Didn't get value referenced by 'c'";
		WANT_TRUE( elm->hardName() == name::get( "one" ) )
			<< "Wrong value referenced by 'c'";
	}
	TEST( RefTest, find_two_steps )
	{
		string in{ "a=one\nb=@a\nc=@b" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		tup::variables vars;
		REQUIRE_NO_EXCEPT( docs.doc( name_test ).resolveAllReferences( vars ) );

		auto elm = docs.find(
			{ name::get( "docs" ), name_test, name::get( "c" ) } );
		REQUIRE_TRUE( elm ) << "Didn't get 'c'";
		REQUIRE_TRUE( elm->isName() ) << "Found value of 'c' must be a 'name'";
		WANT_TRUE( elm->hardName() == name::get( "one" ) )
			<< "Wrong value referenced by 'c'";
	}
	TEST( RefTest, find_circular )
	{
		string in{ "a=@c\nb=@a\nc=@b" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) );
		tup::variables vars;
		REQUIRE_EXCEPT( docs.doc( name_test ).resolveAllReferences( vars ),
			tup::CircularReferencing );
	}

	TEST( VarTest, basic )
	{
		string in{ "a=b" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}

	TEST( ExprTest, basic )
	{
		string in{ "x=(2 - 4)" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
		auto x = doc.find( { name::get( "x" ) } );
		REQUIRE_TRUE( x ) << "Didn't find 'x'";
		REQUIRE_TRUE( x->isExpression() ) << "Wrong type for 'x'";
		auto& expr = x->hardExpression();
		tup::valueptr result;
		REQUIRE_NO_EXCEPT( result = expr.evaluate() ) << "Failed to evaluate";
		REQUIRE_TRUE( result && result->isInt() ) << "Bad result";
		WANT_EQ( -2, result->hardInt() ) << "Wrong value";
	}
	TEST( ExprTest, pi )
	{
		string in{ "x<var=pi>=3.14\ny<var=rad>=45.2\ncircumference=(2.0 * pi * rad)" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
		auto circumference = doc.find( { name::get( "circumference" ) } );
		REQUIRE_TRUE( circumference ) << "Didn't find 'circumference'";
		REQUIRE_TRUE( circumference->isExpression() ) << "Wrong type for 'circumference'";
		auto& expr = circumference->hardExpression();
		tup::valueptr result;
		REQUIRE_NO_EXCEPT( result = expr.evaluate( docs.variables() ) ) << "Failed to evaluate";
		REQUIRE_TRUE( result && result->isFloat() ) << "Bad result";
		WANT_EQ( 2.0 * 3.14 * 45.2, result->hardFloat() ) << "Wrong value";
		REQUIRE_EQ( 2.0 * 3.14 * 45.2, circumference->softFloat( docs.variables() ) );
	}
	TEST( ExprTest, invalid )
	{
		// Missing closing ')' in expression
		string in{ "loop<while(token != '}'>=one" };
		eonof docs;
		REQUIRE_EXCEPT( docs.parseExclusive( in, name_test ), EtfBadSyntax )
			<< "Failed to parse";
	}

	TEST( TupleTest, empty )
	{
		string in{ "{}" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( TupleTest, singleton )
	{
		string in{ "{one}" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( TupleTest, single_line )
	{
		string in{ "a={one two three four}" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( TupleTest, single_line_comma )
	{
		string in{ "{one, two, three, four}" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "{one two three four}", out.stdstr() );
	}
	TEST( TupleTest, multiline )
	{
		string in{ "first=\n  - one\n    two\n    three\n  - alpha\n    beta\nsecond=2" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( TupleTest, single_line_too_long )
	{
		string in{ "{one two three four five six seven eight nine ten eleven twelve thirteen fourteen fifteen}" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "{one two three four five six seven eight nine ten eleven twelve thirteen\n  fourteen fifteen}", out.stdstr() );
	}
	TEST( TupleTest, unnamed )
	{
		string in{ "one\ntwo\nthree\nfour" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( TupleTest, unnamed_unnamed )
	{
		string in{ "{one}\n{two three}\n{four five six}" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( TupleTest, unnamed_unnamed_dash )
	{
		string in{ "- one\n- two\n  three\n- four\n  five\n  six" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( TupleTest, unnamed_unnamed_large )
	{
		string in{ "- one\n  number=1\n  alpha\n  uno\n- two\n  number=2\n  beta\n  dos\n- three\n  number=3\n  gamma\n  tres\n- four\n  number=4\n  delta\n  quatro" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( TupleTest, named )
	{
		string in{ "a=one\nb=two" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( TupleTest, named_named )
	{
		string in{ "a=\n  uno=one\nb=\n  dos=two" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( TupleTest, exclusive1 )
	{
		string in{ "---test\none=1\n---test\none=2" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "one=2", out.stdstr() );
	}
	TEST( TupleTest, exclusive2 )
	{
		string in{ "---test\none={alpha, beta}\n---test\none={alpha gamma}" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "one={alpha gamma}", out.stdstr() );
	}
	TEST( TupleTest, merge1 )
	{
		string in{ "---test\none=1\n---test\none=2" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseMerge( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "one=2", out.stdstr() );
	}
	TEST( TupleTest, merge2 )
	{
		string in{ "---test\none={alpha=1, beta=2}\n---test\none={alpha=1, gamma=3}" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseMerge( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "one={alpha=1 beta=2 gamma=3}", out.stdstr() );
	}
	TEST( TupleTest, levels1 )
	{
		string in{ "alpha=\n  one=\n    - <a b>\n    <c d>\n  two=\n    - <e f>\n    gh\n  three=\n    - <i j>" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( TupleTest, levels2 )
	{
		string in{ "- quoted=\n    <match '\"'>\n    alpha\n  braced=\n    <match '<'>\n    beta" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( TupleTest, levels3 )
	{
		string in{ "mappers=\n  //- *Includes* are references to other files, that the doc-gen should\n  //- follow.\n  include=\n    <skip {space '\n'}>\n    <match '#'>                     // Match the hash sign\n    <skip space>\n    <match include>                 // Match \"include\"\n    <skip space>\n    - quoted=                       // Sub-section of alternate options\n        <match '\"'>                // Start with double quotes\n        (_path=\"\")                // Create/reset '_path' variable\n        loop<while(token != '\"')>=\n          <append (token) to _path> // Build path from tokens\n          <skip>                    // Move forward to next token\n        (trim _path)                // Remove leading and trailing spaces\n        <match '\"'>                // Match the finishing double quote\n        @mappers/scan_include       // Scan the file\n      braced=                       // Second option for what to match next\n        <match '<'>                 // Opening angle brace\n        loop<while(token != '>')>=\n          <append (token) to _path> // Build path from tokens\n          <skip>                    // Move forward to next token\n        (trim _path)                // Remove leading and trailing spaces\n        <match '>'>                 // And the closing angle brace\n        @mappers/scan_include       // Scan the file\n  //- Scan included source file\n  scan_include=\n    if<(exists _path)>=\n      <scan (_path)>\n      <return>\n    loop<list=known_include_paths element=include_path>=\n      (file_path = include_path + '/' + _path)\n      if<(exists file_path)>=\n        <scan (file_path)>\n        <return>" };
		string exp{ "mappers=\n  include=\n    <skip {space '\\n'}>\n    <match '#'>\n    <skip space>\n    <match include>\n    <skip space>\n    - quoted=\n        <match '\"'>\n        (_path = \"\")\n        loop<while (token != '\"')>=\n          <append (token) to _path>\n          <skip>\n        (trim _path)\n        <match '\"'>\n        @mappers/scan_include\n      braced=\n        <match '<'>\n        loop<while (token != '>')>=\n          <append (token) to _path>\n          <skip>\n        (trim _path)\n        <match '>'>\n        @mappers/scan_include\n  scan_include=\n    if<(exists _path)>=\n      <scan (_path)>\n      <return>\n    loop<list=known_include_paths element=include_path>=\n      (file_path = include_path + '/' + _path)\n      if<(exists file_path)>=\n        <scan (file_path)>\n        <return>" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( exp.stdstr(), out.stdstr() );
	}
	TEST( TupleTest, levels4 )
	{
		string in{ "alpha=\n\n  one=a\n\n  two=b" };
//		string in{ "output=\"EDOC\"\noutput_suffix=\".edoc\"\nsource_type=\"C++\"\nsource_pattern=?/.+\\.h/i\nmappers=\n  include=\n    <skip {space '\\n'}>\n    <match '#'>\n    <skip space>\n    <match include>\n    <skip space>\n    - quoted=\n        <match '\"'>\n        (_path = \"\")\n        loop<while (token != '\"')>=\n          <append (token) to _path>\n          <skip>\n        (trim _path)\n        <match '\"'>\n        @mappers/scan_include\n      braced=\n        <match '<'>\n        loop<while (token != '>')>=\n          <append (token) to _path>\n          <skip>\n        (trim _path)\n        <match '>'>\n        @mappers/scan_include\n  scan_include=\n    if<(exists _path)>=\n      <scan (_path)>\n      <return>\n    loop<list=known_include_paths element=include_path>=\n      (file_path = include_path + '/' + _path)\n      if<(exists file_path)>=\n        <scan (file_path)>\n        <return>\n\n  documentation_comments=\n    loop<while (token)>=\n      <skip {space '\\n'}>\n      <match '/'>\nscan=\n  loop<while(token)>={@mappers/global}" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "alpha=\n  one=a\n  two=b", out.stdstr() );
	}
	TEST( TupleTest, metaval1 )
	{
		string in{ "include=<skip {space '\\n'}>" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( TupleTest, metaval2 )
	{
		string in{ "include=\n  <skip {space '\\n'}>\n  <another>" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}

	TEST( MetaTest, empty)
	{
		string in{ "<>" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( MetaTest, basic )
	{
		string in{ "<alpha>" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( MetaTest, large1 )
	{
		string in{ "<type=signed_int, format=?/[+\\-]\\d+/ options={alpha, beta gamma}>" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "<type=signed_int format=?/[+\\-]\\d+/ options={alpha beta gamma}>", out.stdstr() );
	}
	TEST( MetaTest, large2 )
	{
		string in{ "<type={alpha beta gamma delta\n  epsilon zeta eta theta}>" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "<type={alpha beta gamma delta epsilon zeta eta theta}>", out.stdstr() );
	}
	TEST( MetaTest, for_name )
	{
		string in{ "a<alpha>=\"a\"" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( MetaTest, multiline )
	{
		string in{
			"loop<init (i=1), (type=toname(\"tpl_spec_type_\" + i)),\n"
			"  (name=toname(\"tpl_spec_name_\" + i)), while(i<num_tpl_specs),"
			" (i=i+1)>=one" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "loop<init (i = 1) (type = toname \"tpl_spec_type_\" + i)\n  "
			"(name = toname \"tpl_spec_name_\" + i) while (i < num_tpl_specs) "
			"(i = i + 1)>=one", out.stdstr() );
	}

	TEST( ValidateTest, max_depth_good )
	{
		string in{ "root<max_depth=1>=\n  (child1)\n  (child2)" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, max_depth_bad )
	{
		string in{ "root<max_depth=1>=\n  {child1}\n  child2=\n    {child21}\n    {child22}" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, min_length_string_good )
	{
		string in{ "root<min_length=5>=\"12345\"" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, min_length_string_bad )
	{
		string in{ "root<min_length=5>=\"1234\"" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, min_length_binary_good )
	{
		string in{ "root<min_length=5>=%5465737421" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, min_length_binary_bad )
	{
		string in{ "root<min_length=5>=%54657374" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, min_length_tuple_good )
	{
		string in{ "root<min_length=5>=\n  (child1)\n  (child2)\n  (child3)\n  (child4)\n  (child5)" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, min_length_tuple_bad )
	{
		string in{ "root<min_length=5>=\n  (child1)\n  (child2)\n  (child3)\n  (child4)" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, max_length_string_good )
	{
		string in{ "root<max_length=5>=\"12345\"" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, max_length_string_bad )
	{
		string in{ "root<max_length=5>=\"123456\"" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, max_length_binary_good )
	{
		string in{ "root<max_length=5>=%5465737421" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, max_length_binary_bad )
	{
		string in{ "root<max_length=5>=%546573742120" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, max_length_tuple_good )
	{
		string in{ "root<max_length=5>=\n  (child1)\n  (child2)\n  (child3)\n  (child4)\n  (child5)" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, max_length_tuple_bad )
	{
		string in{ "root<max_length=5>=\n  (child1)\n  (child2)\n  (child3)\n  (child4)\n  (child5)\n  (child6)" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, min_int_good )
	{
		string in{ "root<min=5>=5" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, min_int_bad )
	{
		string in{ "root<min=5>=4" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, min_float_good )
	{
		string in{ "root<min=5.5>=5.5" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, min_float_bad )
	{
		string in{ "root<min=5.5>=5.4" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, max_int_good )
	{
		string in{ "root<max=5>=5" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, max_int_bad )
	{
		string in{ "root<max=5>=6" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, max_float_good )
	{
		string in{ "root<max=5.5>=5.5" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, max_float_bad )
	{
		string in{ "root<max=5.5>=5.6" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, format_string_good )
	{
		string in{ R"(root<format=?/\d{4}-\d\d-\d\d/>="2021-05-19")" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, format_string_bad )
	{
		string in{ R"(root<format=?/\d{4}\-\d\d\-\d\d/>="2021-5-19")" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, format_raw_good )
	{
		string in{ R"(root<format=?/\d{4}-\d\d-\d\d/>=|2021-05-19)" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, format_raw_bad )
	{
		string in{ R"(root<format=?/\d{4}\-\d\d\-\d\d/>=|2021-5-19)" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, flags_good )
	{
		string in{ "root<flags={one two three}>={two one}" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, flags_bad )
	{
		string in{ "root<flags={one two three}>={two four}" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, options_good )
	{
		string in{ "root<options={one two three}>=two" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, options_bad )
	{
		string in{ "root<options={one two three}>=four" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}

	TEST( ValidateTest, pattern_good_1 )
	{
		auto pattern = name::get( "ptrn" );
		string in{ "---ptrn<pattern>\nname=<type=name>\nage=<type=int min=10>"
			"\n---\nname=John\nage=30" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test, pattern ) )
			<< "Failed to validate";
	}
	TEST( ValidateTest, pattern_good_2 )
	{
		auto pattern = name::get( "ptrn" );
		string in{ "---ptrn<pattern>\nname=\n  first=<type=name>\n  last=\"Doe\""
			"\n---\nname=\n  first=John\n  last=\"Doe\"" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test, pattern ) )
			<< "Failed to validate";
	}
	TEST( ValidateTest, pattern_good_3 )
	{
		auto pattern = name::get( "ptrn" );
		string in{ "---ptrn<pattern>\nnames={one two three}"
			"\n---\nnames={one two three}" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test, pattern ) )
			<< "Failed to validate";
	}
	TEST( ValidateTest, pattern_bad_1 )
	{
		auto pattern = name::get( "ptrn" );
		string in{ "---ptrn<pattern>\nname=<type=name>\nage=<type=int min=10>"
			"\n---\nname=John\nage=9" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test, pattern ), tup::Invalid )
			<< "Failed to validate";
	}
	TEST( ValidateTest, pattern_bad_2 )
	{
		auto pattern = name::get( "ptrn" );
		string in{ "---ptrn<pattern>\nname=<type=name>\nage=<type=int min=10>"
			"\n---\nname=\"John\"\nage=30" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test, pattern ), tup::Invalid )
			<< "Failed to validate";
	}
	TEST( ValidateTest, pattern_bad_3 )
	{
		auto pattern = name::get( "ptrn" );
		string in{ "---ptrn<pattern>\nname=<type=name>\nage=<type=int min=10>"
			"\n---\nname=John\nage=30.5" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test, pattern ), tup::Invalid )
			<< "Failed to validate";
	}
	TEST( ValidateTest, pattern_bad_4)
	{
		auto pattern = name::get( "ptrn" );
		string in{ "---ptrn<pattern>\nname=\n  first=<type=name>\n  last=\"Doe\""
			"\n---\nname=\n  first=John\n  last=\"doe\"" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test, pattern ), tup::Invalid )
			<< "Failed to validate";
	}
	TEST( ValidateTest, pattern_bad_5 )
	{
		auto pattern = name::get( "ptrn" );
		string in{ "---ptrn<pattern>\nnames={one two three}"
			"\n---\nnames={one tw three}" };
		eonof docs;
		REQUIRE_NO_EXCEPT( docs.parseExclusive( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test, pattern ), tup::Invalid )
			<< "Failed to validate";
	}
}
