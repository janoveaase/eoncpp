#pragma once

#include <eontest/Test.h>
#include <eonfile/File.h>
#include <fstream>


namespace eon
{
	class FileTest : public eontest::EonTest
	{
	public:
		path testfile;


		void prepare() override
		{
#ifdef EON_WINDOWS
			path testdir{ getenv( "TMP" ) };
#else
			path testdir{ "/tmp" };
#endif
			testfile = testdir + "/testfile";
			if( std::ifstream( testfile.c_str() ) )
				remove( testfile.c_str() );
		}
		void cleanup() override
		{
			if( std::ifstream( testfile.c_str() ) )
				remove( testfile.c_str() );
		}
	};
}
