#include "Regression.h"



namespace eon
{
	TEST( EtfTest, empty_unnamed_01 )
	{
		string in{ "---" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_FALSE( doc ) << "Got document contents";
	}
	TEST( EtfTest, empty_unnamed_02 )
	{
		string in{ "---\n" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_FALSE( doc ) << "Got document contents";
	}
	TEST( EtfTest, empty_named_01 )
	{
		string in{ "---title" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_title ) );
		auto& doc = docs.doc( name_title );
		REQUIRE_FALSE( doc ) << "Got document contents";
	}
	TEST( EtfTest, empty_named_02 )
	{
		string in{ "---       title              \n" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_TRUE( docs.exists( one ) );
		WANT_TRUE( docs.exists( two ) );
	}

	TEST( IntTest, true )
	{
		string in{ "true" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_EXCEPT( docs.parse( in, name_test ), EtfBadSyntax )
			<< "Failed to detect bad syntax";
	}
	TEST( RegexTest, basic )
	{
		string in{ R"(?/./)" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_EXCEPT( docs.parse( in, name_test ), EtfBadSyntax )
			<< "Failed to detect bad syntax";
	}
	TEST( RegexTest, boundary )
	{
		string in{ R"(?~^/b~)" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_EXCEPT( docs.parse( in, name_test ), EtfBadSyntax )
			<< "Failed to detect bad syntax";
	}
	TEST( RefTest, one )
	{
		string in{ "@one" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();

		auto elm = doc.find( { name::get( "c" ) } );
		REQUIRE_TRUE( elm ) << "Didn't get value referenced by 'c'";
		WANT_TRUE( elm->hardName() == name::get( "one" ) )
			<< "Wrong value referenced by 'c'";
	}
	TEST( RefTest, find_two_steps )
	{
		string in{ "a=one\nb=@a\nc=@b" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );

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
		etf docs;
		REQUIRE_EXCEPT( docs.parse( in, name_test ),
			tup::CircularReferencing );
	}

	TEST( VarTest, basic )
	{
		string in{ "a=b" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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

	TEST( TupleTest, empty )
	{
		string in{ "{}" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( TupleTest, single_line )
	{
		string in{ "{one two three four}" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "{one two three four}", out.stdstr() );
	}
	TEST( TupleTest, single_line_too_long )
	{
		string in{ "{one two three four five six seven eight nine ten eleven twelve thirteen fourteen fifteen}" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "- one\n  two\n  three\n  four\n  five\n  six\n  seven\n  eight\n  nine\n  ten\n  eleven\n  twelve\n  thirteen\n  fourteen\n  fifteen", out.stdstr() );
	}
	TEST( TupleTest, unnamed )
	{
		string in{ "one\ntwo\nthree\nfour" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "{one}\n{two three}\n{four five six}", out.stdstr() );
	}
	TEST( TupleTest, unnamed_unnamed_large )
	{
		string in{ "- one\n  number=1\n  alpha\n  uno\n- two\n  number=2\n  beta\n  dos\n- three\n  number=3\n  gamma\n  tres\n- four\n  number=4\n  delta\n  quatro" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "a={uno=one}\nb={dos=two}", out.stdstr() );
	}

	TEST( MetaTest, empty)
	{
		string in{ "<>" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
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
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}
	TEST( MetaTest, large )
	{
		string in{ "<type=signed_int, format=?/[+\\-]\\d+/ options={alpha, beta gamma}>" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( "<type=signed_int format=?/[+\\-]\\d+/ options={alpha beta gamma}>", out.stdstr() );
	}
	TEST( MetaTest, for_name )
	{
		string in{ "a<alpha>=\"a\"" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		REQUIRE_TRUE( docs.exists( name_test ) );
		auto& doc = docs.doc( name_test );
		REQUIRE_TRUE( doc ) << "Got no document";
		auto out = doc.str();
		WANT_EQ( in.stdstr(), out.stdstr() );
	}

	TEST( ValidateTest, max_depth_good )
	{
		string in{ "root<max_depth=1>=\n  (child1)\n  (child2)" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, max_depth_bad )
	{
		string in{ "root<max_depth=1>=\n  {child1}\n  child2=\n    {child21}\n    {child22}" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, min_length_string_good )
	{
		string in{ "root<min_length=5>=\"12345\"" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, min_length_string_bad )
	{
		string in{ "root<min_length=5>=\"1234\"" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, min_length_binary_good )
	{
		string in{ "root<min_length=5>=%5465737421" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, min_length_binary_bad )
	{
		string in{ "root<min_length=5>=%54657374" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, min_length_tuple_good )
	{
		string in{ "root<min_length=5>=\n  (child1)\n  (child2)\n  (child3)\n  (child4)\n  (child5)" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, min_length_tuple_bad )
	{
		string in{ "root<min_length=5>=\n  (child1)\n  (child2)\n  (child3)\n  (child4)" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, max_length_string_good )
	{
		string in{ "root<max_length=5>=\"12345\"" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, max_length_string_bad )
	{
		string in{ "root<max_length=5>=\"123456\"" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, max_length_binary_good )
	{
		string in{ "root<max_length=5>=%5465737421" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, max_length_binary_bad )
	{
		string in{ "root<max_length=5>=%546573742120" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, max_length_tuple_good )
	{
		string in{ "root<max_length=5>=\n  (child1)\n  (child2)\n  (child3)\n  (child4)\n  (child5)" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, max_length_tuple_bad )
	{
		string in{ "root<max_length=5>=\n  (child1)\n  (child2)\n  (child3)\n  (child4)\n  (child5)\n  (child6)" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, min_int_good )
	{
		string in{ "root<min=5>=5" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, min_int_bad )
	{
		string in{ "root<min=5>=4" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, min_float_good )
	{
		string in{ "root<min=5.5>=5.5" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, min_float_bad )
	{
		string in{ "root<min=5.5>=5.4" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, max_int_good )
	{
		string in{ "root<max=5>=5" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, max_int_bad )
	{
		string in{ "root<max=5>=6" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, max_float_good )
	{
		string in{ "root<max=5.5>=5.5" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, max_float_bad )
	{
		string in{ "root<max=5.5>=5.6" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, format_string_good )
	{
		string in{ R"(root<format=?/\d{4}-\d\d-\d\d/>="2021-05-19")" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, format_string_bad )
	{
		string in{ R"(root<format=?/\d{4}\-\d\d\-\d\d/>="2021-5-19")" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, format_raw_good )
	{
		string in{ R"(root<format=?/\d{4}-\d\d-\d\d/>=|2021-05-19)" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, format_raw_bad )
	{
		string in{ R"(root<format=?/\d{4}\-\d\d\-\d\d/>=|2021-5-19)" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, flags_good )
	{
		string in{ "root<flags={one two three}>={two one}" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, flags_bad )
	{
		string in{ "root<flags={one two three}>={two four}" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}
	TEST( ValidateTest, options_good )
	{
		string in{ "root<options={one two three}>=two" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test ) ) << "Failed to validate";
	}
	TEST( ValidateTest, options_bad )
	{
		string in{ "root<options={one two three}>=four" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test ), tup::Invalid )
			<< "Failed to detect invalid document";
	}

	TEST( ValidateTest, pattern_good_1 )
	{
		auto pattern = name::get( "ptrn" );
		string in{ "---ptrn<pattern>\nname=<type=name>\nage=<type=int min=10>"
			"\n---\nname=John\nage=30" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test, pattern ) )
			<< "Failed to validate";
	}
	TEST( ValidateTest, pattern_good_2 )
	{
		auto pattern = name::get( "ptrn" );
		string in{ "---ptrn<pattern>\nname=\n  first=<type=name>\n  last=\"Doe\""
			"\n---\nname=\n  first=John\n  last=\"Doe\"" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test, pattern ) )
			<< "Failed to validate";
	}
	TEST( ValidateTest, pattern_good_3 )
	{
		auto pattern = name::get( "ptrn" );
		string in{ "---ptrn<pattern>\nnames={one two three}"
			"\n---\nnames={one two three}" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_NO_EXCEPT( docs.validate( name_test, pattern ) )
			<< "Failed to validate";
	}
	TEST( ValidateTest, pattern_bad_1 )
	{
		auto pattern = name::get( "ptrn" );
		string in{ "---ptrn<pattern>\nname=<type=name>\nage=<type=int min=10>"
			"\n---\nname=John\nage=9" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test, pattern ), tup::Invalid )
			<< "Failed to validate";
	}
	TEST( ValidateTest, pattern_bad_2 )
	{
		auto pattern = name::get( "ptrn" );
		string in{ "---ptrn<pattern>\nname=<type=name>\nage=<type=int min=10>"
			"\n---\nname=\"John\"\nage=30" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test, pattern ), tup::Invalid )
			<< "Failed to validate";
	}
	TEST( ValidateTest, pattern_bad_3 )
	{
		auto pattern = name::get( "ptrn" );
		string in{ "---ptrn<pattern>\nname=<type=name>\nage=<type=int min=10>"
			"\n---\nname=John\nage=30.5" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test, pattern ), tup::Invalid )
			<< "Failed to validate";
	}
	TEST( ValidateTest, pattern_bad_4)
	{
		auto pattern = name::get( "ptrn" );
		string in{ "---ptrn<pattern>\nname=\n  first=<type=name>\n  last=\"Doe\""
			"\n---\nname=\n  first=John\n  last=\"doe\"" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test, pattern ), tup::Invalid )
			<< "Failed to validate";
	}
	TEST( ValidateTest, pattern_bad_5 )
	{
		auto pattern = name::get( "ptrn" );
		string in{ "---ptrn<pattern>\nnames={one two three}"
			"\n---\nnames={one tw three}" };
		etf docs;
		REQUIRE_NO_EXCEPT( docs.parse( in, name_test ) )
			<< "Failed to parse";
		WANT_EXCEPT( docs.validate( name_test, pattern ), tup::Invalid )
			<< "Failed to validate";
	}
}
