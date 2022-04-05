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
		std::filesystem::path sandbox_dir;


		void prepare() override
		{
#ifdef EON_WINDOWS
			char* buffer{ nullptr };
			size_t bufsize{ 0 };
			auto error = _dupenv_s( &buffer, &bufsize, "TMP" );
			if( error == 0 )
			{
				sandbox_dir = buffer;
				free( buffer );
			}
#else
			sandbox_dir = "/tmp";
#endif
			sandbox_dir /= "eonfilesystest";

			std::error_code std_error;
			REQUIRE_TRUE( createSandboxDir( sandbox_dir, std_error ) ) << std_error.message();
		}
		void cleanup() override
		{
			std::error_code std_error;
			REQUIRE_TRUE( removeSandboxDir( sandbox_dir, std_error ) ) << std_error.message();
		}
	};
}
