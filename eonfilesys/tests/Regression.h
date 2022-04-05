#pragma once

#include <eontest/Test.h>
#include <eonfilesys/FileSys.h>
#include <fstream>


namespace eon
{
	class PathTest : public eontest::EonTest {};
	class DirTest : public eontest::EonTestSandbox
	{
	public:
		DirTest() : eontest::EonTestSandbox( "filesys", "dir" ) {}
	};
	class FileTest : public eontest::EonTestSandbox
	{
	public:
		FileTest() : eontest::EonTestSandbox( "filesys", "file" ) {}
	};
}
