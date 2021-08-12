#include "Regression.h"



namespace eon
{
	TEST( PathTest, basic1 )
	{
		WANT_NO_EXCEPT( path p{} ) << "No path";
		WANT_NO_EXCEPT( path p{ "" } ) << "Empty path";
		WANT_NO_EXCEPT( path p{ "/" } ) << "Unix root";
		WANT_NO_EXCEPT( path p{ "C:" } ) << "Windows root, short";
		WANT_NO_EXCEPT( path p{ "C:/" } ) << "Windows root, long";
		WANT_NO_EXCEPT( path p{ "//somehost" } ) << "UNC root";
		WANT_NO_EXCEPT( path p{ "a/relative/path" } ) << "Relative";
		WANT_NO_EXCEPT( path p{ "//some/absolute/path" } ) << "Absolute";
		WANT_NO_EXCEPT( path p{ "//some/../any/./absolute/path" } )
			<< "Dirty absolute";
		WANT_NO_EXCEPT( path p{ "some/../any/./relative/path" } )
			<< "Dirty relative";
		WANT_NO_EXCEPT( path p{ "C:\\some\\windows\\path" } )
			<< "Back-slashes";
	}
	TEST( PathTest, basic2 )
	{
		WANT_EQ( "", path{}.stdstr() ) << "No path";
		WANT_EQ( "", path{ "" }.stdstr() ) << "Empty path";
		WANT_EQ( "/", path{ "/" }.stdstr() ) << "Unix root";
		WANT_EQ( "C:/", path{ "C:" }.stdstr() ) << "Windows root, short";
		WANT_EQ( "C:/", path{ "C:/" }.stdstr() ) << "Windows root, long";
		WANT_EQ( "//somehost/", path{ "//somehost" }.stdstr() ) << "UNC root";
		WANT_EQ( "a/relative/path", path{ "a/relative/path" }.stdstr() )
			<< "Relative";
		WANT_EQ( "//some/absolute/path",
			path{ "//some/absolute/path" }.stdstr() ) << "Absolute";
		WANT_EQ( "//some/any/absolute/path",
			path{ "//some/../any/./absolute/path" }.stdstr() ) << "Dirty";
		WANT_EQ( "any/relative/path",
			path{ "some/../any/./relative/path" }.stdstr() ) << "Dirty";
		WANT_EQ( "C:/some/windows/path",
			path{ "C:\\some\\windows\\path" }.stdstr() ) << "Back-slashes";
	}
	TEST( PathTest, disect )
	{
		path p1;
		path p2{ "" };
		path p3{ "/" };
		path p4{ "C:/" };
		path p5{ "//somehost" };
		path p6{ "a/relative/path" };
		path p7{ "//some/absolute/path" };
		path p8{ "C:\\some\\windows\\path.ex" };

		WANT_FALSE( p1.hasRoot() );
		WANT_FALSE( p1.hasDirs() );
		WANT_FALSE( p1.hasParent() );
		WANT_FALSE( p1.hasBase() );
		WANT_FALSE( p1.hasName() );
		WANT_FALSE( p1.hasExt() );

		WANT_FALSE( p2.hasRoot() );
		WANT_FALSE( p2.hasDirs() );
		WANT_FALSE( p2.hasParent() );
		WANT_FALSE( p2.hasBase() );
		WANT_FALSE( p2.hasName() );
		WANT_FALSE( p2.hasExt() );

		WANT_TRUE( p3.hasRoot() );
		WANT_EQ( "/", p3.root().stdstr() );
		WANT_FALSE( p3.hasDirs() );
		WANT_TRUE( p3.hasParent() );
		WANT_EQ( "/", p3.parent().stdstr() );
		WANT_FALSE( p3.hasBase() );
		WANT_FALSE( p3.hasName() );
		WANT_FALSE( p3.hasExt() );

		WANT_TRUE( p4.hasRoot() );
		WANT_EQ( "C:/", p4.root().stdstr() );
		WANT_FALSE( p4.hasDirs() );
		WANT_TRUE( p4.hasParent() );
		WANT_EQ( "C:/", p4.parent().stdstr() );
		WANT_FALSE( p4.hasBase() );
		WANT_FALSE( p4.hasName() );
		WANT_FALSE( p4.hasExt() );

		WANT_TRUE( p5.hasRoot() );
		WANT_EQ( "//somehost/", p5.root().stdstr() );
		WANT_FALSE( p5.hasDirs() );
		WANT_TRUE( p5.hasParent() );
		WANT_EQ( "//somehost/", p5.parent().stdstr() );
		WANT_FALSE( p5.hasBase() );
		WANT_FALSE( p5.hasName() );
		WANT_FALSE( p5.hasExt() );

		WANT_FALSE( p6.hasRoot() );
		WANT_TRUE( p6.hasDirs() );
		WANT_EQ( "a/relative/", p6.dirs().stdstr() );
		WANT_TRUE( p6.hasParent() );
		WANT_EQ( "a/relative/", p6.parent().stdstr() );
		WANT_TRUE( p6.hasBase() );
		WANT_EQ( "path", p6.base().stdstr() );
		WANT_TRUE( p6.hasName() );
		WANT_EQ( "path", p6.name().stdstr() );
		WANT_FALSE( p6.hasExt() );

		WANT_TRUE( p7.hasRoot() );
		WANT_EQ( "//some/", p7.root().stdstr() );
		WANT_TRUE( p7.hasDirs() );
		WANT_EQ( "absolute/", p7.dirs().stdstr() );
		WANT_TRUE( p7.hasParent() );
		WANT_EQ( "//some/absolute/", p7.parent().stdstr() );
		WANT_TRUE( p7.hasBase() );
		WANT_EQ( "path", p7.base().stdstr() );
		WANT_TRUE( p7.hasName() );
		WANT_EQ( "path", p7.name().stdstr() );
		WANT_FALSE( p7.hasExt() );

		WANT_TRUE( p8.hasRoot() );
		WANT_EQ( "C:/", p8.root().stdstr() );
		WANT_TRUE( p8.hasDirs() );
		WANT_EQ( "some/windows/", p8.dirs().stdstr() );
		WANT_TRUE( p8.hasParent() );
		WANT_EQ( "C:/some/windows/", p8.parent().stdstr() );
		WANT_TRUE( p8.hasBase() );
		WANT_EQ( "path.ex", p8.base().stdstr() );
		WANT_TRUE( p8.hasName() );
		WANT_EQ( "path", p8.name().stdstr() );
		WANT_TRUE( p8.hasExt() );
		WANT_EQ( "ex", p8.ext().stdstr() );
	}

	TEST( FileSysTest, dir_basic )
	{
		REQUIRE_FALSE( testdir.exists() ) << "Dir exists before create";
		REQUIRE_NO_EXCEPT( testdir.ensureExists() ) << "Failed to create dir";
		REQUIRE_TRUE( testdir.exists() ) << "Dir doesn't exist after create";
		REQUIRE_NO_EXCEPT( testdir.remove() ) << "Failed to remove dir";
		REQUIRE_FALSE( testdir.exists() ) << "Dir exists after remove";
	}
	TEST( FileSysTest, dir_rename)
	{
		REQUIRE_NO_EXCEPT( testdir.ensureExists() )
			<< "Failed to create test dir";

		directory dir = testdir.dpath() + "alpha/";
		REQUIRE_NO_EXCEPT( dir.ensureExists() ) << "Failed to create dir";
		REQUIRE_TRUE( dir.exists() ) << "Dir doesn't exist after create";
		REQUIRE_NO_EXCEPT( dir.rename( "beta" ) ) << "Failed to rename dir";
		WANT_EQ( "beta/", dir.dpath().name().stdstr() )
			<< "Wrong name after rename";
		REQUIRE_TRUE( dir.exists() ) << "Dir doesn't exist after rename";
		REQUIRE_FALSE( directory( testdir + "alpha" ).exists() )
			<< "Pre-rename dir still exists";
	}
	TEST( FileSysTest, dir_iterate )
	{
		REQUIRE_NO_EXCEPT( testdir.ensureExists() )
			<< "Failed to create test dir";

		directory alpha = testdir.dpath() + "alpha";
		directory beta = testdir.dpath() + "beta";
		directory gamma = testdir.dpath() + "gamma";
		file f1 = testdir.dpath() + "f1";
		file f2 = testdir.dpath() + "f2";

		REQUIRE_NO_EXCEPT( alpha.ensureExists() );
		REQUIRE_NO_EXCEPT( beta.ensureExists() );
		REQUIRE_NO_EXCEPT( gamma.ensureExists() );
		REQUIRE_NO_EXCEPT( f1.touch() );
		REQUIRE_NO_EXCEPT( f2.touch() );

		std::set<string> paths;
		try
		{
			for( auto& p : testdir )
				paths.insert( p.base() );
		}
		catch( ... )
		{
			FATAL_MESSAGE << "Failed to obtain contents of directory";
		}
		auto str = string( ";" ).join( paths );
		WANT_EQ( "alpha/;beta/;f1;f2;gamma/", str.stdstr() );
	}

	TEST( FileSysTest, file_basic )
	{
		REQUIRE_NO_EXCEPT( testdir.ensureExists() )
			<< "Failed to create test dir";

		file testfile{ testdir.dpath() + "test1" };
		REQUIRE_FALSE( testfile.exists() ) << "File exists before create";
		REQUIRE_NO_EXCEPT( testfile.touch() ) << "Failed to create file";
		REQUIRE_TRUE( testfile.exists() ) << "File doesn't exist after create";
		REQUIRE_NO_EXCEPT( testfile.remove() ) << "Failed to remove file";
		REQUIRE_FALSE( testfile.exists() ) << "File exists after remove";
	}
	TEST( FileSysTest, file_rename )
	{
		REQUIRE_NO_EXCEPT( testdir.ensureExists() )
			<< "Failed to create test dir";

		file f = testdir.dpath() + "alpha";
		REQUIRE_NO_EXCEPT( f.touch() ) << "Failed to create file";
		REQUIRE_TRUE( f.exists() ) << "File doesn't exist after create";
		REQUIRE_NO_EXCEPT( f.rename( "beta" ) ) << "Failed to rename file";
		WANT_EQ( "beta", f.fpath().name().stdstr() )
			<< "Wrong name after rename";
		REQUIRE_TRUE( f.exists() ) << "File doesn't exist after rename";
		REQUIRE_FALSE( file( testdir + "alpha" ).exists() )
			<< "Pre-rename file still exists";
	}
	TEST( FileSysTest, file_iterate )
	{
		REQUIRE_NO_EXCEPT( testdir.ensureExists() )
			<< "Failed to create test dir";

		file f = testdir.dpath() + "alpha";
		REQUIRE_NO_EXCEPT( f.save( string( "Line 1\nSecond line\nThird" ) ) );
		std::list<string> lines;
		try
		{
			for( auto& line : f )
				lines.push_back( line );
		}
		catch( ... )
		{
			FATAL_MESSAGE << "Failed to load file lines";
		}
		auto str = string( ";" ).join( lines );
		WANT_EQ( "Line 1;Second line;Third", str.stdstr() );
	}
}
