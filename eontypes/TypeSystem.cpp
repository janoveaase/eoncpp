#include "TypeSystem.h"
#include "Action.h"


namespace eon
{
	namespace type
	{
		DynamicActionMap::~DynamicActionMap()
		{
			for( auto& action : Actions )
				action.second->die();
		}
		void DynamicActionMap::ensureFree( name_t signature ) noexcept
		{
			auto found = Actions.find( signature );
			if( found != Actions.end() )
			{
				found->second->die();
				Actions.erase( found );
			}
		}
	}
}
