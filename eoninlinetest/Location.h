#pragma once

#include <eonstring/String.h>



///////////////////////////////////////////////////////////////////////////////
//
// The 'eonitest' namespace encloses all public inline test functionality
//
namespace eonitest
{
	// Class for source location.
	class __EonLocation
	{
	public:
		__EonLocation() = default;
		inline __EonLocation( eon::string file, eon::string line ) { File = std::move( file ); Line = std::move( line ); }

		inline const eon::string& file() const noexcept { return File; }
		inline const eon::string& line() const noexcept { return Line; }

	private:
		eon::string File;
		eon::string Line;
	};
}
