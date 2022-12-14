#pragma once

#include <filesystem>
#include <eonexcept/Exception.h>
#include <eonstring/String.h>
#include <eonterminal/Terminal.h>
#include "Macros.h"
#include "TestFactory.h"
#include "Value.h"
#include "Result.h"
#include "TestName.h"



///////////////////////////////////////////////////////////////////////////////
//
// The 'eonitest' namespace encloses all public inline test functionality
//
namespace eonitest
{
	// Super-class for all tests.
	class __EonTestBase
	{
		///////////////////////////////////////////////////////////////////////
		//
		// Construction
		//
	public:

		// No default construction!
		__EonTestBase() = delete;

		// Construct with a specific test name.
		inline __EonTestBase( __EonTestName name ) { Name = std::move( name ); }




		///////////////////////////////////////////////////////////////////////
		//
		// Definitions
		//
	public:

		// A test reference, to be stored and used when a specific test is to be run.
		struct TestRef
		{
			__EonTestName Name;
			__EonLocation Location;
			FactoryMain* Factory{ nullptr };
			TestRef() = default;
			inline TestRef( __EonTestName name, __EonLocation location, FactoryMain* factory ) {
				Name = std::move( name ); Location = std::move( location ); Factory = factory; }
		};




		///////////////////////////////////////////////////////////////////////
		//
		// Execution
		//
	public:

		// Run test and return result.
		inline __Result _runEonTest_() { return test_body(); }


	protected:

		// Sub-classes must override this method for the actual test body.
		virtual __Result test_body() = 0;

		// Sub-classes must override this method and return an empty string if ready to run, error message if not.
		virtual eon::string canRun() const = 0;

		__Result _runCompare(
			const __Value& expected,
			const eon::string& cmp_operator,
			const __Value& actual,
			eon::string expected_expr,
			eon::string actual_expr ) const;




		///////////////////////////////////////////////////////////////////////
		//
		// Static methods.
		//
	public:

		// Register a new test
		static bool _registerEonTest_( __EonTestName name, __EonLocation location, FactoryMain* test );




		///////////////////////////////////////////////////////////////////////
		//
		// Attributes
		//
	protected:
		__EonTestName Name;

	public:
		static std::unique_ptr<std::list<TestRef>> TestsList;
		static std::unique_ptr<std::set<__EonTestName>> TestNames;
	};



	// Super-class for tests without sandbox.
	class __EonTest : public __EonTestBase
	{
	public:
		__EonTest() = delete;
		__EonTest( __EonTestName name ) : __EonTestBase( name ) {}
		eon::string canRun() const override { return eon::string::Empty; }
	};


	// Super-class for tests with sandbox.
	class __EonTestSandbox : public __EonTestBase
	{
	public:
		__EonTestSandbox() = delete;
		__EonTestSandbox( __EonTestName name ) : __EonTestBase( name ){ _prepareSandboxDir(); }
		virtual ~__EonTestSandbox() {}

		eon::string canRun() const override { return sandboxDir().empty() ? "Could not create sandbox!" : ""; }

		const std::filesystem::path& sandboxDir() const noexcept { return SandboxDir; }
		const eon::string& sandboxDirStr() const noexcept { return SandboxDirStr; }

	private:
		void _prepareSandboxDir() noexcept;
		bool _ensureNewDir( std::filesystem::path& path ) noexcept;
		bool _enum( std::filesystem::path& path );
		bool _removeDir( const std::filesystem::path& path ) noexcept;
		bool _makeDir( const std::filesystem::path& path ) noexcept;

	private:
		std::filesystem::path SandboxDir;
		eon::string SandboxDirStr;
	};
}
