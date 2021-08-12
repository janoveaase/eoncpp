#pragma once

#include <eontest/Test.h>
#include <eonexpression/Expression.h>
#include <eonfilesys/Dir.h>
#include <eonfilesys/File.h>


namespace eon
{
	class ExpressionTest : public eontest::EonTest {};
	class ReferenceTest : public eontest::EonTest, public vars::refsource
	{
	public:
		std::map<nameref, vars::valueptr> Vars;
		inline vars::valueptr _get( const nameref& ref, vars::valueptr context ) const noexcept override {
			auto found = Vars.find( ref ); return found != Vars.end() ? found->second : vars::valueptr(); }
		inline void _set( const nameref& ref, vars::valueptr context, vars::valueptr value ) override {
			Vars[ ref ] = value; }
	};
	class FileTest : public eontest::EonTest
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
