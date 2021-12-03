#pragma once

#include <eontest/Test.h>
#include <eonsource/SourceReporter.h>
#include <eonsource/String.h>
#include <eonsource/File.h>
#include <chrono>


namespace eon
{
	class Source : public eontest::EonTest
	{
	public:
		string TestFile;


		void prepare() override
		{
			string testdir;
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
			TestFile = testdir + "/eon_test.txt";
		}

		bool prepareTestFile( string test_data )
		{
			std::ofstream file( TestFile.c_str(), std::ios_base::out | std::ios_base::binary );
			if( !file )
				return false;
			file.write( test_data.c_str(), test_data.numBytes() );
			file.close();
			return true;
		}
	};
	class SourceRef : public Source {};
	class SourceReporter : public eontest::EonTest, public source::MessageTarget
	{
	public:
		std::vector<string> Messages;

		inline void report( const source::Message& message ) override { Messages.push_back( message.str() ); }
	};
}
