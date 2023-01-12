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
		inline bool operator<( const TestName& other ) const noexcept {
			return ClsName != other.ClsName ? ClsName < other.ClsName
				: ( FuncName != other.FuncName ? FuncName < other.FuncName : TstName < other.TstName ); }
	public:
		eon::string ClsName;
		eon::string FuncName;
		eon::string TstName;
	};
}
