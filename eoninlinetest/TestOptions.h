#pragma once

#include <eonstring/String.h>
#include "Location.h"



///////////////////////////////////////////////////////////////////////////////
//
// The 'eonitest' namespace encloses all public inline test functionality
//
namespace eonitest
{
	// Options for how to do a test
	class __TestOptions
	{
	public:

		// Construct without any details.
		__TestOptions() = default;

		// Construct based on string details
		__TestOptions( eon::string details );


		inline const eon::string operation() const noexcept { return Operation; }
		bool compareOperation() const noexcept;
		unsigned int roundOff() const noexcept { return RoundOff; }


	private:
		eon::string Operation;
		unsigned int RoundOff{ 2 };
	};
}
