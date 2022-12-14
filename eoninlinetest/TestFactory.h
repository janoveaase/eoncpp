#pragma once

#include <eonstring/String.h>


///////////////////////////////////////////////////////////////////////////////
//
// The 'eonitest' namespace encloses all public inline test functionality
//
namespace eonitest
{
	class __EonTestBase;
	using __EonTestPtr = std::shared_ptr<__EonTestBase>;

	// Factory class for creating test object
	class FactoryMain
	{
	public:
		FactoryMain() = default;
		virtual ~FactoryMain() = default;
		virtual __EonTestPtr createTest() = 0;
	};

	// Factory class for specific test.
	template<typename T>
	class TestFactory : public FactoryMain
	{
	public:
		TestFactory() = default;
		__EonTestPtr createTest() override {
			return std::make_shared<T>(); }
	};
}
