#include "FunctionValue.h"
#include "ControlValue.h"


namespace eon
{
	namespace vars
	{
		valueptr functionval::execute( variables& varcache, const tuple& args ) const
		{
			valueptr return_value;
			varcache.push();
			for( auto arg : args )
			{
				if( arg->named() )
					varcache.setTop( arg->name(), arg->value()->target( varcache, arg->value() ) );
			}
			for( auto& expr : Val )
			{
				auto result = expr->value()->actualExpression().evaluate( varcache );
				if( result && result->isControl() )
				{
					auto& ctrl = *dynamic_cast<controlval*>( &*result );
					if( ctrl.code() == ctrl_t::_return )
					{
						return_value = ctrl.val();
						break;
					}
				}
			}
			varcache.pop();
			return return_value;
		}
	}
}
