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
			char* buffer{ nullptr };
			size_t bufsize{ 0 };
			auto error = _dupenv_s( &buffer, &bufsize, "TMP" );
			path testdir;
			if( error == 0 )
			{
				testdir = string( buffer, bufsize );
				free( buffer );
			}
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
