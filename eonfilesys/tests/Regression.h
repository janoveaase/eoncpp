#pragma once

#include <eontest/Test.h>
#include <eonfilesys/FileSys.h>
#include <fstream>


namespace eon
{
	class PathTest : public eontest::EonTest {};
	class FileSysTest : public eontest::EonTest
	{
	public:
		directory testdir;


		void prepare() override
		{
#ifdef EON_WINDOWS
			char* buffer{ nullptr };
			size_t bufsize{ 0 };
			auto error = _dupenv_s( &buffer, &bufsize, "TMP" );
			if( error == 0 )
			{
				testdir = buffer;
				free( buffer );
			}
#else
			testdir = "/tmp";
#endif
			testdir = testdir.dpath() + "eonfilesystest";
			if( testdir.exists() )
				testdir.remove();
		}
		void cleanup() override
		{
			if( testdir.exists() )
				testdir.remove();
		}
	};
}
