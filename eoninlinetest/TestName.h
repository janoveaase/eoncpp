#pragma once

#include <eonstring/String.h>



///////////////////////////////////////////////////////////////////////////////
//
// The 'eonitest' namespace encloses all public inline test functionality
//
namespace eonitest
{
	// Class for unique test name.
	class __EonTestName
	{
	public:
		__EonTestName() = default;
		inline __EonTestName( eon::string classname, eon::string methodname, eon::string testname ) {
			ClassName = std::move( classname ); MethodName = std::move( methodname ); TestName = std::move( testname ); }

		inline const eon::string& className() const noexcept { return ClassName; }
		inline const eon::string& methodName() const noexcept { return MethodName; }
		inline const eon::string& testName() const noexcept { return TestName; }

		// inline eon::string str() const { return ClassName + " " + MethodName + "." + TestName; }
		inline size_t length() const noexcept { return ClassName.numChars() + MethodName.numChars() + TestName.numChars(); }

		inline bool operator<( const __EonTestName& other ) const noexcept {
			return ClassName != other.ClassName ? ClassName < other.ClassName
				: ( MethodName != other.MethodName ? MethodName < other.MethodName : TestName < other.TestName ); }

	private:
		eon::string ClassName;
		eon::string MethodName;
		eon::string TestName;
	};
}
