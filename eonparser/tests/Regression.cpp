#include "Regression.h"



namespace eon
{
	TEST( RawParserTest, bool )
	{
		type::Handler::init();
		string raw{ "true" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();
		WANT_EQ( "bool=true", format( elements ) );
	}
	TEST( RawParserTest, byte )
	{
		type::Handler::init();
		string raw{ "b'h'" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();
		WANT_EQ( "byte=b'h'", format( elements ) );
	}
	TEST( RawParserTest, char )
	{
		type::Handler::init();
		string raw{ "'H'" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();
		WANT_EQ( "char='H'", format( elements ) );
	}
	TEST( RawParserTest, int )
	{
		type::Handler::init();
		string raw{ "9876" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();
		WANT_EQ( "int=9876", format( elements ) );
	}
	TEST( RawParserTest, float )
	{
		type::Handler::init();
		string raw{ "3.14" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();
		WANT_EQ( "float=3.14", format( elements ) );
	}
	TEST( RawParserTest, name )
	{
		type::Handler::init();
		string raw{ "hello" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();
		WANT_EQ( "name=hello", format( elements ) );
	}
	TEST( RawParserTest, bytes )
	{
		type::Handler::init();
		string raw{ "b\"abc\"" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();
		WANT_EQ( "bytes=b\"abc\"", format( elements ) );
	}
	TEST( RawParserTest, string )
	{
		type::Handler::init();
		string raw{ "\"abc\"" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();
		WANT_EQ( "string=\"abc\"", format( elements ) );
	}
	TEST( RawParserTest, regex )
	{
		type::Handler::init();
		string raw{ "r\"\\w+\"" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();
		WANT_EQ( "regex=r\"\\w+\"", format( elements ) );
	}
	TEST( RawParserTest, namepath )
	{
		type::Handler::init();
		string raw{ "@alpha/beta" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();
		WANT_EQ( "namepath=@alpha/beta", format( elements ) );
	}
	TEST( RawParserTest, path )
	{
		type::Handler::init();
		string raw{ "p\"alpha/beta\"" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();
		WANT_EQ( "path=p\"alpha/beta\"", format( elements ) );
	}

	TEST( RawParserTest, expression )
	{
		type::Handler::init();
		string raw{ "(1 + 2 * 3) / 4" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();
		WANT_EQ( "open|int=1|operator=+|int=2|operator=*|int=3|close|operator=/|int=4", format( elements ) );
	}

	TEST( RawParserTest, indentation )
	{
		type::Handler::init();
		string raw{ "a =\n  b + c\n  + d" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();
		WANT_EQ( "name=a|operator==|newline|indentation=1|name=b|operator=+|name=c|newline|indentation=1|operator=+|name=d",
			format( elements ) );
	}


	TEST( ParseExpressionTest, basic_singleton )
	{
		type::Handler::init();
		string raw{ "3.14" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();

		scope::Global global;
		parser::ToolBox tools( std::move( elements ), rep, global );
		parser::ParseExpression pex;
		type::Node root;
		REQUIRE_NO_EXCEPT( pex( tools, root ) );
		WANT_EQ( "3.14", root.str() );
	}

	TEST( ParseExpressionTest, basic_expression )
	{
		type::Handler::init();
		string raw{ "1 + 2" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();

		scope::Global global;
		registerPrimitives( global );
		registerBasicTupleActions( global );
		registerStandardTypes( global );
		parser::ToolBox tools( std::move( elements ), rep, global );
		parser::ParseExpression pex;
		type::Node root;
		REQUIRE_NO_EXCEPT( pex( tools, root ) );
		WANT_EQ( "1 + 2", root.str() );
	}
	TEST( ParseExpressionTest, sub_expression )
	{
		type::Handler::init();
		string raw{ "(1 + 2 * 3) / 4" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();

		scope::Global global;
		registerPrimitives( global );
		registerBasicTupleActions( global );
		registerStandardTypes( global );
		parser::ToolBox tools( std::move( elements ), rep, global );
		parser::ParseExpression pex;
		type::Node root;
		REQUIRE_NO_EXCEPT( pex( tools, root ) );
		WANT_EQ( "(1 + 2 * 3) / 4", root.str() );
	}
	TEST( ParseExpressionTest, bad_sub_expression )
	{
		type::Handler::init();
		string raw{ "1 + 2 * 3) / 4" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();

		scope::Global global;
		registerPrimitives( global );
		registerBasicTupleActions( global );
		registerStandardTypes( global );
		parser::ToolBox tools( std::move( elements ), rep, global );
		parser::ParseExpression pex;
		type::Node root;
		REQUIRE_FALSE( pex( tools, root ) );
	}

	TEST( ParseExpressionTest, plaintuples_in_expression )
	{
		type::Handler::init();
		string raw{ "p(1, 2) + p(3, 4)" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();

		scope::Global global;
		registerPrimitives( global );
		registerBasicTupleActions( global );
		registerStandardTypes( global );
		parser::ToolBox tools( std::move( elements ), rep, global );
		parser::ParseExpression pex;
		type::Node root;
		REQUIRE_NO_EXCEPT( pex( tools, root ) );
		WANT_EQ( "p(1, 2) + p(3, 4)", root.str() );
	}
	TEST( ParseExpressionTest, datatuples_in_expression )
	{
		type::Handler::init();
		string raw{ "data(1, 2) + data(3, 4)" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();

		scope::Global global;
		registerPrimitives( global );
		registerBasicTupleActions( global );
		registerStandardTypes( global );
		parser::ToolBox tools( std::move( elements ), rep, global );
		parser::ParseExpression pex;
		type::Node root;
		REQUIRE_NO_EXCEPT( pex( tools, root ) );
		WANT_EQ( "data(1, 2) + data(3, 4)", root.str() );
	}
	TEST( ParseExpressionTest, metadatatuples_in_expression )
	{
		type::Handler::init();
		string raw{ "meta(1, 2) + meta(3, 4)" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();

		scope::Global global;
		registerPrimitives( global );
		registerBasicTupleActions( global );
		registerStandardTypes( global );
		parser::ToolBox tools( std::move( elements ), rep, global );
		parser::ParseExpression pex;
		type::Node root;
		REQUIRE_NO_EXCEPT( pex( tools, root ) );
		WANT_EQ( "meta(1, 2) + meta(3, 4)", root.str() );
	}
	TEST( ParseExpressionTest, dynamictuples_in_expression )
	{
		type::Handler::init();
		string raw{ "dynamic(1, 2) + dynamic(3, 4)" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();

		scope::Global global;
		registerPrimitives( global );
		registerBasicTupleActions( global );
		registerStandardTypes( global );
		parser::ToolBox tools( std::move( elements ), rep, global );
		parser::ParseExpression pex;
		type::Node root;
		REQUIRE_NO_EXCEPT( pex( tools, root ) );
		WANT_EQ( "dynamic(1, 2) + dynamic(3, 4)", root.str() );
	}

	TEST( ParseExpressionTest, expression_as_value )
	{
		type::Handler::init();
		string raw{ "define a = e(1 + 2)" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();

		scope::Global global;
		registerPrimitives( global );
		registerBasicTupleActions( global );
		registerStandardTypes( global );
		parser::ToolBox tools( std::move( elements ), rep, global );
		parser::ParseExpression pex;
		type::Node root;
		REQUIRE_NO_EXCEPT( pex( tools, root ) );
		WANT_EQ( "define a = e(1 + 2)", root.str() );
	}

	TEST( ParseExpressionTest, implicit_plain_tuples1 )
	{
		type::Handler::init();
		string raw{ "define a = 1, 2" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();

		scope::Global global;
		registerPrimitives( global );
		registerBasicTupleActions( global );
		registerStandardTypes( global );
		parser::ToolBox tools( std::move( elements ), rep, global );
		parser::ParseExpression pex;
		type::Node root;
		REQUIRE_NO_EXCEPT( pex( tools, root ) );
		WANT_EQ( "define a = p(1, 2)", root.str() );
	}
	TEST( ParseExpressionTest, implicit_plain_tuples2 )
	{
		type::Handler::init();
		string raw{ "1, 2 + 3, 4" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();

		scope::Global global;
		registerPrimitives( global );
		registerBasicTupleActions( global );
		registerStandardTypes( global );
		parser::ToolBox tools( std::move( elements ), rep, global );
		parser::ParseExpression pex;
		type::Node root;
		REQUIRE_NO_EXCEPT( pex( tools, root ) );
		WANT_EQ( "p(1, 2 + 3, 4)", root.str() );
	}

	TEST( ParseExpressionTest, define_as )
	{
		type::Handler::init();
		string raw{ "define a as string" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp;
		REQUIRE_NO_EXCEPT( rawp = parser::Raw( src, rep ) );
		auto elements = rawp();

		scope::Global global;
		registerPrimitives( global );
		registerBasicTupleActions( global );
		registerStandardTypes( global );
		parser::ToolBox tools( std::move( elements ), rep, global );
		parser::ParseExpression pex;
		type::Node root;
		REQUIRE_NO_EXCEPT( pex( tools, root ) );
		WANT_EQ( "define a as string", root.str() );
	}

/*	TEST( ParseExpressionTest, edt_1 )
	{
		type::Handler::init();
		string raw{ "test (EDT, version=0,0,1):\n  Hello" };
		source::String src( "string", std::move( raw ) );
		source::Reporter rep;
		rep.add( new source::StdOut( source::severity::fatal, source::severity::note ) );
		parser::Raw rawp( src, rep );
		auto elements = rawp();

		scope::Global global;
		registerPrimitives( global );
		registerBasicTupleActions( global );
		registerStandardTypes( global );
		parser::ToolBox tools( std::move( elements ), rep, global );
		parser::ParseExpression pex;
		type::Node root;
		REQUIRE_NO_EXCEPT( pex( tools, root ) );
		WANT_EQ( "test (EDT, version=0,0,1):\n  Hello", root.str() );
	}*/




	string RawParserTest::format( const std::vector<parser::Element>& elements )
	{
		string str;
		for( auto& elm : elements )
		{
			if( !str.empty() )
				str += "|";
			if( elm.isName() )
				str += eon::str( elm.type() ) + "=" + eon::str( elm.nameValue() );
			else if( elm.isNumber() )
				str += eon::str( elm.type() ) + "=" + string( elm.numValue() );
			else if( elm.isOperator() )
				str += eon::str( elm.type() ) + "=" + type::operators::mapCode( elm.opValue() );
			else if( elm.isSymbol() )
				str += eon::str( elm.type() ) + "=" + elm.symbValue();
			else
			{
				str += eon::str( elm.type() );
				if( elm.objValue() )
				{
//					type::Stringifier strf;
//					elm.objValue()->str( strf );
//					str += "=" + strf.output();
				}
			}
		}
		return str;
	}
}
