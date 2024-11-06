#pragma once

#include <eonstring/String.h>



///////////////////////////////////////////////////////////////////////////////
//
// The 'eonitest' namespace encloses all public inline test functionality
//
namespace eonitest
{
	struct TestName
	{
	public:
		TestName() = default;
		inline TestName( eon::string cls_name, eon::string func_name, eon::string tst_name ) noexcept {
			ClsName = std::move( cls_name ); FuncName = std::move( func_name ); TstName = std::move( tst_name ); }

		inline size_t length() const noexcept { return ClsName.numChars() + FuncName.numChars() + TstName.numChars(); }
		inline friend bool operator<( const TestName& a, const TestName& b ) noexcept
		{
			if( a.ClsName != b.ClsName )
				return a.ClsName < b.ClsName;
			else if( a.FuncName != b.FuncName )
				return a.FuncName < b.FuncName;
			else
				return a.TstName < b.TstName;
		}
	public:
		eon::string ClsName;
		eon::string FuncName;
		eon::string TstName;
	};
}
