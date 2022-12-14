#include "TestOptions.h"


namespace eonitest
{
	__TestOptions::__TestOptions( eon::string details )
	{
		auto elements = details.splitSequential<std::vector<eon::string>>( ':' );
		if( elements.size() > 0 )
			Operation = elements[ 0 ];
		for( size_t i = 1; i < elements.size(); ++i )
		{
			auto& element = elements[ i ];
			if( element.startsWith( "roundoff=" ) )
				RoundOff = element.substr( element.begin() + 9 ).toInt();
		}
	}



	bool __TestOptions::compareOperation() const noexcept
	{
		static std::set<eon::string> compare{ "<", "<=", ">", ">=", "==", "!=" };
		return compare.find( Operation ) != compare.end();
	}
}
