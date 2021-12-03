#include "OperatorAction.h"


namespace eon
{
	namespace actions
	{
		type::Object* OperatorAction::execute( scope::Scope& scope, type::Node& action_node )
		{
			throw type::AccessDenied( "Cannot execute 'operator action' - action not implemented!" );
		}
	}
}
