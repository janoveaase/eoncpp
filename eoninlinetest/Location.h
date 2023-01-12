#pragma once

#include <eonstring/String.h>



///////////////////////////////////////////////////////////////////////////////
//
// The 'eonitest' namespace encloses all public inline test functionality
//
namespace eonitest
{
	class TestLocation
	{
	public:
		TestLocation() = default;
		inline TestLocation( eon::string file, eon::string num_line ) noexcept {
			File = std::move( file ); NumLine = std::move( num_line ); }
	public:
		eon::string File;
		eon::string NumLine;
	};
}
