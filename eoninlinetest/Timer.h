#pragma once

#include <chrono>
#include <eonstring/String.h>



///////////////////////////////////////////////////////////////////////////////
//
// The 'eonitest' namespace encloses all public inline test functionality
//
namespace eonitest
{
	class Timer
	{
	public:
		Timer() { Time = std::chrono::high_resolution_clock::now(); }

		eon::string elapsed() const;

	private:
		std::chrono::high_resolution_clock::time_point Time;
	};
}
