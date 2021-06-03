#include "Regression.h"



namespace eon
{
	TEST( PathTest, parse_empty )
	{
		string exp = "";
		eon::path path( exp );
		WANT_EQ( exp.stdstr(), path.stdstr() );
	}

	TEST( PathTest, parse_protocol )
	{
		string exp = "file://";
		eon::path path( exp );
		REQUIRE_EQ( exp.stdstr(), path.stdstr() );
		WANT_EQ( "file", path.protocol().stdstr() )
			<< "Wrong protocol element";
	}

	TEST( PathTest, parse_unix_root )
	{
		string exp = "/home";
		eon::path path( exp );
		REQUIRE_EQ( exp.stdstr(), path.stdstr() );
		WANT_EQ( "/", path.root().stdstr() ) << "Wrong root element";
	}
	TEST( PathTest, parse_win_root )
	{
		string exp = "d:";
		eon::path path( exp );
		REQUIRE_EQ( exp.stdstr(), path.stdstr() );
		WANT_EQ( exp.stdstr(), path.root().stdstr() ) << "Wrong root element";
	}

	TEST( PathTest, parse_elements )
	{
		string exp = "one/two/three/";
		eon::path path( exp );
		REQUIRE_EQ( exp, *path );
		WANT_EQ( "", path.protocol().stdstr() ) << "Should not have protocol";
		WANT_EQ( "", path.root().stdstr() ) << "Should not have root";
		WANT_EQ( "", path.base().stdstr() ) << "Should not have base";
		WANT_EQ( "", path.ext().stdstr() ) << "Should not have extension";
	}
	TEST( PathTest, parse_base )
	{
		string exp = "one";
		eon::path path( exp );
		REQUIRE_EQ( exp.stdstr(), path.stdstr() );
		WANT_EQ( exp.stdstr(), path.base().stdstr() ) << "Wrong base element";
	}
	TEST( PathTest, parse_extension )
	{
		string exp = ".one";
		eon::path path( exp );
		REQUIRE_EQ( exp.stdstr(), path.stdstr() );
		WANT_EQ( "one", path.ext().stdstr() ) << "Wrong extension element";
	}
	TEST( PathTest, parse_complete )
	{
		string exp = "https://one.two.three/four/five/six.seven";
		eon::path path( exp );
		REQUIRE_EQ( exp.stdstr(), path.stdstr() );
		WANT_EQ( "https", path.protocol().stdstr() ) << "Wrong protocol";
		WANT_EQ( "six.seven", path.base().stdstr() ) << "Wrong base";
		WANT_EQ( "six", path.baseWithoutExt().stdstr() )
			<< "Wrong base without extension";
		WANT_EQ( "seven", path.ext().stdstr() ) << "Wrong extension element";
	}
	TEST( PathTest, append_1 )
	{
		eon::path path( "one/two/three" );
		eon::path other( "four/five" );
		path += other;
		WANT_EQ( "one/two/threefour/five", path.stdstr() );
	}
	TEST( PathTest, append_2 )
	{
		eon::path path( "one/two/three" );
		eon::path other( "four/five" );
		path += "/";
		path += other;
		WANT_EQ( "one/two/three/four/five", path.stdstr() );
	}
	TEST( PathTest, append_3 )
	{
		eon::path path( "one/two/three" );
		eon::path other( "four/five" );
 		path += "/four/five";
		WANT_EQ( "one/two/three/four/five", path.stdstr() );
	}
	TEST( PathTest, parent )
	{
		eon::path path( "one/two/three" );
		auto parent = path.parent();
		WANT_EQ( "one/two", parent.stdstr() ) << "Wrong parent";
	}
	TEST( PathTest, removeProtocol )
	{
		eon::path path( "file://one/two/three" );
		path.removeProtocol();
		WANT_EQ( "one/two/three", path.stdstr() ) << "Wrong path";
	}
	TEST( PathTest, removeBase )
	{
		eon::path path( "one/two/three" );
		path.removeBase();
		WANT_EQ( "one/two", path.stdstr() ) << "Wrong path";
	}
	TEST( PathTest, removeExtension )
	{
		eon::path path( "one/two/three.four" );
		path.removeExt();
		WANT_EQ( "one/two/three", path.stdstr() ) << "Wrong path";
	}
}
