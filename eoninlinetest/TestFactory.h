#pragma once

#include <eonstring/String.h>


///////////////////////////////////////////////////////////////////////////////
//
// The 'eonitest' namespace encloses all public inline test functionality
//
namespace eonitest
{
	class TestBase;
	using TestPtr = std::shared_ptr<TestBase>;

	class __EonTestBase;

	// Factory class for creating test object
	class FactoryMain
	{
	public:
		FactoryMain() = default;
		virtual ~FactoryMain() = default;
		virtual TestPtr create() = 0;
	};

	// Factory class for specific test.
	template<typename T>
	class TstFactory : public FactoryMain
	{
	public:
		TstFactory() = default;
		TestPtr create() override { return std::make_shared<T>(); }
	};
}
