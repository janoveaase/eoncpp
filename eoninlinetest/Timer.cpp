#include "Timer.h"


namespace eonitest
{
	eon::string Timer::elapsed() const
	{
		auto duration = std::chrono::high_resolution_clock::now() - Time;
		auto dur = std::chrono::duration_cast<std::chrono::microseconds>( duration ).count();
		std::vector<eon::string> elms;
		elms.push_back( eon::string( dur / 1000 ).padLeft( 3, ' ' ) );
		elms.push_back( eon::string( dur % 1000 ).padLeft( 3, '0' ) );
		return eon::string( "." ).join( elms ) + " ms";
	}
}
